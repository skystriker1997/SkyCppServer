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
    std::unique_ptr<Socket> sock_;
    std::unique_ptr<Channel> channel_;
    State state_;
    std::unique_ptr<Buffer> read_buffer_;
    std::unique_ptr<Buffer> write_buffer_;
    std::function<void(Connection*)> on_receive_callback_;
    std::function<void(Socket*)> delete_self_on_server_callback_;
    Logger logger_;

    void ReadNonBlocking();
    void WriteNonBlocking();

public:
    DISALLOW_COPY_AND_MOVE(Connection);
    Connection(EventLoop* eloop, std::unique_ptr<Socket> sock);
    ~Connection();

    void Read();
    void Write();

    void SetChannelWriteCallback();

    void SetChannelReadCallback();

    void SetOnReceiveCallback(const std::function<void(Connection *)>& callback);

    template<typename F>
    void SetDeleteSelfOnServerCallback(F&& callback) {
        delete_self_on_server_callback_ = callback;
    };

    void DeleteSelf(Socket* sock);


    Buffer* GetReadBuffer();
    Buffer* GetSendBuffer();

    void EnableChannelWrite();
    void EnableChannelRead();

    Socket* GetSocket() const;

};

#endif
