// @file: Logger.h
//
// @brief: Header file for a singleton used for Logging data.
// 
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-29

#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

class Logger
{
private:
	std::ofstream _file;

	Logger() = default;
	~Logger();
	inline explicit Logger(Logger const&) = delete;
	inline Logger& operator=(Logger const&) = delete;

public:
	inline static Logger& Get() {
		static Logger instance;
		return instance;
	}

	// Used for opening the log file.
	// Private so that only Engine can setup logger.
	void SetLogfile();

	void Log(const std::string&);
};

#endif // !_LOGGER_H_
