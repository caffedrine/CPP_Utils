/*

Big number library - arithmetic on multiple-precision unsigned integers.

This library is an implementation of arithmetic on arbitrarily large integers.

The difference between this and other implementations, is that the data structure
has optimal memory utilization (i.e. a 1024 bit integer takes up 128 bytes RAM),
and all memory is allocated statically: no dynamic allocation for better or worse.

Primary goals are correctness, clarity of code and clean, portable implementation.
Secondary goal is a memory footprint small enough to make it suitable for use in
embedded applications.


The current state is correct functionality and adequate performance.
There may well be room for performance-optimizations and improvements.

*/

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "BigNum.h"

/* Functions for shifting number in-place. */
static void _lshift_one_bit(struct bn *a);
static void _rshift_one_bit(struct bn *a);
static void _lshift_word(struct bn *a, int nwords);
static void _rshift_word(struct bn *a, int nwords);

/* Public / Exported functions. */
void BigNum_init(struct bn *n)
{
    require(n, "n is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        n->array[i] = 0;
    }
}

void BigNum_from_int(struct bn *n, DTYPE_TMP i)
{
    require(n, "n is null");
    
    BigNum_init(n);
    
    /* Endianness issue if machine is not little-endian? */
#ifdef WORD_SIZE
    #if (WORD_SIZE == 1)
    n->array[0] = (i & 0x000000ff);
    n->array[1] = (i & 0x0000ff00) >> 8;
    n->array[2] = (i & 0x00ff0000) >> 16;
    n->array[3] = (i & 0xff000000) >> 24;
    #elif (WORD_SIZE == 2)
    n->array[0] = (i & 0x0000ffff);
    n->array[1] = (i & 0xffff0000) >> 16;
    #elif (WORD_SIZE == 4)
    n->array[0] = i;
    DTYPE_TMP num_32 = 32;
    DTYPE_TMP tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
    n->array[1] = tmp;
    #endif
#endif
}

int BigNum_to_int(struct bn *n)
{
    require(n, "n is null");
    
    int ret = 0;
    
    /* Endianness issue if machine is not little-endian? */
#if (WORD_SIZE == 1)
    ret += n->array[0];
  ret += n->array[1] << 8;
  ret += n->array[2] << 16;
  ret += n->array[3] << 24;
#elif (WORD_SIZE == 2)
    ret += n->array[0];
  ret += n->array[1] << 16;
#elif (WORD_SIZE == 4)
    ret += n->array[0];
#endif
    
    return ret;
}

void BigNum_from_string(struct bn *n, char *str, int nbytes)
{
    require(n, "n is null");
    require(str, "str is null");
    require(nbytes > 0, "nbytes must be positive");
    require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");
    
    BigNum_init(n);
    
    DTYPE tmp;                        /* DTYPE is defined in bn.h - uint{8,16,32,64}_t */
    int i = nbytes - (2 * WORD_SIZE); /* index into string */
    int j = 0;                        /* index into array */
    
    /* reading last hex-byte "MSB" from string first -> big endian */
    /* MSB ~= most significant byte / block ? :) */
    while( i >= 0 )
    {
        tmp = 0;
        sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
        n->array[j] = tmp;
        i -= (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) back in the string. */
        j += 1;               /* step one element forward in the array. */
    }
}

void BigNum_to_string(struct bn *n, char *str, int nbytes)
{
    require(n, "n is null");
    require(str, "str is null");
    require(nbytes > 0, "nbytes must be positive");
    require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");
    
    int j = BN_ARRAY_SIZE - 1; /* index into array - reading "MSB" first -> big-endian */
    int i = 0;                 /* index into string representation. */
    
    /* reading last array-element "MSB" first -> big endian */
    while( (j >= 0) && (nbytes > (i + 1)) )
    {
        sprintf(&str[i], SPRINTF_FORMAT_STR, n->array[j]);
        i += (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) forward in the string. */
        j -= 1;               /* step one element back in the array. */
    }
    
    /* Count leading zeros: */
    j = 0;
    while( str[j] == '0' )
    {
        j += 1;
    }
    
    /* Move string j places ahead, effectively skipping leading zeros */
    for( i = 0; i < (nbytes - j); ++i )
    {
        str[i] = str[i + j];
    }
    
    /* Zero-terminate string */
    str[i] = 0;
}

void BigNum_dec(struct bn *n)
{
    require(n, "n is null");
    
    DTYPE tmp; /* copy of n */
    DTYPE res;
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        tmp = n->array[i];
        res = tmp - 1;
        n->array[i] = res;
        
        if( !(res > tmp) )
        {
            break;
        }
    }
}

void BigNum_inc(struct bn *n)
{
    require(n, "n is null");
    
    DTYPE res;
    DTYPE_TMP tmp; /* copy of n */
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        tmp = n->array[i];
        res = tmp + 1;
        n->array[i] = res;
        
        if( res > tmp )
        {
            break;
        }
    }
}

void BigNum_add(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    DTYPE_TMP tmp;
    int carry = 0;
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        tmp = (DTYPE_TMP) a->array[i] + b->array[i] + carry;
        carry = (tmp > MAX_VAL);
        c->array[i] = (tmp & MAX_VAL);
    }
}

void BigNum_sub(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    DTYPE_TMP res;
    DTYPE_TMP tmp1;
    DTYPE_TMP tmp2;
    int borrow = 0;
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        tmp1 = (DTYPE_TMP) a->array[i] + (MAX_VAL + 1); /* + number_base */
        tmp2 = (DTYPE_TMP) b->array[i] + borrow;;
        res = (tmp1 - tmp2);
        c->array[i] = (DTYPE)(res & MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
        borrow = (res <= MAX_VAL);
    }
}

void BigNum_mul(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    struct bn row;
    struct bn tmp;
    int i, j;
    
    BigNum_init(c);
    
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        BigNum_init(&row);
        
        for( j = 0; j < BN_ARRAY_SIZE; ++j )
        {
            if( i + j < BN_ARRAY_SIZE )
            {
                BigNum_init(&tmp);
                DTYPE_TMP intermediate = ((DTYPE_TMP) a->array[i] * (DTYPE_TMP) b->array[j]);
                BigNum_from_int(&tmp, intermediate);
                _lshift_word(&tmp, i + j);
                BigNum_add(&tmp, &row, &row);
            }
        }
        BigNum_add(c, &row, c);
    }
}

void BigNum_div(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    struct bn current;
    struct bn denom;
    struct bn tmp;
    
    BigNum_from_int(&current, 1);               // int current = 1;
    BigNum_assign(&denom, b);                   // denom = b
    BigNum_assign(&tmp, a);                     // tmp   = a
    
    const DTYPE_TMP half_max = 1 + (DTYPE_TMP)(MAX_VAL / 2);
    bool overflow = false;
    while( BigNum_cmp(&denom, a) != LARGER )     // while (denom <= a) {
    {
        if( denom.array[BN_ARRAY_SIZE - 1] >= half_max )
        {
            overflow = true;
            break;
        }
        _lshift_one_bit(&current);                //   current <<= 1;
        _lshift_one_bit(&denom);                  //   denom <<= 1;
    }
    if( !overflow )
    {
        _rshift_one_bit(&denom);                  // denom >>= 1;
        _rshift_one_bit(&current);                // current >>= 1;
    }
    BigNum_init(c);                             // int answer = 0;
    
    while( !BigNum_is_zero(&current) )           // while (current != 0)
    {
        if( BigNum_cmp(&tmp, &denom) != SMALLER )  //   if (dividend >= denom)
        {
            BigNum_sub(&tmp, &denom, &tmp);         //     dividend -= denom;
            BigNum_or(c, &current, c);              //     answer |= current;
        }
        _rshift_one_bit(&current);                //   current >>= 1;
        _rshift_one_bit(&denom);                  //   denom >>= 1;
    }                                           // return answer;
}

void BigNum_lshift(struct bn *a, struct bn *b, int nbits)
{
    require(a, "a is null");
    require(b, "b is null");
    require(nbits >= 0, "no negative shifts");
    
    BigNum_assign(b, a);
    /* Handle shift in multiples of word-size */
    const int nbits_pr_word = (WORD_SIZE * 8);
    int nwords = nbits / nbits_pr_word;
    if( nwords != 0 )
    {
        _lshift_word(b, nwords);
        nbits -= (nwords * nbits_pr_word);
    }
    
    if( nbits != 0 )
    {
        int i;
        for( i = (BN_ARRAY_SIZE - 1); i > 0; --i )
        {
            b->array[i] = (b->array[i] << nbits) | (b->array[i - 1] >> ((8 * WORD_SIZE) - nbits));
        }
        b->array[i] <<= nbits;
    }
}

void BigNum_rshift(struct bn *a, struct bn *b, int nbits)
{
    require(a, "a is null");
    require(b, "b is null");
    require(nbits >= 0, "no negative shifts");
    
    BigNum_assign(b, a);
    /* Handle shift in multiples of word-size */
    const int nbits_pr_word = (WORD_SIZE * 8);
    int nwords = nbits / nbits_pr_word;
    if( nwords != 0 )
    {
        _rshift_word(b, nwords);
        nbits -= (nwords * nbits_pr_word);
    }
    
    if( nbits != 0 )
    {
        int i;
        for( i = 0; i < (BN_ARRAY_SIZE - 1); ++i )
        {
            b->array[i] = (b->array[i] >> nbits) | (b->array[i + 1] << ((8 * WORD_SIZE) - nbits));
        }
        b->array[i] >>= nbits;
    }
    
}

void BigNum_mod(struct bn *a, struct bn *b, struct bn *c)
{
    /*
      Take divmod and throw away div part
    */
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    struct bn tmp;
    
    BigNum_divmod(a, b, &tmp, c);
}

void BigNum_divmod(struct bn *a, struct bn *b, struct bn *c, struct bn *d)
{
    /*
      Puts a%b in d
      and a/b in c
  
      mod(a,b) = a - ((a / b) * b)
  
      example:
        mod(8, 3) = 8 - ((8 / 3) * 3) = 2
    */
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    struct bn tmp;
    
    /* c = (a / b) */
    BigNum_div(a, b, c);
    
    /* tmp = (c * b) */
    BigNum_mul(c, b, &tmp);
    
    /* c = a - tmp */
    BigNum_sub(a, &tmp, d);
}

void BigNum_and(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        c->array[i] = (a->array[i] & b->array[i]);
    }
}

void BigNum_or(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        c->array[i] = (a->array[i] | b->array[i]);
    }
}

void BigNum_xor(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        c->array[i] = (a->array[i] ^ b->array[i]);
    }
}

int BigNum_cmp(struct bn *a, struct bn *b)
{
    require(a, "a is null");
    require(b, "b is null");
    
    int i = BN_ARRAY_SIZE;
    do
    {
        i -= 1; /* Decrement first, to start with last array element */
        if( a->array[i] > b->array[i] )
        {
            return LARGER;
        }
        else if( a->array[i] < b->array[i] )
        {
            return SMALLER;
        }
    } while( i != 0 );
    
    return EQUAL;
}

int BigNum_is_zero(struct bn *n)
{
    require(n, "n is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        if( n->array[i] )
        {
            return 0;
        }
    }
    
    return 1;
}

void BigNum_pow(struct bn *a, struct bn *b, struct bn *c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    
    struct bn tmp;
    
    BigNum_init(c);
    
    if( BigNum_cmp(b, c) == EQUAL )
    {
        /* Return 1 when exponent is 0 -- n^0 = 1 */
        BigNum_inc(c);
    }
    else
    {
        struct bn bcopy;
        BigNum_assign(&bcopy, b);
        
        /* Copy a -> tmp */
        BigNum_assign(&tmp, a);
        
        BigNum_dec(&bcopy);
        
        /* Begin summing products: */
        while( !BigNum_is_zero(&bcopy) )
        {
            
            /* c = tmp * tmp */
            BigNum_mul(&tmp, a, c);
            /* Decrement b by one */
            BigNum_dec(&bcopy);
            
            BigNum_assign(&tmp, c);
        }
        
        /* c = tmp */
        BigNum_assign(c, &tmp);
    }
}

void BigNum_isqrt(struct bn *a, struct bn *b)
{
    require(a, "a is null");
    require(b, "b is null");
    
    struct bn low, high, mid, tmp;
    
    BigNum_init(&low);
    BigNum_assign(&high, a);
    BigNum_rshift(&high, &mid, 1);
    BigNum_inc(&mid);
    
    while( BigNum_cmp(&high, &low) > 0 )
    {
        BigNum_mul(&mid, &mid, &tmp);
        if( BigNum_cmp(&tmp, a) > 0 )
        {
            BigNum_assign(&high, &mid);
            BigNum_dec(&high);
        }
        else
        {
            BigNum_assign(&low, &mid);
        }
        BigNum_sub(&high, &low, &mid);
        _rshift_one_bit(&mid);
        BigNum_add(&low, &mid, &mid);
        BigNum_inc(&mid);
    }
    BigNum_assign(b, &low);
}

void BigNum_assign(struct bn *dst, struct bn *src)
{
    require(dst, "dst is null");
    require(src, "src is null");
    
    int i;
    for( i = 0; i < BN_ARRAY_SIZE; ++i )
    {
        dst->array[i] = src->array[i];
    }
}

/* Private / Static functions. */
static void _rshift_word(struct bn *a, int nwords)
{
    /* Naive method: */
    require(a, "a is null");
    require(nwords >= 0, "no negative shifts");
    
    int i;
    if( nwords >= BN_ARRAY_SIZE )
    {
        for( i = 0; i < BN_ARRAY_SIZE; ++i )
        {
            a->array[i] = 0;
        }
        return;
    }
    
    for( i = 0; i < BN_ARRAY_SIZE - nwords; ++i )
    {
        a->array[i] = a->array[i + nwords];
    }
    for( ; i < BN_ARRAY_SIZE; ++i )
    {
        a->array[i] = 0;
    }
}

static void _lshift_word(struct bn *a, int nwords)
{
    require(a, "a is null");
    require(nwords >= 0, "no negative shifts");
    
    int i;
    /* Shift whole words */
    for( i = (BN_ARRAY_SIZE - 1); i >= nwords; --i )
    {
        a->array[i] = a->array[i - nwords];
    }
    /* Zero pad shifted words. */
    for( ; i >= 0; --i )
    {
        a->array[i] = 0;
    }
}

static void _lshift_one_bit(struct bn *a)
{
    require(a, "a is null");
    
    int i;
    for( i = (BN_ARRAY_SIZE - 1); i > 0; --i )
    {
        a->array[i] = (a->array[i] << 1) | (a->array[i - 1] >> ((8 * WORD_SIZE) - 1));
    }
    a->array[0] <<= 1;
}

static void _rshift_one_bit(struct bn *a)
{
    require(a, "a is null");
    
    int i;
    for( i = 0; i < (BN_ARRAY_SIZE - 1); ++i )
    {
        a->array[i] = (a->array[i] >> 1) | (a->array[i + 1] << ((8 * WORD_SIZE) - 1));
    }
    a->array[BN_ARRAY_SIZE - 1] >>= 1;
}

