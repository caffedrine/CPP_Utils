/*
 *  Created on: 18.06.2019
 *      Author: curiosul
 */

#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
public:
    explicit ThreadPool(size_t ThreadsNo);
protected:

private:
    ssize_t _Threads_No;
};

#endif //_THREADPOOL_H_
