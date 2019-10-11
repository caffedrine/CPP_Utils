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
#include <utility>

struct Exception : public std::exception
{
    std::string s;
    explicit Exception(std::string ss) noexcept : s(std::move(ss)) {}
    Exception(const std::string& function, const std::string& ss) noexcept: s( function + "(): " + ss ) {}
    ~Exception() noexcept override = default; // Updated
    const char* what() const noexcept override { return s.c_str(); }
};

#endif // _EXCEPTION_H_
