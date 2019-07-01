/*  
 *  File      : StdTypes.h
 *  Created on: 6/28/2019
 *      Author: caffedrine
 */

#ifndef _STDTYPES_H_
#define _STDTYPES_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#define ERR_OK                  0
#define ERR_NOK                 1
#define ERR_INVALID             2
#define ERR_NOT_IMPLEMENTED     3
#define ERR_FAIL                4

#ifdef _ENABLE_UINT128_
    #ifndef uint128_t
        typedef unsigned __int128 uint128_t;
    #endif
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _STDTYPES_H_
