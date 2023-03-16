#include "InetAddress.h"



InetAddress::InetAddress(const char* ip, uint16_t port) {
    memset(addr_.get(), 0, sizeof(*addr_));
    addr_->sin_family = AF_INET;
    addr_->sin_addr.s_addr = inet_addr(ip);
    addr_->sin_port = htons(port);
}



InetAddress::InetAddress() = default;



InetAddress::~InetAddress() = default;



sockaddr_in* InetAddress::GetAddr() {
    return addr_.get();
}



const char* InetAddress::GetIp() {
    return inet_ntoa(addr_->sin_addr);
}



uint16_t InetAddress::GetPort() {
    return ntohs(addr_->sin_port);
}
