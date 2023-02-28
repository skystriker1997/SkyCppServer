
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "FileLock.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


#ifndef MYNETWORKLIB_LOGGER_H
#define MYNETWORKLIB_LOGGER_H


//  The strerror() function returns a pointer to an error message string that is associated with errno

//logger should be thread safe


std::string CurrentDateTime() {
    time_t now = time(0);
    tm tmstruct = *localtime(&now);
    char buf[80];
    if(strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tmstruct) == 0)
        perror("failed to format datetime: ");
    return buf;
}


class Logger{

public:
    enum log_level{debug, info, warning, error};
    enum log_target{file, terminal, file_and_terminal};


private:
    std::ofstream outfile_;    //This data type represents the output file stream and is used to create files and to write information to files.
    std::string path_;              // 日志文件路径
    log_level level_;          // 日志等级
    log_target target_;
    void Output(const char* text, log_level actual_level);            // 输出行为

public:

  //  Logger();  // 默认构造函数
    Logger();
    Logger(log_level level, std::string path, log_target target);
    void DEBUG(const char* text);
    void INFO(const char* text);
    void WARNING(const char* text);
    void ERROR(const char* text);

};


#endif //MYNETWORKLIB_LOGGER_H
