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
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#if WIN32
#define strdup _strdup
#endif

// Debug mode
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// ========
#define LOGGER_DEBUG false
// ========
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// ========
#define LOGGER_DEBUG false
// ========
#else // win32, mac
// ========
#define LOGGER_DEBUG true
// ========
#endif

/**
 In TCP, because of the stream transmission, it's necessary to know a data package's length for extracting the number of bytes of its content (divide stream data into smaller package data). Therefore, we need a data length value, which should be attached in the header of each package. All TCP connections which connect to our server must follow this rule.
 */
#define HEADER_BYTES 2

#endif // __CONFIGURATION_H__
