#include <windows.h>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>

extern "C" {
    extern char tokens[32][32];
    extern char nodeList[33][24];
    void CodeGeneration(double* output);
    const char* parser();
    char* lexer(char* source);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit, hButton, hOutput;
    static HFONT hFont, hEditFont;
    static std::array<char, 256> buffer;
    static std::array<char, 256> buffer_re;
    PAINTSTRUCT ps;
    HDC hdc;
    double result;

    switch (message) {
    case WM_CREATE:
        hEdit = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 550, 35, hwnd, (HMENU)1, NULL, NULL);

        hButton = CreateWindowA("BUTTON", "Calculate", WS_CHILD | WS_VISIBLE,
            0, 35, 550, 35, hwnd, (HMENU)2, NULL, NULL);

        hOutput = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE,
            0, 70, 570, 150, hwnd, (HMENU)3, NULL, NULL);

        hEditFont = CreateFontA(35, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, "Consolas");

        hFont = CreateFontA(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, "Consolas");

        SendMessage(hEdit, WM_SETFONT, (WPARAM)hEditFont, TRUE);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hOutput, WM_SETFONT, (WPARAM)hFont, TRUE);

        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 2) {
            memset(nodeList, 0, sizeof(nodeList));
            memset(tokens, 0, sizeof(tokens));

            GetWindowTextA(hEdit, buffer.data(), buffer.size());

            if (lexer(buffer.data()) == nullptr || parser() == nullptr) {
                SetWindowTextA(hOutput, "Result: Error");
                return 0;
            }

            CodeGeneration(&result);

            std::ostringstream oss;
            oss << "Result : " << std::fixed << std::setprecision(2) << result;
            SetWindowTextA(hOutput, oss.str().c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "CalculatorWindow";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(2));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(3));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowA(
        CLASS_NAME,
        "CalculatorX",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        700, 400, 550, 250,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
