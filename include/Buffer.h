#include <string>
#include <memory>

#ifndef SKYSERVER_BUFFER_H
#define SKYSERVER_BUFFER_H


class Buffer {
private:
    	std::string buffer_;

public:
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
