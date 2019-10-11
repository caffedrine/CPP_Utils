/*  
 *  File      : Formats.h
 *  Created on: 12.10.2019
 *      Author: curiosul
 */

#ifndef _FORMATS_H_
#define _FORMATS_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <stdint.h>

uint8_t ConvertHexStringToDecString(uint8_t *hexIn, uint8_t *decOut);
uint8_t ConvertHexStringToBinString(uint8_t *hexIn, uint8_t *binOut);
uint8_t ConvertDecStringToHexString(uint8_t *decIn, uint8_t *hexOut);
uint8_t ConvertDecStringToBinString(uint8_t *decIn, uint8_t *binOut);
uint8_t ConvertBinStringToHexString(uint8_t *binIn, uint8_t *hexOut );
uint8_t ConvertBinStringToDecString(uint8_t *binIn, uint8_t *decOut );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _FORMATS_H_
