#include <Logger/Logger.h>


Logger* Logger::logger      = nullptr;
LogCache Logger::logBuffer;
string Logger::logLevels[4] = {"INFO", "DEBUG", "WARNING", "ERROR"};
mutex Logger::logMutex;
ofstream Logger::outLogFile;
LogLevel Logger::logLevel;
LogTarget Logger::logTarget;
int Logger::len = 10000;

Logger::Logger()
{
    this->logLevel = LOG_LOVEL_INFO;
    this->logTarget = LOG_TARGET_FILE;
    this->outLogFile = ofstream(DEFAULT_FILEPATH, ios::app);
    this->logBuffer += __TIMESTAMP__;
    this->logBuffer += " start logger\n";
}

Logger::~Logger()
{
    this->outLogFile.close();
}

void Logger::closeLogger()
{
    {
        std::unique_lock<mutex> il(logMutex);
        this->logBuffer += __TIMESTAMP__;
        this->logBuffer += " end logger\n";
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

void Logger::setLogger(LogLevel level, LogTarget target, const string filename)
{
    this->outToTarget();
    std::unique_lock<mutex> il(logMutex);
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
    if ((logTarget | LOG_TARGET_TERMINAL) == 1)
        cout << logBuffer.out();
    if ((logTarget | LOG_TARGET_FILE) == 2)
        outLogFile << logBuffer.out();
    logBuffer.clear();
    return 0;
}