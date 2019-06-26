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
 
	FileWrite("test123", (const char *)"123", 3);
    FileAppend("test123", (const char *)"_456", 4);
    
    logger->info( "Recv: {}", FileRead("test123", 2) );
    
}