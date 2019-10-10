/*  
 *  File      : AlgebraFunctions.h
 *  Created on: 10.10.2019
 *      Author: curiosul
 *  Helper: https://planetcalc.com/978/
 */

#ifndef _ALGEBRAFUNCTIONS_H_
#define _ALGEBRAFUNCTIONS_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <stdio.h>

static uint64_t nFactorial(unsigned int n)
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

static uint64_t PermutationsFromN(unsigned int n)
{
    return nFactorial(n);
}

static uint64_t ArrangementsOfMfromN(unsigned int m, unsigned int n)
{
    /* Exception case */
    if( n < m  || m == 0 || n == 0)
    {
        return 0;
    }
    
    uint64_t Result = nFactorial(n) / nFactorial(n - m);
    
    return Result;
}

static uint64_t CombinationsOfMfromNwithRepetitions(unsigned int m, unsigned int n)
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

static uint64_t CombinationsOfMfromN(unsigned int m, unsigned int n)
{
    /* Exception case */
    if( n < m  || m == 0 || n == 0)
    {
        return 0;
    }
    
    uint64_t Result = (nFactorial(n) / (nFactorial(m)*nFactorial(n - m)));
    
    return Result;
}

#ifdef TEST_ALGEBRA
static void test_Algebra()
{
    unsigned int m = 4, n = 5;
    
    printf("M = %d, N=%d\n", m, n);
    printf("Number of permutations from %d: %lld\n", n, (unsigned long long int)PermutationsFromN(n));
    printf("Number of arrangements of %d from %d: %lld\n", m, n, (unsigned long long int)ArrangementsOfMfromN(m, n));
    printf("Number of combinations of %d from %d with repetitions: %lld\n", m, n, (unsigned long long int)CombinationsOfMfromNwithRepetitions(m, n));
    printf("Number of combinations of %d from %d: %lld\n", m, n, (unsigned long long int)CombinationsOfMfromN(m, n));
}
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ALGEBRAFUNCTIONS_H_
