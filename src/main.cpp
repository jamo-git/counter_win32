#ifndef UNICODE
#define UNICODE
#endif

#define BTN_SET_PRESS 111

#include <windows.h>
#include <string>
#include "timer.h"

const int TimerId = 1001;

Timer timer;
HWND TextBox, ActButton;

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
        CW_USEDEFAULT, CW_USEDEFAULT, 280, 150,

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

void UpdateUI(HWND hwnd) {
    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, true);

    hdc = BeginPaint(hwnd, &ps);


    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    TextOut(hdc, 10, 0, L"Time now", 9);
    TextOut(hdc, 130, 0, L"Time then", 10);
    TextOut(hdc, 10, 60, L"Timer", 6);
    TextOut(hdc, 10, 20, timer.getCurrentTime(), lstrlen(timer.getCurrentTime()));
    TextOut(hdc, 130, 20, timer.getTimerTime(), lstrlen(timer.getTimerTime()));
    
    EndPaint(hwnd, &ps);
}

void RestartTimer(HWND hwnd) {
    SetTimer(hwnd, TimerId, 1000, NULL);
}

void CompareTimes(HWND hwnd) {
    std::wstring value1, value2;
    value1 = timer.getCurrentTime();
    value2 = timer.getTimerTime();
    if (value1 == value2) {
        MessageBox(hwnd, L"Notice!\nTime is up!", L"Info", MB_OK | MB_ICONINFORMATION);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            TextBox = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE,
                        10, 80, 60, 20, hwnd, (HMENU) 1, NULL, NULL);

            ActButton = CreateWindow(L"BUTTON", L"Set", WS_VISIBLE | WS_CHILD | WS_BORDER,
                         130, 80, 65, 21, hwnd, (HMENU)BTN_SET_PRESS, NULL, NULL);

        case WM_TIMER:
            RestartTimer(hwnd);
            CompareTimes(hwnd);

        case WM_PAINT:
            UpdateUI(hwnd);
            break;
        
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_SET_PRESS) {
                LPWSTR input;
                TCHAR buff[1024];
                GetWindowText(TextBox, buff, 1024);
                if (!timer.validateInput(buff)) {
                    MessageBox(hwnd, L"Invalid format!\nUse hh:mm", NULL, MB_OK | MB_ICONERROR);
                }
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