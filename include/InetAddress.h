#include <arpa/inet.h>
#include "Macros.h"
#include <cstring>



#ifndef SKYSERVER_INETADDRESS_H
#define SKYSERVER_INETADDRESS_H


class InetAddress {
private:
    struct sockaddr_in addr_ {};
public:
    DISALLOW_COPY_AND_MOVE(InetAddress);
    InetAddress() = default;
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress() = default;

    void SetAddr(sockaddr_in addr);
    sockaddr_in GetAddr();
    const char* GetIp();
    uint16_t GetPort();
};

#endif