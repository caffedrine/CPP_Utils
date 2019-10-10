/*  
 *  File      : BigNum.h
 *  Created on: 2019-10-10
 *      Author: uib74520
 *      Source: https://github.com/kokke/tiny-bignum-c
 *      Lib   : https://gmplib.org
 */

#ifndef _BIGNUM_H_
#define _BIGNUM_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <StdTypes.h>
#include <stdint.h>
#include <assert.h>


/* This macro defines the word size in bytes of the array that constitues the big-number data structure. */
#ifndef WORD_SIZE
    #define WORD_SIZE 4
#endif

/* Size of big-numbers in bytes */
#define BN_ARRAY_SIZE    (128 / WORD_SIZE)


/* Here comes the compile-time specialization for how large the underlying array size should be. */
/* The choices are 1, 2 and 4 bytes in size with uint32, uint64 for WORD_SIZE==4, as temporary. */
#ifndef WORD_SIZE
    #error Must define WORD_SIZE to be 1, 2, 4
#elif (WORD_SIZE == 1)
/* Data type of array in structure */
    #define DTYPE                    uint8_t
  /* bitmask for getting MSB */
    #define DTYPE_MSB                ((DTYPE_TMP)(0x80))
  /* Data-type larger than DTYPE, for holding intermediate results of calculations */
    #define DTYPE_TMP                uint32_t
  /* sprintf format string */
    #define SPRINTF_FORMAT_STR       "%.02x"
    #define SSCANF_FORMAT_STR        "%2hhx"
  /* Max value of integer type */
    #define MAX_VAL                  ((DTYPE_TMP)0xFF)
#elif (WORD_SIZE == 2)
#define DTYPE                    uint16_t
    #define DTYPE_TMP                uint32_t
    #define DTYPE_MSB                ((DTYPE_TMP)(0x8000))
    #define SPRINTF_FORMAT_STR       "%.04x"
    #define SSCANF_FORMAT_STR        "%4hx"
    #define MAX_VAL                  ((DTYPE_TMP)0xFFFF)
#elif (WORD_SIZE == 4)
    #define DTYPE                    uint32_t
    #define DTYPE_TMP                uint64_t
    #define DTYPE_MSB                ((DTYPE_TMP)(0x80000000))
    #define SPRINTF_FORMAT_STR       "%.08x"
    #define SSCANF_FORMAT_STR        "%8x"
    #define MAX_VAL                  ((DTYPE_TMP)0xFFFFFFFF)
#endif
#ifndef DTYPE
    #error DTYPE must be defined to uint8_t, uint16_t uint32_t or whatever
#endif


/* Custom assert macro - easy to disable */
#define require(p, msg) assert(p && #msg)

/* Data-holding structure: array of DTYPEs */
struct bn
{
    DTYPE array[BN_ARRAY_SIZE];
};

/* Tokens returned by BigNum_cmp() for value comparison */
enum
{
    SMALLER = -1, EQUAL = 0, LARGER = 1
};

/* Initialization functions: */
void BigNum_init(struct bn *n);
void BigNum_from_int(struct bn *n, DTYPE_TMP i);
int BigNum_to_int(struct bn *n);
void BigNum_from_string(struct bn *n, char *str, int nbytes);
void BigNum_to_string(struct bn *n, char *str, int maxsize);

/* Basic arithmetic operations: */
void BigNum_add(struct bn *a, struct bn *b, struct bn *c); /* c = a + b */
void BigNum_sub(struct bn *a, struct bn *b, struct bn *c); /* c = a - b */
void BigNum_mul(struct bn *a, struct bn *b, struct bn *c); /* c = a * b */
void BigNum_div(struct bn *a, struct bn *b, struct bn *c); /* c = a / b */
void BigNum_mod(struct bn *a, struct bn *b, struct bn *c); /* c = a % b */
void BigNum_divmod(struct bn *a, struct bn *b, struct bn *c, struct bn *d); /* c = a/b, d = a%b */

/* Bitwise operations: */
void BigNum_and(struct bn *a, struct bn *b, struct bn *c); /* c = a & b */
void BigNum_or(struct bn *a, struct bn *b, struct bn *c);  /* c = a | b */
void BigNum_xor(struct bn *a, struct bn *b, struct bn *c); /* c = a ^ b */
void BigNum_lshift(struct bn *a, struct bn *b, int nbits); /* b = a << nbits */
void BigNum_rshift(struct bn *a, struct bn *b, int nbits); /* b = a >> nbits */

/* Special operators and comparison */
int BigNum_cmp(struct bn *a, struct bn *b);               /* Compare: returns LARGER, EQUAL or SMALLER */
int BigNum_is_zero(struct bn *n);                         /* For comparison with zero */
void BigNum_inc(struct bn *n);                             /* Increment: add one to n */
void BigNum_dec(struct bn *n);                             /* Decrement: subtract one from n */
void BigNum_pow(struct bn *a, struct bn *b, struct bn *c); /* Calculate a^b -- e.g. 2^10 => 1024 */
void BigNum_isqrt(struct bn *a, struct bn *b);             /* Integer square root -- e.g. isqrt(5) => 2*/
void BigNum_assign(struct bn *dst, struct bn *src);        /* Copy src into dst -- dst := src */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _BIGNUM_H_
