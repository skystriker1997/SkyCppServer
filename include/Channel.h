#include <sys/epoll.h>
#include <functional>
#include "Macros.h"
#include <unistd.h>
#include <memory>
#include <string>
#include "Logger.h"

#ifndef SKY_CHANNEL_H
#define SKY_CHANNEL_H


class EventLoop;


class Channel {
private:
    	EventLoop* eloop_;
    	int fd_;
   		uint32_t listen_events_;
    	uint32_t ready_events_;
    	bool in_epoll_;
    	std::function<void()> read_callback_;
    	std::function<void()> write_callback_;
		Logger logger_;

public:
    	DISALLOW_COPY_AND_MOVE(Channel);
    	Channel(EventLoop* eloop, int fd);
    	~Channel();

    	int GetFd() const;
    	uint32_t GetListenEvents() const;
    	uint32_t GetReadyEvents() const;
    	bool CheckInEpoll() const;
    	void SetInEpoll(bool in);

    	void EnableRead();
    	void EnableWrite();

    	void SetReadyEvents(uint32_t ev);

    	template<typename F>
    	void SetReadCallback(F&& callback) {
        		read_callback_ = callback;
    	};

    	template<typename F>
    	void SetWriteCallback(F&& callback) {
        		write_callback_ = callback;
    	};

    	void HandleEvent();

};



#endif
