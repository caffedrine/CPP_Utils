#include <vector>
#include <string>

#define _TEST_

#include "Utils/Logger.h"
#include "NetworkUtils.h"
#include "IpUtils.h"
#include "Ping.h"
#include "PortScan.h"
#include "FileUtils.h"

using std::vector;
using std::string;



int main()
{
	setup_logger();
	
	IpUtils_TEST();
}