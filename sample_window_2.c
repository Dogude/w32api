#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#include <shlobj.h>
#include <objbase.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

#define BUTTON_ID 1

HBITMAP hBitmap = NULL;

int check = 1;

void AutoRun() 
{

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    wchar_t exepath[MAX_PATH] = {0};  
    wchar_t autoRun[MAX_PATH] = { 0 };
    wchar_t working_directory[MAX_PATH] = { 0 };

    GetModuleFileNameW(NULL, exepath, MAX_PATH);  
    wchar_t* lastBackslash = wcsrchr(exepath, L'\\');
    *lastBackslash = '\0';
    swprintf(working_directory, MAX_PATH, L"%s\\data", exepath);
    swprintf(autoRun, MAX_PATH, L"%s\\data\\autorun.exe", exepath);

    SetCurrentDirectoryW(working_directory);

    if (!CreateProcessW(
        autoRun,   // Application name
        NULL,      // Command line arguments
        NULL,      // Process security attributes
        NULL,      // Thread security attributes
        FALSE,     // Inherit handles
        0,         // Creation flags
        NULL,      // Environment variables
        NULL,      // Current directory
        &si,       // Pointer to STARTUPINFO
        &pi        // Pointer to PROCESS_INFORMATION
    )) {
        char error[100];
        int error_code = GetLastError();
        if (error_code == 740)
        {
            snprintf(error, 100, "Error :: Run as a administrator!");
            MessageBoxA(0, error, "Error", 0);
             
        }
        else {

            snprintf(error, 100, "Error = %d",GetLastError());
            MessageBoxA(0, error, "", 0);


        }

        exit(1);

    }

    
    // WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    

}


void CreateRoamingFile() 
{
        
    char appDataPath[MAX_PATH] = { 0 };
    
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);
    
    strcat_s(appDataPath,MAX_PATH, "\\newfolder");
    
    CreateDirectoryA(appDataPath, NULL);
     
    SetCurrentDirectoryA(appDataPath);

    FILE* file = fopen("hello.txt", "w");
    
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);
     
    fprintf(file, "Resolution = %d %d", width, height);
    
    fclose(file);

    
}

void TimerProc()
{
    
    char exepath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, exepath, MAX_PATH);
    
    char* lastBackslash = strrchr(exepath, '\\');
    *lastBackslash = '\0';

    char autoRunPath[MAX_PATH] = { 0 };
    snprintf(autoRunPath, MAX_PATH, "%s\\data", exepath);
    
    SetCurrentDirectoryA(autoRunPath);
    
    char Execute[MAX_PATH] = { 0 };
    snprintf(Execute, MAX_PATH, "start %s\\folder1\\another.exe", autoRunPath);
    
    system(Execute);

    
}
 
void TimerProc_2() {

    wchar_t exepath[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, exepath, MAX_PATH);

    wchar_t* lastBackslash = wcsrchr(exepath, L'\\');
    *lastBackslash = L'\0';
    
    wchar_t newpath[MAX_PATH] = { 0 };
    swprintf(newpath, MAX_PATH, L"%s\\data\\music\\sharpening.wav", exepath);
    
    const wchar_t* dest_path = L"C:\\Program Files (x86)\\Folder\\sharpening.wav";
        
    if(!CopyFileW(newpath, dest_path, FALSE))
    {
        MessageBoxA(0, "STEP 3 FAILED.", "", 0);
    }
    
    else
    {
        MessageBoxA(0, "Copy Complete.", "", 0);
    }
    

    if (hBitmap) {
        DeleteObject(hBitmap);
    }

    PostQuitMessage(0);


}

int random_index() 
{
    
    IP_ADAPTER_INFO AdapterInfo[16]; // Allocate space for adapter info
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    
    PIP_ADAPTER_INFO pAdapter = AdapterInfo;

    /*char buffer[100];

    snprintf(buffer, 100, "%x %x %x %x %x %x", pAdapter->Address[0], pAdapter->Address[1],
        pAdapter->Address[2],
        pAdapter->Address[3],
        pAdapter->Address[4], pAdapter->Address[5]);  this prints mac address */

    int re = pAdapter->Address[4]; // use this as random number

    return  re % 8;
     
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {

        case WM_CREATE:
        {
            
            
            hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
            
            HICON hSmallIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PROGRAM));
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hSmallIcon);

            CreateWindowExW(
                0, L"BUTTON", L"STEP 1",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                76, 75, 100, 30, // x, y, width, height
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );

            CreateWindowExW(
                0, L"BUTTON", L"STEP 2",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                195, 75, 100, 30, // x, y, width, height
                hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
            );

            CreateWindowExW(
                0, L"BUTTON", L"STEP 3",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                320, 75, 100, 30, // x, y, width, height
                hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
            );

            break;
        }

        case WM_COMMAND:
        {
            if (LOWORD(wParam) == 1) { // Check if the button was clicked
                
                check = 2;
                CreateRoamingFile();
                AutoRun();

            }

            if (LOWORD(wParam) == 2) { // Check if the button was clicked
                
                if(check == 2)
                {
                    TimerProc();
                    check = 3;

                }
                
                else 
                {
                    MessageBoxA(0,"Please Click Step 1 first","Error", 0);
                }


            }

            if (LOWORD(wParam) == 3) { // Check if the button was clicked

                if(check == 3)
                {
                    TimerProc_2();
                }
                    
                else {

                    MessageBoxA(0, "Please Click Previous Step", "Error", 0);

                }

            }

            break;
        }

        case WM_DESTROY:
        {
            if (hBitmap) {
                DeleteObject(hBitmap);
            }

            PostQuitMessage(0);
            break;

        }

        case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;

            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
            
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);

            GetClientRect(hwnd, &rc);
            
            StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
            
            int random = random_index();
            DrawTextA(hdc, "Do Not Close This Window !!!", -1, &rc, DT_SINGLELINE | DT_CENTER);
            DrawTextA(hdc, "Hello from world", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
            
            EndPaint(hwnd, &ps);

            break;

        }


    default:
        return DefWindowProc(hwnd, message, wParam, lParam);


    }
    return 0;
}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int CmdShow)
{

    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {0};
    
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_PROGRAM));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"LOTR Setup",     // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,            // Window style

        // Size and position
        200, 200, 500, 150,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );


    ShowWindow(hwnd, CmdShow);
    UpdateWindow(hwnd);

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;


}

