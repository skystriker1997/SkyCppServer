#include <arpa/inet.h>
#include "Macros.h"
#include <cstring>
#include <memory>

#ifndef SKY_INETADDRESS_H
#define SKY_INETADDRESS_H


class InetAddress {
private:
    	std::unique_ptr<struct sockaddr_in> addr_;

public:
    	DISALLOW_COPY_AND_MOVE(InetAddress);
    	InetAddress();
    	InetAddress(const char* ip, uint16_t port);
    	~InetAddress();

    	sockaddr_in* GetAddr();
    	const char* GetIp();
    	uint16_t GetPort();
};

#endif
