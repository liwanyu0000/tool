#ifndef __LOGGER__
#define __LOGGER__

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <queue>
#include <mutex>
#include <thread>
#include <Logger/LogCache.h>
#include <Logger/LogTarget.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using std::cout;
using std::endl;
using std::string;
using std::ios;
using std::mutex;
using std::ofstream;
using std::to_string;

// 日志水平
enum LogLevel
{
    LOG_LOVEL_INFO, 
    LOG_LOVEL_DEBUG, 
    LOG_LOVEL_WARNING, 
    LOG_LOVEL_ERROR
};

class Logger
{
private:
    Logger();
    ~Logger();
    static Logger* logger;
    // 互斥量
    static mutex logMutex;
    // 存储log的buffer
    static LogCache logBuffer;
    
    static LogLevel logLevel;
    static string logLevels[4];
    static LogTarget* logTarget;
    static int len;
public:
    // 禁用拷贝构造和= 
    Logger (const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    // 设置logLevel与logTarget
    void setLogger(LogLevel, LogTarget*);
    // 获取唯一的一个实例对象
    static Logger& getInstance();
    // 写log
    int writeLog(LogLevel, const char* fileName, int lineNumber, string format, ...);
    // 输出log
    int outToTarget();
    // 关闭日志系统
    void closeLogger();
};

#define LOGSTART Logger::getInstance();
#define LOGSET(LOGLEVEL, LOGTARGET) Logger::getInstance().setLogger(LOGLEVEL, new LOGTARGET);
#define LOGSETF(LOGLEVEL, LOGTARGET, FILENAME) Logger::getInstance().setLogger(LOGLEVEL, new LOGTARGET(FILENAME));
#define INFOLOG(FORMAT, ...) Logger::getInstance().writeLog(LOG_LOVEL_INFO, __FILE__, __LINE__, FORMAT);
#define DEBUGLOG(FORMAT, ...) Logger::getInstance().writeLog(LOG_LOVEL_DEBUG, __FILE__, __LINE__, FORMAT);
#define WARNINGLOG(FORMAT, ...) Logger::getInstance().writeLog(LOG_LOVEL_WARNING, __FILE__, __LINE__, FORMAT);
#define ERRORLOG(FORMAT, ...) Logger::getInstance().writeLog(LOG_LOVEL_ERROR, __FILE__, __LINE__, FORMAT);
#define LOGEND Logger::getInstance().closeLogger();

#endif