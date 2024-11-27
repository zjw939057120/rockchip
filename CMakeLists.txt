cmake_minimum_required(VERSION 3.18)
project(mpp_test_1.0.7)
#https://github.com/rockchip-linux/mpp/tree/1.0.7/test
#tail -f /var/log/syslog

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

include_directories(inc inc/base inc/osal inc/utils inc/librga librga_utils/inc)
link_directories(lib)
link_libraries(-lrockchip_mpp -lmpp_base -lpthread -lrga -ldl)

#add_executable(mpp_test main.cpp)

# mpp info test
#add_executable(mpp_info mpp_info_test.c)

# mpi decoder unit test
#add_executable(mpi_dec mpi_dec_test.c)

# mpi decoder multi-thread input / output unit test
#add_executable(mpi_dec_mt mpi_dec_mt_test.c)

# mpi decoder no-thread input / output unit test
#add_executable(mpi_dec_nt mpi_dec_nt_test.c)

# mpi encoder unit test
add_executable(mpi_enc mpi_enc_test.c
        fifo.c
        #utils
        mpp_enc_roi_utils.c
        mpi_enc_utils.c
        mpi_dec_utils.c
        mpp_opt.c
        mpp_utils.c
        iniparser.c
        dictionary.c
        camera_source.c
        #librga
        rga_resize_demo.cpp
        librga_utils/src/dma_alloc.cpp
        librga_utils/src/drm_alloc.cpp
        librga_utils/src/librga_utils.cpp
        )

# mpi encoder multi-thread input / output unit test
add_executable(mpi_enc_mt mpi_enc_mt_test.cpp
        #utils
        mpp_enc_roi_utils.c
        mpi_enc_utils.c
        mpi_dec_utils.c
        mpp_opt.c
        mpp_utils.c
        iniparser.c
        dictionary.c
        camera_source.c
        )

# new mpi rc unit test
#add_executable(mpi_rc2 mpi_rc2_test.c)

# new dec multi unit test
#add_executable(mpi_dec_multi_test mpi_dec_multi_test.c)