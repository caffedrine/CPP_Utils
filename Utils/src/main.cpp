#include "Utils/Logger.h"
#include <vector>
#include <string>
#include <combinations/Combinations.h>

const uint8_t Charset[] = "abcdefghij";//klmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
	setup_logger();
	
    Combinations possibleKeys(strlen((const char *)Charset), 5, Charset );
    
    log("C({0}, {1}) = {2}", strlen((const char *)Charset), 5, possibleKeys.CalculatePossibleCombinations());
	for(int i = 0; i < 100; i++)
    {
	    log("{0}. {1}", i, possibleKeys.GetNextCombination());
    }
}