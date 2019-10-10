#include "Algebra.h"

uint64_t Algebra_nFactorial(unsigned int n)
{
    if( n == 0 )
    {
        /* 0! = 1 */
        return 1;
    }
    
    uint64_t result = 1;
    for( int i = 1; i <= n; i++ )
        result *= i;
    
    return result;
}

uint64_t Algebra_PermutationsFromN(unsigned int n)
{
    return Algebra_nFactorial(n);
}

uint64_t Algebra_ArrangementsOfMfromN(unsigned int m, unsigned int n)
{
    /* Exception case */
    if( n < m  || m == 0 || n == 0)
    {
        return 0;
    }
    
    uint64_t Result = Algebra_nFactorial(n) / Algebra_nFactorial(n - m);
    
    return Result;
}

uint64_t Algebra_CombinationsOfMfromNwithRepetitions(unsigned int m, unsigned int n)
{
    /* Exception case */
    if( m == 0 || n == 0)
    {
        return 0;
    }
    
    uint64_t Result = n;
    for( unsigned int i = 1; i < m; i++)
    {
        Result *= n;
    }
    return Result;
}

uint64_t Algebra_CombinationsOfMfromN(unsigned int m, unsigned int n)
{
    /* Exception case */
    if( n < m  || m == 0 || n == 0)
    {
        return 0;
    }
    
    uint64_t Result = (Algebra_nFactorial(n) / (Algebra_nFactorial(m)* Algebra_nFactorial(n - m)));
    
    return Result;
}