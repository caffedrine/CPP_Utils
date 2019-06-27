/*  
 *  File      : FileUtils.h
 *  Created on: 26.06.2019
 *      Author: curiosul
 */

#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <fstream>

#ifdef __cplusplus
extern "C" { // Allow module to be used within a C++ application
#endif

#ifdef __cplusplus
    #include <vector>
    #include <string>
#endif

bool FileExists(const char *Filename)
{
    if( access(Filename, F_OK) != -1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool FileClear(const char *Filename)
{
    if( !FileExists(Filename) )
        return false;
    
    FILE *f = fopen(Filename, "w");
    if( f )
    {
        fclose(f);
        return true;
    }
    else
    {
        return false;
    }
}

bool FileCreate(const char *Filename)
{
    FILE *f = fopen(Filename, "w");
    if( f )
    {
        fclose(f);
        return true;
    }
    else
    {
        return false;
    }
}

ssize_t FileAppend(const char *Filename, const char *dataPtr, ssize_t length)
{
    if( !FileExists(Filename) )
        return -1;
    
    FILE *f = fopen(Filename, "a");
    ssize_t writtenBytes = fwrite(dataPtr, sizeof(char), length, f);
    fclose(f);
    return writtenBytes;
}

ssize_t FileWrite(const char *Filename, const char *dataPtr, ssize_t length)
{
    if( !FileExists(Filename) )
    {
        if( !FileCreate(Filename) )
        {
            return -1;
        }
    }
    
    FILE *f = fopen(Filename, "w");
    ssize_t writtenBytes = fwrite(dataPtr, sizeof(char), length, f);
    fclose(f);
    return writtenBytes;
}

std::string FileReadString(const char *Filename, ssize_t MaxLen = 0)
{
    if( !FileExists(Filename) )
        return NULL;
    
    char *buffer = NULL;
    long length = 0;
    FILE *f = fopen(Filename, "rb");
    
    if( f )
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        if( length > MaxLen && MaxLen > 0 )
            length = MaxLen;
        fseek(f, 0, SEEK_SET);
        buffer = (char *) malloc(length);
        if( buffer )
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    
    if( buffer )
    {
        std::string result = std::string(buffer, length);
        free(buffer);
        return result;
    }
}

std::vector<std::string> FileReadLines(const char *Filename, int MaxLines = 0)
{
    std::vector<std::string> result;
    if( !FileExists(Filename) )
    {
        return result;
    }
    
    std::ifstream in(Filename);
    
    std::string str;
    // Read the next line from File untill it reaches the end.
    ssize_t appended_lines = 0;
    while (std::getline(in, str))
    {
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0 )
        {
            result.push_back(str);
            appended_lines++;
        }
        
        if(appended_lines > MaxLines && MaxLines > 0)
        {
            break;
        }
    }
    //Close The File
    in.close();
    return result;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _FILEUTILS_H_
