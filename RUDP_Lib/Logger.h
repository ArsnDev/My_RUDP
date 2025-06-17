#pragma once

class Logger
{
public:
	static void Init(const std::string& loggerName);
	static std::shared_ptr<spdlog::logger>& GetLogger();
private:
	static std::shared_ptr<spdlog::logger> globalLogger_;
};