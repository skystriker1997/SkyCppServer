#include "FileLock.h"



// Note : if_wait accepts F_SETLK(non-waiting), F_SETLKW(waiting); l_type accepts F_UNLCK, F_RDLCK, F_WRLCK
FileLock::FileLock(int fd, int if_wait , short l_type, short l_whence, int l_offset, int l_len): fd_(fd), if_wait_(if_wait) {
    flck_.l_type = l_type;
    flck_.l_whence = l_whence;
    flck_.l_start = l_offset;
    flck_.l_len = l_len;
    ret_ = fcntl(fd_, if_wait_, &flck_);  // 0 or -1
    if( ret_ == -1 ) {
        perror("failed to lock log file");
    }
}



FileLock::~FileLock() {
    if (ret_ != -1) {
        flck_.l_type = F_UNLCK;
        if(fcntl(fd_, F_SETLKW, &flck_) == -1) {
            perror("failed to unlock log file");
        }
    }
}