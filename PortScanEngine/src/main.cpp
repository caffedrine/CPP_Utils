#include <TimeUtils.h>
#include <FileUtils.h>

#include "PortScanEngine.h"
#include "Logger.h"

void OnIpScannedCb(IpScanResult *result)
{
    static int i = 0;
    log("{}. Finished scanning on {}. Total number of ports is {} which translates into {} open ports and {} closed ports.",
            ++i, result->GetIpAddress(), result->GetPortsRaw().size(), result->GetPortsOpen().size(), result->GetPortsClosed().size());
}

int main(int argc, char *argv[])
{
    LoggerInit();
    
    /** Scanner handler and callback event */
    PortScanEngine scanner(2000, &OnIpScannedCb);
    
    /** Select all ports */
    vector<uint16_t> ports;
    for( uint16_t i = 1; i < 1002; i++ )
        ports.push_back(i);
    
    /** IPs to be scanned */
    std::vector<string> ips;
    if(argc > 1)
    {
        for(int i = 1; i < argc; i++)
            ips.emplace_back(argv[i]);
    }
    else
    {
        /** Read all IP addresses from a file */
        ips = FileReadLines("/home/curiosul/Desktop/ips.txt");
        /* Reverse vector in order to use pop_back() and back() methods */
        std::reverse(ips.begin(), ips.end());
    }
    log("Scanner started on {} ip counting {}  ports...", ips.size(), ips.size() * ports.size());
    
    /** Schedule scans and wait for results*/
    while(true)
    {
        /* Add tasks as soon as queue is feed */
        if(!ips.empty() && scanner.StartScanTcp(ips.back(), ports) )
        {
            ips.pop_back();
        }
    
        if( scanner.GetStatus() == PortScanEngine::State::IDLE && ips.empty() )
        {
            break;
        }
        
        /* Async processing */
        scanner.Tick();
        
        /* Prevent high CPU load */
        TimeUtils::SleepMs(1);
    }
    
    return 0;
}
