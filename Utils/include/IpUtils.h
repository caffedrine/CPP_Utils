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

#include <arpa/inet.h>

/*
 *      ____            _                 _   _
 *     |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___
 *     | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 *     | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 *     |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
 */

/** Functions */
int8_t IpUtils_IsValidIP(const char *IpStr);
char const * IpUtils_GetReversedIpAddressBytes(const char *IpStr);
uint8_t const * IpUtils_IpDottedToBytes(const char *IPv4);
char const * IpUtils_IpDecimal2Dotted(uint32_t IpNumber);
char const * IpUtils_GetHostFromUrl(const char *url);

/*
 *      ____        __ _       _ _   _
 *     |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
 *     | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 *     | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 *     |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
 *
 */

/** Check whether given string is a valid IPv4 or IPv6 */
int8_t IpUtils_IsValidIP(const char *IpStr)
{
    struct sockaddr_in sa;
    
    /* Is valid IPv4? */
    if( inet_pton(AF_INET, IpStr, &(sa.sin_addr)) == 1 )
        return 4;
    
    /* Is valid IPv6? */
    if(inet_pton(AF_INET6, IpStr, &(sa.sin_addr)) == 1)
        return 6;
    
    return 0;
}

/** Return the given string using bytes in reversed order */
char const * IpUtils_GetReversedIpAddressBytes(const char *IpStr)
{
    char *resultPtr = NULL;
    
    if( IpUtils_IsValidIP(IpStr) == 4)
    {
        char reversed_ip[INET_ADDRSTRLEN];
        in_addr_t addr;
        /* Get the textual address into binary format */
        inet_pton(AF_INET, IpStr, &addr);
        /* Reverse the bytes in the binary address */
        addr =
                ((addr & 0xff000000) >> 24) |
                ((addr & 0x00ff0000) >> 8) |
                ((addr & 0x0000ff00) << 8) |
                ((addr & 0x000000ff) << 24);
        /* And lastly get a textual representation back again */
        inet_ntop(AF_INET, &addr, reversed_ip, sizeof(reversed_ip));
        resultPtr = reversed_ip;
    }
    else if( IpUtils_IsValidIP(IpStr) == 6)
    {
        /// IPv6 not implemented
        resultPtr = (char *)"not_implemented";
    }
    return resultPtr;
}

uint8_t const * IpUtils_IpDottedToBytes(const char *IpStr)
{
    uint8_t *retP = NULL;
    uint8_t buff[16];
    
    if( IpUtils_IsValidIP(IpStr) == 4 )
    {
        struct sockaddr_in sa = {};
        if( inet_pton(AF_INET, IpStr, buff) == 1)
        {
            retP = buff;
        }
    }
    else if( IpUtils_IsValidIP(IpStr) == 6 )
    {
        struct sockaddr_in sa = {};
        if( inet_pton(AF_INET6, IpStr, buff) == 1)
        {
            retP = buff;
        }
        retP = buff;
    }

    return retP;
}

char const * IpUtils_IpDecimal2Dotted(uint32_t IpNumber)
{
    // https://www.systutorials.com/docs/linux/man/3-inet_pton/
//    if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
//        perror("inet_ntop");
//        exit(EXIT_FAILURE);
//    }
}


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

uint32_t ARR4TOUINT32(const uint8_t *IPv4_bytes)
{
    if( IPv4_bytes == NULL)
        return 0;
    
    return ((IPv4_bytes[0] << 24) | (IPv4_bytes[1] << 16) | (IPv4_bytes[2] << 8) | IPv4_bytes[3]);
}

const char *ARR16TOSTR(const uint8_t *IPv6_bytes)
{
    if(IPv6_bytes == NULL)
        return NULL;
    char buff[40];
    
    uint32_t msb = ARR4TOUINT32( (const uint8_t *)&IPv6_bytes[12]);
    
    sprintf(buff, "%d", msb);
    
    char *retP = buff;
    return retP;
}

void IpUtils_TEST()
{
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
    
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> %s\n", IPv6_Valid, IpUtils_GetReversedIpAddressBytes(IPv6_Valid));
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> %s\n", IPv6_Invalid, IpUtils_GetReversedIpAddressBytes(IPv6_Invalid));
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> %s\n", IPv4_Valid, IpUtils_GetReversedIpAddressBytes(IPv4_Valid));
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> %s\n", IPv4_Invalid, IpUtils_GetReversedIpAddressBytes(IPv4_Invalid));
    printf("\n");
    
    printf("IpUtils_IpDottedToBytes(\"%s\") -> %lu\n", IPv4_Valid, (unsigned long int) ARR4TOUINT32( IpUtils_IpDottedToBytes(IPv4_Valid) ) );
    printf("IpUtils_IpDottedToBytes(\"%s\") -> %lu\n", IPv4_Invalid, (unsigned long int)ARR4TOUINT32( IpUtils_IpDottedToBytes(IPv4_Invalid) ));
    printf("IpUtils_IpDottedToBytes(\"%s\") -> \"%s\"\n", IPv6_Valid, ARR16TOSTR(IpUtils_IpDottedToBytes(IPv6_Valid)));
    printf("IpUtils_IpDottedToBytes(\"%s\") -> \"%s\"\n", IPv6_Invalid, ARR16TOSTR(IpUtils_IpDottedToBytes(IPv6_Invalid)));
    printf("\n");
    
//    printf("IpUtils_IpDecimal2Dotted(\"%lu\") -> %s\n", (long unsigned int) IpUtils_Ip4DottedToDecimal(IPv6_Valid), IpUtils_IpDecimal2Dotted(
//            IpUtils_Ip4DottedToDecimal(IPv6_Valid)));
//    printf("IpUtils_IpDecimal2Dotted(\"%lu\") -> %s\n", (long unsigned int) IpUtils_Ip4DottedToDecimal(IPv6_Invalid), IpUtils_IpDecimal2Dotted(
//            IpUtils_Ip4DottedToDecimal(IPv6_Invalid)));
//    printf("IpUtils_Ip4DottedToDecimal(\"%lu\") -> %s\n", (long unsigned int) IpUtils_Ip4DottedToDecimal(IPv4_Valid), IpUtils_IpDecimal2Dotted(
//            IpUtils_Ip4DottedToDecimal(IPv4_Valid)));
//    printf("IpUtils_Ip4DottedToDecimal(\"%lu\") -> %s\n", (long unsigned int) IpUtils_Ip4DottedToDecimal(IPv4_Invalid), IpUtils_IpDecimal2Dotted(
//            IpUtils_Ip4DottedToDecimal(IPv4_Invalid)));
//    printf("\n");
    
}

#endif // _TEST_


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _IP_UTILS_H_
