#include "pch.h"
#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::globalLogger_;

void Logger::Init(const std::string& loggerName)
{
	spdlog::set_pattern("[%n] [%Y-%m-%d %H:%M:%S.%e] [%l] %v");
	globalLogger_ = spdlog::stdout_color_mt(loggerName);
	globalLogger_->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::GetLogger()
{
	return globalLogger_;
}