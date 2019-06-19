#include "Logger.h"
#include <ThreadPool.h>
#include <time_utils.h>
#include <miscelanous.h>


int taskTodo(int a)
{
    TimeUtils::SleepMs(GetRandom(10, 500));
    return a;
}

int main()
{
    setup_logger();
    
    ThreadPool pool(100);
    std::vector< std::future<int> > results;
    
    for(int i = 0; i < 100; ++i)
    {
        results.emplace_back( pool.enqueue( taskTodo, i ) );
    }
    
    logger->info("Wait 1s...");
    TimeUtils::SleepMs(1000);
    logger->info("Start check...");
    
    for(auto && result: results)
        std::cout << result.get() << result. << ' ';
    std::cout << std::endl;
    
    
    while(true)
    {
        TimeUtils::SleepMs(1000);
        logger->info("Tick...");
    }


}