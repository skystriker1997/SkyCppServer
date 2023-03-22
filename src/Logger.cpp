#include "Logger.h"



Logger::Logger(): level_(debug), target_(file_and_terminal), path_("../log/logger.txt") {}



std::string Logger::CurrentDateTime() {
    time_t now = time(nullptr);
    tm tmstruct = *localtime(&now);
    char buf[80];
    if(strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tmstruct) == 0)
        perror("failed to format datetime: ");
    return buf;
}



void Logger::Output(const char* text, log_level actual_level) {
    std::string prefix;
    switch(actual_level) {
        case debug: prefix = "[DEBUG] "; break;
        case info: prefix = "[INFO] "; break;
        case warning: prefix = "[WARNING] "; break;
        case error: prefix = "[ERROR] ";
    }
    std::string output_content = prefix + CurrentDateTime() + " : " + text + "\n";
    if(level_ <= actual_level && target_ != file) {
        printf("%s", output_content.c_str());
    }
    if(target_ != terminal) {
        {
            int fd = open(this->path_.c_str(), O_WRONLY|O_APPEND);
            if(fd == -1) {
                perror("failed to open log file: ");
                return;
            }
            FileLock file_lock(fd, F_SETLKW, F_WRLCK, SEEK_SET, 0, 0);
            if(file_lock.ret_ == -1)
                return;
            const char* buf = output_content.c_str();
            size_t nbytes = strlen(buf);
            ssize_t bytes_written = write(fd, buf, nbytes);
            if(bytes_written == -1) {
                char message[] = "failed to write message into log file, which says ";
                perror(std::strcat(message, buf));
            }
        }  
    }
}



void Logger::DEBUG(const char* text){
    this->Output(text, debug);
}



void Logger::INFO(const char* text){
    this->Output(text, info);
}



void Logger::WARNING(const char* text){
    this->Output(text, warning);
}



void Logger::ERROR(const char* text){
    this->Output(text, error);
}
