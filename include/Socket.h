

#include "Macros.h"
#include <arpa/inet.h>
#include <string.h>
#include "InetAddress.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Logger.h"
#include <memory>


#ifndef SKYSERVER_SOCKET_H
#define SKYSERVER_SOCKET_H


class Socket {
private:
    int fd_;
    Logger logger_;
public:
    DISALLOW_COPY_AND_MOVE(Socket);
    Socket();
    Socket(int fd);
    ~Socket();

    int GetFd();
    void SetNonBlocking();
    void Bind(InetAddress* addr);
    void Listen();
    bool CheckNonBlocking() const;
    int Accept(InetAddress* addr);

};


#endif