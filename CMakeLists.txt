#https://github.com/rockchip-linux/mpp/tree/1.0.8/test
#tail -f /var/log/syslog
cmake_minimum_required(VERSION 3.19)
project(mpi_enc)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        inc
        inc/mpp
        inc/osal
        inc/utils)

link_directories(lib)

add_executable(mpi_enc mpi_enc.cpp)
target_link_libraries(
        mpi_enc
        -lrockchip_mpp
        -lutils)