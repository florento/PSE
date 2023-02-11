//
//  trace.hpp
//  pypse
//
//  Created by Florent Jacquemard on 18/11/2022.
//

#ifndef trace_hpp
#define trace_hpp

#include <iostream>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>  // support for loading levels from the environment variable
#include <spdlog/fmt/ostr.h> // support for user defined types
//#include "spdlog/sinks/stdout_color_sinks.h"

#define ERROR(...) spdlog::error(__VA_ARGS__)
#define WARN(...)  spdlog::warn(__VA_ARGS__)
#define INFO(...)  spdlog::info(__VA_ARGS__)
#define TRACE(...) spdlog::trace(__VA_ARGS__)
#define DEBUGU(...) spdlog::debug(__VA_ARGS__)

//inline void ERROR(...) { spdlog::error(__VA_ARGS__); }
//inline void WARN(...)  { spdlog::warn(__VA_ARGS__); }
//inline void INFO(...)  { spdlog::info(__VA_ARGS__); }
//inline void TRACE(...) { spdlog::trace(__VA_ARGS__); }
//inline void DEBUG(...) { spdlog::debug(__VA_ARGS__); }

/// apply custom format globally to all registered loggers
/// @see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
int spdlog_setPattern(const std::string& s = "[%^pse_%l%$] %v");

/// set verbosity level. log levels:
/// trace    = 6,
/// debug    = 5,
/// info     = 4,
/// warn     = 3,
/// error    = 2,
/// critical = 1,
/// off      = 0
int spdlog_setVerbosity(int verbosity);


#endif /* trace_hpp */
