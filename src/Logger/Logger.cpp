#include <Logger/Logger.h>


Logger* Logger::logger      = nullptr;
string Logger::logBuffer    = "";
string Logger::logLevels[4] = {"INFO", "DEBUG", "WARNING", "ERROR"};
mutex Logger::logMutex;
ofstream Logger::outLogFile;
LogLevel Logger::logLevel;
LogTarget Logger::logTarget;

Logger::Logger()
{
    this->logLevel = LOG_LOVEL_INFO;
    this->logTarget = LOG_TARGET_TERMINAL;
    this->logBuffer += __TIMESTAMP__;
    this->logBuffer += " start logger\n";
}

Logger::~Logger()
{
    
    this->logBuffer += __TIMESTAMP__;
    this->logBuffer += " end logger\n";
    this->outToTarget();
    this->outLogFile.close();
}

void Logger::closeLogger()
{
    delete logger;
    logger = nullptr;

}

Logger& Logger::getInstance()
{
    logMutex.lock();
    if (!logger)
    {
        logger = new Logger();

    }
    logMutex.unlock();
    return *logger;
}

void Logger::setLogger(LogLevel level, LogTarget target, const string filename)
{
    this->logLevel = level;
    this->logTarget = target;
    if (this->outLogFile.is_open())
        this->outLogFile.close();
    if ((logTarget | LOG_TARGET_FILE) == 2)
        this->outLogFile = ofstream(filename, ios::app);
}

// 写log
int Logger::writeLog(LogLevel level, const char* fileName, int lineNumber, string format, ...)
{
    if (level >= logLevel)
    {
        std::unique_lock<mutex>(logMutex);
        logBuffer += __TIMESTAMP__;
        logBuffer += " ";
        logBuffer += ("[" + logLevels[level] + "] ");
        logBuffer += fileName;
        logBuffer += " ";
        logBuffer += std::to_string(lineNumber);
        logBuffer += " ";
        logBuffer += format;
        logBuffer += "\n";
    }
    outToTarget();
    return 0;
}
    // 输出log
int Logger::outToTarget()
{
    std::unique_lock<mutex>(logMutex);
    if ((logTarget | LOG_TARGET_TERMINAL) == 1)
        cout << logBuffer;
    if ((logTarget | LOG_TARGET_FILE) == 2)
        outLogFile << logBuffer;
    logBuffer.clear();
    return 0;
}