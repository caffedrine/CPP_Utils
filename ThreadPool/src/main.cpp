#include "Logger.h"
#include <ThreadPool.h>
#include <time_utils.h>
#include <miscelanous.h>

using namespace TimeUtils;

int taskTodo(int a)
{
    TimeUtils::SleepMs(GetRandom(100, 1000));
    return a;
}

int main()
{
    setup_logger();
    
    ThreadPool pool(10);
    std::list< std::future<int> > results;
    
    for(int i = 0; i < 100; ++i)
    {
        results.emplace_back( pool.enqueue( taskTodo, i ) );
    }
    
    logger->info("Wait 1s...");
    TimeUtils::SleepMs(1000);
    logger->info("Start check...");
    
    /* Wait for all threads without blocking */
    while(!results.empty())
    {
        auto it = results.begin();
        while( it != results.end() )
        {
            if( it->wait_for(std::chrono::seconds(0)) == std::future_status::ready )
            {
                std::cout << it->get() << ' ';
                it = results.erase(it);
            }
            it++;
        }
        TimeUtils::SleepMs(1);
        logger->info("Tick...");
    }
    std::cout << std::endl;
}