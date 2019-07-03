#include <TimeUtils.h>

#include "PortScanEngine.h"
#include "Logger.h"

void OnIpScannedCb(IpScanResult *result)
{
    log("Finished scanning on {} and found {} open ports.", result->GetIpAddress(), result->GetPortsOpen().size());
}

int main()
{
    LoggerInit();
    
    /* Scanned handler and callback event */
    PortScanEngine scanner(1000, &OnIpScannedCb);
    scanner.StartScanTcp("127.0.0.1");
    
    while( true ) // scanner.GetStatus() == PortScanEngine::State::IDLE)
    {
        scanner.Tick();
        TimeUtils::SleepMs(1);
    }
}
