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
#include "MainScene.h"

template<> MainScene* Singleton<MainScene>::_singleton = nullptr;

MainScene* MainScene::getInstance()
{
	return _singleton;
}

MainScene::MainScene()
{
    __hostName = "localhost";
	__port = 8032;
    _tcpClient = new SocketClient(SocketClient::Protocol::TCP);
    __sender = new Sender();
    __sender->setTcpClient(_tcpClient);
}

MainScene::~MainScene()
{
    free(_tcpClient);
    free(__sender);
	removeAllChildrenWithCleanup(true);
}

Scene *MainScene::createScene()
{
	Scene *scene = NULL;

	do 
	{
		scene = Scene::create();

		CC_BREAK_IF(!scene);

		auto layer = MainScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);
	} while (0);

	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Server
	initServerEvents();
	
    // UI
	initialize();

	// Update
	schedule(schedule_selector(MainScene::update));

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
}

void MainScene::initialize()
{
    // Connect to the server
    _tcpClient->connectServer(__hostName.c_str(), __port);
}

void MainScene::quit()
{
	// exit game
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	exit(0);
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::initServerEvents()
{	
	_tcpClient->onConnect = [=](bool success){
		if (success)
		{
            // When connection is established successful
            // Send a login request to the server
            CCLOG("Success to connect to Server");
			__sender->login("kong", "signature");
		}
		else
		{
            // Failed to connect to Server
            CCLOG("Failed to connect to Server");
		}
	};

	_tcpClient->onDisconnect = [=](){
        // Disconnect from Server
        
	};

	_tcpClient->onReceive = [=](Document& JSON){
		
		if (JSON.HasParseError())
		{
            // Data failed to parse data
			return;
		}
        
		// Error code
		if (JSON.HasMember(Command::ERROR))
		{
            
		}
		// Alert
		else if (JSON.HasMember(Command::ALERT))
		{
            
		}
		// Main conversation
		else if (JSON.HasMember(Command::REQUES))
		{
			int resp = JSON[Command::REQUES].GetInt();
			
			if (resp >= 100 && resp < 200)
            {
                if (JSON.HasMember(Field::DATA))
                    onRoomResponse(resp, JSON[Field::DATA]);
                else
                    onRoomResponse(resp);
            }
			else if (resp >= 200 && resp < 300)
            {
                if (JSON.HasMember(Field::DATA))
                    onPlayerResponse(resp, JSON[Field::DATA]);
                else
                    onPlayerResponse(resp);
            }
		}
	};

}

void MainScene::onRoomResponse(int resp, rapidjson::Value &DATA)
{
    /*
    switch (resp)
    {
        
    }
    */
}

void MainScene::onRoomResponse(int resp)
{
    /*
    switch (resp)
    {
        
    }
    */
}

void MainScene::onPlayerResponse(int resp, rapidjson::Value &DATA)
{
    /*
    switch (resp)
    {
        
    }
    */
}

void MainScene::onPlayerResponse(int resp)
{
    /*
    switch (resp)
    {
        
    }
    */
}
