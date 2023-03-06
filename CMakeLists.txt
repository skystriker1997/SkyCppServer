cmake_minimum_required(VERSION 3.24)
project(skycppserver)

set(CMAKE_CXX_STANDARD 14)

add_library(Acceptor
        src/Acceptor.cpp)

target_include_directories(Acceptor
        PUBLIC include/)

add_library(Buffer
        src/Buffer.cpp)

target_include_directories(Buffer
        PUBLIC include/)

add_library(Connection
        src/Connection.cpp)

target_include_directories(Connection
        PUBLIC include/)

add_library(Channel
        src/Channel.cpp)

target_include_directories(Channel
        PUBLIC include/)

add_library(EventLoop
        src/EventLoop.cpp)

target_include_directories(EventLoop
        PUBLIC include/)

add_library(FileLock
        src/FileLock.cpp)

target_include_directories(FileLock
        PUBLIC include/)

add_library(InetAddress
        src/InetAddress.cpp)

target_include_directories(InetAddress
        PUBLIC include/)

add_library(Logger
        src/Logger.cpp)

target_include_directories(Logger
        PUBLIC include/)

add_library(TcpServer
        src/TcpServer.cpp)

target_include_directories(TcpServer
        PUBLIC include/)

add_library(Socket
        src/Socket.cpp)

target_include_directories(Socket
        PUBLIC include/)

add_library(ThreadPool
        src/ThreadPool.cpp)

target_include_directories(ThreadPool
        PUBLIC include/)

add_library(Poller
        src/Poller.cpp)

target_include_directories(Poller
        PUBLIC include/)




