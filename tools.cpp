//
// Created by Administrator on 2025/8/29.
//

#include "tools.h"

bool tools::isProgramRunning(const std::string &programName) {
    std::string command = "pidof " + programName + " > /dev/null 2>&1";
    int result = system(command.c_str());
    return result == 0; // 如果返回 0，表示程序正在运行
}
