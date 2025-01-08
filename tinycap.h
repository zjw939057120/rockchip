//
// Created by zjw93 on 2025/1/3.
//

#ifndef MPP_TINYCAP_H
#define MPP_TINYCAP_H

#include "fdk-aac/aacenc_lib.h"

//#define DEBUG(fmt, args...)
#define DEBUG(fmt, args...)    printf(fmt, ##args)

int tinycap_capture();
void tinycap_capture_thread();

#ifndef XS_STREAM_AUDIO_AAC
#define XS_STREAM_AUDIO_AAC 1
#endif

#define AUDIO_CHANNEL_0 0
#define AUDIO_CHANNEL_1 1
//#define AUDIO_CHANNEL_2 2
//#define AUDIO_CHANNEL_3 3

//#define FILE_OUTPUT_WAV
//#define FILE_OUTPUT_AAC

#endif //MPP_TINYCAP_H
