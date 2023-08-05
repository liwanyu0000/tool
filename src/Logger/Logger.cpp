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
    cout << "start logger" << endl;
}

Logger::~Logger()
{
    this->outLogFile.close();
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

void Logger::setLogger(LogLevel level, LogTarget target)
{
    this->logLevel = level;
    this->logTarget = target;
    if (logTarget == LOG_TARGET_FILE || logTarget == LOG_TARGET_FILE_AND_TERMINAL)
        this->outLogFile = ofstream(DEFAULT_FILEPATH, ios::app);


}
void Logger::setLogger(LogLevel level, LogTarget target, string filename)
{
    this->logLevel = level;
    this->logTarget = target;
    this->outLogFile = ofstream(filename, ios::app);
}

// 写log
int Logger::writeLog(LogLevel level, unsigned char* fileName, unsigned char* function, int lineNumber, char* format, ...)
{
    return 0;
}
    // 输出log
int Logger::outToTarget()
{
    if (logTarget == LOG_TARGET_TERMINAL || logTarget == LOG_TARGET_FILE_AND_TERMINAL)
        cout << "LOG" << endl;
    if (logTarget == LOG_TARGET_FILE || logTarget == LOG_TARGET_FILE_AND_TERMINAL)
        outLogFile << "LOG" << endl;
    return 0;
}