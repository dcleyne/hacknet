/*
 * HNLogger.h
 *
 *  Created on: 24/06/2012
 *      Author: daniel
 */

#ifndef HNLOGGER_H_
#define HNLOGGER_H_

#include <string>

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"

class HN_Logger {
public:
	virtual ~HN_Logger();

	static void SetLogFile(std::string logFileName);

	static void LogEmerg(const char* text, ...);
	static void LogAlert(const char* text, ...);
	static void LogCrit(const char* text, ...);
	static void LogError(const char* text, ...);
	static void LogWarn(const char* text, ...);
	static void LogNotice(const char* text, ...);
	static void LogInfo(const char* text, ...);
	static void LogDebug(const char* text, ...);
protected:
private:
	static HN_Logger* _Instance;
	static HN_Logger* GetInstance();

	HN_Logger();
};

#endif /* HNLOGGER_H_ */
