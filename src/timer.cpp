#include "timer.h"

#define TIMELOCALE LOCALE_SYSTEM_DEFAULT
#define TIMEFORMAT TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER


LPCWSTR Timer::getCurrentTime() {
    currentTime = getTimeNow();
    return currentTime.c_str();

}

LPCWSTR Timer::getTimerTime() {
    return timerTime.c_str();
}

bool Timer::validateInput(TCHAR* buff) {
    const std::wstring tmpInput(&buff[0]);
    const std::string input(tmpInput.begin(), tmpInput.end());

    const std::regex inputRegex("^\\d+(\\:|\\.)\\d+$");

    if (std::regex_match(input, inputRegex)) {
        std::smatch captureNumbers;
        std::regex_search(input, captureNumbers, std::regex("^(\\d+).(\\d+)$"));
        // for (int i = 0; i < captureNumbers.size(); i++) {
        //     printf("%d is %s\n", i, captureNumbers[i].str().c_str());
        // }
        int hours, minutes;
        hours = std::stoi(captureNumbers[1].str().c_str());
        minutes = std::stoi(captureNumbers[2].str().c_str());
        convertTime(hours, minutes);
        return true;
    } else {
        return false;
    }
}

std::wstring Timer::getTimeNow() {
    wchar_t time[32];
    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER,
        NULL, NULL, time, sizeof(time));
    return time;
}

void Timer::convertTime(int hours, int minutes) {
    SYSTEMTIME st = {0};
    FILETIME ft = {0};

    int addition = 0;
    wchar_t timeBuff[32];

    GetLocalTime(&st);

    // Debug
    wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    SystemTimeToFileTime(&st, &ft);

    ULARGE_INTEGER u = {0};

    memcpy(&u, &ft, sizeof(u));
    if (hours > 0) {
        u.QuadPart += (3600 * hours) * 10000000LLU;
    }
    if (minutes > 0) {
        u.QuadPart += (60 * minutes) * 10000000LLU;
    }
    memcpy(&ft, &u, sizeof(ft));

    FileTimeToSystemTime(&ft, &st);

    // Debug
    wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER,
        &st, NULL, timeBuff, sizeof(timeBuff));

    timerTime = timeBuff;
}