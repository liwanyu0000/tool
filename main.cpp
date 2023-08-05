#include <Logger/Logger.h>

using namespace std;

int main(int argc, char *argv[])
{
    Logger::getInstance().setLogger(LOG_LOVEL_INFO, LOG_TARGET_FILE_AND_TERMINAL);
    Logger::getInstance().outToTarget();
    return 0;
}