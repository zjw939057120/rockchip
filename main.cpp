#include <iostream>
#include "mpp_enc.h"
#include "tools.h"
#include <thread>

#define DEMO_MODE
#define CAM_MAX_MUN 4
#define MEDIA_SERVER_PATH "/root/release/MediaServer"

struct CONFIG {
    char *file_input;
    char *file_output;
    char *rtsp_output;
    RK_S32 width;
    RK_S32 height;
};

int main() {
    char cmd[PATH_MAX];
    //启动服务端
    if (!tools::isProgramRunning(MEDIA_SERVER_PATH)) {
        sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_SERVER_PATH);
        std::system(cmd);
    }

    //延迟500ms
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CONFIG config[CAM_MAX_MUN];

    uint8_t index = 0;
    config[index].file_input = "/dev/video44";
    config[index].file_output = "/root/video0.h264";
    config[index].rtsp_output = "rtsp://127.0.0.1/live/video0";
    config[index].width = 1280;
    config[index].height = 720;
    index++;
    config[index].file_input = "/dev/video54";
    config[index].file_output = "/root/video1.h264";
    config[index].rtsp_output = "rtsp://127.0.0.1/live/video1";
    config[index].width = 1280;
    config[index].height = 720;
    index++;
    config[index].file_input = "/dev/video62";
    config[index].file_output = "/root/video2.h264";
    config[index].rtsp_output = "rtsp://127.0.0.1/live/video2";
    config[index].width = 1280;
    config[index].height = 720;
    index++;
    config[index].file_input = "/dev/video71";
    config[index].file_output = "/root/video3.h264";
    config[index].rtsp_output = "rtsp://127.0.0.1/live/video3";
    config[index].width = 1280;
    config[index].height = 720;

#ifdef DEMO_MODE
    config[0].file_input = "/dev/video10";
    config[1].file_input = "/dev/video12";
#endif


    //相机线程
    for (int i = 0; i < CAM_MAX_MUN; ++i) {
        std::thread t([&config, i]() {
            mpi_enc m_mpi_enc;
            m_mpi_enc.start(config[i].file_input, config[i].file_output, config[i].rtsp_output, config[i].width,
                            config[i].height);
        });
        t.detach();
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3600));
    }

    return 0;
}
