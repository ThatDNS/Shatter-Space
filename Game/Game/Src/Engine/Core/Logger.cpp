// @file: Logger.h
//
// @brief: Header file for a singleton used for Logging data.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-29

#include "stdafx.h"
#include "Logger.h"

Logger::~Logger()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

void Logger::SetLogfile()
{
	// Get the current date in YYYYMMDD format
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	auto tm = *std::localtime(&time);

	std::stringstream ss;
	ss << std::put_time(&tm, "%Y%m%d");
#ifdef _DEBUG
	std::string logfile = "Logs/log_" + ss.str() + ".txt";
#else
	std::string logfile = "Logs/prod_log_" + ss.str() + ".txt";
#endif

	_file.open(logfile, std::ios::out | std::ios::app);
	if (!_file.is_open())
	{
		std::cerr << "Error in opening logfile: " << logfile << std::endl;
	}
}

void Logger::Log(const std::string& message)
{
	if (_file.is_open())
	{
		// Get the timestamp for prepending it to log line
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		auto tm = *std::localtime(&time);

		_file << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;
		_file.flush();
	}
}
