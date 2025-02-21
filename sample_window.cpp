#include "framework.h"
#include "WindowsProject1.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
        case WM_DESTROY:
        
            PostQuitMessage(0);
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rc;
            
            GetClientRect(hwnd, &rc);

            DrawTextW(hdc, L"HELLO WIN32", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            EndPaint(hwnd, &ps);

        }


            default:
                return DefWindowProc(hwnd, message, wParam, lParam);
    
    
    }
    return 0;
}


int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int CmdShow)
{

    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc {};
    
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",     // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

   
    ShowWindow(hwnd, CmdShow);
    UpdateWindow(hwnd);

    MSG msg{};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return (int)msg.wParam;

						
}


