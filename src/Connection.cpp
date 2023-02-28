
#include "Connection.h"


Connection::Connection(EventLoop *eloop, Socket *sock) : eloop_(eloop), sock_(sock), already_closed_(false), send_buffer_data_left_(-1) {
    channel_ = std::make_unique<Channel>(eloop_, sock->GetFd());
    channel_->EnableRead();
    channel_->EnableWrite();
    SetChannelReadCallback();
    SetChannelWriteCallback();
    read_buffer_ = std::make_unique<Buffer>();
    send_buffer_ = std::make_unique<Buffer>();
    state_ = State::Free;
}


Connection::~Connection() {
    if(!already_closed_) {
        eloop_->DeleteChannel(channel_.get());  //remove the channel from corresponding epoll when gets the end of connection
        already_closed_ = true;
    }
}


void Connection::Read() {
    if(state_ == State::Occupied) {
        char info_occupied[] = "connection is now occupied!";
        logger_.INFO(info_occupied);
        return;
    }
    if(state_ == State::Closed) {
        char error_disconnect[] = "connection is now closed!";
        logger_.ERROR(error_disconnect);
        return;
    }
    if (sock_->CheckNonBlocking()) {
        this->ReadNonBlocking();
    } else {
        this->ReadBlocking();
    }
}


void Connection::Write() {
    if (sock_->CheckNonBlocking()) {
        this->WriteNonBlocking();
    } else {
        this->WriteBlocking();
    }
}


void Connection::ReadNonBlocking() {
    state_ = State::Occupied;
    int sockfd = sock_->GetFd();
    char buf[1024];
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);     // append read_buffer with new read outcome
        } else if (bytes_read == -1 && errno == EINTR) {  // A return value of EINTR means that the function was interrupted by a signal before the function could finish its normal job.
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // EAGAIN is often raised when performing non-blocking I/O. It means "there is no data available right now, try again later".
            // It might (or might not) be the same as EWOULDBLOCK, which means "your thread would have to block in order to do that".
            state_ = State::Free;
            break;    // if really need to wait, get out of loop and wait for being called later
        } else if (bytes_read == 0) {    //
            on_receive_callback_(this);    // if reach EOF, call specific function to handle contents in read_buffer
            break;
        } else {
            logger_.ERROR(strerror(errno));
            read_buffer_->Clear();
            state_ = State::Free;
            break;
        }
    }
}


void Connection::ReadBlocking() {
    state_ = State::Occupied;
    int sockfd = sock_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    // To manipulate options at the socket level, the level parameter must be set to SOL_SOCKET as defined in sys/socket.h
    if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len) == -1) {   // get the length of socket receive length
        // For getsockopt(), optval and optlen are parameters used to identify a buffer in which the value for the requested option(s) are to be returned.
        logger_.ERROR(strerror(errno));
        state_ = State::Free;
    } else {
        char buf[rcv_size];
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);
            state_ = State::Free;
        } else if (bytes_read == 0) {
            printf("read EOF\n");
            on_receive_callback_(this);
        } else if (bytes_read == -1) {
            logger_.ERROR(strerror(errno));
            state_ = State::Free;
        }
    }
}


void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf, send_buffer_->ToCstr(), send_buffer_->Size());   //memcpy copies n characters from memory area source to memory area destination
    long data_size = send_buffer_->Size();
    if(send_buffer_data_left_ == -1)
        send_buffer_data_left_ = data_size;
    while (send_buffer_data_left_ > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - send_buffer_data_left_, send_buffer_data_left_);   // each loop tries to send all remaining data
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            return;    // if really need to wait because of the congestion in socket's buffer, just return and wait for being called later
        }
        if (bytes_write == -1) {
            logger_.ERROR(strerror(errno));
            break;
        }
        send_buffer_data_left_ -= bytes_write;   // update how many data remaining
    }
    read_buffer_->Clear();
    send_buffer_->Clear();
    state_ = State::Free;
}


void Connection::WriteBlocking() {
    ssize_t snd_size = send_buffer_->Size();
    int sockfd = sock_->GetFd();
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_size, (socklen_t) sizeof(snd_size)) == -1) {   // set socket send buffer to be as large as the content within the send buffer object
        logger_.ERROR(strerror(errno));
    } else {
        ssize_t bytes_write = write(sockfd, send_buffer_->ToCstr(), snd_size);
        if(bytes_write < snd_size) {   // error happened or have reached the end of stream
            if(bytes_write == -1)
                logger_.ERROR(strerror(errno));
        } else
            return;
    }
    read_buffer_->Clear();
    send_buffer_->Clear();
    state_ = State::Free;
}



void Connection::RemoveFromEpoll() {
    state_ = State::Closed;
    eloop_->DeleteChannel(channel_.get());  //remove the channel from corresponding epoll when the connection is about to be closed
    already_closed_ = true;
}


Connection::State Connection::GetState() {
    return state_;
}


void Connection::SetSendBuffer(const char* content) {
    send_buffer_->SetBuf(content);
}



Buffer *Connection::GetSendBuffer() {
    return send_buffer_.get();
}


Buffer *Connection::GetReadBuffer() {
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


Socket *Connection::GetSocket() {
    return sock_;
}