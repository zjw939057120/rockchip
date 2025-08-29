//
// Created by Administrator on 2025/8/29.
//

#include "ffmpeg_utils.h"

int ffmpeg_utils::push_h264_to_rtsp_init(const char *rtsp_url, int width, int height, int fps, int bitrate) {
    this->m_rtsp_url = rtsp_url;
    this->m_width = width;
    this->m_height = height;
    this->m_fps = fps;
    this->m_bitrate = bitrate;
    // 初始化 FFmpeg
    avformat_network_init();

    // 创建输出上下文
    ret = avformat_alloc_output_context2(&fmt_ctx, NULL, "rtsp", m_rtsp_url);
    if (ret < 0 || !fmt_ctx) {
        fprintf(stderr, "Could not allocate output context: %d\n", ret);
        return -1;
    }

    // 创建视频流
    video_stream = avformat_new_stream(fmt_ctx, NULL);
    if (!video_stream) {
        fprintf(stderr, "Could not create video stream\n");
        avformat_free_context(fmt_ctx);
        return -1;
    }

    // 设置视频流的编码参数
    codecpar = video_stream->codecpar;
    codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    codecpar->codec_id = AV_CODEC_ID_H264;
    codecpar->width = m_width;
    codecpar->height = m_height;
    codecpar->format = AV_PIX_FMT_YUV420P;
    codecpar->bit_rate = m_bitrate; // 比特率
    video_stream->time_base = (AVRational) {1, m_fps};

    // 打开输出 URL
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&fmt_ctx->pb, m_rtsp_url, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output URL: %d\n", ret);
            avformat_free_context(fmt_ctx);
            return -1;
        }
    }

    // 写入流头部信息
    ret = avformat_write_header(fmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when writing header: %d\n", ret);
        avio_close(fmt_ctx->pb);
        avformat_free_context(fmt_ctx);
        return -1;
    }
    return ret;
}

static int64_t pts_counter = 0; // 全局计数器
static int64_t dts_counter = 0;

int ffmpeg_utils::push_h264_to_rtsp(const void *data, size_t len) {
    // 初始化 AVPacket
    av_init_packet(&pkt);
    pkt.data = (uint8_t *) data;
    pkt.size = len;
    pkt.stream_index = video_stream->index;
    pkt.pts = pts_counter;
    pkt.dts = pts_counter;
    pkt.duration = 1;
    pts_counter += 90000 / m_fps; // 90000 是 H.264 的时间基
    dts_counter += 90000 / m_fps;

    // 推送数据包
    ret = av_interleaved_write_frame(fmt_ctx, &pkt);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when writing frame: %s %d\n", m_rtsp_url, ret);
    }
    return ret;
}

int ffmpeg_utils::push_h264_to_rtsp_end() {
    // 写入流尾部信息
    av_write_trailer(fmt_ctx);

    // 释放资源
    avio_close(fmt_ctx->pb);
    avformat_free_context(fmt_ctx);
    avformat_network_deinit();

    return ret;
}
