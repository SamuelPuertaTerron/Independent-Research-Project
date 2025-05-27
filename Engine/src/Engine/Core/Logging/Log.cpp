#include "Engine.h"
#include "Log.h"

namespace Engine
{
	void Logger::Log(const std::string& message, ELogLevel level)
	{
		/* Only print in Debug Mode */
#ifdef E_DEBUG
		std::stringstream ss;
		ss << GetLogLevelString(level) + " " + message + "\n";
		std::cout << ss.str();
		ResetTextColour();
#elif E_RELEASE
		ResetTextColour();
#endif

	}

	void Logger::SetShouldSave(bool value)
	{
		s_bShouldSave = value;
	}

	std::string Logger::GetLogLevelString(ELogLevel level)
	{
		switch (level)
		{
		case ELogLevel::Debug:
			SetTextColour(36); //Cyan
			return "[DEBUG]";
		case ELogLevel::Info:
			SetTextColour(32); //Green
			return "[INFO]";
		case ELogLevel::Warning:
			SetTextColour(33); //Yellow
			return "[WARNING]";
		case ELogLevel::Error:
			SetTextColour(91); //Bright Red
			return "[ERROR]";
		case ELogLevel::Fatal:
			SetTextColour(31); //Dark Red
			return "[FATAL]";
		}

		return "[NONE]";
	}

	void Logger::SetTextColour(int colour)
	{
		std::cout << "\033[" << colour << "m";
	}

	void Logger::ResetTextColour()
	{
		std::cout << "\033[0m";
	}

	void Assertion::Assert(bool condition, const std::string& message)
	{
		if (!condition)
		{
			Logger::Log(message, ELogLevel::Fatal);
			std::abort();
		}
	}
}//namespace