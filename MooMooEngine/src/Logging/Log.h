#pragma once

#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace MooMooEngine
{

	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

// Core log macros
// these act as shortcuts so we do not have to type:
// Log::GetLogger()->TYPE
// each time we want to log something to the console

#define LOG_TRACE(...) :: MooMooEngine::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) :: MooMooEngine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) :: MooMooEngine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) :: MooMooEngine::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) :: MooMooEngine::Log::GetLogger()->fatal(__VA_ARGS__)