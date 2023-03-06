
#include <string>

#ifndef SKYSERVER_BUFFER_H
#define SKYSERVER_BUFFER_H

class Buffer {
private:
    std::string buf_;
public:
    Buffer();
    ~Buffer();
    void Append(const char* str, long size);
    ssize_t Size();
    const char* ToCstr();
    void Clear();
    void GetLine();
    void SetBuf(const char* content);
};


#endif //SKYSERVER_BUFFER_H
