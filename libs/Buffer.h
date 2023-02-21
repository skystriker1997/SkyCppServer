
#include <string>

#ifndef TINYWEBSERVER_BUFFER_H
#define TINYWEBSERVER_BUFFER_H

class Buffer {
private:
    std::string buf_;
public:
    Buffer();
    ~Buffer();
    void Append(const char* str, int size);
    ssize_t Size();
    const char* ToCstr();
    void Clear();
    void GetLine();
    void SetBuf(const char* content);
};


#endif //TINYWEBSERVER_BUFFER_H
