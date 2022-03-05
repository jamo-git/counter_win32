#include <string>
#include <datetimeapi.h>
#include <windows.h>
#include <regex>

class Timer {
    public:
        LPCWSTR getCurrentTime();
        LPCWSTR getTimerTime();
        bool validateInput(TCHAR* buff);
    private:
        std::wstring currentTime = {0};
        std::wstring timerTime = {0};
        std::wstring getTimeNow();
        void convertTime(int hours, int minutes);

};