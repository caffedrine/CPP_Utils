#include <iostream>
#include <chrono>
#include <thread>

#include "TcpServerAsync.h"

int main(int argc , char *argv[])
{
	std::cout << "---STARTED---" << std::endl;
	TcpServerAsync server(1337, 2);
	
	while(true)
	{
		int c = getchar();
		
		if(c == '1') server.Start();
		else if (c == '0') server.Stop();
		
		std::this_thread::sleep_for( std::chrono::milliseconds(1) );
	}
	return 0;
} 
