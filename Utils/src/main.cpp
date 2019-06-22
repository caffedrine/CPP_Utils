#include <vector>
#include <string>
#include "Utils/Logger.h"
#include "NetworkUtils.h"
#include "IpUtils.h"

using std::vector;
using std::string;

#define log logger->info

int main()
{
	setup_logger();
    vector<string> IPs;
    IPs.emplace_back("62.4.15.44");
    IPs.emplace_back("direct.1way.pro");
    IPs.emplace_back("8.8.8.8");
    IPs.emplace_back("www.google.com");
    IPs.emplace_back("random_wrong_str");
    
    
    for( string &ip: IPs )
    {
        log("'{}' is valid ip       : {}", ip, IpUtils::IsValidIPv4(ip.c_str()) );
        log("'{}' is valid hostname : {}", ip, NetworkUtils::IsValidHostname(ip.c_str()) );
        log("'{}' host2ip           : {}\n", ip, string(NetworkUtils::Hostname2Ip(ip.c_str())) );
    }
}