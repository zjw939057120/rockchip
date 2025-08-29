//
// Created by Administrator on 2025/8/29.
//

#ifndef MPI_ENC_TOOLS_H
#define MPI_ENC_TOOLS_H


#include <string>

class tools {
public:
    static bool isProgramRunning(const std::string &programName);

    static bool fileExists(const std::string &filename);
};


#endif //MPI_ENC_TOOLS_H
