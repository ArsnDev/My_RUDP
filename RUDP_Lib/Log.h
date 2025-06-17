#pragma once

#ifdef NDEBUG
#define LOG_TRACE(...)    (void)0
#define LOG_INFO(...)     (void)0
#define LOG_WARN(...)     (void)0
#define LOG_ERROR(...)    Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetLogger()->critical(__VA_ARGS__)
#else
#define LOG_TRACE(...)    Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetLogger()->critical(__VA_ARGS__)
#endif