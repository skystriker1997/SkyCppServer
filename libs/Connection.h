
#include <functional>
#include <memory>
#include "EventLoop.h"
#include "Socket.h"
#include "Buffer.h"
#include "Logger.h"


#ifndef TINYWEBSERVER_CONNECTION_H
#define TINYWEBSERVER_CONNECTION_H


class Connection {

public:
    enum State {Invalid, Handshaking, Connected, Closed, Failed};
private:
    EventLoop* eloop_;
    Socket *sock_;
    std::unique_ptr<Channel> channel_;
    State state_{State::Invalid};
    std::unique_ptr<Buffer> read_buffer_;
    std::unique_ptr<Buffer> send_buffer_;
    std::function<void(Socket *)> delete_connection_callback_for_server_;

    std::function<void(Connection *)> on_receive_callback_;

    Logger logger_;
    bool already_closed_;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();


public:

    Connection(EventLoop* eloop, Socket *sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();

    void SetDeleteConnectionCallback(std::function<void(Socket *)>&& callback);
    void SetOnReceiveCallback(const std::function<void(Connection *)>& callback);
    State GetState();
    void Close();
    void SetSendBuffer(const char *str);
    Buffer *GetReadBuffer();
    const char *SeeReadBuffer();
    Buffer *GetSendBuffer();
    const char *SeeSendBuffer();
    void SendBufferGetLine();
    Socket *GetSocket();
    void Business();




};

#endif //TINYWEBSERVER_CONNECTION_H
