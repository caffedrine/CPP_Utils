#include <TimeUtils.h>

#include "PortScanEngine.h"
#include "Logger.h"

void OnIpScannedCb(IpScanResult *result)
{
    log("Finished scanning on {}", result->GetIpAddress());
}

int main()
{
    LoggerInit();
    
    PortScanEngine scanner(1000, &OnIpScannedCb);
    
    for(;;)
    {
        scanner.Tick();
        /* If scan is finished then break the loop */
        if(scanner.GetStatus() == PortScanEngineStatus::IDLE)
        {
            break;
        }
        
        TimeUtils::SleepMs(1);
    }
    
}
