#pragma once

#include "spdlog/spdlog.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return _logger; }

private:
	static std::shared_ptr<spdlog::logger> _logger;
};

// Log macros
#define LOG_TRACE(...)	::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)	::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)	::Log::GetLogger()->critical(__VA_ARGS__)
