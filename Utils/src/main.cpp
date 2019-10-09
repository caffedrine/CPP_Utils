#include "Utils/Logger.h"
#include <vector>
#include <string>
#include <combinations/Combinations.h>

uint8_t Charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
	setup_logger();
	
    Combinations possibleKeys(sizeof(Charset), 11, Charset );
    
    log("C({0}, {1}) = {2}\n", sizeof(Charset), 11, possibleKeys.CalculatePossibleCombinations());
	for(int i = 0; i < 99; i++)
    {
	    log("{0}. {1}", i, possibleKeys.GetNextCombination());
    }
}