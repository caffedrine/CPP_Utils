/*  
 *  File      : ${HEADER_FILENAME}
 *  Created on: 19.06.2019
 *      Author: curiosul
 */

#ifndef _MISCELANOUS_H_
#define _MISCELANOUS_H_

int GetRandom(int min, int max) //range : [min, max)
{
    static bool first = true;
    if (first)
    {
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}



#endif // _MISCELANOUS_H_
