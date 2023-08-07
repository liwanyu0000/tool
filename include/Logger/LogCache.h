#include <vector>
#include <cstring>

using std::vector;


// 定义单位
// #ifdef _WIN32
constexpr size_t operator ""_mb(unsigned long long num)
{
    return num * 1024 * 1024;
}
constexpr size_t operator ""_kb(unsigned long long num)
{
    return num * 1024;
}
// #else
//     size_t operator ""_mb(unsigned long long num)
//     {
//         return num * 1024 * 1024;
//     }
//     size_t operator ""_kb(unsigned long long num)
//     {
//         return num * 1024;
//     }
// #endif

class LogCache
{
private:
    // 记录未使用的字节 
    size_t usedByte;
    size_t totalByte;
    char* buffer;
public:
    LogCache(size_t size=1_mb);
    ~LogCache();
    // 清除缓存
    void clear();
    // 添加数据
    LogCache &operator+ (const char *);
    void operator+= (const char *);
    // 获取未使用空间
    size_t getUnused();
    // 输出数据
    const char* out() const;
};


