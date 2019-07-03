/*  
 *  File      : PortScanEngine.h
 *  Created on: 02.07.2019
 *      Author: curiosul
 */

#ifndef _PORTSCANENGINE_H_
#define _PORTSCANENGINE_H_

#include <Exception.h>
#include <ThreadPool.h>
//#include <NetworkUtils.h>

class PortScanEngine
{
public:
    PortScanEngine(size_t ThreadsNumber);
    ~PortScanEngine();
    
    void Start();
    void Stop();

protected:

private:

};

#endif // _PORTSCANENGINE_H_