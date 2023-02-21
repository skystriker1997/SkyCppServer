
#include "Connection.h"


Connection::Connection(EventLoop *eloop, Socket *sock) : eloop_(eloop), sock_(sock), already_closed_(false) {
    channel_ = std::make_unique<Channel>(eloop_, sock->GetFd());
    channel_->EnableRead();
    read_buffer_ = std::make_unique<Buffer>();
    send_buffer_ = std::make_unique<Buffer>();
    state_ = State::Connected;
}


Connection::~Connection() {
    if(!already_closed_) {
        eloop_->DeleteChannel(channel_.get());  //remove the channel from corresponding epoll when gets the end of connection
        delete_connection_callback_for_server_(sock_);
        already_closed_ = true;
    }
}



void Connection::ReadNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[1024];
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);     // append read_buffer with new read outcome
        } else if (bytes_read == -1 && errno == EINTR) {  // A return value of EINTR means that the function was interrupted by a signal before the function could finish its normal job.
            printf("continue reading\n");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // EAGAIN is often raised when performing non-blocking I/O. It means "there is no data available right now, try again later".
            // It might (or might not) be the same as EWOULDBLOCK, which means "your thread would have to block in order to do that".
            break;
        } else if (bytes_read == 0) {    // reached EOF
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            this->Close();     // whenever the state becomes closed, call close method
            break;
        } else {
            logger_.ERROR(strerror(errno));
            state_ = State::Closed;
            this->Close();
            break;
        }
    }
}


void Connection::ReadBlocking() {
    int sockfd = sock_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    // To manipulate options at the socket level, the level parameter must be set to SOL_SOCKET as defined in sys/socket.h
    if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len) == -1) {   // get the length of socket receive length
        // For getsockopt(), optval and optlen are parameters used to identify a buffer in which the value for the requested option(s) are to be returned.
        logger_.ERROR(strerror(errno));    // if failed to get socket option, no need to append buffer then
    } else {
        char buf[rcv_size];
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);
        } else if (bytes_read == 0) {
            printf("read EOF, blocking client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            this->Close();
        } else if (bytes_read == -1) {
            logger_.ERROR(strerror(errno));
            state_ = State::Closed;
            this->Close();
        }
    }
}


void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf, send_buffer_->ToCstr(), send_buffer_->Size());   //memcpy copies n characters from memory area source to memory area destination


    long data_size = send_buffer_->Size();
    long data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);   // each loop tries to send all remaining data
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            logger_.ERROR(strerror(errno));
            state_ = State::Closed;
            this->Close();
            break;
        }
        data_left -= bytes_write;   // update how many data remaining
    }
}


void Connection::WriteBlocking() {
    ssize_t snd_size = send_buffer_->Size();
    int sockfd = sock_->GetFd();
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_size, (socklen_t) sizeof(snd_size)) == -1) {   // set socket send buffer to be as large as the content within the send buffer object
        logger_.ERROR(strerror(errno));
    } else {
        ssize_t bytes_write = write(sockfd, send_buffer_->ToCstr(), snd_size);
        if (bytes_write == -1) {
            logger_.ERROR(strerror(errno));
            state_ = State::Closed;
            this->Close();
        }
    }

}



void Connection::Read() {
    if(state_ != State::Connected) {
        char error_disconnect[] = "connection state is disconnected!";
        logger_.ERROR(error_disconnect);
        return;
    }
    read_buffer_->Clear();
    if (sock_->CheckNonBlocking()) {
        this->ReadNonBlocking();
    } else {
        this->ReadBlocking();
    }
}


void Connection::Write() {
    if(state_ != State::Connected) {
        char error_disconnect[] = "connection state is disconnected!";
        logger_.ERROR(error_disconnect);
        return;
    }
    if (sock_->CheckNonBlocking()) {
        this->WriteNonBlocking();
    } else {
        this->WriteBlocking();
    }
    send_buffer_->Clear();
}



void Connection::Close() {
    eloop_->DeleteChannel(channel_.get());  //remove the channel from corresponding epoll when gets the end of connection
    delete_connection_callback_for_server_(sock_);
    already_closed_ = true;
}


Connection::State Connection::GetState() {
    return state_;
}

void Connection::SetSendBuffer(const char* content) {
    send_buffer_->SetBuf(content);
}


const char *Connection::SeeReadBuffer() {
    return read_buffer_->ToCstr();
}


const char *Connection::SeeSendBuffer() {
    return send_buffer_->ToCstr();
}


Buffer *Connection::GetSendBuffer() {
    return send_buffer_.get();
}


Buffer *Connection::GetReadBuffer() {
    return read_buffer_.get();
}


void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)>&& callback) {
    delete_connection_callback_for_server_ = callback;
}

void Connection::Business() {
    Read();
    on_receive_callback_(this);
}


void Connection::SetOnReceiveCallback(const std::function<void(Connection *)>& callback) {
    on_receive_callback_ = callback;
    channel_->SetReadCallback([this](){this->Business();});
}


void Connection::SendBufferGetLine() {
    send_buffer_->GetLine();
}

Socket *Connection::GetSocket() {
    return sock_;
}