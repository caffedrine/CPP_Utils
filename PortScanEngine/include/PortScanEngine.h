/*  
 *  File      : PortScanEngine.h
 *  Created on: 02.07.2019
 *      Author: curiosul
 */

#ifndef _PORTSCANENGINE_H_
#define _PORTSCANENGINE_H_

#include <cstdint>
#include <vector>
#include <list>
#include <Exception.h>
#include <ThreadPool.h>
//#include <NetworkUtils.h>

using std::string;
using std::vector;

enum class  PortScanEngineStatus
{
    RUNNING,
    PAUSED,
    IDLE
};

class IpScanResult
{
public:
    IpScanResult(string Ip)
    {
        PortsOpen[10] = 5;
    }
    
    string GetIpAddress()
    {
        return this->IpAddress;
    }
    
    
    
private:
    string IpAddress = "";
    vector<uint16_t> PortsOpen;
    vector<uint16_t> PortsClosed;
    
};

/* Callback */
typedef void (*On_IpScannedCbFunc_t)(IpScanResult *);

class PortScanEngine
{
public:
    explicit PortScanEngine(size_t ThreadsNumber, On_IpScannedCbFunc_t = nullptr);
    ~PortScanEngine();
    bool StartScanTcp(string IPAddress, vector<uint16_t> PortsList = vector<uint16_t>());
    PortScanEngineStatus GetStatus();
    bool IsResultAvailable();
    IpScanResult PopResult();
    void Tick();

protected:
    virtual void On_IpScanCompleted(IpScanResult *ip);

private:
    ThreadPool *threadPool = nullptr;
    std::list< std::future<IpScanResult> > results;
    PortScanEngineStatus scannerStatus = PortScanEngineStatus::IDLE;
    On_IpScannedCbFunc_t On_IpScanCompletedCbFunc = nullptr;
    
};

#endif // _PORTSCANENGINE_H_