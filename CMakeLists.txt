cmake_minimum_required(VERSION 3.24)
project(skycppserver)

set(CMAKE_CXX_STANDARD 14)

add_executable(HttpServer
        application/HttpServer.cpp)

target_include_directories(HttpServer
        PRIVATE include/)

target_link_libraries(HttpServer
        PRIVATE Connection TcpServer HttpRequestParser
        )

add_library(HttpRequestParser
        src/HttpRequestParser.cpp)

target_include_directories(HttpRequestParser
        PRIVATE include/)

add_library(Acceptor
        src/Acceptor.cpp)

target_include_directories(Acceptor
        PRIVATE include/)

target_link_libraries(Acceptor
        PRIVATE Channel InetAddress Socket
        )

add_library(Buffer
        src/Buffer.cpp)

target_include_directories(Buffer
        PRIVATE include/)

add_library(Connection
        src/Connection.cpp)

target_include_directories(Connection
        PRIVATE include/)

target_link_libraries(Connection
        PRIVATE EventLoop Socket Buffer Logger
        )

add_library(Channel
        src/Channel.cpp)

target_include_directories(Channel
        PRIVATE include/)

target_link_libraries(Channel
        PRIVATE EventLoop
        )

add_library(EventLoop
        src/EventLoop.cpp)

target_include_directories(EventLoop
        PRIVATE include/)

target_link_libraries(EventLoop
        PRIVATE Poller Logger
        )

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

target_link_libraries(Logger
        PRIVATE FileLock
        )

add_library(TcpServer
        src/TcpServer.cpp)

target_include_directories(TcpServer
        PRIVATE include/)

target_link_libraries(TcpServer
        PRIVATE Socket Acceptor Connection EventLoop ThreadPool Logger
        )

add_library(Socket
        src/Socket.cpp)

target_include_directories(Socket
        PRIVATE include/)

target_link_libraries(Socket
        PRIVATE Logger InetAddress
        )

add_library(ThreadPool
        src/ThreadPool.cpp)

target_include_directories(ThreadPool
        PRIVATE include/)

add_library(Poller
        src/Poller.cpp)

target_include_directories(Poller
        PRIVATE include/)

target_link_libraries(Poller
        PRIVATE Channel Logger
        )




