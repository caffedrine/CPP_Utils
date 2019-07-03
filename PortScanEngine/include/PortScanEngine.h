/*  
 *  File      : PortScanEngine.h
 *  Created on: 02.07.2019
 *      Author: curiosul
 */

#ifndef _PORTSCANENGINE_H_
#define _PORTSCANENGINE_H_

#include <cstdint>
#include <utility>
#include <vector>
#include <list>
#include <tuple>
#include <Exception.h>
#include <ThreadPool.h>
#include <nss.h>
//#include <NetworkUtils.h>

using std::string;
using std::vector;

class IpScanResult
{
public:
    enum class PortSate
    {
        OPEN,
        CLOSED,
        UNKNOWN,
        NOT_CHECKED
    };
    
    typedef struct
    {
        uint16_t Number;
        PortSate State;
    }port_t;
    
    explicit IpScanResult(string Ip) : IpAddress(std::move(Ip)) {};
    
    string GetIpAddress() const
    {
        return this->IpAddress;
    }
    
    vector<port_t> GetPortsRaw() const
    {
        return this->Ports;
    }
    
    vector<uint16_t> GetPortsOpen() const
    {
        vector<uint16_t> result;
        for(port_t port: Ports)
        {
            if(port.State == PortSate::OPEN)
            {
                result.emplace_back(port.Number);
            }
        }
        return result;
    }
    
    vector<uint16_t> GetPortsClosed() const
    {
        vector<uint16_t> result;
        for(port_t port: Ports)
        {
            if(port.State == PortSate::CLOSED)
            {
                result.emplace_back(port.Number);
            }
        }
        return result;
    }
    
    PortSate GetPortState(uint16_t PortNumber) const
    {
        for(port_t port: Ports)
        {
            if(PortNumber == port.Number)
            {
                return port.State;
            }
        }
        throw Exception("GetPortState()", "Port does not exist or it was not scheduled to be scanned");
    }
    
private:
    string IpAddress = "";
    vector<port_t> Ports;
    
    /* Allow this class access to private variables */
    friend class PortScanEngine;
    
    void EmplacePortScanResult(uint16_t PortNumber, PortSate State)
    {
       for(port_t port: Ports)
       {
           if(port.Number == PortNumber)
           {
               port.State = State;
               return;
           }
       }
       throw Exception("IpScanResult::EmplaceResult()", "Given port was not supposed to be scanned!");
    }
};

/* Callback */
typedef void (*On_IpScannedCbFunc_t)(IpScanResult *);

class PortScanEngine
{
public:
    enum class  State
    {
        RUNNING,
        PAUSED,
        IDLE,
    };
    
    explicit PortScanEngine(size_t ThreadsNumber, On_IpScannedCbFunc_t = nullptr);
    ~PortScanEngine();
    bool StartScanTcp(string IPAddress, vector<uint16_t> PortsList = vector<uint16_t>());
    State GetStatus();
    bool IsResultAvailable();
    IpScanResult PopResult();
    void Tick();

protected:
    virtual void On_IpScanCompleted(IpScanResult *ip);

private:
    ThreadPool *threadPool = nullptr;
    std::list< std::future<std::tuple<std::string, IpScanResult::port_t>> > tasksResults;
    std::list<IpScanResult> IpScanResults;
    State ScannerStatus = State::IDLE;
    On_IpScannedCbFunc_t On_IpScanCompletedCbFunc = nullptr;
    
    /* This is passed to all threads */
    static std::tuple<std::string, IpScanResult::port_t> TaskScanPort( const std::string& ip, uint16_t port );
    
};

#endif // _PORTSCANENGINE_H_