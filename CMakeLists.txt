cmake_minimum_required(VERSION 3.24)
project(skycppserver)

set(CMAKE_CXX_STANDARD 14)

add_executable(HttpServer
        application/HttpServer.cpp)

target_include_directories(HttpServer
        PRIVATE include/)

target_link_libraries(HttpServer
        PRIVATE libA libC
        )

add_library(HttpRequestParser
        src/HttpRequestParser.cpp)

target_include_directories(HttpRequest
        PRIVATE include/)

add_library(Acceptor
        src/Acceptor.cpp)

target_include_directories(Acceptor
        PRIVATE include/)

add_library(Buffer
        src/Buffer.cpp)

target_include_directories(Buffer
        PRIVATE include/)

add_library(Connection
        src/Connection.cpp)

target_include_directories(Connection
        PRIVATE include/)

add_library(Channel
        src/Channel.cpp)

target_include_directories(Channel
        PRIVATE include/)

add_library(EventLoop
        src/EventLoop.cpp)

target_include_directories(EventLoop
        PRIVATE include/)

add_library(FileLock
        src/FileLock.cpp)

target_include_directories(FileLock
        PRIVATE include/)

add_library(InetAddress
        src/InetAddress.cpp)

target_include_directories(InetAddress
        PRIVATE include/)

add_library(Logger
        src/Logger.cpp)

target_include_directories(Logger
        PRIVATE include/)

add_library(TcpServer
        src/TcpServer.cpp)

target_include_directories(TcpServer
        PRIVATE include/)

add_library(Socket
        src/Socket.cpp)

target_include_directories(Socket
        PRIVATE include/)

add_library(ThreadPool
        src/ThreadPool.cpp)

target_include_directories(ThreadPool
        PRIVATE include/)

add_library(Poller
        src/Poller.cpp)

target_include_directories(Poller
        PRIVATE include/)




