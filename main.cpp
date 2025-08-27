#include <iostream>
#include "mpp_enc.h"
#include <thread>
#include <csignal>

int main() {
    //相机线程
    std::thread video44([]() {
        mpi_enc("/dev/video44", "/root/out44.h264", 1280, 720);
    });
    video44.detach();

    //相机线程
    std::thread video53([]() {
        mpi_enc("/dev/video53", "/root/out53.h264", 1280, 720);
    });
    video53.detach();

    //相机线程
    std::thread video62([]() {
        mpi_enc("/dev/video62", "/root/out62.h264", 1280, 720);
    });
    video62.detach();

    //相机线程
    std::thread video71([]() {
        mpi_enc("/dev/video71", "/root/out71.h264", 1280, 720);
    });
    video71.detach();

    while (true) {
        sleep(60);
    }

    return 0;
}
