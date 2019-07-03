#include "Logger.h"
#include <DirectoryUtils.h>

bool LoggerIsInitialized = false;
spdlog::logger *logger;

void LoggerInit()
{
    if(LoggerIsInitialized == true)
    {
        logger->warn("Logger already initialized...");
        return;
    }
    
    /* Create folder 'logs' if it does not exist */
    if(DirectoryExists("logs") == false)
    {
        if( !DirectoryCreate("logs") )
        {
            std::cout << "[ERROR] Failed to create directory 'logs' for logging...";
            return;
        }
    }
    
    try
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/logs.log", 1024 * 1024 * 20, 10);

        logger = new spdlog::logger("Logger", {stdout_sink, rotating_sink});
        logger->set_level(spdlog::level::debug);

        logger->info("Application logs enabled...");
        LoggerIsInitialized = true;
    }
    catch( const spdlog::spdlog_ex &ex )
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
