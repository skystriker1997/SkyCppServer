#include "Acceptor.h"



Acceptor::Acceptor(EventLoop* eloop, uint16_t port) : eloop_(eloop) {
    auto addr = std::make_unique<InetAddress>("127.0.0.1", port);
    sock_ = std::make_unique<Socket>(std::move(addr));
    sock_->SetNonBlocking();
    sock_->Bind();
    sock_->Listen();
    // Listen of socket does not block
    accept_channel_ = std::make_unique<Channel>(eloop, sock_->GetFd());
    accept_channel_->SetReadCallback([this](){this->AcceptConnection();});
    //If read is active for acceptor's socket, connection has come and acceptance is necessary
    accept_channel_->EnableRead();
    // Note: calling EnableRead here will make epoll add this newly created channel onto its RB tree, as it's not mounted yet
}


Acceptor::~Acceptor() = default;



void Acceptor::AcceptConnection() {
    auto clnt_sock = std::make_unique<Socket>();
    sock_->Accept(clnt_sock->GetAddr());
    // Note: as the accept method is called after being ready to read, execution flow won't be blocked
    // printf("new client fd: %d, with IP at: %d, with port at: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr), ntohs(clnt_addr->GetAddr().sin_port);
    clnt_sock->SetNonBlocking();
    new_connection_callback_(std::move(clnt_sock));
}


