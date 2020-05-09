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
#include "BaseSocket.h"

BaseSocket::BaseSocket()
{
	__initSucess = false;
    
#if WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0)
	{
        if (LOGGER_DEBUG)
            CCLOG("[BASE SOCKET] Initialize error");
		return;
	}
	_initSucess = true;
#endif
}

BaseSocket::~BaseSocket()
{
#if WIN32
	if (_initSucess)
		WSACleanup();
#endif
}

void BaseSocket::_closeConnect(HSocket socket)
{
#if WIN32
	closesocket(socket);
#else
	close(socket);
#endif
}

bool BaseSocket::_error(HSocket socket)
{
#if WIN32
	return socket == SOCKET_ERROR;
#else
	return socket < 0;
#endif
}
