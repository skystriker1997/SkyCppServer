#include <map>
#include <functional>
#include <vector>
#include "Macros.h"
#include <memory>
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "ThreadPool.h"
#include "Logger.h"
#include <algorithm>
#include <cstdio>


#ifndef SKYSERVER_SERVER_H
#define SKYSERVER_SERVER_H



struct KV {
    int sock_fd_;
    std::unique_ptr<Connection> connection_;
    KV(EventLoop* eloop, std::unique_ptr<Socket> sock): sock_fd_(sock->GetFd()) {
        connection_ = std::make_unique<Connection>(eloop, std::move(sock));
    }
};



class Connections {
private:
    std::vector<std::unique_ptr<KV>> connections_;

public:
    Connections() = default;
    ~Connections() = default;
    void PushBack(EventLoop* eloop, std::unique_ptr<Socket> sock) {
        connections_.emplace_back(std::make_unique<KV>(eloop, std::move(sock)));
    }

    void DeleteConnection(int sock_fd) {
        for(auto iter = connections_.begin(); iter < connections_.end(); iter++) {
            if((*iter)->sock_fd_ == sock_fd) {
                connections_.erase(iter);
                break;
            }
        }
    }

    void PopHead() {
        if(!connections_.empty()) {
            connections_.erase(connections_.begin());
        }
    }

    Connection* GetBack() {
        return connections_.back()->connection_.get();
    }

    unsigned long GetLength() {
        return connections_.size();
    }
};



class TcpServer {
private:
    uint16_t port_;
    std::unique_ptr<EventLoop> main_reactor_;
    std::unique_ptr<Acceptor> acceptor_;
    Connections connections_;
    std::vector<std::unique_ptr<EventLoop>> sub_reactors_;
    std::unique_ptr<ThreadPool> thread_pool_;
    std::function<void(Connection*)> on_receive_callback_;
    Logger logger_;

public:
    DISALLOW_COPY_AND_MOVE(TcpServer)
    explicit TcpServer(uint16_t port);
    ~TcpServer();
    void NewConnection(std::unique_ptr<Socket> sock);
    void DeleteConnection(Socket* sock);

    void Start();

    template<typename F>
    void SetOnReceiveCallback(F&& callback) {
        on_receive_callback_ = callback;
    };
};


#endif
