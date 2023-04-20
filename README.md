## SkyNetlib

### Overview

SkyNetlib is still in development, but I briefly accomplished the framework of a CPP network library and demonstrations of application established based on the library. 
SkyNetlib's core mechanism embodies reactor pattern, which is realised by deploying epolls monitoring file descriptors assigned proper callback function to handle events(read/write). To make better use of computer resources, I assign each epoll's execution to a separate thread.

Besides, I've been writing my understanding about algorithms and data structures, along with my solution of typical problems from LeetCode, and updating under static_files folder. Welcome to comments. 

&nbsp;

###  Structure

.src/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  source code of library

.include/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  header files

.application/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  test files that could be executed to verify the functionality

.static_files/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  store static files, an important part is my elaboration of algorithms & data structures      

&nbsp;

### Tested on

Ubuntu 20.04.5

&nbsp;

### Prerequisites

Clang >= 6.0  or  GCC >= 5.0

CMake >= 3.8 

&nbsp;

### Build

`git clone https://github.com/skystriker1997/skynetlib.git /your/directory`

`cd /your/directory; mkdir build; cd build`

`cmake /your/directory`

`cmake --build .`







