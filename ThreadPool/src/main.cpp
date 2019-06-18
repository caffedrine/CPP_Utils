#include "Logger.h"
#include <ThreadPool.h>
#include <time_utils.h>


int taskTodo(int a)
{
    TimeUtils::SleepMs(1);
    return a*a;
}



int main()
{
    setup_logger();

    // create thread pool with 4 worker threads
    ThreadPool pool(4);
    
    while(true)
    {
        TimeUtils::SleepMs(1);
    }
}