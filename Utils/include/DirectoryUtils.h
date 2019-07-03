/*  
 *  File      : DirectoryUtils.h
 *  Created on: 02.07.2019
 *      Author: curiosul
 */

#ifndef _DIRECTORYUTILS_H_
#define _DIRECTORYUTILS_H_

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <StdTypes.h>

bool DirectoryExists(const char *DirName)
{
    DIR *dir = opendir(DirName);
    if( dir )
    {
        /* Directory exists. */
        closedir(dir);
        return true;
    }
    else if( ENOENT == errno )
    {
        /* Directory does not exist. */
        return false;
    }
    else
    {
        /* opendir() failed for some other reason. */
        return false;
    }
}

bool DirectoryCreate(const char *DirName)
{
    const int dir_err = mkdir(DirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        return false;
    }
    else
    {
        return true;
    }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _DIRECTORYUTILS_H_
