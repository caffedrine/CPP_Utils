/*  
 *  File      : ${HEADER_FILENAME}
 *  Created on: 22.06.2019
 *      Author: curiosul
 */

#ifndef _IP_UTILS_H_
#define _IP_UTILS_H_

#include <string>
#include <vector>
#include <arpa/inet.h>

using std::string;
using std::vector;

namespace IpUtils
{
    /* Utils used within this module */
    namespace _Utils
    {
        /* Helper functions */
        vector<string> StringSplit(string str, string token);
        std::string StringEraseAllSubStr(const std::string *mainStr, const std::string *toErase);
    }
    bool IsValidIPv4(const std::string *ipv4);
    std::string GetReversedIpAddress(const std::string *ip);
    unsigned long IpStringToDecimal(const std::string *ip);
    std::string IpDecimal2Dotted(unsigned long ipAddr);
    std::string GetHostFromUrl(const std::string *url);
}

namespace IpUtils
{
    /* Utils used within this module */
    namespace _Utils
    {
        /* Helper functions */
        vector<string> StringSplit(string str, string token)
        {
            vector<string> result;
            while( str.size() )
            {
                int index = str.find(token);
                if( index != string::npos )
                {
                    result.push_back(str.substr(0, index));
                    str = str.substr(index + token.size());
                    if( str.size() == 0 )result.push_back(str);
                }
                else
                {
                    result.push_back(str);
                    str = "";
                }
            }
            return result;
        }
        std::string StringEraseAllSubStr(const std::string *mainStr, const std::string *toErase)
        {
            std::string result = *mainStr;
            size_t pos = std::string::npos;
            
            // Search for the substring in string in a loop untill nothing is found
            while( (pos = result.find(*toErase)) != std::string::npos )
            {
                // If found then erase it from string
                result.erase(pos, toErase->length());
            }
            return result;
        }
    }
    
    bool IsValidIPv4(const char *IPv4)
    {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, IPv4, &(sa.sin_addr));
        if(result != 0)
            return true;
        return false;
    }
    
    std::string GetReversedIpAddress(const std::string *ip)
    {
        std::vector<std::string> octeti = _Utils::StringSplit(*ip, ".");
        return (octeti[3] + "." + octeti[2] + "." + octeti[1] + "." + octeti[0]);
    }
    
    unsigned long IpStringToDecimal(const char *IPv4)
    {
        unsigned long a, b, c, d, base10IP;
        sscanf(IPv4, "%lu.%lu.%lu.%lu", &a, &b, &c, &d);
        
        // Do calculations to convert IP to base 10
        a *= 16777216;
        b *= 65536;
        c *= 256;
        base10IP = a + b + c + d;
        
        return base10IP;
    }
    
    std::string IpDecimal2Dotted(unsigned long ipAddr)
    {
        unsigned short a, b, c, d;
        std::ostringstream os;
        std::string ip = "";
        
        a = (ipAddr & (0xff << 24)) >> 24;
        b = (ipAddr & (0xff << 16)) >> 16;
        c = (ipAddr & (0xff << 8)) >> 8;
        d = ipAddr & 0xff;
        
        os << d << "." << c << "." << b << "." << a;
        ip = os.str();
        
        return ip;
    }
    
}

#endif // _IP_UTILS_H_
