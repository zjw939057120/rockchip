//
// Created by zjw93 on 2024/11/7.
//

#ifndef MPP_MPI_ENC_TEST_H
#define MPP_MPI_ENC_TEST_H

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

void enc_test_multi_ex(MpiEncTestArgs* cmd);

void mpp_packet_send(uint8_t chn_id, void *pVoid, uint8_t streamType, int i, size_t len);

void env_init();

#define _OUTPUT_CHANNEL_MAX_ 4
#define _FILE_OUTPUT_H264_
#define _FILE_OUTPUT_OSD_
#define _FILE_OUTPUT_YUV_
MpiEncTestArgs mpiEncTestArgs[_OUTPUT_CHANNEL_MAX_];

#ifndef XS_STREAM_VIDEO_H264
#define XS_STREAM_VIDEO_H264 0
#endif

#endif //MPP_MPI_ENC_TEST_H
