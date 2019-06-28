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