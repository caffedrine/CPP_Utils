

#include "Utils/Logger.h"
#include <vector>
#include <string>
#include "TimeUtils.h"
#include <combinations/Combinations.h>

const uint8_t Charset[] = "1234";//abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
	setup_logger();
	
    Combinations possibleKeys(strlen((const char *)Charset), 4, Charset );
    
    log("C({0}, {1}) = {2}", strlen((const char *)Charset), 4, possibleKeys.CalculatePossibleCombinations());
	
    uint64_t prevMillis = 0;
    for(uint64_t i = 0; i < 65000000000; i++)
    {
        std::string combination = possibleKeys.GetNextCombination();
        log("{0}. {1}", i, combination);
        if(combination == "1111")
            break;
        
//        if(TimeUtils::millis() - prevMillis >= 1500)
//        {
//            prevMillis = TimeUtils::millis();
//            log("{0}. {1}", i, combination);
//        }
    }
}