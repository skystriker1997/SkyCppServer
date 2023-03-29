#include <string>
#include <memory>
#include "Macros.h"

#ifndef SKY_BUFFER_H
#define SKY_BUFFER_H


class Buffer {
private:
    std::string buffer_;

public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer();
    ~Buffer();

    void Append(const char* str, long size);
    unsigned long Size();
    const char* ToCstr();
    void Clear();
    void SetBuf(std::string&& temp_buf);
    void Erase(unsigned long nrm);
};


#endif
