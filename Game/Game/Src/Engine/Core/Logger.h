// @file: Logger.h
//
// @brief: Header file for a singleton used for Logging data.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-29

#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

enum LOG_LEVEL
{
	DEBUG_LOG,
	WARNING_LOG,
	ERROR_LOG
};

class Logger
{
private:
	std::ofstream _file;

	Logger() = default;
	~Logger();
	inline explicit Logger(Logger const&) = delete;
	inline Logger& operator=(Logger const&) = delete;

protected:
	// Used for opening the log file.
	// Not public so that only Engine can setup logger.
	void SetLogfile();

public:
	inline static Logger& Get() {
		static Logger instance;
		return instance;
	}

	void Log(const std::string&, LOG_LEVEL);
	// Log with default log level (Debug)
	void Log(const std::string&);

	friend class Engine;
};

#endif // !_LOGGER_H_
