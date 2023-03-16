#include "TcpServer.h"



// Note: the Server builds up the main frame, including an acceptor receiving messages from clients, a thread pool undertaking that many epolls handling existing connections
TcpServer::TcpServer(uint16_t port) : port_(port) {
    main_reactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get(), port_);
    acceptor_->SetNewConnectionCallback([this](std::unique_ptr<Socket> sock){this->NewConnection(std::move(sock));});
    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = std::make_unique<ThreadPool>(size);
    for(int i = 0; i < size; ++i) {
        auto sub_reactor = std::make_unique<EventLoop>();
        if(sub_reactor->GetEpfd() != -1) {
            sub_reactors_.push_back(std::move(sub_reactor));  // Note: move constructor implemented; the release() member function of original unique_ptr is called to swap internal pointer
        }
    }
    unsigned long length = sub_reactors_.size();
    for(int i = 0; i < length; ++i) {        // Note: one event-loop(sub-reactor) per thread
        thread_pool_->AddTask([sub_reactor = sub_reactors_[i].get()]()-> void {sub_reactor->Loop();});
    }
}



TcpServer::~TcpServer() = default;



void TcpServer::Start() {
    main_reactor_->Loop();
}



void TcpServer::NewConnection(std::unique_ptr<Socket> sock) {
    if(sock->GetFd() == -1) {
        char message[] = "a socket with fd -1 is not eligible to create Connection object";
        logger_.ERROR(message);
        return;
    }
    // Note: here need to find the epoll that has the least fds mounted on it
    if(!sub_reactors_.empty()) {
        unsigned long index = 0;
        unsigned long minimal_fd = sub_reactors_[0]->FdCount();
        for(unsigned long i=0; i<sub_reactors_.size(); i++) {
            if(sub_reactors_[i]->FdCount() < minimal_fd)
                index = i;
        }
        connections_.PushBack(sub_reactors_[index].get(), std::move(sock));
        Connection* connection = connections_.GetBack();
        connection->SetOnReceiveCallback(on_receive_callback_);
        connection->SetDeleteSelfOnServerCallback([this](Socket* sock){this->DeleteConnection(sock);});
        // Note: tell connections how to handle received data
    } else {
        char message[] = "no epoll is now available for connection to mount on";
        logger_.ERROR(message);
    }
}



// Note: the server maintains a vector of connections, being responsible for removing connections when it reaches end of life
void TcpServer::DeleteConnection(Socket *sock) {
    connections_.DeleteConnection(sock->GetFd());
}









