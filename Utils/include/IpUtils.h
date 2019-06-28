/*  
 *  File      : IpUtils.h
 *  Created on: 22.06.2019
 *      Author: curiosul
 */

#ifndef _IP_UTILS_H_
#define _IP_UTILS_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

/* Standard includes*/
#include <arpa/inet.h>
/* Custom includes */
#include <Target.h>
#include <StdTypes.h>
#include <MacroUtils.h>

/*
 *      ____            _                 _   _
 *     |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___
 *     | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 *     |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
 */

/** C Functions */
uint8_t IpUtils_IsValidIP(const char *IpStr);
uint8_t IpUtils_GetReversedIpAddressBytes(const char *IpStr, char *OutputIpStr);
uint8_t IpUtils_IpDottedToDecimal(const char *InputIpStr, uint8_t *OutputBytes);
char const *IpUtils_IpDecimal2Dotted(uint32_t IpNumber);
char const *IpUtils_GetHostFromUrl(const char *url);
/** C++ Functions */
#ifdef __cplusplus
std::string IpUtils_GetReversedIpAddressStr(const char *IpAddr);
#endif

/*
 *      ____        __ _       _ _   _
 *     |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
 *     | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 *     | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 *     |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
 *
 */

/** Check whether given string is a valid IPv4 or IPv6 */
uint8_t IpUtils_IsValidIP(const char *IpStr)
{
    const char *safeCopy = IpStr; /* cygwin? */
    struct sockaddr_in sa = {};
    
    /* Is valid IPv4? */
    if( inet_pton(AF_INET, safeCopy, &(sa.sin_addr)) == 1 )
        return 4;
    
    /* Is valid IPv6? */
    if( inet_pton(AF_INET6, safeCopy, &(sa.sin_addr)) == 1 )
        return 6;
    
    return 0;
}

/** Return the given string using bytes in reversed order */
uint8_t IpUtils_GetReversedIpAddressBytes(const char *InputIpStr, char *OutputIpStr)
{
    uint8_t RetVal = ERR_NOK;
    
    if( IpUtils_IsValidIP(InputIpStr) == 4 )
    {
        char reversed_ip[INET_ADDRSTRLEN];
        in_addr_t addr;
        /* Get the textual address into binary format */
        inet_pton(AF_INET, InputIpStr, &addr);
        /* Reverse the bytes in the binary address */
        addr =
                ((addr & 0xff000000) >> 24) |
                ((addr & 0x00ff0000) >> 8) |
                ((addr & 0x0000ff00) << 8) |
                ((addr & 0x000000ff) << 24);
        /* And lastly get a textual representation back again */
        inet_ntop(AF_INET, &addr, reversed_ip, sizeof(reversed_ip));
        /* Copy result to the output */
        strcpy(OutputIpStr, reversed_ip);
        RetVal = 0;
    }
    else if( IpUtils_IsValidIP(InputIpStr) == 6 )
    {
        /// IPv6 not implemented
        RetVal = ERR_NOT_IMPLEMENTED;
    }
    else
    {
        RetVal = ERR_INVALID;
    }
    return RetVal;
}

uint8_t IpUtils_IpDottedToDecimal(const char *InputIpStr, uint8_t *OutputBytes)
{
    uint8_t RetVal = ERR_NOK;
    
    if( IpUtils_IsValidIP(InputIpStr) == 4 )
    {
        if( inet_pton(AF_INET, InputIpStr, OutputBytes) == 1 )
        {
            RetVal = ERR_OK;
        }
        else
        {
            RetVal = ERR_FAIL;
        }
    }
    else if( IpUtils_IsValidIP(InputIpStr) == 6 )
    {
        if( inet_pton(AF_INET6, InputIpStr, OutputBytes) == 1 )
        {
            RetVal = ERR_OK;
        }
        else
        {
            RetVal = ERR_FAIL;
        }
    }
    else
    {
        RetVal = ERR_INVALID;
    }
    
    return RetVal;
}

char const *IpUtils_IpDecimal2Dotted(uint32_t IpNumber)
{
    // https://www.systutorials.com/docs/linux/man/3-inet_pton/
//    if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
//        perror("inet_ntop");
//        exit(EXIT_FAILURE);
//    }
}

#ifdef __cplusplus
std::string IpUtils_GetReversedIpAddressStr(const char *IpAddr)
{
    return "123";
}
#endif

#ifdef _TEST_
/*
 *      _____         _
 *     |_   _|__  ___| |_
 *       | |/ _ \/ __| __|
 *       | |  __/\__ \ |_
 *       |_|\___||___/\__|
 *
 */
#include <stdio.h>

uint8_t BCDToDecimal (uint8_t bcdByte)
{
    return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

uint8_t DecimalToBCD (uint8_t decimalByte)
{
    return (((decimalByte / 10) << 4) | (decimalByte % 10));
}

void IpUtils_TEST()
{
    char buffer[255];
    uint8_t arr[255];
    int result = 0;
    
    const char *IPv6_Valid = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
    const char *IPv6_Invalid = "2001";
    const char *IPv4_Valid = "192.168.1.1";
    const char *IPv4_Invalid = "123.456.789.012";
    
    printf("IpUtils Test:\n-------------------\n");
    printf("IpUtils_IsValidIP(\"%s\") -> %d\n", IPv6_Valid, IpUtils_IsValidIP(IPv6_Valid));
    printf("IpUtils_IsValidIP(\"%s\") -> %d\n", IPv6_Invalid, IpUtils_IsValidIP(IPv6_Invalid));
    printf("IpUtils_IsValidIP(\"%s\") -> %d\n", IPv4_Valid, IpUtils_IsValidIP(IPv4_Valid));
    printf("IpUtils_IsValidIP(\"%s\") -> %d\n", IPv4_Invalid, IpUtils_IsValidIP(IPv4_Invalid));
    printf("\n");
    
    STR_CLR(buffer);
    result = IpUtils_GetReversedIpAddressBytes(IPv4_Valid, (char *) buffer);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv4_Valid, result, buffer);
    STR_CLR(buffer);
    result = IpUtils_GetReversedIpAddressBytes(IPv4_Invalid, (char *) buffer);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv4_Invalid, result, buffer);
    STR_CLR(buffer);
    result = IpUtils_GetReversedIpAddressBytes(IPv6_Valid, (char *) buffer);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv6_Valid, result, buffer);
    STR_CLR(buffer);
    result = IpUtils_GetReversedIpAddressBytes(IPv6_Invalid, (char *) buffer);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv6_Invalid, result, buffer);
    printf("\n");
    
    ARR_CLR(arr);
    result = IpUtils_IpDottedToDecimal(IPv4_Valid, arr);
    printf("IpUtils_IpDottedToDecimal(\"%s\") -> Response code %d (%lu)\n", IPv4_Valid, result, (unsigned long) ARR4TOUINT32(arr));
    ARR_CLR(arr);
    result = IpUtils_IpDottedToDecimal(IPv4_Invalid, arr);
    printf("IpUtils_IpDottedToDecimal(\"%s\") -> Response code %d (%lu)\n", IPv4_Invalid, result, (unsigned long) ARR4TOUINT32(arr));
    
    
    
    ARR_CLR(arr);
    result = IpUtils_IpDottedToDecimal(IPv6_Valid, arr);
    
    
    printf("BDC: %d %d\n", BCDToDecimal(arr[0]),  BCDToDecimal(arr[1]));
    
    printf("IpUtils_IpDottedToDecimal(\"%s\") -> Response code %d (%lu)\n", IPv6_Valid, result, (unsigned long) ARR4TOUINT32(arr));
    
    
    
    ARR_CLR(arr);
    result = IpUtils_IpDottedToDecimal(IPv6_Invalid, arr);
    printf("IpUtils_IpDottedToDecimal(\"%s\") -> Response code %d (%lu)\n", IPv6_Invalid, result, (unsigned long) ARR4TOUINT32(arr));
    printf("\n");
    
}

#endif // _TEST_

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _IP_UTILS_H_
