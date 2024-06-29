//
//  trace.cpp
//  pypse
//
//  Created by Florent Jacquemard on 18/11/2022.
//

#include "pstrace.hpp"


//#if QP_PLATFORM == PLATFORM_DARWIN
//#include "spdlog/sinks/stdout_color_sinks.h"
//std::shared_ptr<spdlog::logger> _spd_console = spdlog::stdout_color_mt("concon");
//#elif QP_PLATFORM == PLATFORM_LINUX
//#include "spdlog/sinks/stdout_sinks.h"
//std::shared_ptr<spdlog::logger> _spd_console = spdlog::stdout_logger_mt("concon");
//#elif QP_PLATFORM == PLATFORM_FREEBSD
//#include "spdlog/sinks/stdout_sinks.h"
//std::shared_ptr<spdlog::logger> _spd_console = spdlog::stdout_logger_mt("concon");
//#elif QP_PLATFORM == PLATFORM_CYGWIN
//#include "spdlog/sinks/stdout_sinks.h"
//std::shared_ptr<spdlog::logger> _spd_console = spdlog::stdout_logger_mt("concon");
//#endif


//spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
int spdlog_setPattern(const std::string& s)
{
    spdlog::set_pattern(s);
    return 0;
}


int spdlog_setVerbosity(int verbosity)
{
    switch (verbosity)
    {
        case 0:
            spdlog::set_level(spdlog::level::off);
            break;
        case 1:
            spdlog::set_level(spdlog::level::critical);
            break;
        case 2:
            spdlog::set_level(spdlog::level::err);
            break;
        case 3:
            spdlog::set_level(spdlog::level::warn);
            break;
        case 4:
            spdlog::set_level(spdlog::level::info);
            break;
        case 5:
            spdlog::set_level(spdlog::level::debug);
            break;
        case 6:
            spdlog::set_level(spdlog::level::trace);
            break;
        default:
        {
            //spdlog::warn("verbosity level {} too high, set to 6 (trace).", verbosity);
            spdlog::set_level(spdlog::level::trace);
        }
    }
    
    return 0;
}

