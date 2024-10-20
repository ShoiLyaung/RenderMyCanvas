#pragma once

#define SPDLOG_ACTIVE_LEVEL 0
#include "spdlog/spdlog.h"
#include "Types.hpp"

namespace RMC::utils {
    class Log
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };
} // RMC::utils

#ifdef RMC_DEBUG
#define RMC_TRACE(...)    SPDLOG_LOGGER_TRACE(::RMC::utils::Log::GetLogger(), __VA_ARGS__)
#define RMC_INFO(...)     SPDLOG_LOGGER_INFO(::RMC::utils::Log::GetLogger(), __VA_ARGS__)
#define RMC_WARN(...)     SPDLOG_LOGGER_WARN(::RMC::utils::Log::GetLogger(), __VA_ARGS__)
#define RMC_ERROR(...)    SPDLOG_LOGGER_ERROR(::RMC::utils::Log::GetLogger(), __VA_ARGS__)
#define RMC_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::RMC::utils::Log::GetLogger(), __VA_ARGS__)

#else
#define RMC_TRACE(...)
#define RMC_INFO(...)
#define RMC_WARN(...)
#define RMC_ERROR(...)
#define RMC_CRITICAL(...)
#endif