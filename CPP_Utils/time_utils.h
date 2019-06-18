#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace TimeUtils
{
/* Get timestamp in milliseconds */
static inline long millis()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

/* Get timestamp in microseconds */
static inline long  micros()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

/* Get timestamp in nanoseconds */
static inline long nanos()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

/* Pause current thread */
static inline void SleepMs(long DurationMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(DurationMs));
}

/* Current timestamp for logging */
static std::string formatted_time_now()
{
	using namespace std::chrono;
	
	// get current time
	auto now = system_clock::now();
	
	// get number of milliseconds for the current second
	// (remainder after division into seconds)
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
	
	// convert to std::time_t in order to convert to std::tm (broken time)
	auto timer = system_clock::to_time_t(now);
	
	// convert to broken time
	std::tm bt = *std::localtime(&timer);
	
	std::ostringstream oss;
	
	oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
	oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
	
	return oss.str();
}

class Timer
{
public:
	void Restart()
	{
		this->_PreviousUs.store( std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() );
		this->_IsRunning.store(true);
	}
	
	void Stop()
	{
		this->_IsRunning.store(false);
	}
	
	bool IsRunning()
	{
		return this->_IsRunning.load();
	}
	
	long ElapsedMs()
	{
		return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - (this->_PreviousUs.load()/1000u) );
		
	}
	
	long ElapsedUs()
	{
		return (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - this->_PreviousUs.load());
	}
	
private:
	std::atomic<long> _PreviousUs;
	std::atomic<bool> _IsRunning;
};/*Timer class */

}/* Namespace */

#endif //TIME_UTILS_H