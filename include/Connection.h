#include <functional>
#include <memory>
#include "EventLoop.h"
#include "Socket.h"
#include "Buffer.h"
#include "Logger.h"
#include <queue>


#ifndef SKYSERVER_CONNECTION_H
#define SKYSERVER_CONNECTION_H


class Connection {

public:
    enum State {Available, Closed};
private:
    EventLoop* eloop_;
    Socket* sock_;
    std::unique_ptr<Channel> channel_;
    State state_;
    std::unique_ptr<std::queue<std::unique_ptr<Buffer>>> read_buffers_;
    std::unique_ptr<std::queue<std::unique_ptr<Buffer>>> send_buffers_;
    std::function<void(Connection *)> on_receive_callback_;
    Logger logger_;
    bool already_closed_;

    void ReadNonBlocking();
    void WriteNonBlocking();

public:
    DISALLOW_COPY_AND_MOVE(Connection);
    Connection(EventLoop* eloop, Socket *sock);
    ~Connection();

    void Read();
    void Write();

    void SetChannelWriteCallback();

    void SetChannelReadCallback();

    void SetOnReceiveCallback(const std::function<void(Connection *)>& callback);

    void RemoveFromEpoll();

    State GetState() const;

    std::queue<std::unique_ptr<Buffer>>* GetReadBuffers();
    std::queue<std::unique_ptr<Buffer>>* GetSendBuffers();

    void EnableChannelWrite();
    void EnableChannelRead();

    Socket* GetSocket() const;

};

#endif
