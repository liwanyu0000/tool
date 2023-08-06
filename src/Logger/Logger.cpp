#include <Logger/Logger.h>


Logger* Logger::logger      = nullptr;
string Logger::logBuffer    = "";
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
        std::unique_lock<mutex> il(logMutex);
        logBuffer += "[datetime: ";
        logBuffer += __TIMESTAMP__;
        logBuffer += "] ";
        logBuffer += ("[" + logLevels[level] + "] ");
        logBuffer += "[pid: ";
        # ifdef _WIN32
            logBuffer += to_string(GetCurrentProcessId());
        #else
            logBuffer += to_string(getpid());
        #endif
        logBuffer += "] ";
        logBuffer += "[tid: ";
        # ifdef _WIN32
            logBuffer += to_string(GetCurrentThreadId());
        #else
            logBuffer += to_string(gettid());
        #endif
        logBuffer += "] ";
        logBuffer += "[file: ";
        logBuffer += fileName;
        logBuffer += " ";
        logBuffer += to_string(lineNumber);
        logBuffer += "] ";
        logBuffer += format;
        logBuffer += "\n";
        len--;
    }
    if (len <= 0)
    {
        outToTarget();
        len = 10000;
    }
    
    return 0;
}
    // 输出log
int Logger::outToTarget()
{
    std::unique_lock<mutex> il(logMutex);
    if ((logTarget | LOG_TARGET_TERMINAL) == 1)
        cout << logBuffer;
    if ((logTarget | LOG_TARGET_FILE) == 2)
        outLogFile << logBuffer;
    logBuffer.clear();
    return 0;
}