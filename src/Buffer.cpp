#include "Buffer.h"



Buffer::Buffer() = default;

Buffer::~Buffer() = default;;



void Buffer::Append(const char* buffer, long size) {
    	buffer_.append(buffer, size);
}



unsigned long  Buffer::Size() {
    	return buffer_.size();
}



const char* Buffer::ToCstr() {
    	return buffer_.c_str();
}



void Buffer::Clear() {
    	buffer_.clear();
}



void Buffer::SetBuf(std::string&& temp_buf) {
    	buffer_ = temp_buf;
}



void Buffer::Erase(unsigned long nrm) {
    	buffer_.erase(0, nrm);
}






