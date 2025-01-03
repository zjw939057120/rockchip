//
// Created by zjw93 on 2025/1/3.
//

#ifndef MPP_TINYCAP_H
#define MPP_TINYCAP_H

#include "fdk-aac/aacenc_lib.h"

//#define DEBUG(fmt, args...)
#define DEBUG(fmt, args...)    printf(fmt, ##args)

int tinycap_capture();

#endif //MPP_TINYCAP_H
