#pragma once
#include <cassert>

#ifdef _DEBUG
#define ASSERT(exp) assert(exp)
#define LOGERROR(...) Logger::DebugLogError(__VA_ARGS__)
#define LOGWARNING(...) Logger::DebugLogWarning(__VA_ARGS__)
#define LOG(...) Logger::DebugLog(__VA_ARGS__)
#else
#define ASSERT(exp) void(0)
#define LOGERROR(...) void(0)
#define WARNING(...) void(0)
#define LOG(...) void(0)
#endif

constexpr auto MAX_LOG_LEN = 1000;

class Logger
{
public:
	static void DebugLog(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char msg[MAX_LOG_LEN];
		vsnprintf(msg, (MAX_LOG_LEN - 2), format, args);
		strcat_s(msg, sizeof(msg), "\n");
		vprintf(msg, args);
		va_end(args);
	}

	static void DebugLogError(const char* format, ...)
	{
#if _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 4);
#endif
		va_list args;
		va_start(args, format);
		char msg[MAX_LOG_LEN];
		vsnprintf(msg, (MAX_LOG_LEN - 2), format, args);
		strcat_s(msg, sizeof(msg), "\n");
		vprintf(msg, args);
		va_end(args);
#if _WIN32
		SetConsoleTextAttribute(hConsole, 7);
#endif
	}

	static void DebugLogWarning(const char* format, ...)
	{
#if _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 6);
#endif
		va_list args;
		va_start(args, format);
		char msg[MAX_LOG_LEN];
		vsnprintf(msg, (MAX_LOG_LEN - 2), format, args);
		strcat_s(msg, sizeof(msg), "\n");
		vprintf(msg, args);
		va_end(args);
#if _WIN32
		SetConsoleTextAttribute(hConsole, 7);
#endif
	}
};