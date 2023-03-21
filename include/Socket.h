#include "Macros.h"
#include <arpa/inet.h>
#include <cstring>
#include "InetAddress.h"
#include <cerrno>
#include <fcntl.h>
#include <sys/socket.h>
#include <string>
#include "Logger.h"
#include <memory>


#ifndef SKYSERVER_SOCKET_H
#define SKYSERVER_SOCKET_H


class Socket {
private:
    	int fd_{};
    	Logger logger_;
    	std::unique_ptr<InetAddress> addr_;

public:
    	DISALLOW_COPY_AND_MOVE(Socket);
    	explicit Socket(std::unique_ptr<InetAddress> addr);
    	Socket();
    	~Socket();
    	int GetFd() const;
    	void SetNonBlocking();
    	void Bind();
    	void Listen();
    	bool CheckNonBlocking() const;
    	int Accept(InetAddress* addr);
    	InetAddress* GetAddr();
};


#endif
