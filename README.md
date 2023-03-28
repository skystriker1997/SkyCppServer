## SkyNetlib

### Overview

SkyNetlib is still in development, briefly accomplished the framework of a CPP network library and examples of application established based on the library. 
SkyNetlib's core mechanism embodies reactor pattern, which is realised by deploying epolls monitoring file descriptors assigned proper callback function to handle events(read/write). To make better use of computer resources, I assign each epoll's execution to a separate thread. 

&nbsp;

###  Structure

.src/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  source code of library

.include/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  header files

.application/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  test files that could be executed to verify the functionality   

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







