#include <Logger/LogTarget.h>

std::string getFormatTime()
{
    auto now = std::chrono::system_clock::now();
    auto timet = std::chrono::system_clock::to_time_t(now);
    auto localTime = *std::gmtime(&timet);
    std::stringstream ss; 
    std::string str;
    ss << std::put_time(&localTime, "%Y-%m-%d");
    ss >> str;
    return str;
}

LogFileTarget::LogFileTarget(std::string filePath)
{
    std::string filename = filePath + getFormatTime() + ".log";
    this->out = std::ofstream(filename, std::ios::app);
}

LogFileTarget::~LogFileTarget()
{
    this->out.close();
}

void LogFileTarget::operator()(const char * buf)
{
    this->out << buf;
}

void LogTerminalTarget::operator()(const char * buf)
{
    std::cout << buf;
}

LogFileAndTerminalTarget::LogFileAndTerminalTarget(std::string filePath)
{
    std::string filename = filePath + getFormatTime() + ".log";
    this->out = std::ofstream(filename, std::ios::app);
}

LogFileAndTerminalTarget::~LogFileAndTerminalTarget()
{
    this->out.close();
}

void LogFileAndTerminalTarget::operator()(const char * buf)
{
    this->out << buf;
    std::cout << buf;
}