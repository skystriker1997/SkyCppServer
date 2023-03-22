## SkyNetlib

&nbsp;

&nbsp;

#### Overview

SkyNetlib accomplishes a cpp network library and examples of application established based on the library. SkyNetlib's core execution is based on reactor pattern, which is realized by deploying epolls monitoring file descriptors that have been assigned proper callback function to handle events. To make better use of computer resoures, I assign each epoll to a sprarate thread. 

&nbsp;

&nbsp;

#### Structure

&nbsp;&nbsp;&nbsp;&nbsp;.src/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  source code of library

&nbsp;&nbsp;&nbsp;&nbsp;.include/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  header files

&nbsp;&nbsp;&nbsp;&nbsp;.application/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<==  test files that could be executed to verify the functionality   

&nbsp;

&nbsp;

#### Tested on

&nbsp;&nbsp;&nbsp;&nbsp;Ubuntu 20.04.5

&nbsp;

 &nbsp;

#### Prerequisites

&nbsp;&nbsp;&nbsp;&nbsp;Clang >= 6.0  or  GCC >= 5.0

&nbsp;&nbsp;&nbsp;&nbsp;CMake >= 3.8 

&nbsp;

&nbsp;

#### Build

`git clone https://github.com/skystriker1997/skynetlib.git /your/directory`

`cmake /your/directory`

` cmake --build /your/directory/build`







