#pragma once

#include "common.h"
#include <iostream>
#include <format>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <stacktrace>

namespace logger
{
	enum class LogLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

#ifdef SYSTEM_LOG_LEVEL
	inline constexpr LogLevel SystemLevel = LogLevel::##SYSTEM_LOG_LEVEL
#elif defined(DEBUG) || defined(_DEBUG)
	inline constexpr LogLevel SystemLevel = LogLevel::Debug;
#else
	inline constexpr LogLevel SystemLevel = LogLevel::Warning;
#endif

	inline void log(LogLevel level, std::string_view message);

	template <LogLevel __Level>
	struct LogChannel
	{
		static inline constexpr LogLevel Level = __Level;

		constexpr LogChannel() = default;
		LogChannel(const LogChannel&) = delete;
		LogChannel& operator=(const LogChannel&) = delete;
		constexpr ~LogChannel() = default;

		LogChannel& operator=(const LogChannel&) = delete;
		LogChannel& operator=(LogChannel&&) = delete;

		forceinline void operator()(std::string_view message) const
		{
			if constexpr (Level >= SystemLevel)
				log(Level, message);
		}

		template <typename... Args>
		forceinline void operator()(std::string_view format, Args&&... args) const
		{
			if constexpr (Level >= SystemLevel)
			{
				auto formattedMessage = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
				log(Level, formattedMessage);
			}
		}

		forceinline void operator()(const std::exception& ex) const
		{
			if constexpr (Level >= SystemLevel)
			{
				std::stringstream ss;
				ss << "Exception: " << ex.what() << "\n";
				ss << "Stack trace:\n" << std::stacktrace::current();
				log(Level, ss.str());
			}
		}

		template <typename T> requires requires(std::stringstream& ss, const T& value) { ss << value; }
		forceinline void operator<<(const T& value) const
		{
			if constexpr (Level >= SystemLevel)
			{
				std::stringstream ss;
				ss << value;
				log(Level, ss.str());
			}
		}
	};

	inline constexpr LogChannel<LogLevel::Debug> debug;
	inline constexpr LogChannel<LogLevel::Info> info;
	inline constexpr LogChannel<LogLevel::Warning> warning;
	inline constexpr LogChannel<LogLevel::Error> error;
	inline constexpr LogChannel<LogLevel::Fatal> fatal;

	namespace detail
	{
		static inline std::ostream& getStreamForLevel(LogLevel level)
		{
			switch (level)
			{
				case LogLevel::Debug:
				case LogLevel::Info:
					return std::cout;
				case LogLevel::Warning:
				case LogLevel::Error:
				case LogLevel::Fatal:
					return std::cerr;

				default:
					return std::cout;
			}
		}

		static inline std::string getCurrentTime()
		{
			auto now = std::chrono::system_clock::now();
			auto now_time = std::chrono::system_clock::to_time_t(now);
			auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

			auto now_tm = std::localtime(&now_time);
			if (!now_tm)
				return "Invalid Time";

			std::ostringstream oss;
			oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << now_ms.count();
			return oss.str();
		}
	}

	inline void log(LogLevel level, std::string_view message)
	{
		auto& buffer = detail::getStreamForLevel(level);
		buffer << "[" << detail::getCurrentTime() << "]";
		switch (level)
		{
			case LogLevel::Debug:   buffer << "[DEBUG]: "; break;
			case LogLevel::Info:    buffer << "[INFO]: "; break;
			case LogLevel::Warning: buffer << "[WARNING]: "; break;
			case LogLevel::Error:   buffer << "[ERROR]: "; break;
			case LogLevel::Fatal:	buffer << "[FATAL]: "; break;
			default:                buffer << "[UNKNOWN]: "; break;
		}
		buffer << message << std::endl;
	}
}
