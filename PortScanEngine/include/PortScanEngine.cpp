#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "PortScanEngine.h"

PortScanEngine::PortScanEngine(size_t ThreadsNumber, On_IpScannedCbFunc_t cbFunc)
{
    this->threadPool = new ThreadPool(ThreadsNumber);
    this->On_IpScanCompletedCbFunc = cbFunc;
}

PortScanEngine::~PortScanEngine()
{
    if( this->threadPool != nullptr )
    {
        delete this->threadPool;
        this->threadPool = nullptr;
    }
}

void PortScanEngine::Tick()
{
    /* Check whether there some ports finished */
    if( !tasksResults.empty() )
    {
        auto it = tasksResults.begin();
        while( it != tasksResults.end() )
        {
            if( it->wait_for(std::chrono::seconds(0)) == std::future_status::ready ) /* Prevent from blocking */
            {
                std::tuple<std::string, IpScanResult::port_t> result = it->get();
                for( IpScanResult &ip : this->IpScanResults )
                {
                    if( ip.GetIpAddress() == std::get<0>(result) )
                    {
                        ip.EmplacePortScanResult(std::get<1>(result).Number, std::get<1>(result).State);
                    }
                }
                it = tasksResults.erase(it);
            }
            it++;
        }
    }
    
    /* Notify upper layers about new results being available */
    if( !IpScanResults.empty() )
    {
        auto it = IpScanResults.begin();
        while(it != IpScanResults.end())
        {
            if(it->IsScanFinished())
            {
                On_IpScanCompleted(&it.operator*());
                it = IpScanResults.erase(it);
            }
            it++;
        }
    }
}

PortScanEngine::State PortScanEngine::GetStatus()
{
    if( this->tasksResults.empty() && this->IpScanResults.empty() )
    {
        return State::IDLE;
    }
    else
    {
        return State::RUNNING;
    }
    
}

void PortScanEngine::On_IpScanCompleted(IpScanResult *ip)
{
    if( this->On_IpScanCompletedCbFunc != nullptr )
    {
        On_IpScanCompletedCbFunc(ip);
    }
}

bool PortScanEngine::StartScanTcp(string IPAddress, vector<uint16_t> PortsList)
{
    /* Scan all ports if not provided a list of selective ports */
    if( PortsList.empty() )
    {
        this->IpScanResults.emplace_back( IpScanResult( IPAddress ) );
        for( uint16_t i = 1; i < 65535; i++ )
        {
            IpScanResult::port_t currPort;
            currPort.Number = i;
            currPort.State = IpScanResult::PortSate::NOT_CHECKED;
    
            this->tasksResults.emplace_back (threadPool->enqueue(TaskScanPort, IPAddress, i) );
            IpScanResults.back().Ports.emplace_back( currPort );
        }
        return true;
    }
    else
    {
        this->IpScanResults.emplace_back( IpScanResult( IPAddress ) );
        for( uint16_t port: PortsList )
        {
            IpScanResult::port_t currPort;
            currPort.Number = port;
            currPort.State = IpScanResult::PortSate::NOT_CHECKED;
        
            this->tasksResults.emplace_back( threadPool->enqueue(TaskScanPort, IPAddress, port) );
            IpScanResults.back().Ports.emplace_back( currPort );
        }
        return true;
    }
}

bool PortScanEngine::IsResultAvailable()
{
    if(IpScanResults.empty())
        return false;
    
    /* Check whether there some results available */
    for( IpScanResult &result: IpScanResults )
    {
        if(result.IsScanFinished())
            return true;
    }
    return false;
}

IpScanResult PortScanEngine::PopResult()
{
    /* Check whether there some results available or not */
    if( !IpScanResults.empty() )
    {
        auto it = IpScanResults.begin();
        while(it != IpScanResults.end())
        {
            if(it->IsScanFinished())
            {
                IpScanResult ret = it.operator*();
                it = IpScanResults.erase(it);
                return ret;
            }
            it++;
        }
    }
    
    /* Throw exception if there is no result, forcing to check for availability first */
    throw Exception("PortScanEngine::PopResult()", "No results available!");
}

std::tuple<std::string, IpScanResult::port_t> PortScanEngine::TaskScanPort(const std::string &ip, uint16_t port)
{
    std::tuple<std::string, IpScanResult::port_t> resultTuple;
    
    struct sockaddr_in sa;
    if( inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 1 )
    {
        throw Exception("TaskScanPort(ip, port)", "Invalid IP address");
    }
    
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    bool IsPortOpen = false;
    int sfd = -1;
    int RetVal = 0;
    
    /* Obtain address(es) matching host/port */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */
    if( getaddrinfo(ip.c_str(), NULL, &hints, &result) != 0 )
    {
        throw Exception("TaskScanPort(ip, port)", "getaddrinfo()");
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
        if( setsockopt(sfd, IPPROTO_TCP, 7/*TCP_SYNCNT*/, &synRetries, sizeof(synRetries)) < 0 )
        {
            throw Exception("TaskScanPort(ip, port)", "setsockopt()");
        }
        
        ((struct sockaddr_in *) rp->ai_addr)->sin_port = htons(port);
        if( connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1 )
        {
            IsPortOpen = true;
            break;
        }
    }
    
    if( IsPortOpen == true && sfd >= 0 )
    {
        close(sfd);
    }
    freeaddrinfo(result);           /* No longer needed */
    
    std::get<0>(resultTuple) = ip;
    std::get<1>(resultTuple).Number = port;
    std::get<1>(resultTuple).State = (IsPortOpen == true ? IpScanResult::PortSate::OPEN : IpScanResult::PortSate::CLOSED);
    return resultTuple;
}

