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
#ifndef __SENDER_H__
#define __SENDER_H__

#include <MsgPack/msgpack.hpp>

#include "Socket/SocketClient.h"
#include "Command.h"
#include "ErrorCode.h"
#include "Field.h"
#include "Req.h"
#include "Resp.h"

class Sender : public Ref
{
public:
	Sender();
    
    void setTcpClient(SocketClient *tcpClient);
    // FIXME: need to be implemented
    void setUdpClient(SocketClient *udpClient);
    
    // Game communication
	void login(std::string userName, std::string signature);
	void sendNormaMsg();
	void sendArrayMsg();
    
private:
    
    SocketClient *__tcpClient;

};

#endif // __SENDER_H__
