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

#include "log.hpp"
#include "stringops.hpp"

#include <ctime>
#include <iostream>
#include <stdexcept>

const std::string logTypeStrings[3] = { "INFO", "WARNING", "ERROR" };

Log::~Log()
{
	if (isLogging())
	{
		log("Log object is being destroyed; closing log.", LOG_INFO);
		stop();
	}
}

std::string Log::timestamp()
{
	std::time_t t = std::time(0);
	std::tm* timestamp = std::gmtime(&t);
	std::string yearstr = std::to_string(timestamp->tm_year + 1900);
	std::string monthstr = std::to_string(timestamp->tm_mon + 1);
	while (monthstr.length() < 2)
	{
		monthstr = "0" + monthstr;
	}
	std::string daystr = std::to_string(timestamp->tm_mday);
	while (daystr.length() < 2)
	{
		daystr = "0" + daystr;
	}
	std::string hourstr = std::to_string(timestamp->tm_hour);
	while (hourstr.length() < 2)
	{
		hourstr = "0" + hourstr;
	}
	std::string minstr = std::to_string(timestamp->tm_min);
	while (minstr.length() < 2)
	{
		minstr = "0" + minstr;
	}
	std::string secstr = std::to_string(timestamp->tm_sec);
	while (secstr.length() < 2)
	{
		secstr = "0" + secstr;
	}
	return yearstr + "-" + monthstr + "-" + daystr + " " + hourstr + ":" + minstr + ":" + secstr;
}

void Log::start()
{
	stop();
	logFilePath = "";
	logTarget = LOGTARGET_STDOUT;
}
void Log::start(const std::string& pathToLog)
{
	stop();
	logFilePath = pathToLog;
	outputFile.open(logFilePath, std::ios::out | std::ios::trunc);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("Couldn't open log file.");
	}
	logTarget = LOGTARGET_FILE;
}
void Log::start(const char* pathToLog)
{
	start((std::string)pathToLog);
}
void Log::stop()
{
	if (logTarget == LOGTARGET_FILE)
	{
		outputFile.close();
	}
	logTarget = LOGTARGET_UNSPECIFIED;
}
void Log::log(std::string textToLog, LogMessageTypes type)
{
	if (textToLog.length() < 1)
	{
		log("Logging an empty log of type " + logTypeStrings[type], LOG_WARNING);
	}
	switch (logTarget) // -Wswitch ok - Rest of them addressed below.
	{
		case LOGTARGET_UNSPECIFIED:
			throw std::runtime_error("Log not open.");
			break;
	}
	std::string logged = "[" + timestamp() + "] ";
	logged += logTypeStrings[type];
	logged += ": ";
	std::vector<std::string> lines = splitString(textToLog, "\n");
	logged += lines[0];
	for (size_t index = 1; index < lines.size(); ++index)
	{
		logged += "\n\t";
		logged += lines[index];
	}
	switch (logTarget) // -Wswitch ok - Rest of them addressed above.
	{
		case LOGTARGET_FILE:
			outputFile << logged << std::endl;
			break;
		case LOGTARGET_STDOUT:
			std::cout << logged << std::endl;
			break;
	}
}
bool Log::isLogging() const
{
	switch (logTarget)
	{
		case LOGTARGET_FILE:
		case LOGTARGET_STDOUT:
			return true;
			break;
		case LOGTARGET_UNSPECIFIED:
			return false;
			break;
	}
	return false;
}

Log runtime_log;