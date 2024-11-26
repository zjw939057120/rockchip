//
// Created by zjw93 on 2024/11/26.
//

#ifndef MPI_ENC_TEST_H
#define MPI_ENC_TEST_H

#if defined(_WIN32)
#include "vld.h"
#endif

#define MODULE_TAG "mpi_enc_test"

#include <string.h>
#include "rk_mpi.h"

#include "mpp_env.h"
#include "mpp_mem.h"
#include "mpp_time.h"
#include "mpp_debug.h"
#include "mpp_common.h"

#include "mpp_utils.h"
#include "mpi_enc_utils.h"
#include "camera_source.h"
#include "mpp_enc_roi_utils.h"
#include "mpp_rc_api.h"

struct MPPResize{
    void *yuv_data;
    RK_U32 yuv_size;
    MppMeta meta_resize;
    MppFrame frame_resize;
    MppPacket packet_resize;
    void *buf_resize;
    RK_S32 cam_frm_idx_resize;
    MppBuffer cam_buf_resize;
    RK_U32 eoi_resize;
};

#define _RGA_RESIZE_
#define MAX_CHANNEL 8


#endif //MPI_ENC_TEST_H
