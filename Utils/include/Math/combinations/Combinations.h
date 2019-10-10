/*  
 *  File      : Combinations.h
 *  Created on: 2019-10-09
 *      Author: uib74520
 */

#ifndef _COMBINATIONS_H_
#define _COMBINATIONS_H_

#include <string>
#include <Exception.h>
#include <Math/Algebra.h>

class Combinations
{
public:
    Combinations(int NumberOfElementsInCharset, int SampleLength, const uint8_t *Charset);
    ~Combinations();

    uint64_t CalculatePossibleCombinations();
    void GetNextCombination(uint8_t *pBuffer);
    std::string GetNextCombination();
    
protected:

private:
    uint8_t *Charset = nullptr;
    int CharsetLength;
    int SampleLength;
    uint8_t *LastCombination = nullptr;

};

#endif // _COMBINATIONS_H_