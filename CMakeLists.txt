#https://github.com/rockchip-linux/mpp/tree/1.0.8/test
#tail -f /var/log/syslog
cmake_minimum_required(VERSION 3.18)
project(mpi_enc)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        inc
        inc/base
        inc/mpp
        inc/osal
        utils)

link_directories(
        lib
        /root/usr/lib)

add_library(utils STATIC
        utils/mpp_enc_roi_utils.c
        utils/mpi_enc_utils.c
        utils/mpi_dec_utils.c
        utils/mpp_opt.c
        utils/utils.c
        utils/iniparser.c
        utils/dictionary.c
        utils/camera_source.c)

add_executable(mpi_enc main.cpp mpi_enc.cpp)

target_link_libraries(
        mpi_enc
        utils
        -lrockchip_mpp
        -lpthread)
