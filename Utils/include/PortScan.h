/*  
 *  File      : PortScan.h
 *  Created on: 23.06.2019
 *      Author: curiosul
 */

#ifndef _PORTSCAN_H_
#define _PORTSCAN_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

namespace NetworkUtils
{
    bool IsTcpPortOpen(const char *HostnameOrIp, uint16_t port_no)
    {
        if( IsValidHostnameFormat(HostnameOrIp) == false && _Utils::IsValidIPv4(HostnameOrIp) == false )
            return false;
        
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        bool IsPortOpen = false;
        int sfd = -1;
        
        
        /* Obtain address(es) matching host/port */
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_STREAM; /* TCP socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */
        if( getaddrinfo(HostnameOrIp, NULL, &hints, &result) != 0 )
        {
            return false;
        }
        
        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully connect(2).
           If socket(2) (or connect(2)) fails, we (close the socket
           and) try the next address. */
        
        for( rp = result; rp != NULL; rp = rp->ai_next )
        {
            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if( sfd == -1 )
            {
                continue;
            }
            
            int synRetries = 1; // Send a total of 3 SYN packets => Timeout ~7s
            if( setsockopt(sfd, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries)) < 0 )
            {
                return false;
            }
            
            ((struct sockaddr_in *) rp->ai_addr)->sin_port = htons(port_no);
            if( connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1 )
            {
                IsPortOpen = true;
                break;
            }
        }
        
        if( IsPortOpen == true && sfd >= 0 )
            close(sfd);
        
        freeaddrinfo(result);           /* No longer needed */
        
        return IsPortOpen;
    }
    
    bool IsUdpPortOpen(const char *HostnameOrIp, uint16_t port_no)
    {
        /* Not implemented */
        return false;
        
        
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int sfd = -1;
        bool IsPortOpen = false;
        
        /* Obtain address(es) matching host/port */
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */
        
        if( getaddrinfo(HostnameOrIp, NULL, &hints, &result) != 0 )
        {
            return false;
        }
        
        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully connect(2).
           If socket(2) (or connect(2)) fails, we (close the socket
           and) try the next address. */
        
        for( rp = result; rp != NULL; rp = rp->ai_next )
        {
            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if( sfd == -1 )
            {
                continue;
            }
    
            int synRetries = 1; // Send a total of 3 SYN packets => Timeout ~7s
            if( setsockopt(sfd, IPPROTO_UDP, 0, &synRetries, sizeof(synRetries)) < 0 ) /* Check */
            {
                return false;
            }
    
            ((struct sockaddr_in *) rp->ai_addr)->sin_port = htons(port_no);
            if( connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1 )
            {
                /* Additional checks to be added */
                IsPortOpen = true;
                
                break;                  /* Success */
            }
            
            
        }
        
        if( IsPortOpen == true && sfd >= 0 )
        {               /* No address succeeded */
            close(sfd);
        }
        return IsPortOpen;
    }
    
    char const *GetOpenTcpPorts(const char *HostnameOrIp)
    {
        if( IsValidHostnameOrIp(HostnameOrIp) == false )
            return "";
        
        char ports_status[65535 + 1];
        
        char port[65535];
    }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _PORTSCAN_H_
