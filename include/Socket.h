#include "Macros.h"
#include <arpa/inet.h>
#include <cstring>
#include "InetAddress.h"
#include <cerrno>
#include <fcntl.h>
#include <sys/socket.h>
#include "Logger.h"


#ifndef SKYSERVER_SOCKET_H
#define SKYSERVER_SOCKET_H


class Socket {
private:
    int fd_;
    Logger logger_;

public:
    DISALLOW_COPY_AND_MOVE(Socket);
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int GetFd() const;
    void SetNonBlocking();
    void Bind(InetAddress* addr);
    void Listen();
    bool CheckNonBlocking() const;
    int Accept(InetAddress* addr);
};


#endif