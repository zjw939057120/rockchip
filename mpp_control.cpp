//
// Created by zjw93 on 2025/8/30.
//

#include <climits>
#include <iostream>
#include <thread>
#include "mpp_control.h"


void mpp_control::init() {
    std::thread t([]() {
        //延迟10秒启动服务端
        std::this_thread::sleep_for(std::chrono::seconds(10));
        restartProc(MEDIA_SERVER_PATH);
    });
    t.detach();
}

void mpp_control::startProc(const char *progress) {
    std::cout << __func__ << " " << progress << std::endl;

    std::thread t([progress]() {
        char cmd[PATH_MAX];
        sprintf(cmd, "chmod 755 %s", progress);
        std::system(cmd);
        sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", progress);
        std::system(cmd);
    });
    t.detach();
}

void mpp_control::stopProc(const char *progress) {
    std::cout << __func__ << " " << progress << std::endl;

    std::thread t([progress]() {
        char cmd[PATH_MAX];
        sprintf(cmd, "killall -9 %s", progress);
        std::system(cmd);
    });
    t.detach();
}

void mpp_control::restartProc(const char *progress) {
    std::cout << __func__ << " " << progress << std::endl;

    std::thread t([progress]() {
        char cmd[PATH_MAX];
        sprintf(cmd, "killall -9 %s", progress);
        std::system(cmd);
        sprintf(cmd, "chmod 755 %s", progress);
        std::system(cmd);
        sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", progress);
        std::system(cmd);
    });
    t.detach();

}
