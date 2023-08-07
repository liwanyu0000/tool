#include <Logger/Logger.h>
#include <Logger/LogTarget.h>
#include <ctime>

using namespace std;

void test(int i)
{
    INFOLOG("info thread " + to_string(i))
    // std::this_thread::sleep_for(2ms);
    DEBUGLOG("debug thread " + to_string(i))
    // std::this_thread::sleep_for(2ms);
    WARNINGLOG("warning thread " + to_string(i))
    // std::this_thread::sleep_for(2ms);
    ERRORLOG("error thread " + to_string(i))
    // std::this_thread::sleep_for(2ms);
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();
    // LOGSTART
    for (int i = 0; i < 1000; i++)
    {
        thread th1(test, 1);
        thread th2(test, 2);
        thread th3(test, 3);
        thread th4(test, 4);
        thread th5(test, 5);
        thread th6(test, 6);
        thread th7(test, 7);
        thread th8(test, 8);
        thread th9(test, 9);
        INFOLOG("info main")
        DEBUGLOG("debug main")
        WARNINGLOG("warning main")
        ERRORLOG("error main")
        if (th1.joinable()) th1.join();
        if (th2.joinable()) th2.join();
        if (th3.joinable()) th3.join();
        if (th4.joinable()) th4.join();
        if (th5.joinable()) th5.join();
        if (th6.joinable()) th6.join();
        if (th7.joinable()) th7.join();
        if (th8.joinable()) th8.join();
        if (th9.joinable()) th9.join();
    }
    LOGEND
    end = clock();
    cout << "time: " << double(end -start) / CLOCKS_PER_SEC << "s" << endl;
    // LOGSET(LOG_LOVEL_INFO, LogTerminalTarget)
    // INFOLOG("info Terminal")
    // DEBUGLOG("debug Terminal")
    // WARNINGLOG("warning Terminal")
    // ERRORLOG("error Terminal")
    // LOGSET(LOG_LOVEL_INFO, LogFileTarget)
    // INFOLOG("info File")
    // DEBUGLOG("debug File")
    // WARNINGLOG("warning File")
    // ERRORLOG("error File")
    // LOGSET(LOG_LOVEL_INFO, LogFileAndTerminalTarget)
    // INFOLOG("info File Terminal")
    // DEBUGLOG("debug File Terminal")
    // WARNINGLOG("warning File Terminal")
    // ERRORLOG("error File Terminal")
    // LOGSETF(LOG_LOVEL_INFO, LogFileAndTerminalTarget, "./")
    // INFOLOG("info File Terminal ./")
    // DEBUGLOG("debug File Terminal ./")
    // WARNINGLOG("warning File Terminal ./")
    // ERRORLOG("error File Terminal ./")
    // LOGSETF(LOG_LOVEL_INFO, LogFileTarget, "./")
    // INFOLOG("info File ./")
    // DEBUGLOG("debug File ./")
    // WARNINGLOG("warning File ./")
    // ERRORLOG("error File ./")
    // LOGEND
    return 0;
}