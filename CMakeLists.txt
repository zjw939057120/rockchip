#https://github.com/rockchip-linux/mpp/tree/1.0.8/test
#tail -f /var/log/syslog
cmake_minimum_required(VERSION 3.18)

if(WIN32)
# 设置目标系统
set(CMAKE_SYSTEM_NAME Linux)
# 设置目标系统的处理器架构
set(CMAKE_SYSTEM_PROCESSOR arm64)
# 设置交叉编译工具链的路径
set(CMAKE_C_COMPILER D:/gcc-linaro-7.5.0-2019.12-i686-mingw32_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc.exe)
set(CMAKE_CXX_COMPILER D:/gcc-linaro-7.5.0-2019.12-i686-mingw32_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++.exe)
# 设置查找库和头文件的路径
set(CMAKE_FIND_ROOT_PATH D:/gcc-linaro-7.5.0-2019.12-i686-mingw32_aarch64-linux-gnu/aarch64-linux-gnu)
endif()

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

add_executable(mpi_enc main.cpp mpi_enc.cpp ffmpeg_utils.cpp tools.cpp)

target_link_libraries(
        mpi_enc
        utils
        -lrockchip_mpp
        -lpthread
        -lavformat -lavcodec -lavutil -lavdevice -lswresample
        -lfdk-aac -lopus)
