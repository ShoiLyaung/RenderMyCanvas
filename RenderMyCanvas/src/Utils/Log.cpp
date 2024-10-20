#include "pch.hpp"

#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace RMC::utils {
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T.%f]>>>>>> %v <<<<<<[%!]%$");
        s_Logger = spdlog::stdout_color_mt("RMC");
        s_Logger->set_level(spdlog::level::trace);
    }

    std::shared_ptr<spdlog::logger> Log::s_Logger;
} // namespace RMC::utils