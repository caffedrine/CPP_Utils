#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>
#include <vector>
#include <StringUtils.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

namespace NetworkUtils
{
    namespace _Utils
    {
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
        
        bool IsValidIPv4(const char *IPv4)
        {
            struct sockaddr_in sa;
            int result = inet_pton(AF_INET, IPv4, &(sa.sin_addr));
            if(result != 0)
                return true;
            return false;
        }
        
    }
    
    bool IsValidHostnameFormat(const char *hostname)
    {
        return true;
    }
    
    bool IsValidHostname(const char *hostname)
    {
        struct addrinfo hints = {}, *res;
        
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags |= AI_CANONNAME;
        
        if( getaddrinfo(hostname, NULL, &hints, &res) != 0 )
        {
            return false;
        }
        return true;
    }
    
    bool IsValidHostnameOrIp(const char *HostnameOrIp)
    {
        if( IsValidHostname(HostnameOrIp) == false && _Utils::IsValidIPv4(HostnameOrIp) == false )
            return false;
        return true;
    }
    
    char const *Ip2Hostname(const char *ip)
    {
        struct hostent *hent;
        struct in_addr addr = {};
        char result[255];
        
        if(_Utils::IsValidIPv4(ip) == false)
            return "";
        
        if( !inet_aton(ip, &addr) )
        {
            return (ip);
        }
        if( (hent = gethostbyaddr((char *) &(addr.s_addr), sizeof(addr.s_addr), AF_INET)) )
        {
            return hent->h_name;
        }
        return "";
    }
    
    char const *Hostname2Ip(const char *hostname)
    {
        /* If it is already a valid IP just return it */
        struct sockaddr_in sa = {};
        int isIp = inet_pton(AF_INET, hostname, &(sa.sin_addr));
        if( isIp != 0 )
            return hostname;
        
        /* Return if hostname is invalid */
        if( !IsValidHostnameFormat(hostname) )
            return "invalid_hostname";
        
        struct addrinfo hints = {}, *res;
        char addrstr[100];
        void *ptr;
        
        char result[116]; // max 1 IPv4 and 1IPv6 and ','
        memset(result, '\0', sizeof(result));
        
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags |= AI_CANONNAME;
        
        if( getaddrinfo(hostname, NULL, &hints, &res) != 0 )
        {
            return "";
        }
        
        int ResultsNo = 0;
        while( res )
        {
            inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);
            
            switch( res->ai_family )
            {
                case AF_INET:
                    ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                    break;
                case AF_INET6:
                    ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                    break;
            }
            inet_ntop(res->ai_family, ptr, addrstr, 100);
            if( ResultsNo > 0 )
            {
                strcat(result, ",");
            }
            
            if( res->ai_family == PF_INET6 )
            {
                strcat(result, addrstr);
            }
            else
            {
                strcpy(result, addrstr);
            }
            res = res->ai_next;
            ResultsNo++;
        }
        
        char *ret = result;
        return ret;
    }
    
    std::string GetHostFromUrl(const std::string *url)
    {
        std::string urlcopy = *url;
        
        urlcopy = _Utils::StringEraseAllSubStr(&urlcopy, (std::string *) "http://");
        urlcopy = _Utils::StringEraseAllSubStr(&urlcopy, (std::string *) "https://");
        urlcopy = urlcopy.substr(0, urlcopy.find("/"));
        
        return urlcopy;
    }
    
}

#endif