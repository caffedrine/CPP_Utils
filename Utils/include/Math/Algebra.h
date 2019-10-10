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

uint64_t Algebra_nFactorial(unsigned int n);

uint64_t Algebra_PermutationsFromN(unsigned int n);

uint64_t Algebra_ArrangementsOfMfromN(unsigned int m, unsigned int n);

uint64_t Algebra_CombinationsOfMfromNwithRepetitions(unsigned int m, unsigned int n);

uint64_t Algebra_CombinationsOfMfromN(unsigned int m, unsigned int n);

#ifdef TEST_ALGEBRA
static void Algebra_Test()
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
