## SkyNetlib

### Overview

SkyNetlib is a C++ network library that I developed, encompassing a well-established framework and a demonstrated application. The library's core mechanism adheres to the reactor pattern, utilizing epolls to monitor file descriptors and assigning appropriate callback functions for event handling (read/write). To optimize computer resource utilization, each epoll's execution is assigned to a dedicated thread. 

**UML Diagram:**
![UML](./static_files/skynetlib.png)

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

&nbsp;

### Inspiration

The design of event-driven mode is inspired by **[muduo](https://github.com/chenshuo/muduo)** developed by Shuo Chen.













