#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "FileLock.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>


#ifndef SKYSERVER_LOGGER_H
#define SKYSERVER_LOGGER_H



std::string CurrentDateTime() {
    time_t now = time(nullptr);
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
    std::ofstream outfile_;    //Note: This variable type represents the output file stream and is used to create files and to write information to files.
    std::string path_;              // Note: path to logger file
    log_level level_;          // Note: level of message
    log_target target_;
    void Output(const char* text, log_level actual_level);

public:
    Logger();
    void DEBUG(const char* text);
    void INFO(const char* text);
    void WARNING(const char* text);
    void ERROR(const char* text);

};


#endif
