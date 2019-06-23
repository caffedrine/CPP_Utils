#include <vector>
#include <string>
#include "Utils/Logger.h"
#include "NetworkUtils.h"
#include "IpUtils.h"
#include "Ping.h"
#include "PortScan.h"

using std::vector;
using std::string;

#define log logger->info

int main()
{
	setup_logger();
    vector<string> IPs;
    IPs.emplace_back("1.2.3.4");
    IPs.emplace_back("62.4.15.44");
    IPs.emplace_back("direct.1way.pro");
    IPs.emplace_back("8.8.8.8");
    IPs.emplace_back("www.google.com");
    IPs.emplace_back("random_wrong_str");
    IPs.emplace_back("192.168.1.1");
    IPs.emplace_back("123.123.123.123");
    
    for( string &ip: IPs )
    {
//        log("'{}' is valid ip       : {}", ip, IpUtils::IsValidIPv4(ip.c_str()) );
//        log("'{}' is valid hostname : {}", ip, NetworkUtils::IsValidHostname(ip.c_str()) );
//        log("'{}' host2ip           : {}", ip, string(NetworkUtils::Hostname2Ip(ip.c_str())) );
//        log("'{}' ip2host           : {}", ip, string(NetworkUtils::Ip2Hostname(ip.c_str())) );
//        log("'{}' ping attempt      : {}", ip, NetworkUtils::Ping(ip.c_str(), 1000));
        log("'{}' port {} state     : {}", ip, 53, NetworkUtils::IsPortOpen(ip.c_str(), 53));
    }
}