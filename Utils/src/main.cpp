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
    
    BigNum a = "04";
    BigNum b = "05";
    BigNum sum = a + b;
    BigNum sub = a - b;
    BigNum mul = a * b;
    BigNum div = a / b;
    log("{} + {} = {}", a.ToStringHEX(), b.ToStringHEX(), sum.ToStringHEX());
    log("{} - {} = {}", a.ToStringHEX(), b.ToStringHEX(), sub.ToStringHEX());
    log("{} * {} = {}", a.ToStringHEX(), b.ToStringHEX(), mul.ToStringHEX());
    log("{} / {} = {}", a.ToStringHEX(), b.ToStringHEX(), div.ToStringHEX());
}