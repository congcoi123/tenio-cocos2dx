/*
 The MIT License
 
 Copyright (c) 2016-2020 kong <congcoi123@gmail.com>
 
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
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "Singleton.h"

#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "network/HttpRequest.h"

#include "Server/Sender.h"
#include "Server/Socket/SocketClient.h"

USING_NS_CC;
using namespace rapidjson;
using namespace cocos2d::network;

class ProgressWidget;

class MainScene : public Layer, public Singleton<MainScene>
{
public:
	
	MainScene();
	~MainScene();

	static MainScene* getInstance();
	static Scene* createScene();
	void onEnter();
	bool init();
	CREATE_FUNC(MainScene);

	// Server
	void initServerEvents();
	void onRoomResponse(int resp, rapidjson::Value &DATA);
    void onRoomResponse(int resp);
	void onPlayerResponse(int resp, rapidjson::Value &DATA);
    void onPlayerResponse(int resp);

	void initialize();

	void quit();
    
private:

    std::string __getRandomName(size_t length);
    
	// server's information
	std::string __hostName;
	int __port;
    
    // socket client
    Sender *__sender;
    SocketClient *_tcpClient;
    
	// try sync
	std::mutex __mutex;

};

#endif // __MAIN_SCENE_H__
