#include "timer.h"

LPCWSTR Timer::getCurrentTime() {
    currentTime = getTimeNow();
    return currentTime.c_str();

}

LPCWSTR Timer::getTimerTime() {
    return timerTime.c_str();
}

bool Timer::validateInput(TCHAR* buff) {
    std::wstring tmpInput(&buff[0]);

    std::regex inputValidatorRx("^\\d+\\:?\\.?\\d+$");

    // if (std::regex_match(tmpInput.begin(), tmpInput.end(), inputValidatorRx)) {
    //     printf("Regex match!");
    //     setTimerTime(tmpInput);
    //     return true;
    // } else {
    //     return false;
    // }
    return true;
}

std::wstring Timer::getTimeNow() {
    wchar_t time[32];
    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, NULL, NULL, time, 32);
    return time;
}

void Timer::setTimerTime(std::wstring input) {
    // TODO: Convert input to ints and pass to convertTime -> save to timerTime
}

std::wstring Timer::convertTime(int hours, int minutes) {
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

    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &st, NULL, timeBuff, 32);

    return timeBuff;
}