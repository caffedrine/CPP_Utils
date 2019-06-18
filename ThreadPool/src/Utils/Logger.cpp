#include "Logger.h"

spdlog::logger *logger;

void setup_logger()
{
    try
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs.log", 1024 * 1024 * 20, 10);
        
        logger = new spdlog::logger("Logger", {stdout_sink, rotating_sink});
        logger->set_level(spdlog::level::debug);
        
        logger->info("---Application logs enabled---");
    }
    catch( const spdlog::spdlog_ex &ex )
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
