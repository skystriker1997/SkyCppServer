
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


#ifndef TINYWEBSERVER_SERVER_H
#define TINYWEBSERVER_SERVER_H


class Server
{
private:
    EventLoop* main_reactor_;
    std::unique_ptr<Acceptor> acceptor_;
    std::map<int, std::unique_ptr<Connection>> connections_;
    std::vector<std::shared_ptr<EventLoop>> sub_reactors_;
    std::unique_ptr<ThreadPool> thread_pool_;
    std::function<void(Connection*)> on_receive_callback_;
    Logger logger_;

public:
    DISALLOW_COPY_AND_MOVE(Server)
    explicit Server(EventLoop* main_eloop);
    ~Server();
    void NewConnection(Socket* sock);
    void DeleteConnection(Socket* sock);
    void SetOnReceiveCallback(const std::function<void(Connection*)>& callback);
};


#endif //TINYWEBSERVER_SERVER_H
