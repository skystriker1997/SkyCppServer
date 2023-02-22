
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <iostream>
#include <memory>

#ifndef TINYWEBSERVER_ACCEPTOR_H
#define TINYWEBSERVER_ACCEPTOR_H

class EventLoop;
class Socket;
class Channel;

class Acceptor {
private:
    EventLoop *eloop_;
    std::unique_ptr<Socket> sock_;
    std::unique_ptr<Channel> accept_channel_;
    std::function<void(Socket *)> new_connection_callback_;
public:
    Acceptor(EventLoop* eloop);

    ~Acceptor();

    void AcceptConnection();

    template<typename F>
    void SetNewConnectionCallback(F&& callback) {
        new_connection_callback_ = callback;
    };
};

#endif //TINYWEBSERVER_ACCEPTOR_H
