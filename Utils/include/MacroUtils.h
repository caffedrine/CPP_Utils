/*  
 *  File      : MacroUtils.h
 *  Created on: 6/28/2019
 *      Author: caffedrine
 */

#ifndef _MACROUTILS_H_
#define _MACROUTILS_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <stdint.h>
#include <stdbool.h>

#ifndef STR_CLR
    #define STR_CLR(str) memset(str, '\0', sizeof(str))
#endif

#ifndef STR_LEN
    #define STR_LEN(str) (sizeof(str))
#endif

#ifndef ARR_CLR
    #define ARR_CLR(arr) memset(arr, 0, sizeof(arr))
#endif

#ifndef ARR_LEN
    #define ARR_LEN(arr)  (sizeof(arr)/sizeof(arr[0]))
#endif

// print items of an array by a format
#define PRINT_ARRAY(array, length, format) \
{ \
    putchar('['); \
    for (size_t i = 0; i < length; ++i) { \
        printf(format, array[i]); \
        if (i < length - 1) printf(", "); \
    } \
    puts("]"); \
}


// reverse an array in place
#define REVERSE_ARRAY(array, length, status) \
    if (length > 0) { \
        for (int i = 0; i < length / 2; ++i) { \
            double temp; \
            temp = array[i]; \
            array[i] = array[length - i - 1]; \
            array[length - i - 1] = temp; \
        } \
        *status = 0; \
    } \
    else if (length < 0) *status = -1; \
    else *status = 1;

    
uint16_t ARR2TOUINT16(const uint8_t *bytes)
{
    if( bytes == NULL )
        return 0;
    
    uint16_t ipnum = 0;
    uint16_t octet = 0;
    
    for( uint8_t i = 0; i < 2; i++ )
    {
        octet = ((uint16_t) bytes[i] << ((uint16_t) (15 - i) * 8));
        ipnum = ipnum + octet;
    }
    return ipnum;
}

uint32_t ARR4TOUINT32(const uint8_t *bytes)
{
    if( bytes == NULL )
        return 0;
    
    uint32_t ipnum = 0;
    uint32_t octet = 0;
    
    for( uint8_t i = 0; i < 4; i++ )
    {
        octet = ((uint32_t) bytes[i] << ((uint32_t) (15 - i) * 8));
        ipnum = ipnum + octet;
    }
    return ipnum;
}

uint64_t ARR8TOUINT64(const uint8_t *bytes)
{
    if( bytes == NULL )
        return 0;
    
    uint64_t ipnum = 0;
    uint64_t octet = 0;
    
    for( uint8_t i = 0; i < 8; i++ )
    {
        octet = ((uint64_t) bytes[i] << ((uint64_t) (15 - i) * 8));
        ipnum = ipnum + octet;
    }
    return ipnum;
}


#ifdef _ENABLE_UINT128_
uint128_t ARR16TOUINT128(const uint8_t *bytes)
{
    if( bytes == NULL )
        return 0;
    
    uint128_t ipnum = 0;
    uint128_t octet = 0;
    
    for( uint8_t i = 0; i < 16; i++ )
    {
        octet = ((uint128_t) bytes[i] << ((uint128_t) (15 - i) * 8));
        ipnum = ipnum + octet;
    }
    return ipnum;
}
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _MACROUTILS_H_
