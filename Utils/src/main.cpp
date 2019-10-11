#include <vector>
#include <string>
#include <TimeUtils.h>
#include <Math/Algebra.h>
#include "Utils/Logger.h"

#include "test_BigNum.hpp"

using std::string;

int main()
{
	setup_logger();
    //BigNum_Test();
    
    for(uint8_t i = 0; i < 0xFF; i++)
    {
        BigNum a = std::to_string(i*300);
        BigNum b = std::to_string(i*100);
    
        BigNum sum = a + b;
        BigNum sub = a - b;
        BigNum mul = a * b;
        BigNum div = a / b;
        BigNum mod = a % b;
        bool aGreaterB = a > b;
        bool aGreaterEqualB = a >= b;
        bool aEqualB = a == b;
        bool aSmallerB = a < b;
        bool aSmallerEqualB = a <= b;
    
        log("{} + {} = {}", a.ToStringHEX(), b.ToStringHEX(), sum.ToStringHEX());
        log("{} - {} = {}", a.ToStringHEX(), b.ToStringHEX(), sub.ToStringHEX());
        log("{} * {} = {}", a.ToStringHEX(), b.ToStringHEX(), mul.ToStringHEX());
        log("{} / {} = {}", a.ToStringHEX(), b.ToStringHEX(), div.ToStringHEX());
        log("{} % {} = {}", a.ToStringHEX(), b.ToStringHEX(), mod.ToStringHEX());
    
        log("{} > {} = {}", a.ToStringHEX(), b.ToStringHEX(), aGreaterB);
        log("{} >= {} = {}", a.ToStringHEX(), b.ToStringHEX(), aGreaterEqualB);
        log("{} == {} = {}", a.ToStringHEX(), b.ToStringHEX(), aEqualB);
        log("{} < {} = {}", a.ToStringHEX(), b.ToStringHEX(), aSmallerB);
        log("{} <= {} = {}", a.ToStringHEX(), b.ToStringHEX(), aSmallerEqualB);
    
        log("a++={}, ++a={}", (a++).ToStringHEX(), (++a).ToStringHEX());
        log("b++={}, ++b={}\n", (b++).ToStringHEX(), (++b).ToStringHEX());
        
    }
}