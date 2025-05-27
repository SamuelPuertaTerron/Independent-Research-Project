/* A class to log to the console */

#pragma once

namespace Engine
{
	enum class ELogLevel : uint8_t
	{
		Debug,
		Info,
		Warning,
		Error,
		Fatal,
	};

	/**
	 * @brief A simple Logger Class. Only prints out during Debug, optionally save to file during Release
	 */
	class Logger
	{
	public:
		static void Log(const std::string& message, ELogLevel level);
		/**
		 * @brief Should log files to disk
		 * @param value true for yes, false for no
		 */
		static void SetShouldSave(bool value);
	private:
		static std::string GetLogLevelString(ELogLevel level);
		static void SetTextColour(int colour);
		static void ResetTextColour();
	private:
		inline static bool s_bShouldSave;
	};

	class Assertion
	{
	public:
		static void Assert(bool condition, const std::string& message);
	};

#define SE_ASSERT(cond, msg) Engine::Assertion::Assert(cond, msg);

}//namespace Engine