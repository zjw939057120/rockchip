//
// Created by zjw93 on 2025/8/30.
//

#ifndef MPI_ENC_MPP_CONTROL_H
#define MPI_ENC_MPP_CONTROL_H


#include <string>

#define MEDIA_SERVER_PATH "/root/release/MediaServer"
#define MEDIA_CLIENT_PATH "/root/release/MediaClient"

class mpp_control {
public:
    static void init();

    static void startProc(const char *progress);

    static void stopProc(const char *progress);

    static void restartProc(const char *progress);
};


#endif //MPI_ENC_MPP_CONTROL_H
