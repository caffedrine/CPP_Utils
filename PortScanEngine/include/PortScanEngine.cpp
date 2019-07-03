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
    /* Check whether there some results available or not */
    if( !results.empty() )
    {
        auto it = results.begin();
        while( it != results.end() )
        {
            if( it->wait_for(std::chrono::seconds(0)) == std::future_status::ready ) /* Prevent from blocking */
            {
                IpScanResult result = it->get();
                On_IpScanCompleted(&result);
                it = results.erase(it);
            }
            it++;
        }
    }
    
}

PortScanEngineStatus PortScanEngine::GetStatus()
{
    return this->scannerStatus;
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
    if(PortsList.empty())
    {
    
    }
}

bool PortScanEngine::IsResultAvailable()
{
    /* Check whether there some results available */
    return results.empty();
}

IpScanResult PortScanEngine::PopResult()
{
    /* Check whether there some results available or not */
    if( !results.empty() )
    {
        auto it = results.begin();
        while( it != results.end() )
        {
            if( it->wait_for(std::chrono::seconds(0)) == std::future_status::ready ) /* Prevent from blocking */
            {
                IpScanResult result = it->get();
                it = results.erase(it);
                return result;
            }
            it++;
        }
    }
    
    /* Throw exception if there is no result, forcing to check for availability first */
    throw Exception("PortScanEngine::PopResult()", "No results available!");
}

