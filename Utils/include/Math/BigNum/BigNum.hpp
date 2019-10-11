/*  
 *  File      : BigNum.hpp
 *  Created on: 11.10.2019
 *      Author: curiosul
 */

#ifndef _BIGNUM_HPP_
#define _BIGNUM_HPP_

#include <iostream>
#include <string>
#include <utility>
#include "BigNum.h" // C Api

using std::string;

class BigNum
{
public:
    BigNum(string  StrNumber = string("0")) : Number(std::move( StrNumber )) {}
    BigNum(const char *StrNumber) : Number(string(StrNumber)) {}
    ~BigNum() = default;
    string ToStringHEX()
    {
        return this->Number;
    }
    
    BigNum operator+(BigNum const &b)
    {
        BigNum bignumResult;
    
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[1024];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_add( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = string(buf);
        return bignumResult;
    }
    
    BigNum operator-(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[1024];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_sub( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = string(buf);
        return bignumResult;
    }
    
    BigNum operator*(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[1024];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_mul( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = string(buf);
        return bignumResult;
    }
    
    BigNum operator/(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[1024];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_div( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = string(buf);
        return bignumResult;
    }
    
private:
    string Number;
};



#endif // _BIGNUM_HPP_