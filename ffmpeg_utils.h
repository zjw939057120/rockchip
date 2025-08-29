//
// Created by Administrator on 2025/8/29.
//

#ifndef MPI_ENC_FFMPEG_UTILS_H
#define MPI_ENC_FFMPEG_UTILS_H


extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ffmpeg_utils {

public:
    int push_h264_to_rtsp_init(const char *rtsp_url, int width, int height, int fps, int bitrate);

    int push_h264_to_rtsp(const void *data, size_t len, int fps);

    int push_h264_to_rtsp_end();

private:
    AVFormatContext *fmt_ctx = NULL;
    AVStream *video_stream = NULL;
    AVCodecParameters *codecpar = NULL;
    AVPacket pkt;
    int ret;
};


#endif //MPI_ENC_FFMPEG_UTILS_H
