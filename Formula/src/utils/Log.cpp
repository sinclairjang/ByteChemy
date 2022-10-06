#include "fm_pch.h"

#include "Log.h"
#include "vendor/spdlog/include/spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::_logger;

void Log::Init()
{
	spdlog::set_pattern("%^[%X] %n: %v%$");
	_logger = spdlog::stdout_color_mt("FM");
	_logger->set_level(spdlog::level::trace);
}
