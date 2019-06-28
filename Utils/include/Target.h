/*  
 *  File      : Target.h
 *  Created on: 6/28/2019
 *      Author: uib74520
 */

#ifndef _TARGET_H_
#define _TARGET_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      #define _TARGET_WINDOWS_
   #else
      #define _TARGET_WINDOWS_
   #endif

#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
         #define _TARGET_IPHONE_SIMULATOR_
    #elif TARGET_OS_IPHONE
        #define _TARGET_OS_IPHONE_
    #elif TARGET_OS_MAC
        #define _TARGET_OS_MAC_
    #else
    #   error "Unknown Apple platform"
    #endif

#elif __linux__
    #define _TARGET_LINUX_

#elif __unix__ // all unices not caught above
    #define _TARGET__UNIX_

#elif defined(_POSIX_VERSION)
#define _TARGET__POSIX_

#else
#   error "Unknown compiler"
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _TARGET_H_
