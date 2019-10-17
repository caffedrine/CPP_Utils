#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <iostream>
#include <chrono>
#include <thread>
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
    Timer()
    {
        this->Restart();
    }
    
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
	
	std::string ElapsedTimeString()
    {
        long Hours = 0, Minutes = 0, Seconds = 0, Milliseconds = 0;
        
        if(this->ElapsedMs() >= 1000)
        {
            Seconds = this->ElapsedMs() / 1000;
            Milliseconds = this->ElapsedMs() % 1000;
        }
        
        if(Seconds >= 60)
        {
            Minutes = Seconds / 60;
            Seconds = Seconds % 60;
        }
        
        if(Minutes >= 60)
        {
            Hours = Minutes/60;
            Minutes = Minutes%60;
        }
        
        std::string result;
        
        if(Hours < 9) result += "0";
        result += std::to_string(Hours);
        
        result += ":";
        
        if(Minutes < 9) result += "0";
        result += std::to_string(Minutes);
        
        result += ":";
        
        if(Seconds < 9) result += "0";
        result += std::to_string(Seconds);
        
        result += ".";
        
        result += std::to_string(Milliseconds);
        
        return result;
    }
	
private:
	std::atomic<long> _PreviousUs;
	std::atomic<bool> _IsRunning;
};/*Timer class */

}/* Namespace */

#endif //TIME_UTILS_H