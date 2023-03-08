#include "Connection.h"



Connection::Connection(EventLoop *eloop, Socket *sock) : state_(Available), eloop_(eloop), sock_(sock), already_closed_(false) {
    channel_ = std::make_unique<Channel>(eloop_, sock->GetFd());
    this->EnableChannelRead();
    read_buffers_ = std::make_unique<std::queue<std::unique_ptr<Buffer>>>();
    send_buffers_ = std::make_unique<std::queue<std::unique_ptr<Buffer>>>();
    this->SetChannelReadCallback();
    this->SetChannelWriteCallback();
    read_buffers_->push(std::make_unique<Buffer>());
    send_buffers_->push(std::make_unique<Buffer>());
}


Connection::~Connection() {
    if(!already_closed_) {
        eloop_->DeleteChannel(channel_.get());
        //Note: remove the channel from corresponding epoll when gets the end of connection
        already_closed_ = true;
    }
}



void Connection::Read() {
    if(state_ == State::Available)
        this->ReadNonBlocking();

    if(state_ == State::Closed) {
        char message[] = "connection is now closed! Read is no longer accessible.";
        logger_.ERROR(message);
        return;
    }
}



void Connection::Write() {
    if(state_ == State::Available)
        this->WriteNonBlocking();

    if(state_ == State::Closed) {
        char message[] = "connection is now closed! Write is no longer accessible.";
        logger_.ERROR(message);
        return;
    }
}



void Connection::ReadNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[1024];
    // Note: try to read all readable data from socket whenever be called
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffers_->back()->Append(buf, bytes_read);
            // Note: append the newest buffer with new read outcome
        } else if (bytes_read == -1 && errno == EINTR) {
            // Note: A return value of EINTR means that the function was interrupted by a signal before the function could finish its normal job, so just continue to loop
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            // Note: EAGAIN and EWOULDBLOCK both mean there is no data available right now, so need to leave it aside for a while
            break;
        } else if (bytes_read == 0) {
            read_buffers_->push(std::make_unique<Buffer>());
            // Note: When reaches EOF, push in a new read buffer.
            break;
        } else {
            char message[100];
            sprintf(message, "failed to read from socket %d because of exception, error info: ", sockfd);
            logger_.ERROR(std::strcat(message, strerror(errno)));
            break;
        }
    }
}



void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffers_->front()->Size()];
    unsigned long data_size = send_buffers_->front()->Size();
    memcpy(buf, send_buffers_->front()->ToCstr(), data_size);
    while (true) {
        ssize_t bytes_write = write(sockfd, 0, data_size);
        if (bytes_write == -1 && errno == EINTR) {
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            char message[100];
            sprintf(message, "failed to write upon socket %d because of exception, error info: ", sockfd);
            logger_.ERROR(std::strcat(message, strerror(errno)));
            break;
        }
        send_buffers_->front()->Erase(bytes_write);
        if(send_buffers_->front()->Size() == 0)
            send_buffers_->pop();
        // Note: Whenever read something, update the buffer, if no data remaining in front buffer, pop it from queue.
    }
}



void Connection::RemoveFromEpoll() {
    state_ = State::Closed;
    eloop_->DeleteChannel(channel_.get());
    // Note: remove the channel from corresponding epoll when the connection is about to be closed
    already_closed_ = true;
}



Connection::State Connection::GetState() const {
    return state_;
}




std::queue<std::unique_ptr<Buffer>>* Connection::GetSendBuffers() {
    return send_buffers_.get();
}



std::queue<std::unique_ptr<Buffer>>* Connection::GetReadBuffers() {
    return read_buffers_.get();
}



void Connection::SetChannelWriteCallback() {
    channel_->SetWriteCallback([this](){this->Write();});
}



void Connection::SetChannelReadCallback() {
    channel_->SetReadCallback([this](){this->Read();});
}



void Connection::SetOnReceiveCallback(const std::function<void(Connection *)>& callback) {
    on_receive_callback_ = callback;
}



Socket* Connection::GetSocket() const {
    return sock_;
}



void Connection::EnableChannelWrite() {
    channel_->EnableWrite();
}



void Connection::EnableChannelRead() {
    channel_->EnableRead();
}

