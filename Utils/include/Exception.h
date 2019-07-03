/*  
 *  File      : Exception.h
 *  Created on: 02.07.2019
 *      Author: curiosul
 */

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <cstdint>
#include <exception>
#include <string>

struct Exception : public std::exception
{
    std::string s;
    explicit Exception(std::string ss): s(ss) {}
    Exception(std::string function, std::string ss) : s( function + "(): " + ss ) {}
    ~Exception() throw () {} // Updated
    const char* what() const throw() override { return s.c_str(); }
};

#endif // _EXCEPTION_H_
