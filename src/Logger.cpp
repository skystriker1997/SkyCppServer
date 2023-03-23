#include "Logger.h"



Logger::Logger(log_level level, log_target target, const char* path): level_(level), target_(target), path_(path) {}

Logger::~Logger() = default;


std::string Logger::CurrentDateTime() {
    time_t now = time(nullptr);
    tm tmstruct = *localtime(&now);
    char buf[80];
    if(strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tmstruct) == 0) {
        perror("failed to format datetime for log: ");
	}
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
            std::string message;				
            int fd = open(this->path_.c_str(), O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
            if(fd == -1) {	
                message = message + "failed to open log file: " + path_ + ", error info: " + strerror(errno);
                perror(message.c_str());
                return;
            }
            FileLock file_lock(fd, F_SETLKW, F_WRLCK, SEEK_SET, 0, 0);
            if(file_lock.ret_ == -1)
                return;
            const char* buf = output_content.c_str();
            size_t nbytes = strlen(buf);
            ssize_t bytes_written = write(fd, buf, nbytes);
            if(bytes_written == -1) {
                message = message + "failed to write message into " + path_ + ", error info: " + strerror(errno);
                perror(message.c_str());
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
