#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <condition_variable>

# include <atomic>


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
	explicit Timer() = default;
	
	
	/**
 	 * @brief Restart the counter
 	 */
	void Restart()
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		{
			this->_PreviousUs = this->micros();
			this->_IsRunning = true;
		}
		mlock.unlock();
		_cond.notify_one();
	}
	
	/**
	 * @brief Stop the timer
	 */
	void Stop()
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		{
			this->_IsRunning = false;
		}
		mlock.unlock();
		_cond.notify_one();
	}
	
	
	/**
	 * @brief Check whether counter is started or not
	 * @return true if timer is running, false otherwise
	 */
	bool IsRunning()
	{
		std::unique_lock<std::mutex> mlock(this->_mutex);
		bool tmp = _IsRunning;
		mlock.unlock();
		_cond.notify_one();
		return tmp;
	}
	
	/**
	 * @brief Calculate number of elapsed milliseconds from current timestamp
	 * @return Return elapsed milliseconds
	 */
	long ElapsedMs()
	{
		std::unique_lock<std::mutex> mlock(this->_mutex);
		auto tmp = _PreviousUs;
		mlock.unlock();
		_cond.notify_one();
		return ( this->millis() - (tmp/1000u) );
	}
	
	/**
	 * @brief Calculate number of elapsed microseconds from current timestamp
	 * @return Return elapsed microseconds
	 */
	long ElapsedUs()
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		auto tmp = _PreviousUs;
		mlock.unlock();
		_cond.notify_one();
		return ( this->micros() - tmp );
	}
	
private:
	/** Timer's state */
	bool _IsRunning = false;
	/** Thread sync for read/write */
	std::mutex _mutex;
	std::condition_variable _cond;
	/** Remember when timer was stated */
	long _PreviousUs = 0;
	
	std::atomic<long> a;
	
	inline long  micros()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
	
	inline long millis()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
};

}

#endif //TIME_UTILS_H