#include "Logger.h"
#include <ThreadPool.h>
#include <TimeUtils.h>
#include <Miscelanous.h>
#include <NetworkUtils.h>
#include <IpUtils.h>
#include <Ping.h>
#include <PortScan.h>

#include <tuple>

#define log logger->info

std::tuple<char const *, uint16_t, bool> TaskIsPortOpen(const char *HostnameOrIp, uint16_t port_no)
{
    std::tuple<char const *, uint16_t, bool> result;
    std::get<0>(result) = HostnameOrIp;
    std::get<1>(result) = port_no;
    
    if( NetworkUtils::IsTcpPortOpen(HostnameOrIp, port_no))
        std::get<2>(result) = true;
    else
        std::get<2>(result) = false;
    
    TimeUtils::SleepMs(200);
    
    return result;
}

int main()
{
    setup_logger();
    
    ThreadPool pool(5000);
    std::list< std::future<std::tuple<char const *, uint16_t, bool>> > results;
    
    for(int i = 15; i <= 65535; ++i)
    {
        results.emplace_back( pool.enqueue(TaskIsPortOpen ,(const char *)"127.0.0.1", i ) );
    }
    
    /* Wait for all threads without blocking */
    while(!results.empty())
    {
        auto it = results.begin();
        while( it != results.end() )
        {
            if( true ) //it->wait_for(std::chrono::seconds(0)) == std::future_status::ready ) /* Prevent from blocking */
            {
                std::tuple<char const *, uint16_t, bool> checkedPortFinished = it->get();
                if(std::get<2>(checkedPortFinished))
                    log("{}:{} -> {}", std::get<0>(checkedPortFinished), std::get<1>(checkedPortFinished), std::get<2>(checkedPortFinished));
                it = results.erase(it);
            }
            it++;
        }
        
        TimeUtils::SleepMs(1);
    }
    std::cout << std::endl;
    
}