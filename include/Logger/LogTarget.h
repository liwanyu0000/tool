#include <iostream>
#include <fstream> 
#include <string>
#include <chrono>
#include <iomanip>
#ifndef __LogTarget__
#define __LogTarget__
// 设置默认log文件存储位置
#define DEFAULT_FILEPATH "./log/"
std::string getFormatTime();
class LogTarget
{
public:
    virtual void operator() (const char *) = 0;
};

class LogFileTarget : public LogTarget
{
private:
    std::ofstream out;
public:
    LogFileTarget(std::string filePath = DEFAULT_FILEPATH);
    ~LogFileTarget();
    void operator() (const char *);
};
class LogTerminalTarget : public LogTarget
{
public:
    void operator() (const char *);
};

class LogFileAndTerminalTarget : public LogTarget
{
private:
    std::ofstream out;
public:
    LogFileAndTerminalTarget(std::string filePath = DEFAULT_FILEPATH);
    ~LogFileAndTerminalTarget();
    void operator() (const char *);
};

#endif
