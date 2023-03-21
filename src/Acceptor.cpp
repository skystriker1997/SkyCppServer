#include "Acceptor.h"



Acceptor::Acceptor(EventLoop* eloop, uint16_t port) : eloop_(eloop) {
    	char ip[] = "127.0.0.1";
    	auto addr = std::make_unique<InetAddress>(ip, port);
    	sock_ = std::make_unique<Socket>(std::move(addr));
    	sock_->SetNonBlocking();
    	sock_->Bind();
    	sock_->Listen();
    	accept_channel_ = std::make_unique<Channel>(eloop, sock_->GetFd());
    	accept_channel_->SetReadCallback([this](){this->AcceptConnection();});
    	accept_channel_->EnableRead();
    	// Note: enable read will make epoll automatically add this newly created channel onto its RB tree, if it's not mounted yet
}


Acceptor::~Acceptor() = default;



void Acceptor::AcceptConnection() {
    	auto clnt_sock = std::make_unique<Socket>();
    	int fd = sock_->Accept(clnt_sock->GetAddr());
		if(fd == -1)
			   return;	
		clnt_sock->SetFd(fd);
    	char new_client_info[100];
    	sprintf(new_client_info, "accept new client fd.%d, with IP %s, port %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_sock->GetAddr()->GetAddr()->sin_addr), ntohs(clnt_sock->GetAddr()->GetAddr()->sin_port));
    	logger_.DEBUG(new_client_info);
    	clnt_sock->SetNonBlocking();
    	new_connection_callback_(std::move(clnt_sock));
}


