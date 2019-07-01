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
uint8_t IpUtils_GetReversedIpAddressBytes(const char *IpStr, char **OutputIpStr);
uint8_t IpUtils_IpDottedToDecimalBytes(const char *InputIpStr, uint8_t **OutputBytes);
uint8_t IpUtils_IpDottedToDecimalString(const char *InputIpStr, char **OutputIpStr);
uint8_t IpUtils_IpDecimalToDottedString(const char *InputIpDecStr, char **OutputIpStr);
uint8_t IpUtils_GetHostFromUrl(const char *url, char **OutputStr);
/** C++ Functions */
#ifdef __cplusplus
std::string IpUtils_GetReversedIpAddressStr(std::string *IpAddr);
std::string IpUtils_IpDottedToDecimalStr(std::string *IpDotted);
std::string IpUtils_IpDecimalToDottedStr(std::string *IpDecimal);
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
uint8_t IpUtils_GetReversedIpAddressBytes(const char *InputIpStr, char **OutputIpStr)
{
    uint8_t RetVal = ERR_NOK;
    *OutputIpStr = NULL;
    
    if( OutputIpStr == NULL )
    {
        return ERR_INVALID;
    }
    
    if( IpUtils_IsValidIP(InputIpStr) == 4 )
    {
        char *reversed_ip = (char *) malloc(INET_ADDRSTRLEN + 1);
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
        inet_ntop(AF_INET, &addr, reversed_ip, INET_ADDRSTRLEN);
        reversed_ip[INET_ADDRSTRLEN] = '\0';
        /* Copy result to the output */
        (*OutputIpStr) = reversed_ip;
        RetVal = 0;
    }
    else if( IpUtils_IsValidIP(InputIpStr) == 6 )
    {
        *OutputIpStr = NULL;
        RetVal = ERR_NOT_IMPLEMENTED;
    }
    else
    {
        *OutputIpStr = NULL;
        RetVal = ERR_INVALID;
    }
    return RetVal;
}

uint8_t IpUtils_IpDottedToDecimalBytes(const char *InputIpStr, uint8_t **OutputBytes)
{
    uint8_t RetVal = ERR_NOK;
    *OutputBytes = NULL;
    
    if( OutputBytes == NULL )
    {
        return ERR_INVALID;
    }
    
    if( IpUtils_IsValidIP(InputIpStr) == 4 )
    {
        uint8_t *ip = (uint8_t *) malloc(INET_ADDRSTRLEN + 1);
        if( inet_pton(AF_INET, InputIpStr, ip) == 1 )
        {
            ip[INET_ADDRSTRLEN] = '\0';
            *OutputBytes = ip;
            RetVal = ERR_OK;
        }
        else
        {
            RetVal = ERR_FAIL;
            free(ip);
        }
    }
    else if( IpUtils_IsValidIP(InputIpStr) == 6 )
    {
        uint8_t *ip = (uint8_t *) malloc(INET6_ADDRSTRLEN + 1);
        if( inet_pton(AF_INET6, InputIpStr, ip) == 1 )
        {
            ip[INET6_ADDRSTRLEN] = '\0';
            *OutputBytes = ip;
            RetVal = ERR_OK;
        }
        else
        {
            RetVal = ERR_FAIL;
            free(ip);
        }
    }
    else
    {
        RetVal = ERR_INVALID;
    }
    
    return RetVal;
}

uint8_t IpUtils_IpDottedToDecimalString(const char *InputIpStr, char **OutputIpStr)
{
    uint8_t RetVal = ERR_NOK;
    *OutputIpStr = NULL;
    
    uint8_t IpVer = IpUtils_IsValidIP(InputIpStr);
    if( IpVer != 4 && IpVer != 6 )
    {
        return ERR_INVALID;
    }
    
    uint8_t *RetAddrBytes = NULL;
    uint8_t ReturnedVal = IpUtils_IpDottedToDecimalBytes(InputIpStr, &RetAddrBytes);
    if( ReturnedVal == ERR_OK && RetAddrBytes != nullptr )
    {
        if( IpVer == 4 )
        {
            uint32_t DecNumber = ARR4TOUINT32( RetAddrBytes );
            ssize_t DigitsNo = DIGITS_NO(DecNumber);
            char *tmp = (char *)malloc( DigitsNo + 1);
            int idx = 0;
            while(DecNumber != 0)
            {
                uint8_t digit = DecNumber % 10;
                DecNumber /= 10;
                tmp[DigitsNo-1-idx] = (char)digit + '0';
                idx++;
            }
            tmp[idx] = '\0';
            *OutputIpStr = tmp;
            RetVal = ERR_OK;
        }
        else if(IpVer == 6)
        {
            /* Not imlpemented for IPv6 */
            *OutputIpStr = NULL;
            RetVal = ERR_NOT_IMPLEMENTED;
        }
        else
        {
            *OutputIpStr = NULL;
            RetVal = ERR_INVALID;
        }
    }
    free(RetAddrBytes);
    return RetVal;
}

uint8_t IpUtils_IpDecimalToDottedString(const char *InputIpDecStr, char **OutputIpStr)
{
    uint8_t RetVal = ERR_NOK;
    *OutputIpStr = NULL;
    
    if( OutputIpStr == NULL )
    {
        return ERR_INVALID;
    }
    
    uint32_t DecInput = ToUInt(  (const char *)InputIpDecStr);
    if(str2int(&DecInput, InputIpDecStr, 10) != ERR_OK)
    {
        return RetVal;
    }
    
    /* Detect IP only by it's size :D */
    if( DecInput <= UINT32_MAX)
    {
    
    }
    
    if( IpUtils_IsValidIP(InputIpStr) == 4 )
    {
        char *reversed_ip = (char *) malloc(INET_ADDRSTRLEN + 1);
        
        /* Get the textual address into binary format */
        inet_pton(AF_INET, InputIpDecStr, &addr);
        /* And lastly get a textual representation back again */
        inet_ntop(AF_INET, &addr, reversed_ip, INET_ADDRSTRLEN);
        reversed_ip[INET_ADDRSTRLEN] = '\0';
        /* Copy result to the output */
        (*OutputIpStr) = reversed_ip;
        RetVal = 0;
    }
    else if( IpUtils_IsValidIP(InputIpStr) == 6 )
    {
        *OutputIpStr = NULL;
        RetVal = ERR_NOT_IMPLEMENTED;
    }
    else
    {
        *OutputIpStr = NULL;
        RetVal = ERR_INVALID;
    }
    return RetVal;
}

#ifdef __cplusplus
std::string IpUtils_GetReversedIpAddressStr(std::string *IpAddr)
{
    std::string result = "";
    
    char *RetStr = nullptr;
    uint8_t RetVal = IpUtils_GetReversedIpAddressBytes(IpAddr->c_str(), &RetStr);
    if( RetVal == ERR_OK && RetStr != nullptr )
    {
        result = std::string(RetStr);
    }
    free(RetStr);
    return result;
}

std::string IpUtils_IpDottedToDecimalStr(std::string *IpDotted)
{
    std::string result = "";
    
    uint8_t *RetStr = nullptr;
    uint8_t IpVer = IpUtils_IsValidIP(IpDotted->c_str());
    if( IpVer != 4 && IpVer != 6 )
    {
        return result;
    }
    
    uint8_t RetVal = IpUtils_IpDottedToDecimalBytes(IpDotted->c_str(), &RetStr);
    if( RetVal == ERR_OK && RetStr != nullptr )
    {
        if( IpVer == 4 )
        {
            result = std::string(std::to_string(ARR4TOUINT32(RetStr)));
        }
        else
        {
            /* Not imlpemented for IPv6 */
            result = "";
        }
        
        result = std::string((char *) RetStr);
    }
    free(RetStr);
    return result;
}

std::string IpUtils_IpDecimalToDottedStr(std::string *IpDecimal)
{
    return "not_implemented";
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

uint8_t IntegersArrayToDecimalStr(uint8_t *arr, ssize_t arr_len, char *OutputStr)
{
    uint8_t RetVal = ERR_INVALID;
    if( arr_len <= 0 )
    {
        RetVal = ERR_INVALID;
        return RetVal;
    }

//    if (arr_len < 8 ) {
//        return RetVal;
//    }
//
//
//    uint64_t* NoOne = (uint64_t*)&arr[0];
//    uint64_t* NoTwo = (uint64_t*)&arr[8];
//
//    uint8_t temp1;
//    uint8_t temp2;
//    uint8_t rest = 0u;
//
//    for ( int a = 0; a < 19u; a++ )
//    {
//        temp1 = (uint8_t)(*NoOne % (10 ^ a));
//        temp2 = (uint8_t)(*NoTwo % (10 ^ a));
//        OutputStr[a] = (uint8_t)(temp1 + temp2 + rest) + '0';
//    }
//    OutputStr[21] = '\0';
    
    
    strcpy(OutputStr, "To be implemented");
    return RetVal;
}

uint8_t CharsArrayAsDecimalToIntegersArray(uint8_t *arr, ssize_t arr_len, char *OutputStr)
{
    uint8_t RetVal = ERR_INVALID;
    if( arr_len <= 0 )
    {
        RetVal = ERR_INVALID;
        return RetVal;
    }
    
    strcpy(OutputStr, "To be implemented");
    return RetVal;
}

void IpUtils_TEST()
{
    char buffer[255];
    uint8_t arr[255];
    int result = 0;
    char *tmpPtr = NULL;
    
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
    
    result = IpUtils_GetReversedIpAddressBytes(IPv4_Valid, &tmpPtr);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv4_Valid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_GetReversedIpAddressBytes(IPv4_Invalid, &tmpPtr);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv4_Invalid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_GetReversedIpAddressBytes(IPv6_Valid, &tmpPtr);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv6_Valid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_GetReversedIpAddressBytes(IPv6_Invalid, &tmpPtr);
    printf("IpUtils_GetReversedIpAddressBytes(\"%s\") -> Response code %d (%s)\n", IPv6_Invalid, result, tmpPtr);
    free(tmpPtr);
    printf("\n");
    
    result = IpUtils_IpDottedToDecimalString(IPv4_Valid, (&tmpPtr) );
    printf("IpUtils_IpDottedToDecimalString(\"%s\") -> Response code %d (\"%s\")\n", IPv4_Valid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_IpDottedToDecimalString(IPv4_Invalid, &tmpPtr);
    printf("IpUtils_IpDottedToDecimalString(\"%s\") -> Response code %d (\"%s\")\n", IPv4_Invalid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_IpDottedToDecimalString(IPv6_Valid, &tmpPtr);
    printf("IpUtils_IpDottedToDecimalString(\"%s\") -> Response code %d (\"%s\")\n", IPv6_Valid, result, tmpPtr);
    free(tmpPtr);
    result = IpUtils_IpDottedToDecimalString(IPv6_Invalid, &tmpPtr);
    printf("IpUtils_IpDottedToDecimalString(\"%s\") -> Response code %d (\"%s\")\n", IPv6_Invalid, result, tmpPtr);
    free(tmpPtr);
    printf("\n");
    
    #ifdef __cplusplus
    printf("Testing C++ functions...\n\n");
    
    std::string IPv4_Valid_string = std::string(IPv4_Valid);
    std::string IPv4_Invalid_string = std::string(IPv4_Invalid);
    std::string IPv6_Valid_string = std::string(IPv6_Valid);
    std::string IPv6_Invalid_string = std::string(IPv6_Invalid);
    
    printf("IpUtils_GetReversedIpAddressStr(%s) -> '%s'\n", IPv4_Valid_string.c_str(), IpUtils_GetReversedIpAddressStr(&IPv4_Valid_string).c_str());
    printf("IpUtils_GetReversedIpAddressStr(%s) -> '%s'\n", IPv4_Invalid_string.c_str(), IpUtils_GetReversedIpAddressStr(&IPv4_Invalid_string).c_str());
    printf("IpUtils_GetReversedIpAddressStr(%s) -> '%s'\n", IPv6_Valid_string.c_str(), IpUtils_GetReversedIpAddressStr(&IPv6_Valid_string).c_str());
    printf("IpUtils_GetReversedIpAddressStr(%s) -> '%s'\n", IPv6_Invalid_string.c_str(), IpUtils_GetReversedIpAddressStr(&IPv6_Invalid_string).c_str());
    printf("\n");
    
    #endif
    
}

#endif // _TEST_

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _IP_UTILS_H_
