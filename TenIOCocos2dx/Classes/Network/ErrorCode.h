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
#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

namespace EError
{

	namespace Login
	{
		static const int SUCCESS = 1;
		static const int USER_NOT_EXIST = 2;
		static const int USER_IS_EXISTED = 3;
		static const int INVALID_PASSWORD = 4;
		static const int USER_IS_BANNED = 5;
		static const int INVALID_CHARACTER = 6;
		static const int OVER_INPUT_LIMIT = 7;
	};

	namespace Common
	{
		static const int BAD_REQUEST = 100;
		static const int INVALID_REQUEST = 101;
        static const int MAX_ROOMS = 305;
        static const int UNKNOWN = 10;
        static const int MAX_CONNECTION = 11;
	};

	namespace Outside
	{
		static const int DUPLICATED_PLAYER = 200;
	};

};

#endif // __ERROR_CODE_H__
