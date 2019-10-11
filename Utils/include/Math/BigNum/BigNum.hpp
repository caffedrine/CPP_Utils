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
#include "Formats.h"
#include "BigNum.h" // C Api
#include "Exception.h"

using std::string;

#define BIGNUM_MAX_DIGITS_NO    256

class BigNum
{
public:
    BigNum(string  StrNumber = string("00")) : Number(std::move( StrNumber )) {}
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
        char buf[BIGNUM_MAX_DIGITS_NO];
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
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_sub( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = !string(buf).empty()?string(buf):"0";
        return bignumResult;
    }
    
    BigNum operator*(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        if(BigNum_to_int(&num1) == 0 || BigNum_to_int(&num2) == 0)
        {
            throw Exception("Multiply", "Both operands must be != 0");
        }
        BigNum_mul( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = !string(buf).empty()?string(buf):"0";
        return bignumResult;
    }
    
    BigNum operator/(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        if(BigNum_to_int(&num2) == 0)
        {
            throw Exception("Division", "You cannot divide by 0");
        }
        BigNum_div( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        bignumResult = !string(buf).empty()?string(buf):"0";
        return bignumResult;
    }
    
    BigNum operator%(BigNum const &b)
    {
        BigNum bignumResult;
        
        struct bn num1{};
        struct bn num2{};
        struct bn result{};
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        BigNum_mod( &num1, &num2, &result );
        BigNum_to_string(&result, buf, sizeof(buf));
        
        bignumResult = !string(buf).empty()?string(buf):"0";
        return bignumResult;
    }
    
    bool operator>(BigNum const &b)
    {
        struct bn num1{};
        struct bn num2{};
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        if( BigNum_cmp( &num1, &num2) == LARGER )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool operator>=(BigNum const &b)
    {
        struct bn num1{};
        struct bn num2{};
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        int compResult = BigNum_cmp( &num1, &num2);
        if(  compResult == LARGER  || compResult == EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool operator<(BigNum const &b)
    {
        struct bn num1{};
        struct bn num2{};
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        if( BigNum_cmp( &num1, &num2) == SMALLER )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool operator<=(BigNum const &b)
    {
        struct bn num1{};
        struct bn num2{};
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        int compResult = BigNum_cmp( &num1, &num2);
        if(  compResult == SMALLER  || compResult == EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool operator==(BigNum const &b)
    {
        struct bn num1{};
        struct bn num2{};
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_from_string(&num2, (char *)b.Number.c_str(), b.Number.length());
        if( BigNum_cmp( &num1, &num2) == EQUAL )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    BigNum operator++() /*PRE */
    {
        struct bn num1{};
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_inc( &num1);
        BigNum_to_string(&num1, buf, sizeof(buf));
        this->Number = ( (string(buf).length()%2==1)?("0"+string(buf)):string(buf));
        return *this;
    }
    
    BigNum operator++(int) /* POST */
    {
        BigNum copy(*this);

        struct bn num1{};
        char buf[BIGNUM_MAX_DIGITS_NO];
        BigNum_from_string(&num1, (char *)this->Number.c_str(), this->Number.length());
        BigNum_inc( &num1);
        BigNum_to_string(&num1, buf, sizeof(buf));
        this->Number = ( (string(buf).length()%2==1)?("0"+string(buf)):string(buf));

        return copy;
    }
    
private:
    string Number;
};



#endif // _BIGNUM_HPP_