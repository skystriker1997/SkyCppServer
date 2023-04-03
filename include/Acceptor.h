#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <memory>
#include "Logger.h"
#include <cstdio>
#include "Macros.h"



#ifndef SKY_ACCEPTOR_H
#define SKY_ACCEPTOR_H


class EventLoop;
class Socket;
class Channel;

class Acceptor {
private:
    EventLoop* eloop_;
    std::unique_ptr<Socket> sock_;
    std::unique_ptr<Channel> accept_channel_;
    std::function<void(std::unique_ptr<Socket>)> new_connection_callback_;
    Logger logger_;

public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    Acceptor(EventLoop* eloop, uint16_t port);
    ~Acceptor();

    void AcceptConnection();

    template<typename F>
    void SetNewConnectionCallback(F&& callback) {
        new_connection_callback_ = callback;
    };
};


#endif
