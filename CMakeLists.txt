cmake_minimum_required(VERSION 3.24)
project(skycppserver)

set(CMAKE_CXX_STANDARD 14)

include_directories(libs)

add_executable(skycppserver
        libs/Acceptor.cpp
        libs/Acceptor.h
        libs/Buffer.cpp
        libs/Buffer.h
        libs/Channel.cpp
        libs/Channel.h
        libs/Connection.cpp
        libs/Connection.h
        libs/EventLoop.cpp
        libs/EventLoop.h
        libs/FileLock.cpp
        libs/FileLock.h
        libs/InetAddress.cpp
        libs/InetAddress.h
        libs/Logger.cpp
        libs/Logger.h
        libs/Macros.h
        libs/Server.cpp
        libs/Server.h
        libs/Socket.cpp
        libs/Socket.h
        libs/ThreadPool.cpp
        libs/ThreadPool.h
        test/http_server.cpp)
