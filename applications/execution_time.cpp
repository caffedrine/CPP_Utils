#include <iostream>
#include "../CPP_Utils/time_utils.h"

using namespace std;

int calc_if(int a)
{
	return (((a % 10) < 6) ? (a - (a % 10)) : (a + (10 - (a % 10))));
}

int calc_math(int a)
{
	return (((a % 10) < 6) * (a - (a % 10))) + (((a % 10) > 5) * (a + (10 - (a % 10))));
}

int main()
{
	int a = 1783;
	int rezultat;
	uint64_t start, stop, duration;
	
	start = nanos();
	rezultat = calc_if(a);
	stop = nanos();
	std::cout << "Ifs:\tb = " << rezultat << endl;
	cout << "[" << formatted_time_now() << "] " << "Operation done in: " << (stop-start) << " nanoseconds\n\n";
	
	
	start = nanos();
	rezultat = calc_if(a);
	stop = nanos();
	std::cout << "Math:\tb = " << rezultat << endl;
	cout << "Operation done in: " << (stop-start) << " nanoseconds\n\n";
	
	
	return 0;
}
