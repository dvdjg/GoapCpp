#include <stdio.h>
#include "time_utils.h"

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(COMPILER_MINGW)

#include <Windows.h>
#ifdef ERROR
#undef ERROR
#endif
namespace goap
{
const char* nowTime(char result[])
{
    // char result[100];
    const int MAX_LEN = 32;
    char buffer[MAX_LEN];
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
        return "Error in NowTime()";

    result[0] = 0;
    static DWORD first = GetTickCount();
    sprintf_s(result, MAX_LEN, "%s.%06ld", buffer, (long)(GetTickCount() - first));
    return result;
}
}
#else

#include <sys/time.h>
namespace goap
{
const char* nowTime(char result[])
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    char buffer[32];
    tm r;
    //strftime(buffer, sizeof(buffer), "%X", localtime_r(&tv.tv_sec, &r));
    time_t sec;
    localtime_s(&r, &sec);
    tv.tv_sec = sec;
    //strftime(buffer, sizeof buffer, "%FT%T", &r);
    strftime(buffer, sizeof buffer, "%X", &r);
    result[0] = 0;
    sprintf(result, "%s.%06ld", buffer, (long)tv.tv_usec);
    return result;
}
}
#endif //WIN32
