#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include "Macros.h"

#ifndef SKY_FILELOCK_H
#define SKY_FILELOCK_H


class FileLock {
private:
    struct flock flck_{};
    int fd_;
    int if_wait_;

public:
    int ret_ = -1;
    DISALLOW_COPY_AND_MOVE(FileLock);
    FileLock(int fd, int if_wait , short l_type, short l_whence, int l_offset, int l_len);
    ~FileLock();
};


#endif
