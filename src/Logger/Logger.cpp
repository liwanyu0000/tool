#include <Logger/Logger.h>


Logger* Logger::logger      = nullptr;
LogCache Logger::logBuffer;
string Logger::logLevels[4] = {"INFO", "DEBUG", "WARNING", "ERROR"};
mutex Logger::logMutex;
LogLevel Logger::logLevel;
LogTarget* Logger::logTarget = nullptr;
int Logger::len = 10000;

Logger::Logger()
{
    this->logLevel = LOG_LOVEL_INFO;
    this->logTarget = new LogFileTarget;
    this->logBuffer += __TIMESTAMP__;
    this->logBuffer += "[Welcome] start logger";
}

Logger::~Logger()
{
 
}

void Logger::closeLogger()
{
    {
        std::unique_lock<mutex> il(logMutex);
        this->logBuffer += __TIMESTAMP__;
        this->logBuffer += "[Bye] end logger";
    }
    this->outToTarget();
    std::unique_lock<mutex> il(logMutex);
    if (logger)
    {
        delete logger;
        logger = nullptr;
    }
}

Logger& Logger::getInstance()
{
    std::unique_lock<mutex> il(logMutex);
    if (!logger)
        logger = new Logger();
    return *logger;
}

void Logger::setLogger(LogLevel level, LogTarget* target)
{
    this->outToTarget();
    std::unique_lock<mutex> il(logMutex);
    delete this->logTarget;
    this->logLevel = level;
    this->logTarget = target;
}

// 写log
int Logger::writeLog(LogLevel level, const char* fileName, int lineNumber, string format, ...)
{
    if (level >= logLevel)
    {
        string logInfo = "";
        logInfo += "[datetime: ";
        logInfo += __TIMESTAMP__;
        logInfo += "] ";
        logInfo += ("[" + logLevels[level] + "] ");
        logInfo += "[pid: ";
        # ifdef _WIN32
            logInfo += to_string(GetCurrentProcessId());
        #else
            logInfo += to_string(getpid());
        #endif
        logInfo += "] ";
        logInfo += "[tid: ";
        # ifdef _WIN32
            logInfo += to_string(GetCurrentThreadId());
        #else
            logInfo += to_string(gettid());
        #endif
        logInfo += "] ";
        logInfo += "[file: ";
        logInfo += fileName;
        logInfo += " ";
        logInfo += to_string(lineNumber);
        logInfo += "] ";
        logInfo += format;
        logInfo += "\n";
        len--;
        while (logBuffer.getUnused() <= logInfo.size())
            outToTarget();
        std::unique_lock<mutex> il(logMutex);
        logBuffer += logInfo.c_str();

    }
    
    return 0;
}
    // 输出log
int Logger::outToTarget()
{
    std::unique_lock<mutex> il(logMutex);
    (*this->logTarget)(this->logBuffer.out());
    logBuffer.clear();
    return 0;
}