/*  
 *  File      : test_Combinations.h
 *  Created on: 2019-10-10
 *      Author: uib74520
 */

#ifndef _TEST_COMBINATIONS_H_
#define _TEST_COMBINATIONS_H_

#include <stdio.h>
#include "Combinations.h"

const uint8_t Charset[] = "abcdefghijkl";//mnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static int test_Combinations()
{
    Combinations possibleKeys(strlen((const char *)Charset), 11, Charset );
    printf("C(%ld, %d) = %lld\n", strlen((const char *)Charset), 11, (unsigned long long int)possibleKeys.CalculatePossibleCombinations());
    
    uint64_t prevMillis = 0;
    for(uint64_t i =0;;i++)
    {
        std::string combination = possibleKeys.GetNextCombination();
        if(TimeUtils::millis() - prevMillis >= 1500)
        {
            prevMillis = TimeUtils::millis();
            printf("%ld. %s\n", i, combination.c_str());
        }
        
        /* back to initial state? */
        if(combination == "aaaaaaaaaaa")
            break;
        
    }
}

#endif // _TEST_COMBINATIONS_H_
