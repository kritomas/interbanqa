/*
 *	This file was taken out of Kritos Game Engine 2D, and subsequently modified
 *	for use in interbanqa.
 *
 *	MIT License
 *
 *	Copyright (c) 2024 kritomas
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *	Source code: https://gitlab.com/kritomas/kritos-game-engine-2d
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <mutex>

enum LogMessageTypes { LOG_INFO = 0, LOG_WARNING = 1, LOG_ERROR = 2 };
extern const std::string logTypeStrings[3];
enum LogTargets { LOGTARGET_UNSPECIFIED, LOGTARGET_FILE, LOGTARGET_STDOUT };

class Log
{
private:
	std::fstream outputFile;
	std::string logFilePath;
	LogTargets logTarget = LOGTARGET_UNSPECIFIED;
	std::mutex mutex;

public:
	Log() {}

	Log(const Log& other) = delete;

	~Log();

	/**
	 * @return A timestamp from the system clock in the ISO format.
	 */
	static std::string timestamp();

	/**
	 * Starts logging. Logs to std::cout.
	 */
	void start();
	/**
	 * Starts logging. Logs to a file.
	 *
	 * @param pathToLog Path to the log file.
	 */
	void start(const std::string& pathToLog);
	/**
	 * Starts logging. Logs to a file.
	 *
	 * @param pathToLog Path to the log file.
	 */
	void start(const char* pathToLog);
	/**
	 * Stops logging.
	 */
	void stop();
	/**
	 * Creates a log entry. Timestamp is added automatically.
	 *
	 * @param textToLog Text of the log entry.
	 * @param type Type of the log entry.
	 */
	void log(std::string textToLog, LogMessageTypes type);
	/**
	 * @return Whether this Log is currently logging.
	 */
	bool isLogging() const;
};

extern Log runtime_log;

#endif