/*
 * Copyright (C) 2022  Rockchip Electronics Co., Ltd.
 * Authors:
 *     YuQiaowei <cerf.yu@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rga_resize_demo.h"

const char *inputFilePath = "/opt/in0w1280-h720-nv12.yuv";
const char *outputFilePath = "/opt/out0w1280-h720-nv12.yuv";

void rga_resize_init(RGAResizeParam *resizeParam) {
    memset(&resizeParam->src_img, 0, sizeof(resizeParam->src_img));
    memset(&resizeParam->dst_img, 0, sizeof(resizeParam->dst_img));

    printf("src_format:%s,st_format:%s\n", translate_format_str(resizeParam->src_format),
           translate_format_str(resizeParam->dst_format));
    resizeParam->src_buf_size =
            resizeParam->src_width * resizeParam->src_height * get_bpp_from_format(resizeParam->src_format);
    resizeParam->dst_buf_size =
            resizeParam->dst_width * resizeParam->dst_height * get_bpp_from_format(resizeParam->dst_format);

    resizeParam->src_buf = (char *) malloc(resizeParam->src_buf_size);
    resizeParam->dst_buf = (char *) malloc(resizeParam->dst_buf_size);
}

int rga_resize(RGAResizeParam *resizeParam) {
    int ret = 0;

    /* fill image data */
    if (0 != read_image_from_file(resizeParam->src_buf, inputFilePath, resizeParam->src_width, resizeParam->src_height,
                                  resizeParam->src_format, 0)) {
        printf("src image read err\n");
        draw_rgba(resizeParam->src_buf, resizeParam->src_width, resizeParam->src_height);
    }
    memset(resizeParam->dst_buf, 0x80, resizeParam->dst_buf_size);

    resizeParam->src_handle = importbuffer_virtualaddr(resizeParam->src_buf, resizeParam->src_buf_size);
    resizeParam->dst_handle = importbuffer_virtualaddr(resizeParam->dst_buf, resizeParam->dst_buf_size);
    if (resizeParam->src_handle == 0 || resizeParam->dst_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    resizeParam->src_img = wrapbuffer_handle(resizeParam->src_handle, resizeParam->src_width, resizeParam->src_height,
                                             resizeParam->src_format);
    resizeParam->dst_img = wrapbuffer_handle(resizeParam->dst_handle, resizeParam->dst_width, resizeParam->dst_height,
                                             resizeParam->dst_format);

    /*
     * Scale up the src image to 1920*1080.
        --------------    ---------------------
        |            |    |                   |
        |  src_img   |    |     dst_img       |
        |            | => |                   |
        --------------    |                   |
                          |                   |
                          ---------------------
     */

    ret = imcheck(resizeParam->src_img, resizeParam->dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS) ret));
        return -1;
    }

    ret = imresize(resizeParam->src_img, resizeParam->dst_img);
    if (ret == IM_STATUS_SUCCESS) {
        //printf("%s running success!\n", LOG_TAG);
    } else {
        printf("%s running failed, %s\n", LOG_TAG, imStrError((IM_STATUS) ret));
        goto release_buffer;
    }

    write_image_to_file(resizeParam->dst_buf, outputFilePath, resizeParam->dst_width, resizeParam->dst_height,
                        resizeParam->dst_format, 0);

    release_buffer:
    if (resizeParam->src_handle)
        releasebuffer_handle(resizeParam->src_handle);
    if (resizeParam->dst_handle)
        releasebuffer_handle(resizeParam->dst_handle);

    if (resizeParam->src_buf)
        free(resizeParam->src_buf);
    if (resizeParam->dst_buf)
        free(resizeParam->dst_buf);

    return ret;
}

void *rga_resize_from_frame(RGAResizeParam *resizeParam, const void *frame, int length) {
    int ret = 0;

    /* fill image data */
    if (0 != read_image_from_buf(resizeParam->src_buf, frame, length)) {
        printf("src image read err\n");
        draw_rgba(resizeParam->src_buf, resizeParam->src_width, resizeParam->src_height);
    }
    memset(resizeParam->dst_buf, 0x80, resizeParam->dst_buf_size);

    resizeParam->src_handle = importbuffer_virtualaddr(resizeParam->src_buf, resizeParam->src_buf_size);
    resizeParam->dst_handle = importbuffer_virtualaddr(resizeParam->dst_buf, resizeParam->dst_buf_size);
    if (resizeParam->src_handle == 0 || resizeParam->dst_handle == 0) {
        printf("importbuffer failed!\n");
//        goto release_buffer;
    }

    resizeParam->src_img = wrapbuffer_handle(resizeParam->src_handle, resizeParam->src_width, resizeParam->src_height,
                                             resizeParam->src_format);
    resizeParam->dst_img = wrapbuffer_handle(resizeParam->dst_handle, resizeParam->dst_width, resizeParam->dst_height,
                                             resizeParam->dst_format);

    /*
     * Scale up the src image to 1920*1080.
        --------------    ---------------------
        |            |    |                   |
        |  src_img   |    |     dst_img       |
        |            | => |                   |
        --------------    |                   |
                          |                   |
                          ---------------------
     */

    ret = imcheck(resizeParam->src_img, resizeParam->dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS) ret));
    }

    ret = imresize(resizeParam->src_img, resizeParam->dst_img);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s running success!\n", LOG_TAG);
    } else {
        printf("%s running failed, %s\n", LOG_TAG, imStrError((IM_STATUS) ret));
    }

    return resizeParam->dst_buf;

//    release_buffer:
//    if (resizeParam->src_handle)
//        releasebuffer_handle(resizeParam->src_handle);
//    if (resizeParam->dst_handle)
//        releasebuffer_handle(resizeParam->dst_handle);
//
//    if (resizeParam->src_buf)
//        free(resizeParam->src_buf);
//    if (resizeParam->dst_buf)
//        free(resizeParam->dst_buf);
}

int rga_src_sieze(RGAResizeParam *resizeParam) {
    return resizeParam->src_width * resizeParam->src_height * get_bpp_from_format(resizeParam->src_format);;
}

#if 0
int main() {
    RGAResizeParam resizeParam;
    resizeParam.src_width = 1280;
    resizeParam.src_height = 720;
    resizeParam.src_format = RK_FORMAT_YCbCr_420_SP;

    resizeParam.dst_width = 1280;
    resizeParam.dst_height = 720;
    resizeParam.dst_format = RK_FORMAT_YCbCr_420_SP;

    rga_resize_init(&resizeParam);
    rga_resize(&resizeParam);
}
#endif