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
    bool IsPortOpen(const char *HostnameOrIp, uint16_t port_no)
    {
        if( IsValidHostnameOrIp(HostnameOrIp) == false )
            return false;
        
        int sockfd;
        struct sockaddr_in serv_addr = {};
        struct hostent *server;
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if( sockfd < 0 )
        {
            return false;
        }
        
        server = gethostbyname(HostnameOrIp);
        if( server == NULL )
        {
            return false;
        }
    
        int synRetries = 2; // Send a total of 3 SYN packets => Timeout ~7s
        if( setsockopt(sockfd, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries)) < 0)
        {
            return false;
        }
        
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
        
        serv_addr.sin_port = htons(port_no);
        if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        {
            close(sockfd);
            return false;
        }
        else
        {
            close(sockfd);
            return true;
        }
    }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _PORTSCAN_H_
