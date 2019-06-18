#include <stdio.h>
#include <cstdint>
#include <cwchar>
#include "ThreadPool.h"
int main()
{
    unsigned char aurel[7] = { 0x70, 0x45, 0x56, 0x5E, 0x45, 0x4A, 0x24 };
    
    for (unsigned int fBzxp = 0, XasBf = 0; fBzxp < 7; fBzxp++)
    {
        XasBf = aurel[fBzxp];
        XasBf -= 0x80;
        XasBf ^= 0xA4;
        aurel[fBzxp] = XasBf;
    }
    
    printf("%s", aurel);
}