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
#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include "BaseSocket.h"
#include <MsgPack/msgpack.hpp>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "Configuration/Configuration.h"

using namespace rapidjson;

class SocketClient : public BaseSocket
{
public:
    
    enum Protocol {
        TCP,
        UDP
    };

	SocketClient(Protocol protocol);
	~SocketClient();

	/*
	* BASE
	*/
	bool connectServer(const char* hostname, unsigned int port);
	void sendMessage(msgpack::sbuffer& buffer);
	void disconnect();
    std::string serializeJson(const Document& JSON);
    Document msgToJSON(msgpack::sbuffer& buffer);

	/*
	* EVENTS
	*/
	std::function<void(bool)> onConnect;
	std::function<void(Document& JSON)> onReceive;	
	std::function<void()> onDisconnect;

private:

	bool __initClient();
    bool __isProtocol(Protocol protocol);
	void __recvMessage();
	void __onRecvMessage(const char* data, uint16_t size);
	void __onRecvData();
	void __updateRecvHeaderData();
	void __updateRecvData();
    rapidjson::Value __msgArrayProcess(Document::AllocatorType& allocator, msgpack::object *ptr, msgpack::object *end);

	HSocket __socket;
    Protocol __protocol;
    struct sockaddr_in __serverAddr;
	std::vector<char> __recvBytesBuff;
	uint16_t __dataSize;
	bool __isRecvHeader;

};

#endif // __SOCKET_CLIENT_H__
