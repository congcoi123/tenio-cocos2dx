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
#include "Sender.h"

Sender::Sender()
{
    
}

void Sender::setTcpClient(SocketClient *tcpClient)
{
    __tcpClient = tcpClient;
}

void Sender::login(std::string userName, std::string signature)
{
	msgpack::sbuffer buffer;
	msgpack::packer<msgpack::sbuffer> pk2(&buffer);
	pk2.pack_map(2);
	pk2.pack(Field::USER_NAME);
	pk2.pack(userName);
	pk2.pack(Field::SIGNATURE);
    pk2.pack(signature);

    __tcpClient->sendMessage(buffer);
}

void Sender::sendNormaMsg()
{
	msgpack::sbuffer buffer;
	msgpack::packer<msgpack::sbuffer> pk2(&buffer);
	pk2.pack_map(2);
	pk2.pack(Command::REQUES);
	pk2.pack(Req::NORMAL);
    pk2.pack(Field::DATA);
    pk2.pack("test");

	__tcpClient->sendMessage(buffer);
}

void Sender::sendArrayMsg()
{
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk2(&buffer);
    pk2.pack_map(2);
    pk2.pack(Command::REQUES);
    pk2.pack(Req::ARRAY);
    pk2.pack(Field::DATA);
    pk2.pack_array(3);
    pk2.pack("array 1");
    pk2.pack(true);
    pk2.pack_array(2);
    pk2.pack("array 2");
    pk2.pack(1000);
    
    __tcpClient->sendMessage(buffer);
}
