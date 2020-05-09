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
#include "SocketClient.h"

SocketClient::SocketClient(SocketClient::Protocol protocol) :
onConnect(nullptr),
onReceive(nullptr),
onDisconnect(nullptr),
__socket(0)
{
    __protocol = protocol;
}

SocketClient::~SocketClient()
{
	if (__socket != 0)
	{
		_mutex.lock();
		_closeConnect(__socket);
		_mutex.unlock();
	}
}

/*
* BASE
*/
bool SocketClient::__initClient()
{
    if (__socket != 0)
    {
        return false;
    }

    if (__isProtocol(SocketClient::Protocol::TCP))
        __socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    else if (__isProtocol(SocketClient::Protocol::UDP))
        __socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (_error(__socket) || __socket == 0)
	{
        if (LOGGER_DEBUG)
            CCLOG("[ERROR] Initialize client");
		__socket = 0;
		return false;
	}

	return true;
}

bool SocketClient::connectServer(const char* hostname, unsigned int port)
{
	if (!__initClient())
		return false;

	// reset value
	memset(&__serverAddr, 0, sizeof(struct sockaddr_in));

	struct hostent *host;
	host = gethostbyname(hostname);
	const char* ip = inet_ntoa(*((struct in_addr *) host->h_addr_list[0]));

	__serverAddr.sin_family = AF_INET;
	__serverAddr.sin_port = htons(port);
	__serverAddr.sin_addr.s_addr = inet_addr(ip);

    // TCP need connection
    if (__isProtocol(SocketClient::Protocol::TCP))
    {
        int ret = connect(__socket, (struct sockaddr*)&__serverAddr, sizeof(struct sockaddr));
        if (ret < 0 || __socket == 0)
        {
            onConnect(false);
            if (LOGGER_DEBUG)
                CCLOG("[CONNECT FAILED] %s:%d", hostname, port);
            disconnect();
            return false;
        }
    }
    
	//
	if (LOGGER_DEBUG)
		CCLOG("[CONNECTED] %s:%d >> ID: %d", ip, port, __socket);
    onConnect(true);
	// 
	std::thread recvThread(&SocketClient::__recvMessage, this);
	recvThread.detach();

	return true;
}

void SocketClient::disconnect()
{
	_mutex.lock();
	if (onDisconnect != nullptr)
		onDisconnect();
	__socket = 0;
	_mutex.unlock();
}

// Big Endian
void SocketClient::__recvMessage()
{
	__dataSize = 0;
	__isRecvHeader = true;
	__recvBytesBuff.reserve(102400); //  100KB cache

	char buffer[1024];
	ssize_t ret;
    socklen_t len;
    if (__isProtocol(SocketClient::Protocol::UDP))
        len = sizeof(__serverAddr);
	while (true)
	{
        if (__isProtocol(SocketClient::Protocol::TCP))
            ret = recv(__socket, buffer, 1024, 0);
        else if (__isProtocol(SocketClient::Protocol::UDP))
            ret = recvfrom(__socket, buffer, 1024, 0, (struct sockaddr*)&__serverAddr, &len);
        
		if (ret > 0)
		{
			__recvBytesBuff.insert(__recvBytesBuff.end(), buffer, buffer + ret);
			__onRecvData();
		}
		else
		{
			// recv data error -> break;
			break;
		}
	}
	disconnect();
}

void SocketClient::__onRecvData()
{
	if (__recvBytesBuff.size() <= 0)
		return;
    
    // UDP no need fragment data, so no need header prefix
    if (__isProtocol(SocketClient::Protocol::UDP))
        __isRecvHeader = false;
    
    // TCP need header prefix
    if (__isRecvHeader)
        __updateRecvHeaderData();
    else
		__updateRecvData();
}

void SocketClient::__updateRecvHeaderData()
{
	if (__recvBytesBuff.size() >= HEADER_BYTES) // header length
	{
		memcpy(&__dataSize, __recvBytesBuff.data(), HEADER_BYTES);
		__dataSize = ntohs(__dataSize); // network to host short

		__isRecvHeader = false;
		__recvBytesBuff.erase(__recvBytesBuff.begin(), __recvBytesBuff.begin() + HEADER_BYTES);
		__onRecvData();
	}
}

void SocketClient::__updateRecvData()
{
    if (__isProtocol(SocketClient::Protocol::UDP))
        __dataSize = __recvBytesBuff.size();
    
	if (__recvBytesBuff.size() >= __dataSize)
	{
		__onRecvMessage(__recvBytesBuff.data(), __dataSize);

		__isRecvHeader = true; // reset header count
		__recvBytesBuff.erase(__recvBytesBuff.begin(), __recvBytesBuff.begin() + __dataSize);
		__onRecvData();
	}
}

// main process
void SocketClient::__onRecvMessage(const char* data, uint16_t size)
{
	// Convert to JSON
	msgpack::sbuffer buffer;
	buffer.write(data, size);
	Document JSON = msgToJSON(buffer);
	std::string json = serializeJson(JSON);
	if (json.compare("{}") != 0) // JSON has data
	{
		if (LOGGER_DEBUG)
			CCLOG("[RECV] %s", json.c_str());
		onReceive(JSON);
	}
}

// Big Endian
void SocketClient::sendMessage(msgpack::sbuffer& buffer)
{
	if (__socket != 0)
	{
		_mutex.lock();

        if (__isProtocol(SocketClient::Protocol::TCP))
        {
            uint16_t dataLength = htons(buffer.size()); // host to network short
            char header[HEADER_BYTES];
            memcpy(header, &dataLength, HEADER_BYTES);
            
            size_t totalLength = HEADER_BYTES + buffer.size();
            char *msg = (char*)malloc(totalLength*sizeof(char));
            memcpy(msg, &header, HEADER_BYTES);
            memcpy(msg + HEADER_BYTES, buffer.data(), buffer.size());
            
            send(__socket, msg, totalLength, 0);
            free(msg);
        }
        else if (__isProtocol(SocketClient::Protocol::UDP))
        {
            sendto(__socket, buffer.data(), strlen(buffer.data()), 0, (sockaddr*)&__serverAddr, sizeof(__serverAddr));
        }
        
		if (LOGGER_DEBUG)
		{
			std::string msg = serializeJson(msgToJSON(buffer));
			CCLOG("[SENT] %s", msg.c_str());
		}

		_mutex.unlock();
	}
    else
    {
        if (LOGGER_DEBUG)
        {
            CCLOG("[SENT FAILED] %d", __socket);
        }
    }
}

bool SocketClient::__isProtocol(SocketClient::Protocol protocol)
{
    return (__protocol == protocol);
}

std::string SocketClient::serializeJson(const Document& JSON)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    JSON.Accept(writer);
    
    return buffer.GetString();
}

Document SocketClient::msgToJSON(msgpack::sbuffer& buffer)
{
	// Convert to JSON
	Document JSON;
	JSON.SetObject();
	Document::AllocatorType& allocator = JSON.GetAllocator();

	// Unpacking
	size_t off = 0;
	msgpack::unpacked result = msgpack::unpack(buffer.data(), buffer.size(), off);
	msgpack::object obj = result.get();

	msgpack::object_kv* pkv;
	msgpack::object_kv* pkv_end;
	msgpack::object pk, pv;

	if (obj.via.map.size > 0)
	{
		pkv = obj.via.map.ptr;
		pkv_end = obj.via.map.ptr + obj.via.map.size;

		do
		{
			pk = pkv->key;
			pv = pkv->val;

			// Add key
			std::string key;
			if (pk.type == msgpack::type::STR)
			{
				key = pk.as<std::string>();
			}
			// Add value
			if (pv.type == msgpack::type::STR)
			{
				auto value = pv.as<std::string>();
				JSON.AddMember(StringRef(strdup(key.c_str())), StringRef(strdup(value.c_str())), allocator);
			}
			else if (pv.type == msgpack::type::POSITIVE_INTEGER || pv.type == msgpack::type::NEGATIVE_INTEGER)
			{
				auto value = pv.as<int>();
				JSON.AddMember(StringRef(strdup(key.c_str())), rapidjson::Value(value), allocator);
			}
			else if (pv.type == msgpack::type::FLOAT)
			{
				auto value = pv.as<float>();
				JSON.AddMember(StringRef(strdup(key.c_str())), rapidjson::Value(value), allocator);
			}
			else if (pv.type == msgpack::type::BOOLEAN)
			{
				auto value = pv.as<bool>();
				JSON.AddMember(StringRef(strdup(key.c_str())), rapidjson::Value(value), allocator);
			}
			else if (pv.type == msgpack::type::ARRAY)
			{
                msgpack::object *ptr = pv.via.array.ptr;
                msgpack::object *end = ptr + pv.via.array.size;
                
                // add Array to Map
                JSON.AddMember(StringRef(strdup(key.c_str())), __msgArrayProcess(allocator, ptr, end), allocator);
			}

			++pkv;
		} while (pkv < pkv_end);
	}

	return JSON;
}

rapidjson::Value SocketClient::__msgArrayProcess(Document::AllocatorType& allocator, msgpack::object *ptr, msgpack::object *end)
{
    rapidjson::Value array(rapidjson::kArrayType);
    
	for (; ptr < end; ++ptr)
    {
        // check type
        if (ptr->type == msgpack::type::STR)
        {
            auto value = ptr->as<std::string>();
            array.PushBack(StringRef(strdup(value.c_str())), allocator);
        }
        else if (ptr->type == msgpack::type::POSITIVE_INTEGER || ptr->type == msgpack::type::NEGATIVE_INTEGER)
        {
            auto value = ptr->as<int>();
            array.PushBack(rapidjson::Value(value), allocator);
        }
        else if (ptr->type == msgpack::type::FLOAT)
        {
            auto value = ptr->as<float>();
            array.PushBack(rapidjson::Value(value), allocator);
        }
        else if (ptr->type == msgpack::type::BOOLEAN)
        {
            auto value = ptr->as<bool>();
            array.PushBack(rapidjson::Value(value), allocator);
        }
        else if (ptr->type == msgpack::type::ARRAY)
        {
            // continue check type (sh*t!)
            msgpack::object *ptr2 = ptr->via.array.ptr;
            msgpack::object *end2 = ptr2 + ptr->via.array.size;
            
            array.PushBack(__msgArrayProcess(allocator, ptr2, end2), allocator);
        }
    }
    
    return array;
}
