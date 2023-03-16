#include "Socket.h"



Socket::Socket(std::unique_ptr<InetAddress> addr) : addr_(std::move(addr)) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1) {
        char message[] = "failed to create socket, error info: ";
        logger_.ERROR(std::strcat(message, strerror(errno)));
    }
}



Socket::Socket() {
    addr_ = std::make_unique<InetAddress>();
}



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



void Socket::Bind(){
    if (bind(fd_, (sockaddr*) addr_->GetAddr(), sizeof(*addr_->GetAddr())) == -1) {
        logger_.ERROR(strerror(errno));
    };
}



void Socket::Listen(){
    if (listen(fd_, SOMAXCONN) == -1) {
        logger_.ERROR(strerror(errno));
    }
}



int Socket::Accept(InetAddress* addr) {
    socklen_t addr_len = sizeof(*addr->GetAddr());
    int clnt_sockfd = accept(fd_, (sockaddr*) addr->GetAddr(), &addr_len);
    if(clnt_sockfd == -1) {
        char message[] = "an exception happened when the server socket tried to accept a client, error info: ";
        logger_.ERROR(std::strcat(message, strerror(errno)));
    }
    return clnt_sockfd;
}



int Socket::GetFd() const{
    return fd_;
}


InetAddress* Socket::GetAddr() {
    return addr_.get();
}