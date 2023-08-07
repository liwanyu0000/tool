#include <Logger/LogCache.h>

LogCache::LogCache(size_t size)
    :totalByte(size), usedByte(0)
{
    this->buffer = new char[this->totalByte];
}

LogCache::~LogCache()
{
    delete[] this->buffer;
}

void LogCache::clear()
{
    this->usedByte = 0;
}

void LogCache::operator+= (const char * buf)
{
    size_t len = strlen(buf);
    strcpy(this->buffer + this->usedByte, buf);
    this->usedByte += len;
}


LogCache& LogCache::operator+(const char* buf)
{
    *this += buf;
    return *this;
}
size_t LogCache::getUnused()
{
    return this->totalByte - this->usedByte;
}

const char* LogCache::out() const
{
    if (this->buffer[this->usedByte - 1] == '\n')
        this->buffer[this->usedByte - 1] = 0;
    return this->buffer;
}


