#ifndef UNICODE
#define UNICODE
#endif

#define BTN_SET_PRESS 111

#include <windows.h>
#include <string>
#include <datetimeapi.h>

const int TimerId = 1001;

HWND TextBox, ActButton;
LPCWSTR newTime = {0};

const std::wstring currentDateTime()
{
    wchar_t time[32];

	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, NULL, NULL, time, 32);

    return time;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Counter window";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles.
        CLASS_NAME,          // Window class
        L"Counter sample",   // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 150,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void UpdateUI(HWND hwnd, LPCWSTR* newTime) {
    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, true);

    hdc = BeginPaint(hwnd, &ps);

    LPCWSTR getTime = currentDateTime().c_str();

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    OutputDebugStringW(getTime);
    OutputDebugStringW(*newTime);

    TextOut(hdc, 10, 0, L"Time now", 9);
    TextOut(hdc, 90, 0, L"Time then", 10);
    TextOut(hdc, 10, 60, L"Timer", 6);
    TextOut(hdc, 10, 20, getTime, lstrlen(getTime));
    TextOut(hdc, 90, 20, *newTime, lstrlen(*newTime));
    
    EndPaint(hwnd, &ps);
}

void CalculateNewTime(TCHAR* buff, LPCWSTR* newTime) {
    SYSTEMTIME st = {0};
    FILETIME ft = {0};

    wchar_t anothertime[32];

    int addition = 0;

    GetLocalTime(&st);

    wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    std::wstring additionTmp(&buff[0]);
    
    try {
        addition = std::stoi(additionTmp.c_str());
    } catch (...) {
        printf("Error in conversion!");
    }

    SystemTimeToFileTime(&st, &ft);

    ULARGE_INTEGER u = {0};

    memcpy(&u, &ft, sizeof(u));
    u.QuadPart += (3600 * addition) * 10000000LLU;
    memcpy(&ft, &u, sizeof(ft));

    FileTimeToSystemTime(&ft, &st);

    wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &st, NULL, anothertime, 32);

    std::wstring tmpTime = anothertime;

    *newTime = tmpTime.c_str();
}

void RestartTimer(HWND hwnd) {
    SetTimer(hwnd, TimerId, 1000, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            TextBox = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE,
                        10, 80, 60, 20, hwnd, (HMENU) 1, NULL, NULL);

            ActButton = CreateWindow(L"BUTTON", L"Set", WS_VISIBLE | WS_CHILD | WS_BORDER,
                         100, 80, 65, 21, hwnd, (HMENU)BTN_SET_PRESS, NULL, NULL);

        case WM_TIMER:
            RestartTimer(hwnd);

        case WM_PAINT:
            UpdateUI(hwnd, &newTime);
            break;
        
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_SET_PRESS) {
                LPWSTR input;
                TCHAR buff[1024];
                GetWindowText(TextBox, buff, 1024);
                CalculateNewTime(buff, &newTime);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);

    }
    return 0;    
}