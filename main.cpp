#include <iostream>
#include "mpp_enc.h"
#include <thread>

int main() {
    //相机线程
    std::thread video10([]() {
        mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video10", "/root/out10.h264", 1280, 720);
    });
    video10.detach();
/*
    //相机线程
    std::thread video44([]() {
        mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video44", "/root/out44.h264", 1280, 720);
    });
    video44.detach();

    //相机线程
    std::thread video53([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video53", "/root/out53.h264", 1280, 720);
    });
    video53.detach();

    //相机线程
    std::thread video62([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video62", "/root/out62.h264", 1280, 720);
    });
    video62.detach();

    //相机线程
    std::thread video71([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video71", "/root/out71.h264", 1280, 720);
    });
    video71.detach();
*/

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3600));
    }

    return 0;
}
