
#include <functional>
#include <memory>
#include "EventLoop.h"
#include "Socket.h"
#include "Buffer.h"
#include "Logger.h"


#ifndef SKYSERVER_CONNECTION_H
#define SKYSERVER_CONNECTION_H


class Connection {

public:
    enum State {Closed, Occupied, Free};
private:
    EventLoop* eloop_;
    Socket* sock_;
    std::unique_ptr<Channel> channel_;
    State state_;
    std::unique_ptr<Buffer> read_buffer_;
    std::unique_ptr<Buffer> send_buffer_;
    long send_buffer_data_left_;
    std::function<void(Connection *)> on_receive_callback_;

    Logger logger_;
    bool already_closed_;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();


public:

    Connection(EventLoop* eloop, Socket *sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();

    void SetChannelWriteCallback();

    void SetChannelReadCallback();

    void SetOnReceiveCallback(const std::function<void(Connection *)>& callback);

    State GetState();
    void RemoveFromEpoll();
    void SetSendBuffer(const char *str);
    Buffer* GetReadBuffer();
    Buffer* GetSendBuffer();
    Socket* GetSocket();
};

#endif
