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
#ifndef __SINGLE_TON__
#define __SINGLE_TON__

#include <string>
#include <assert.h>

/**
 In software engineering, the singleton pattern is a software design pattern that restricts the instantiation of a class to one "single" instance. This is useful when exactly one object is needed to coordinate actions across the system. The term comes from the mathematical concept of a singleton.
 */
template <typename T> class Singleton
{

protected:
    
	static T* _singleton;
    
public:

	Singleton(void)
	{
		//assert( !ms_FMSingleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		_singleton = (T*)((int)this + offset);
#else
		_singleton = static_cast< T* >(this);
#endif
	}

	~Singleton(void)
	{  
		//assert( ms_FMSingleton );  
		_singleton = 0;
	}

	/*
	static T& getInstance( void )
	{	
		//assert( ms_FMSingleton ); 
		return (*_singleton);
	}
	*/

	static T* getInstance(void)
	{
		return _singleton;
	}

};

#endif

// __SINGLE_TON__
