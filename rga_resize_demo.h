//
// Created by zjw93 on 2024/11/21.
//

#ifndef MPP__RGA_RESIZE_DEMO_H
#define MPP__RGA_RESIZE_DEMO_H

#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "rga_resize"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <linux/stddef.h>
#include <rk_type.h>

#include "RgaUtils.h"
#include "im2d.hpp"

#include "librga_utils.h"

typedef struct {
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    char *src_buf, *dst_buf;
    int src_buf_size, dst_buf_size;

    rga_buffer_t src_img, dst_img;
    rga_buffer_handle_t src_handle, dst_handle;

    RK_S32 chn_id;
    int src_size;
} RGAResizeParam;

#ifdef __cplusplus
extern "C" {
#endif

void rga_resize_init(RGAResizeParam *resizeParam);
int rga_resize(RGAResizeParam *resizeParam);
void* rga_resize_from_frame(RGAResizeParam *resizeParam, const void *frame, int length);
int rga_src_sieze(RGAResizeParam *resizeParam);
int read_image_from_buf(void *buf, const void *src_buf, int length);

#ifdef __cplusplus
}
#endif

#endif //MPP__RGA_RESIZE_DEMO_H
