## SkyNetlib



#### Overview

SkyNetlib accomplishes a cpp network library and examples of application established based on the library. SkyNetlib's core execution is based on reactor pattern, which is realized by deploying epolls monitoring file descriptors that have been assigned proper callback function to handle events. To make better use of computer resoures, I assign each epoll to a sprarate thread. 



#### Structure

.src/                                              <==  source code of library

.include/                                          <==  header files

.application/                                      <==  test files that could be executed to verify the functionality   



#### Tested on

Ubuntu 20.04.5



#### Prerequisites

Clang >= 6.0  or  GCC >= 5.0

CMake >= 3.8 



#### Build

`git clone https://github.com/skystriker1997/skynetlib.git /your/directory`

`cmake /your/directory`

`cmake --build /your/directory/build`







