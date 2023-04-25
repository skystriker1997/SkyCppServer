#include "TcpServer.h"
#include "Global.h"


TcpServer::TcpServer(uint16_t port) : port_(port), logger_(Logger::log_level::debug, Logger::log_target::file_and_terminal, log_path) {
    std::string message;
    main_reactor_ = std::make_unique<EventLoop>();
    if(main_reactor_->GetEpfd() < 3) {
        message += "get error when trying to creating epoll as main reactor";
        logger_.ERROR(message.c_str());
        std::exit(0);
    } else {
        message = "epollfd." + std::to_string(main_reactor_->GetEpfd()) + " has been created as a main reactor";
        logger_.DEBUG(message.c_str());
    }

    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get(), port_);
    acceptor_->SetNewConnectionCallback([this](std::unique_ptr<Socket> sock){this->NewConnection(std::move(sock));});
    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = std::make_unique<ThreadPool>(size);
    for(int i = 0; i < size; ++i) {
        auto sub_reactor = std::make_unique<EventLoop>();
        if(sub_reactor->GetEpfd() > 2) {
            sub_reactors_.push_back(std::move(sub_reactor));
            message.clear();
            message = message + "epollfd." + std::to_string(sub_reactors_.back()->GetEpfd()) + " has been created as a sub reactor";    
            logger_.DEBUG(message.c_str());
        }
    }
    int length = sub_reactors_.size();
    for(int i = 0; i < length; ++i) {    
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
    if(!sub_reactors_.empty()) {
        unsigned long index = 0;
        long minimal_fd = sub_reactors_[0]->GetFdCount();
        // Note: look for the epoll that has the least fd mounted on it
        for(unsigned long i=0; i<sub_reactors_.size(); i++) {
            std::string message = "epfd.";
            message = message+std::to_string(sub_reactors_[i]->GetEpfd())+" has "+std::to_string(sub_reactors_[i]->GetFdCount())+" sockfds";
            logger_.DEBUG(message.c_str());
            if(sub_reactors_[i]->FdCountLessThan(minimal_fd))
                index = i;
        }
        connections_.PushBack(sub_reactors_[index].get(), std::move(sock));
        Connection* connection = connections_.GetBack();
        connection->SetOnReceiveCallback(on_receive_callback_);
        connection->SetDeleteSelfOnServerCallback([this](Socket* sock){this->DeleteConnection(sock);});
    } else {
        char message[] = "no epoll is available for connection to mount on";
        logger_.ERROR(message);
    }
}



// Note: the server maintains a vector of connections, being responsible for removing connections when it ends lifespan
void TcpServer::DeleteConnection(Socket *sock) {
    connections_.DeleteConnection(sock->GetFd());
}









