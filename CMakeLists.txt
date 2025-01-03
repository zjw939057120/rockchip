cmake_minimum_required(VERSION 3.18)
project(mpp_test_1.0.7)
#https://github.com/rockchip-linux/mpp/tree/1.0.7/test
#librga_1.10.0_[9].tar.gz
#tail -f /var/log/syslog

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

include_directories(inc inc/base inc/osal inc/utils inc/stb inc/freetype inc/librga librga_utils/inc)
link_directories(lib)
link_libraries(-lrockchip_mpp -lmpp_base -lfreetype -lpthread -lrga -ldl)

add_executable(main_test main.c freetype_test.c)
target_link_libraries(main_test
        -lm)

add_executable(tinyalsa
        tinycap.c
        mixer.c
        mixer_hw.c
        mixer_plugin.c
        pcm.c
        pcm_hw.c
        pcm_plugin.c
        snd_utils.c
        )
target_link_libraries(tinyalsa fdk-aac dl)

add_executable(aac-enc
        aac-enc.c
        )
target_link_libraries(aac-enc fdk-aac dl)

# mpp info test
#add_executable(mpp_inf_testo mpp_info_test.c)

# mpi decoder unit test
#add_executable(mpi_dec_test mpi_dec_test.c)

# mpi decoder multi-thread input / output unit test
#add_executable(mpi_dec_mt_test mpi_dec_mt_test.c)

# mpi decoder no-thread input / output unit test
#add_executable(mpi_dec_nt_test mpi_dec_nt_test.c)

# mpi encoder unit test
add_executable(mpi_enc_test mpi_enc_test.c
        mpi_enc_test.c
        #utils
        mpp_enc_roi_utils.c
        mpi_enc_utils.c
        mpi_dec_utils.c
        mpp_opt.c
        mpp_utils.c
        iniparser.c
        dictionary.c
        camera_source.c
        #freetype
        freetype_test.c
        #librga
        rga_resize_demo.cpp
        librga_utils/src/dma_alloc.cpp
        librga_utils/src/drm_alloc.cpp
        librga_utils/src/librga_utils.cpp
        )
target_link_libraries(mpi_enc_test
        -lm)

# mpi encoder multi-thread input / output unit test
#add_executable(mpi_enc_mt_test mpi_enc_mt_test.cpp
#        #utils
#        mpp_enc_roi_utils.c
#        mpi_enc_utils.c
#        mpi_dec_utils.c
#        mpp_opt.c
#        mpp_utils.c
#        iniparser.c
#        dictionary.c
#        camera_source.c
#        )

# new mpi rc unit test
#add_executable(mpi_rc2_test mpi_rc2_test.c)

# new dec multi unit test
#add_executable(mpi_dec_multi_test mpi_dec_multi_test.c)
