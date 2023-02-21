
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "Macros.h"


#ifndef MYNETWORKLIB_FILELOCK_H
#define MYNETWORKLIB_FILELOCK_H


class FileLock {
private:
    struct flock flck_;
    int fd_;
    int if_wait_;
public:
    int ret_ = -1;
    DISALLOW_COPY_AND_MOVE(FileLock);
    FileLock(int fd, int if_wait , int l_type, int l_whence, int l_offset, int l_len);
    ~FileLock();
};


#endif //MYNETWORKLIB_FILELOCK_H
