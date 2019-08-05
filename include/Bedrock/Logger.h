#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#define LEVEL_INFO 10
#define LEVEL_WARN 20
#define LEVEL_ERROR 30

#ifndef PP_LOG_FILE
#ifndef PP_NO_LOG
#define PP_LOG_FILE "PerfectPixel.log"
#endif
#endif

#ifdef _WIN32
const char *basename(const char *path);
#endif

namespace perfectpixel { namespace bedrock {

	class Logger
	{
	public:

		enum LogTouchType {
			LOG_TOUCH_UPDATE = 0x01,
			LOG_TOUCH_RENDER = 0x02,

			LOG_TOUCH_ALL = LOG_TOUCH_UPDATE | LOG_TOUCH_RENDER
		};

		static std::ofstream m_logStream;
		static std::map<int, std::string> m_logLevels;
		static uint8_t updateIndex;
		static uint8_t renderIndex;
		static std::streamoff m_currentLogSize;

		static void openLogStream();
		static void writeLog(const std::string &msg);
		static void touchLog(LogTouchType touchType = LOG_TOUCH_ALL);

	};

#ifndef PP_NO_LOG
#define PP_LOG(LEVEL, MSGSTREAM) {PP_LOG_ACTUAL(__FILE__, __LINE__, LEVEL, MSGSTREAM);}
#else
#define PP_LOG(LEVEL, MSGSTREAM) {}
#endif


#define PP_LOG_ACTUAL(FNAME, LINENO, LEVEL, MSGSTREAM) {\
	std::stringstream msg; \
	msg << "[" << ::perfectpixel::bedrock::Logger::m_logLevels[LEVEL] << "] (" << basename(FNAME) << ":" << LINENO << ") " << MSGSTREAM;\
	::perfectpixel::bedrock::Logger::writeLog(msg.str()); }

} }
