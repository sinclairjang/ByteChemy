#pragma once
#pragma warning ( push )

//TODO: Compiler-agnostic warning supression
// Suppress warnings from the library to avoid cluttering
#pragma warning( disable : 26812 )	// Prefer 'enum class' over 'enum'
#pragma warning( disable : 6285  )	// <non-zero constant> || <non-zero constant> is always a non-zero constant
#pragma warning( disable : 26437 )	// DONT_SLICE
#pragma warning( disable : 26451 )	// Arithmetic overflow
#pragma warning( disable : 26498 )	// USE_CONSTEXPR_FOR_FUNCTIONCALL
#pragma warning( disable : 26800 )	// Use of a moved from object: <object>
#pragma warning( disable : 26495 )  // MEMBER_UNINIT
#pragma warning( disable : 4819 )  

#ifdef PEARL_DEBUG
	#define SPDLOG_ACTIVE_LEVEL			SPDLOG_LEVEL_TRACE
	
	#include "spdlog/spdlog.h"
	
	#define PEARL_LOG_LEVEL(lvl)		spdlog::set_level(spdlog::level::lvl)
	#define PEARL_LOG_FORMAT(fmt)		spdlog::set_pattern(#fmt)
	
	#define PEARL_CORE_TRACE(...)		SPDLOG_TRACE(__VA_ARGS__)
	#define PEARL_CORE_DEBUG(...)		SPDLOG_DEBUG(__VA_ARGS__)
	#define PEARL_CORE_INFO(...)		SPDLOG_INFO(__VA_ARGS__)	// == spdlog::default_logger_raw()->level()
	#define PEARL_CORE_WARN(...)		SPDLOG_WARN(__VA_ARGS__)
	#define PEARL_CORE_ERROR(...)		SPDLOG_ERROR(__VA_ARGS__)
	#define PEARL_CORE_FATAL(...)		SPDLOG_CRITICAL(__VA_ARGS__)
#else
	#define PEARL_CORE_TRACE
	#define PEARL_CORE_DEBUG
	#define PEARL_CORE_INFO
	#define PEARL_CORE_WARN
	#define PEARL_CORE_ERROR
	#define PEARL_CORE_FATAL
#endif

#pragma warning ( pop )