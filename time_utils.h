#include <chrono>

/* Get time stamp in milliseconds */
static inline uint64_t millis()
{
	return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::
																		now().time_since_epoch()).count();
}

/* Get time stamp in microseconds */
static inline uint64_t micros()
{
	return (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
																		now().time_since_epoch()).count();
}

/* Get time stamp in nanoseconds */
static inline uint64_t nanos()
{
	return (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::
																	   now().time_since_epoch()).count();
}