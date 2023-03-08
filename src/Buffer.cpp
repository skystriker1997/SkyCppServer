#include "Buffer.h"



Buffer::Buffer() = default;;

Buffer::~Buffer() = default;;



void Buffer::Append(const char* buf, long size) {
    buf_.append(buf, size);
}



unsigned long  Buffer::Size() {
    return buf_.size();
}



const char* Buffer::ToCstr() {
    return buf_.c_str();
}



void Buffer::Clear() {
    buf_.clear();
}



void Buffer::SetBuf(const char* content) {
    buf_.clear();
    buf_.append(content);
}



void Buffer::Erase(unsigned long npos) {
    buf_.erase(buf_.size()-npos, npos);
}

