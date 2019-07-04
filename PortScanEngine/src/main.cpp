#include <TimeUtils.h>

#include "PortScanEngine.h"
#include "Logger.h"

void OnIpScannedCb(IpScanResult *result)
{
    log("Finished scanning on {}. Total number of ports is {} which translates into {} open ports and {} closed ports.",
            result->GetIpAddress(), result->GetPortsRaw().size(), result->GetPortsOpen().size(), result->GetPortsClosed().size());
}

int main()
{
    LoggerInit();
    
    /* Scanned handler and callback event */
    PortScanEngine scanner(10, &OnIpScannedCb);
    vector<uint16_t>ports;
    for(uint16_t i = 1; i < 1024; i++)
        ports.push_back(i);
    scanner.StartScanTcp("127.0.0.1", ports);
    log("Scanner started...");
    
    while( scanner.GetStatus() != PortScanEngine::State::IDLE)
    {
        scanner.Tick();
        TimeUtils::SleepMs(1);
    }
}
