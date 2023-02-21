
#include "Buffer.h"
#include <string>
#include <iostream>

Buffer::Buffer() {};

Buffer::~Buffer() {};

void Buffer::Append(const char* str, int size) {
    for(int i = 0; i < size; ++i) {
        if(str[i] == '\0')
            break;
        buf_.push_back(str[i]);
    }
}

ssize_t  Buffer::Size() {
    return buf_.size();
}

const char* Buffer::ToCstr() {
    return buf_.c_str();
}

void Buffer::Clear() {
    buf_.clear();
}

void Buffer::GetLine() {
    buf_.clear();
    std::getline(std::cin, buf_);
}

void Buffer::SetBuf(const char* content) {
    buf_.clear();
    buf_.append(content);
}