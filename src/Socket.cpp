#include "Socket.h"



Socket::Socket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1) {
        char message[] = "failed to create socket, error info: ";
        logger_.ERROR(std::strcat(message, strerror(errno)));
    }
}



Socket::Socket(int fd) : fd_(fd) {}



Socket::~Socket(){
    if(fd_ != -1){
        close(fd_);
        fd_ = -1;
    }
}



void Socket::SetNonBlocking() {
    if(fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK) == -1) {
        logger_.ERROR(strerror(errno));
    };
}



bool Socket::CheckNonBlocking() const {
    return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0;
}



void Socket::Bind(InetAddress* addr){
    struct sockaddr_in temp_addr = addr->GetAddr();
    if (bind(fd_, (sockaddr*) &temp_addr, sizeof(temp_addr)) == -1) {
        logger_.ERROR(strerror(errno));
    };
}



void Socket::Listen(){
    if (listen(fd_, SOMAXCONN) == -1) {
        logger_.ERROR(strerror(errno));
    }
}



int Socket::Accept(InetAddress* addr) {
    struct sockaddr_in temp_addr {};
    socklen_t addr_len = sizeof(temp_addr);
    int clnt_sockfd = accept(fd_, (sockaddr*) &temp_addr, &addr_len);
    if(clnt_sockfd == -1) {
        char message[] = "an exception happened when the server socket tried to accept a client, error info: ";
        logger_.ERROR(std::strcat(message, strerror(errno)));
    } else {
        addr->SetAddr(temp_addr);
    }
    return clnt_sockfd;
}



int Socket::GetFd() const{
    return fd_;
}