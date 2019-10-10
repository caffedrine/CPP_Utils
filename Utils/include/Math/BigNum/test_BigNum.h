/*  
 *  File      : test_BigNum.h
 *  Created on: 2019-10-10
 *      Author: uib74520
 */

#ifndef _TEST_BIGNUM_H_
#define _TEST_BIGNUM_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <stdio.h>
#include <BigNum.h>

static void factorial(struct bn *n, struct bn *res)
{
    struct bn tmp;
    
    /* Copy n -> tmp */
    BigNum_assign(&tmp, n);
    
    /* Decrement n by one */
    BigNum_dec(n);
    
    /* Begin summing products: */
    while( !BigNum_is_zero(n) )
    {
        /* res = tmp * n */
        BigNum_mul(&tmp, n, res);
        
        /* n -= 1 */
        BigNum_dec(n);
        
        /* tmp = res */
        BigNum_assign(&tmp, res);
    }
    
    /* res = tmp */
    BigNum_assign(res, &tmp);
}

static void BigNum_Test()
{
    struct bn num;
    struct bn result;
    char buf[8192];
    
    BigNum_from_int(&num, 4);
    factorial(&num, &result);
    BigNum_to_string(&result, buf, sizeof(buf));
    printf("factorial(100) using bignum = %s\n", buf);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _TEST_BIGNUM_H_
