#include "Combinations.h"

Combinations::Combinations(int NumberOfElementsInCharset, int _SampleLength, const uint8_t *_Charset) : CharsetLength(NumberOfElementsInCharset),
                                                                                                        SampleLength(_SampleLength)
{
    /* Allocate memory to keep charset stored */
    this->Charset = (uint8_t *) malloc(this->CharsetLength);
    
    /* Keep a copy of given charset */
    for( int elementIdx = 0; elementIdx < this->CharsetLength; elementIdx++ )
    {
        this->Charset[elementIdx] = _Charset[elementIdx];
    }
    
    /* Allocate memory to store always last combination */
    this->LastCombination = (uint8_t *) malloc(this->SampleLength);
    
    /* Initialize initial combination */
    for( int i = 0; i < this->SampleLength; i++ )
    {
        this->LastCombination[i] = this->Charset[0];
    }
    
    //TODO: Sort input array()
}

Combinations::~Combinations()
{
    if( this->Charset != nullptr )
        delete this->Charset;
    
    if( this->LastCombination != nullptr )
    {
        delete this->LastCombination;
    }
}

void Combinations::GetNextCombination(uint8_t *pBuffer = nullptr)
{
    static int LastCharsetCharIndex = 0;
    static int LastIncrementedCharPosition = this->SampleLength - 1;
    
    if( LastCharsetCharIndex + 1 >= this->CharsetLength )
    {
        /* Last element from sample is reinitialized as it reached overflow */
        this->LastCombination[this->SampleLength - 1] = this->Charset[0];
        LastCharsetCharIndex = 0;
        
        /* Check backward for the element that will reach overflow */
        for( int i = this->SampleLength - 2 /* Exclude the last element as it is already initialized */; i >= 0; i-- )
        {
            if( this->LastCombination[i] == this->Charset[this->CharsetLength - 1] )
            {
                this->LastCombination[i] = this->Charset[0];
            }
            else
            {
                // TODO: Replace with something better
                int index = 0;
                for( int j = 0; j < this->CharsetLength; j++ )
                {
                    if( this->LastCombination[j] == this->Charset[j] )
                    {
                        index = j;
                        break;
                    }
                }
                
                this->LastCombination[i] = this->Charset[++index];
                break;
            }
        }
        
    }
    else
    {
        /* Increment index in charset to have next element */
        LastCharsetCharIndex++;
        
        /* Always change last char from sample if not reached last character from charset */
        this->LastCombination[this->SampleLength - 1] = this->Charset[LastCharsetCharIndex];
        if( pBuffer != nullptr )
        {
            for( int i = 0; i < this->SampleLength; i++ )
                pBuffer[i] = this->LastCombination[i];
        }
    }
}

std::string Combinations::GetNextCombination()
{
    this->GetNextCombination(nullptr);
    return std::string((const char *) this->LastCombination, this->SampleLength);
}

uint64_t Combinations::CalculatePossibleCombinations()
{
    uint64_t PossibleCombinations = 0;
    
    uint64_t up = (uint64_t) CalculateFactorial(this->CharsetLength);
    uint64_t down = (uint64_t) ((uint64_t) CalculateFactorial(this->SampleLength) * ((uint64_t) CalculateFactorial(this->CharsetLength - this->SampleLength)) );
    
    PossibleCombinations = up/down;
    
    return PossibleCombinations;
}

inline uint64_t Combinations::CalculateFactorial(int n)
{
    if( n <= 0 )
    {
        return 0;
    }
    
    uint64_t result = 1;
    for( int i = 1; i <= n; i++ )
        result *= i;
    
    return result;
}
