#include "Connection.h"



Connection::Connection(EventLoop *eloop, std::unique_ptr<Socket> sock) : state_(Available), eloop_(eloop), sock_(std::move(sock)) {
    channel_ = std::make_unique<Channel>(eloop_, sock_->GetFd());
    this->EnableChannelRead();
    read_buffer_ = std::make_unique<Buffer>();
    write_buffer_ = std::make_unique<Buffer>();
    this->SetChannelReadCallback();
    this->SetChannelWriteCallback();
}


Connection::~Connection() = default;



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
    char buf[8196];
    bool do_business = false;
    // Note: try to read all readable data from socket whenever be called
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read == 8196) {
            read_buffer_->Append(buf, bytes_read);
            continue;
        } else if (bytes_read > 0 && bytes_read < 8196) {
            read_buffer_->Append(buf, bytes_read);
            do_business = true;
            break;
        } else if (bytes_read == -1 && errno == EINTR) {
            // Note: A return value of EINTR means that the function was interrupted by a signal before the function could finish its normal job, so just continue to loop
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            // Note: EAGAIN and EWOULDBLOCK both mean there is no data available right now, so need to leave it aside for a while
            break;
        } else if (bytes_read == 0) {
            state_ = State::Closed;
        } else {
            char message[100];
            sprintf(message, "failed to read from socket %d because of exception, error info: ", sockfd);
            logger_.ERROR(std::strcat(message, strerror(errno)));
            state_ = State::Closed;
            break;
        }
    }
    if(do_business) {
        on_receive_callback_(this);
    } else if(state_ == State::Closed) {
        DeleteSelf(sock_.get());
    }
}



void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    unsigned long data_size = write_buffer_->Size();
    auto temp_buff = std::make_unique<char[]>(data_size);
    memcpy(temp_buff.get(), write_buffer_->ToCstr(), data_size);
    while (true) {
        ssize_t bytes_write = write(sockfd, temp_buff.get(), data_size);
        if (bytes_write == -1) {
            if(errno == EAGAIN) {
                break;
            }
            int error_code;
            socklen_t error_code_size = sizeof(error_code);
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
            if(error_code == ENOTCONN) {
                // Note: Client has terminated connection
                state_ = State::Closed;
                break;
            } else if (errno == EINTR) {
                continue;
            } else {
                char message[100];
                sprintf(message, "failed to write upon socket %d because of unexpected surprise, error info: ", sockfd);
                logger_.ERROR(std::strcat(message, strerror(errno)));
                state_ = State::Closed;
                break;
            }
        } else {
            write_buffer_->Erase(bytes_write);
            if(write_buffer_->Size() == 0) {
                state_ = State::Closed;
                //this->EnableChannelRead();
                // Note: disable write of channel when no data remaining in send buffer, otherwise epoll would always report once the socket write buffer gets free space
                //break;
            }
        }
    }
    if(state_ == State::Closed) {
        DeleteSelf(sock_.get());
    }
}




void Connection::DeleteSelf(Socket* sock) {
    eloop_->DeleteChannel(channel_.get());
    delete_self_on_server_callback_(sock);
    // Note: remove the channel from corresponding epoll when the connection is about to be closed
}



Buffer* Connection::GetSendBuffer() {
    return write_buffer_.get();
}



Buffer* Connection::GetReadBuffer() {
    return read_buffer_.get();
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
    return sock_.get();
}



void Connection::EnableChannelWrite() {
    channel_->EnableWrite();
}



void Connection::EnableChannelRead() {
    channel_->EnableRead();
}

