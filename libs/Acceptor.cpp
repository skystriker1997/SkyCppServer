
#include "Acceptor.h"


Acceptor::Acceptor(EventLoop* eloop) : eloop_(eloop) {
    sock_ = std::make_unique<Socket>();   //the socket of Acceptor had better to be set blocking
    auto addr = std::make_unique<InetAddress>("127.0.0.1", 1234);
    sock_->Bind(addr.get());
    sock_->Listen();
    accept_channel_ = std::make_unique<Channel>(eloop, sock_->GetFd());
    accept_channel_->SetReadCallback([this](){this->AcceptConnection();});
    accept_channel_->EnableRead();
}

Acceptor::~Acceptor() {}


void Acceptor::AcceptConnection() {
    auto clnt_addr = std::make_unique<InetAddress>();
    auto clnt_sock = std::make_unique<Socket>(sock_->Accept(clnt_addr.get()));
    printf("new client fd: %d, with IP at: %d, with port at: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr), ntohs(clnt_addr->GetAddr().sin_port);
    clnt_sock->SetNonBlocking();
    new_connection_callback_(clnt_sock.get());
}


