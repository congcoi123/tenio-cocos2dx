/*
 The MIT License
 
 Copyright (c) 2016-2019 kong <congcoi123@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */
#ifndef __BASE_SOCKET_H__
#define __BASE_SOCKET_H__

#include <list>
#include <thread>
#if WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
#else
#include <arpa/inet.h>   // for inet_**
#include <netdb.h>   // for gethost**
#include <netinet/in.h>  // for sockaddr_in
#include <sys/types.h>   // for socket
#include <sys/socket.h>  // for socket
#include <unistd.h>
#include <stdio.h>        // for printf
#include <stdlib.h>  // for exit
#include <string.h>  // for bzero
#define HSocket int
#endif 

#include "cocos2d.h"
USING_NS_CC;

class BaseSocket : public Ref
{
public:

	BaseSocket();
	~BaseSocket();

protected:

	void _closeConnect(HSocket socket);
	bool _error(HSocket socket);

	std::mutex _mutex;

private:

	bool __initSucess;

};

#endif // __BASE_SOCKET_H__
