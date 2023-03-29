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
#include "Macros.h"



#ifndef SKY_LOGGER_H
#define SKY_LOGGER_H



class Logger {
public:
    enum log_level{debug, info, warning, error};
    enum log_target{file, terminal, file_and_terminal};

private:
    std::ofstream outfile_;    
    std::string path_;            
    log_level level_;          // Note: hierarchy of message
    log_target target_;
    void Output(const char* text, log_level actual_level);

public:
    DISALLOW_COPY_AND_MOVE(Logger);
    Logger(log_level level, log_target target, const char* path);
    ~Logger();
    static std::string CurrentDateTime();
    void DEBUG(const char* text);
    void INFO(const char* text);
    void WARNING(const char* text);
    void ERROR(const char* text);
};


#endif
