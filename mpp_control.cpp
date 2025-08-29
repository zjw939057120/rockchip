#include <iostream>
#include "tools.h"
#include <thread>
#include <climits>

#define MEDIA_SERVER_PATH "/root/release/MediaServer"
#define MEDIA_SERVER_START_FLAG "/tmp/MediaServerStartFlag"
#define MEDIA_SERVER_RESTART_FLAG "/tmp/MediaServerRestartFlag"
#define MEDIA_SERVER_STOP_FLAG "/tmp/MediaServerStopFlag"

#define MEDIA_CLIENT_PATH "/root/release/MediaClient"
#define MEDIA_CLIENT_START_FLAG  "/tmp/MediaClientStartFlag"
#define MEDIA_CLIENT_RESTART_FLAG  "/tmp/MediaClientRestartFlag"
#define MEDIA_CLIENT_STOP_FLAG  "/tmp/MediaClientStopFlag"


int main() {
    auto lambda = []() {
        char cmd[PATH_MAX];
        //服务端进程控制
        if (tools::fileExists(MEDIA_SERVER_START_FLAG)) {
            sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_SERVER_PATH);
            std::system(cmd);
            std::remove(MEDIA_SERVER_START_FLAG);
        }
        if (tools::fileExists(MEDIA_SERVER_STOP_FLAG)) {
            sprintf(cmd, "killall %s", MEDIA_SERVER_PATH);
            std::system(cmd);
            std::remove(MEDIA_SERVER_STOP_FLAG);
        }
        if (tools::fileExists(MEDIA_SERVER_RESTART_FLAG)) {
            sprintf(cmd, "killall %s", MEDIA_SERVER_PATH);
            std::system(cmd);
            sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_SERVER_PATH);
            std::system(cmd);
            std::remove(MEDIA_SERVER_RESTART_FLAG);
        }
        //客户端进程控制
        if (tools::fileExists(MEDIA_CLIENT_START_FLAG)) {
            sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_CLIENT_PATH);
            std::system(cmd);
            std::remove(MEDIA_CLIENT_START_FLAG);
        }
        if (tools::fileExists(MEDIA_CLIENT_STOP_FLAG)) {
            sprintf(cmd, "killall %s", MEDIA_CLIENT_PATH);
            std::system(cmd);
            std::remove(MEDIA_CLIENT_STOP_FLAG);
        }
        if (tools::fileExists(MEDIA_CLIENT_RESTART_FLAG)) {
            sprintf(cmd, "killall %s", MEDIA_CLIENT_PATH);
            std::system(cmd);
            sprintf(cmd, "nohup %s -d > /dev/null 2>&1 &", MEDIA_CLIENT_PATH);
            std::system(cmd);
            std::remove(MEDIA_CLIENT_RESTART_FLAG);
        }
    };

    while (true) {
        lambda();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return 0;
}

