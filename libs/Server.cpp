
#include "Server.h"


// the Server builds up the main frame, including an acceptor receiving messages from clients, a thread pool undertaking that many epolls handling existing connections

Server::Server(EventLoop* main_eloop) : main_reactor_(main_eloop) {
    acceptor_ = std::make_unique<Acceptor>(main_reactor_);
    acceptor_->SetNewConnectionCallback([this](Socket* sock){this->NewConnection(sock);});   //tell Acceptor how to handle new connection
    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = std::make_unique<ThreadPool>(size);
    for(int i = 0; i < size; ++i) {
        auto sub_reactor = std::make_unique<EventLoop>();
        if(sub_reactor->GetEpfd() != -1) {
            sub_reactors_.push_back(std::move(sub_reactor));
        }
    }
    unsigned long length = sub_reactors_.size();
    for(int i = 0; i < length; ++i) {        //one event-loop(sub-reactor) per thread
        std::function<void()> sub_eloop = [sub_reactor = sub_reactors_[i]]()-> void {sub_reactor->Loop();};  // 'this' pointer of EventLoop
        thread_pool_->AddTask(sub_eloop);
    }
}


Server::~Server() {}


void Server::NewConnection(Socket* sock) {
    if(sock->GetFd() == -1) {
        char message[] = "must accept a valid socket to create Connection object";
        logger_.ERROR(message);
        return;
    }
    // here should find the epoll that has the least fd mounted on it
    if(!sub_reactors_.empty()) {
        unsigned long index = 0;
        unsigned long minimal_fd = sub_reactors_[0]->GetFdCount();
        for(unsigned long i=0; i<sub_reactors_.size(); i++) {
            if(sub_reactors_[i]->GetFdCount() < minimal_fd)
                index = i;
        }
        auto connection = std::make_unique<Connection>(sub_reactors_[index].get(), sock);
        connection->SetDeleteConnectionCallback([this](Socket* sock){this->DeleteConnection(sock);});  //tell new connections some necessary steps when close themselves
        connection->SetOnReceiveCallback(on_receive_callback_);    //tell connections how to handle receiving messages
        connections_[sock->GetFd()] = std::move(connection);
    } else {
        char message[] = "no epoll available for connection to mount on";
        logger_.ERROR(message);
    }
}

void Server::DeleteConnection(Socket *sock) {  // the server serves connections by maintaining a map of connections, being responsible for removing connections when it reaches end of life
    int sockfd = sock->GetFd();
    auto iterator = connections_.find(sockfd);   // the find method of std::map returns an iterator
    if(iterator != connections_.end()) {
        connections_.erase(sockfd);   //erase action could trigger deconstruction of that particular connection
    }
}

void Server::SetOnReceiveCallback(const std::function<void(Connection*)>& callback) {on_receive_callback_ = callback;}
// this callback function will finally be passed to and tell channel how to handle read event; epoll is the one that realizes whether messages have arrived, and channel needs to call connection's member functions(with 'this' pointer) to handle events





