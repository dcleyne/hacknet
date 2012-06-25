/*
 * HNLogger.cpp
 *
 *  Created on: 24/06/2012
 *      Author: daniel
 */

#include "stdio.h"
#include "iostream"
#include <string>
#include "HN_Logger.h"
#include "log4cpp/BasicConfigurator.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/OstreamAppender.hh"

#define CODE_LOCATION __FILE__

// DEBUG < INFO < NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG

HN_Logger* HN_Logger::_Instance = 0;

HN_Logger::HN_Logger()
{
	log4cpp::BasicConfigurator::configure();
}

HN_Logger::~HN_Logger()
{
	// TODO Auto-generated destructor stub
}

void HN_Logger::SetLogFile(std::string logFileName)
{
	log4cpp::Appender* appender = new log4cpp::FileAppender("FileAppender",logFileName,false);
	log4cpp::Layout* layout = new log4cpp::BasicLayout();

	appender->setLayout(layout);
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.setAppender(appender);

	appender = new log4cpp::OstreamAppender("stdout",&std::cout);
	layout = new log4cpp::BasicLayout();
	appender->setLayout(layout);
	log_root.setAppender(appender);
}

void HN_Logger::SetLogLevel(LogLevel level)
{
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	switch (level)
	{
		case DEBUG:
			log_root.setPriority(log4cpp::Priority::DEBUG);
			break;
		case INFO:
			log_root.setPriority(log4cpp::Priority::INFO);
			break;
		case NOTICE:
			log_root.setPriority(log4cpp::Priority::NOTICE);
			break;
		case WARNING:
			log_root.setPriority(log4cpp::Priority::WARN);
			break;
		case ERROR:
			log_root.setPriority(log4cpp::Priority::ERROR);
			break;
		case CRITICAL:
			log_root.setPriority(log4cpp::Priority::CRIT);
			break;
		case ALERT:
			log_root.setPriority(log4cpp::Priority::ALERT);
			break;
		case EMERGENCY:
			log_root.setPriority(log4cpp::Priority::EMERG);
			break;
	}
}


HN_Logger* HN_Logger::GetInstance()
{
	if (_Instance == NULL)
		_Instance = new HN_Logger();

	return _Instance;
}

void HN_Logger::LogEmerg(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.emerg(std::string(buf));
}

void HN_Logger::LogAlert(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.alert(std::string(buf));
}

void HN_Logger::LogCrit(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.crit(std::string(buf));
}

void HN_Logger::LogError(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.error(std::string(buf));
}

void HN_Logger::LogWarn(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.warn(std::string(buf));
}

void HN_Logger::LogNotice(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.notice(std::string(buf));
}

void HN_Logger::LogInfo(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.info(std::string(buf));
}

void HN_Logger::LogDebug(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buf, 256, format, argptr);
	va_end(argptr);

	log4cpp::Category &log_root = log4cpp::Category::getRoot();
	log_root.debug(std::string(buf));
}
