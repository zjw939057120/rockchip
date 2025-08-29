//
// Created by zjw93 on 2025/8/27.
//

#ifndef MPI_ENC_MPP_ENC_H
#define MPI_ENC_MPP_ENC_H

#include "ffmpeg_utils.h"

extern "C" {
#if defined(_WIN32)
#include "vld.h"
#endif

#define MODULE_TAG "mpi_enc_test"

#include <string.h>
#include <math.h>
#include "rk_mpi.h"

#include "mpp_env.h"
#include "mpp_mem.h"
#include "mpp_time.h"
#include "mpp_debug.h"
#include "mpp_common.h"
#include "mpp_soc.h"

#include "utils.h"
#include "mpi_enc_utils.h"
#include "camera_source.h"
#include "mpp_enc_roi_utils.h"
#include "mpp_rc_api.h"
#include "mpp_enc.h"
};

//#define FILE_OUTPUT
#define RTSP_OUTPUT

static RK_S32 aq_thd_smart[16] = {
        1, 3, 3, 3, 3, 3, 5, 5,
        8, 8, 8, 15, 15, 20, 25, 28
};

static RK_S32 aq_step_smart[16] = {
        -8, -7, -6, -5, -4, -3, -2, -1,
        0, 1, 2, 3, 4, 6, 8, 10
};

typedef struct {
    // base flow context
    MppCtx ctx;
    MppApi *mpi;
    RK_S32 chn;

    // global flow control flag
    RK_U32 frm_eos;
    RK_U32 pkt_eos;
    RK_U32 frm_pkt_cnt;
    RK_S32 frame_num;
    RK_S32 frame_count;
    RK_U64 stream_size;
    /* end of encoding flag when set quit the loop */
    volatile RK_U32 loop_end;

    // src and dst
    FILE *fp_input;
    FILE *fp_output;
    FILE *fp_verify;

    /* encoder config set */
    MppEncCfg cfg;
    MppEncPrepCfg prep_cfg;
    MppEncRcCfg rc_cfg;
    MppEncCodecCfg codec_cfg;
    MppEncSliceSplit split_cfg;
    MppEncOSDPltCfg osd_plt_cfg;
    MppEncOSDPlt osd_plt;
    MppEncOSDData osd_data;
    RoiRegionCfg roi_region;
    MppEncROICfg roi_cfg;

    // input / output
    MppBufferGroup buf_grp;
    MppBuffer frm_buf;
    MppBuffer pkt_buf;
    MppBuffer md_info;
    MppEncSeiMode sei_mode;
    MppEncHeaderMode header_mode;

    // paramter for resource malloc
    RK_U32 width;
    RK_U32 height;
    RK_U32 hor_stride;
    RK_U32 ver_stride;
    MppFrameFormat fmt;
    MppCodingType type;
    RK_S32 loop_times;
    CamSource *cam_ctx;
    MppEncRoiCtx roi_ctx;

    // resources
    size_t header_size;
    size_t frame_size;
    size_t mdinfo_size;
    /* NOTE: packet buffer may overflow */
    size_t packet_size;

    RK_U32 osd_enable;
    RK_U32 osd_mode;
    RK_U32 split_mode;
    RK_U32 split_arg;
    RK_U32 split_out;

    RK_U32 user_data_enable;
    RK_U32 roi_enable;

    // rate control runtime parameter
    RK_S32 fps_in_flex;
    RK_S32 fps_in_den;
    RK_S32 fps_in_num;
    RK_S32 fps_out_flex;
    RK_S32 fps_out_den;
    RK_S32 fps_out_num;
    RK_S32 bps;
    RK_S32 bps_max;
    RK_S32 bps_min;
    RK_S32 rc_mode;
    RK_S32 gop_mode;
    RK_S32 gop_len;
    RK_S32 vi_len;
    RK_S32 scene_mode;
    RK_S32 cu_qp_delta_depth;
    RK_S32 anti_flicker_str;
    RK_S32 atr_str_i;
    RK_S32 atr_str_p;
    RK_S32 atl_str;
    RK_S32 sao_str_i;
    RK_S32 sao_str_p;
    RK_S64 first_frm;
    RK_S64 first_pkt;
} MpiEncTestData;

/* For each instance thread return value */
typedef struct {
    float frame_rate;
    RK_U64 bit_rate;
    RK_S64 elapsed_time;
    RK_S32 frame_count;
    RK_S64 stream_size;
    RK_S64 delay;
} MpiEncMultiCtxRet;

typedef struct {
    MpiEncTestArgs *cmd;       // pointer to global command line info
    const char *name;
    RK_S32 chn;

    pthread_t thd;        // thread for for each instance
    MpiEncTestData ctx;        // context of encoder
    MpiEncMultiCtxRet ret;        // return of encoder
} MpiEncMultiCtxInfo;

static RK_S32 aq_thd[16] = {
        0, 0, 0, 0,
        3, 3, 5, 5,
        8, 8, 8, 15,
        15, 20, 25, 25
};

static RK_S32 aq_step_i_ipc[16] = {
        -8, -7, -6, -5,
        -4, -3, -2, -1,
        0, 1, 2, 3,
        5, 7, 7, 8,
};

static RK_S32 aq_step_p_ipc[16] = {
        -8, -7, -6, -5,
        -4, -2, -1, -1,
        0, 2, 3, 4,
        6, 8, 9, 10,
};

class mpi_enc {
public:
    RK_S32 get_mdinfo_size(MpiEncTestData *p, MppCodingType type);

    MPP_RET test_ctx_init(MpiEncMultiCtxInfo *info);

    MPP_RET test_ctx_deinit(MpiEncTestData *p);

    MPP_RET test_mpp_enc_cfg_setup(MpiEncMultiCtxInfo *info);

    MPP_RET test_mpp_run(MpiEncMultiCtxInfo *info);

    void *enc_test(void *arg);

    int enc_test_multi(MpiEncTestArgs *cmd, const char *name);

    void start(char *file_input, char *file_output, char *rtsp_output, RK_S32 width, RK_S32 height);

private:
    char *file_input;
    char *file_output;
    char *rtsp_output;
    RK_S32 width;
    RK_S32 height;
#ifdef RTSP_OUTPUT
    ffmpeg_utils m_ffmpeg_utils;
#endif
};


#endif //MPI_ENC_MPP_ENC_H
