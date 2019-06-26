#include <vector>
#include <string>
#include "Utils/Logger.h"
#include "NetworkUtils.h"
#include "IpUtils.h"
#include "Ping.h"
#include "PortScan.h"
#include "FileUtils.h"

using std::vector;
using std::string;

#define log logger->info

int main()
{
	setup_logger();
 
    vector<string> lines = FileReadLines("test123");
    
    log("Lines: {}", lines.size());
    for(string line: lines)
    {
        log("{}", line);
    }
}