#include <iostream>
#include "mpp_enc.h"
#include "tools.h"
#include <thread>

#define MEDIA_SERVER_PATH "/root/release/MediaServer"

int main() {
    char cmd[PATH_MAX];
    //启动服务端
    if (!tools::isProgramRunning(MEDIA_SERVER_PATH)) {
        sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_SERVER_PATH);
        std::system(cmd);
    }

    //延迟500ms
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //相机1线程
    std::thread video1([]() {
        mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video10", "/root/video1.h264", "rtsp://127.0.0.1/live/video1", 1280, 720);
    });
    video1.detach();

    //相机2线程
    std::thread video2([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video12", "/root/video2.h264", "rtsp://127.0.0.1/live/video2", 1280, 720);
    });
    video2.detach();

    //相机3线程
    std::thread video3([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video62", "/root/video3.h264", "rtsp://127.0.0.1/live/video3", 1280, 720);
    });
    video3.detach();

    //相机4线程
    std::thread video4([]() {
        class mpi_enc m_mpi_enc;
        m_mpi_enc.start("/dev/video71", "/root/video4.h264", "rtsp://127.0.0.1/live/video4", 1280, 720);
    });
    video4.detach();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3600));
    }

    return 0;
}
