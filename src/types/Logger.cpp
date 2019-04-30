#include <types/Logger.h>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
const char * basename(const char *path)
{
	const char *start = strrchr(path, '\\');
	return start ? start + 1 : path;
}
#endif // _WIN32

namespace perfectpixel {
	namespace types {

		std::ofstream Logger::m_logStream{};
		std::map<int, std::string> Logger::m_logLevels{ {LEVEL_INFO, "info"}, {LEVEL_WARN, "WARN"}, {LEVEL_ERROR, " ERR"} };
		uint8_t Logger::renderIndex{ 0 };
		uint8_t Logger::updateIndex{ 0 };
		std::streamoff Logger::m_currentLogSize{ 0 };

		void Logger::openLogStream()
		{
			if (m_logStream.is_open())
			{
				return;
			}
			m_logStream.open(PP_LOG_FILE);
			m_currentLogSize = m_logStream.tellp();
		}

		void Logger::writeLog(const std::string &msg)
		{
			openLogStream();

			std::time_t tval = std::time(nullptr);
			std::tm tresult;
			localtime_s(&tresult, &tval);

			char strtime[40];
			std::strftime(strtime, sizeof(strtime), /*"[%EY %b %d %H:%M:%S] " */ "[%H:%M:%S] ", &tresult);

			m_logStream << strtime 
				<< "[" << std::hex << std::setw(2) << std::setfill('0') << (int)updateIndex
				<< "/" << std::setw(2) << std::setfill('0') << (int)renderIndex << "] " << std::dec
				<< msg << std::endl;
			m_currentLogSize = m_logStream.tellp();
		}

		void Logger::touchLog(LogTouchType touchType)
		{
			if ((touchType & LOG_TOUCH_UPDATE) > 0)
			{
				updateIndex++;
			}
			if ((touchType & LOG_TOUCH_RENDER) > 0)
			{
				renderIndex++;
			}
		}

} }


