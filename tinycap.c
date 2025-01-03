/* tinycap.c
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#include <tinyalsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "tinycap.h"

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

int capturing = 1;

unsigned int capture_sample(FILE *file, unsigned int card, unsigned int device,
                            unsigned int channels, unsigned int rate, unsigned int bits,
                            enum pcm_format format, unsigned int period_size,
                            unsigned int period_count, unsigned int cap_time);

void sigint_handler(int sig) {
    capturing = 0;
}

int tinycap_capture() {
#ifdef FILE_OUTPUT_WAV
    const char *file_output_wav = "/opt/file.wav";
#endif
    FILE *fp_output_wav;
    struct wav_header header;
    unsigned int card = 3;
    unsigned int device = 0;
    unsigned int channels = 2;
    unsigned int rate = 44100;
    unsigned int bits = 16;
    unsigned int frames;
    unsigned int period_size = 1024;
    unsigned int period_count = 4;
    unsigned int cap_time = 0;
    enum pcm_format format;

#ifdef FILE_OUTPUT_WAV
    fp_output_wav = fopen(file_output_wav, "wb");
    if (!fp_output_wav) {
        fprintf(stderr, "Unable to create file '%s'\n", file_output_wav);
        return 1;
    }
#endif

    header.riff_id = ID_RIFF;
    header.riff_sz = 0;
    header.riff_fmt = ID_WAVE;
    header.fmt_id = ID_FMT;
    header.fmt_sz = 16;
    header.audio_format = FORMAT_PCM;
    header.num_channels = channels;
    header.sample_rate = rate;

    switch (bits) {
        case 32:
            format = PCM_FORMAT_S32_LE;
            break;
        case 24:
            format = PCM_FORMAT_S24_LE;
            break;
        case 16:
            format = PCM_FORMAT_S16_LE;
            break;
        default:
            fprintf(stderr, "%u bits is not supported.\n", bits);
            if (fp_output_wav)
                fclose(fp_output_wav);
            return 1;
    }

    header.bits_per_sample = pcm_format_to_bits(format);
    header.byte_rate = (header.bits_per_sample / 8) * channels * rate;
    header.block_align = channels * (header.bits_per_sample / 8);
    header.data_id = ID_DATA;

    /* leave enough room for header */
#ifdef FILE_OUTPUT_WAV
    fseek(fp_output_wav, sizeof(struct wav_header), SEEK_SET);
#endif

    /* install signal handler and begin capturing */
    signal(SIGINT, sigint_handler);
    signal(SIGHUP, sigint_handler);
    signal(SIGTERM, sigint_handler);
    frames = capture_sample(fp_output_wav, card, device, header.num_channels,
                            header.sample_rate, bits, format,
                            period_size, period_count, cap_time);
    printf("Captured %u frames\n", frames);

    /* write header now all information is known */
    header.data_sz = frames * header.block_align;
    header.riff_sz = header.data_sz + sizeof(header) - 8;
#ifdef FILE_OUTPUT_WAV
    fseek(fp_output_wav, 0, SEEK_SET);
    fwrite(&header, sizeof(struct wav_header), 1, fp_output_wav);

    fclose(fp_output_wav);
#endif
    return 0;
}

unsigned int capture_sample(FILE *file, unsigned int card, unsigned int device,
                            unsigned int channels, unsigned int rate, unsigned int bits,
                            enum pcm_format format, unsigned int period_size,
                            unsigned int period_count, unsigned int cap_time) {
    struct pcm_config config;
    struct pcm *pcm;
    char *buffer;
    unsigned int s32ReadPcmBytes;
    unsigned int size;
    unsigned int bytes_read = 0;
    unsigned int frames = 0;
    struct timespec end;
    struct timespec now;

    memset(&config, 0, sizeof(config));
    config.channels = channels;
    config.rate = rate;
    config.period_size = period_size;
    config.period_count = period_count;
    config.format = format;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    pcm = pcm_open(card, device, PCM_IN, &config);
    if (!pcm || !pcm_is_ready(pcm)) {
        fprintf(stderr, "Unable to open PCM device (%s)\n",
                pcm_get_error(pcm));
        return 0;
    }

    s32ReadPcmBytes = pcm_get_buffer_size(pcm);
    size = s32ReadPcmBytes;
    buffer = malloc(size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate %u bytes\n", size);
        free(buffer);
        pcm_close(pcm);
        return 0;
    }

#ifdef FILE_OUTPUT_AAC
    const char *file_output_aac = "/opt/file.aac";
    FILE *fp_output_aac;
    fp_output_aac = fopen(file_output_aac, "wb");
    if (!fp_output_aac) {
        fprintf(stderr, "Unable to create file '%s'\n", file_output_aac);
        return 1;
    }
#endif

    /* PCM参数 */
    unsigned int u32PcmSampleRate = 44100; // 采样率
    unsigned int u32PcmSampleBits = 16; // 采样位数
    unsigned int u32PcmChannels = 2; // 声道数

    /* aac编码器 */
    HANDLE_AACENCODER aacEncHandle = NULL; // HANDLE_AACENCODER其实是一个结构体指针
    AACENC_InfoStruct aacEncInfoSt = {0};
    AACENC_ERROR aacErrNum = AACENC_OK; // AACENC_OK:0

    /* 编码相关参数 */
    unsigned int u32PcmInBufBytes = 0;    // 编码时需要传入的PCM数据大小（字节数）
    unsigned int u32AacOutBufMaxBytes = 0; // 编码后得到一帧aac数据最大的大小（字节数）
    unsigned char *pu8AacEncBuf = NULL; // 编码得到的aac缓存，后面根据编码器传出参数malloc分配

    /* 先打开输入/输出文件 */
#ifdef FILE_OUTPUT_AAC
    fp_output_aac = fopen(file_output_aac, "wb");
    if (fp_output_aac == NULL) {
        char errMsg[128] = {0};
        snprintf(errMsg, 128, "open file(%s) error", file_output_aac);
        perror(errMsg);
        return -1;
    }
#endif

    /* AAC编码 1/8：打开编码器，传出编码器句柄 */
    aacErrNum = aacEncOpen(&aacEncHandle, 0, u32PcmChannels);
    if (aacErrNum != AACENC_OK) {
        printf("Open aac encoder error!\n");
    }

    /* AAC编码 2/8：配置/初始化编码器 */
    // 配置编码器 a/b：设置参数
    aacErrNum = aacEncoder_SetParam(aacEncHandle, AACENC_AOT, AOT_AAC_LC);    // Audio object type, 选择输出规格
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_SBR_MODE,
                                     1);        // Spectral Band Replication，是否使能SBR技术，-1:自动配置（默认） 0:关闭 1:开启
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_SAMPLERATE,
                                     u32PcmSampleRate); // Audio input data sampling rate
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_CHANNELMODE,
                                     (u32PcmChannels == 1) ? MODE_1 : MODE_2); // 声道模式，还有多种模式，这里只列出2种
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_CHANNELORDER,
                                     1);    // 输入音频数据通道排序方案，0: MPEG频道排序（默认） 1: WAVE文件格式通道排序
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_BITRATEMODE, 5);        // 比特率模式，0:CBR  1~5:VBR（数值越大动态码率越高）
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_BITRATE,
                                     128000);    // 设置比特率大小，只有AACENC_BITRATEMODE设置为静态码率CBR时生效，VBR时忽略
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_TRANSMUX,
                                     TT_MP4_ADTS); // 传输类型，TT_MP4_ADIF/TT_MP4_ADTS/TT_MP4_LATM_MCP1...
    aacErrNum |= aacEncoder_SetParam(aacEncHandle, AACENC_AFTERBURNER,
                                     1);        // “加力燃烧室”，提高音质，0:关闭（默认） 1:开启。官方推荐内存和性能足够的话开启
    // 配置编码器 b/b：设置到编码器里面去
    aacErrNum |= aacEncEncode(aacEncHandle, NULL, NULL, NULL, NULL);
    if (aacErrNum != AACENC_OK) {
        printf("Configure aac encoder error!\n");
    }

    /* AAC编码 3/8：获取编码器信息，从这里得到输入PCM缓存应该设置多大、输出最大的aac字节数 */
    aacErrNum = aacEncInfo(aacEncHandle, &aacEncInfoSt);
    if (aacErrNum != AACENC_OK) {
        printf("Get aac encoder info error!\n");
    }

    // 根据上面获得的编码器信息得到一些比较重要的参数
    u32PcmInBufBytes = aacEncInfoSt.frameLength * u32PcmSampleBits / 8 * u32PcmChannels;
    u32AacOutBufMaxBytes = aacEncInfoSt.maxOutBufBytes;

    DEBUG("PCM should in bytes: %d \t AAC out max bytes: %d\n", u32PcmInBufBytes, u32AacOutBufMaxBytes);

    /* 根据上面打开编码器信息分配对应大小的缓存 */
    pu8AacEncBuf = (unsigned char *) malloc(u32AacOutBufMaxBytes);


    printf("Capturing sample: %u ch, %u hz, %u bit\n", channels, rate,
           pcm_format_to_bits(format));

    clock_gettime(CLOCK_MONOTONIC, &now);
    end.tv_sec = now.tv_sec + cap_time;
    end.tv_nsec = now.tv_nsec;

    while (capturing && !pcm_read(pcm, buffer, size)) {
#ifdef FILE_OUTPUT_WAV
        if (fwrite(buffer, 1, size, file) != size) {
            fprintf(stderr, "Error capturing sample\n");
            break;
        }
#endif
        bytes_read += size;
        if (cap_time) {
            clock_gettime(CLOCK_MONOTONIC, &now);
            if (now.tv_sec > end.tv_sec ||
                (now.tv_sec == end.tv_sec && now.tv_nsec >= end.tv_nsec))
                break;
        }

        /* aac编码一帧数据用到的参数 */
        AACENC_BufDesc inPcmBufDesc = {0};
        AACENC_BufDesc outAacBufDesc = {0};
        AACENC_InArgs inArgs = {0};
        AACENC_OutArgs outArgs = {0};
        int inIdentifier = IN_AUDIO_DATA;
        int outIdentifier = OUT_BITSTREAM_DATA;
        //int inElsize = 2;
        //int outElsize = 1;
        int inElsize = sizeof(INT_PCM); // 参考aacenc_lib.h:260示例
        int outElsize = sizeof(UCHAR);

        /* AAC编码 5/8：填充编码器需要的参数，包括编码的pcm数据地址，大小等 */
        inPcmBufDesc.numBufs = 1;
        inPcmBufDesc.bufs = (void **) &buffer;
        inPcmBufDesc.bufferIdentifiers = &inIdentifier;
        inPcmBufDesc.bufSizes = &s32ReadPcmBytes;
        inPcmBufDesc.bufElSizes = &inElsize;

        inArgs.numInSamples = (s32ReadPcmBytes <= 0) ? -1 : s32ReadPcmBytes / 2;

        outAacBufDesc.numBufs = 1;
        outAacBufDesc.bufs = (void **) &pu8AacEncBuf;
        outAacBufDesc.bufferIdentifiers = &outIdentifier;
        outAacBufDesc.bufSizes = &u32AacOutBufMaxBytes;
        outAacBufDesc.bufElSizes = &outElsize;

        /* AAC编码 6/8：将pcm编码出aac */
        aacErrNum = aacEncEncode(aacEncHandle, &inPcmBufDesc, &outAacBufDesc, &inArgs, &outArgs);
        if (aacErrNum != AACENC_OK) {
            printf("Aac encoder encode error!\n");
        }

        DEBUG("IN(pcm): [buf bytes: %4d] [channels: %d] [sample cnt per channel: %4d]  ==>   OUT(aac): [encode out bytes: %4d] \n",
              s32ReadPcmBytes, u32PcmChannels, inArgs.numInSamples / u32PcmChannels, outArgs.numOutBytes);

        if (outArgs.numOutBytes == 0) {
            continue;
        }
#ifdef FILE_OUTPUT_AAC
        /* AAC编码 7/8：将编码出的aac数据写入文件 */
        fwrite(pu8AacEncBuf, 1, outArgs.numOutBytes, fp_output_aac);
#endif
#ifndef _ENV_DEBUG_
        mpp_packet_send(3, pu8AacEncBuf, XS_STREAM_AUDIO_AAC, 1, outArgs.numOutBytes);
#endif
    }
    free(pu8AacEncBuf);
    aacEncClose(&aacEncHandle);
#ifdef FILE_OUTPUT_AAC
    fclose(fp_output_aac);
#endif

    frames = pcm_bytes_to_frames(pcm, bytes_read);
    free(buffer);
    pcm_close(pcm);
    return frames;
}

#ifdef _ENV_DEBUG_

int main(int argc, char **argv) {
    tinycap_capture();
}

#else

int tinycap_capture_thread() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void *(*)(void *)) tinycap_capture, NULL);
}

#endif