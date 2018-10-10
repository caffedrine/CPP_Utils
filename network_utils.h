#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>
#include <vector>

namespace NetworkUtils
{
	
	bool isValidIPv4(std::string &ipv4)
	{
		const std::string address = ipv4;
		
		std::vector<std::string> arr;
		int k = 0;
		arr.push_back(std::string());
		for( std::string::const_iterator i = address.begin(); i != address.end(); ++i )
		{
			if( *i == '.' )
			{
				++k;
				arr.push_back(std::string());
				if( k == 4 )
				{
					return false;
				}
				continue;
			}
			if( *i >= '0' && *i <= '9' )
			{
				arr[k] += *i;
			}
			else
			{
				return false;
			}
			if( arr[k].size() > 3 )
			{
				return false;
			}
		}
		
		if( k != 3 )
		{
			return false;
		}
		for( int i = 0; i != 4; ++i )
		{
			const char *nPtr = arr[i].c_str();
			char *endPtr = 0;
			const unsigned long a = ::strtoul(nPtr, &endPtr, 10);
			if( nPtr == endPtr )
			{
				return false;
			}
			if( a > 255 )
			{
				return false;
			}
		}
		return true;
	}
	
	
	std::string GetReversedIpAddress(std::string ip)
	{
		std::vector<std::string> octeti = utils::strings::split(ip, '.');
		return (octeti[3] + "." + octeti[2] + "." + octeti[1] + "." + octeti[0]);
	}
	
	unsigned long IpStringToDecimal(std::string &ip)
	{
		unsigned long a, b, c, d, base10IP;
		sscanf(ip.c_str(), "%lu.%lu.%lu.%lu", &a, &b, &c, &d);
		
		// Do calculations to convert IP to base 10
		a *= 16777216;
		b *= 65536;
		c *= 256;
		base10IP = a + b + c + d;
		
		return base10IP;
	}
	
	std::string IpDecimal2Dotted(unsigned long ipAddr)
	{
		unsigned short a, b, c, d;
		std::ostringstream os;
		std::string ip = "";
		
		a = (ipAddr & (0xff << 24)) >> 24;
		b = (ipAddr & (0xff << 16)) >> 16;
		c = (ipAddr & (0xff << 8)) >> 8;
		d = ipAddr & 0xff;
		
		os << d << "." << c << "." << b << "." << a;
		ip = os.str();
		
		return ip;
	}
	
}

#endif