// include necessary headers here

#include <d2d1.h>
#pragma comment(lib,"d2d1")
#pragma comment(lib,"d3dcompiler")
#pragma comment(lib, "D3D12")
#pragma comment(lib, "dxgi")

template <class T>
void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static ID2D1Factory* pFactory = nullptr;
    static ID2D1HwndRenderTarget* pRenderTarget = nullptr;
    static ID2D1SolidColorBrush* pBrush = nullptr;

    static float velocityX = 3.0f;
    static D2D1_RECT_F rect = D2D1::RectF(0.0f, 65.0f, 155.0f, 50.0f);
    static RECT rc = {};
    static float v = 13.0f;

    switch (message)
    {
    
        case WM_CREATE:
        {   
            
            SetTimer(hwnd, 1, 8, 0);
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
            GetClientRect(hwnd, &rc);
                    
            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hwnd, size),
                &pRenderTarget);
                
            
            pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBrush);

            break;

        }
        
        case WM_DESTROY:
            SafeRelease(&pRenderTarget);
            SafeRelease(&pBrush);
            SafeRelease(&pFactory);
            PostQuitMessage(0);
            break;
        
        
        case WM_PAINT: 
        {   
            
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            
            pRenderTarget->BeginDraw();
            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
            pRenderTarget->DrawRectangle(rect, pBrush);
            
            pRenderTarget->DrawRectangle({ 10.0f, 65.0f, 165.0f, 50.0f }, pBrush);
            pRenderTarget->DrawEllipse({ 300.0f,300.0f + v,30.0f,145.0f }, pBrush);
            //pRenderTarget->DrawLine({ 1.1f , 1.1f }, {330.0f,540.0f}, pBrush);
            
            pRenderTarget->EndDraw();

            EndPaint(hwnd, &ps);
            break;
                
        }
        
        case WM_TIMER:
        {
            rect.left += velocityX;
            rect.right += velocityX;
            v += 1.0f;
            
            if (rect.left == 300.0f || rect.left == 0.0f)
            {
                velocityX = -velocityX;
            }



            InvalidateRect(hwnd, &rc, 1);
            break;
        }
            
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    
    }

    return 0;
}


int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine, int       nCmdShow)
{
  
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

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


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return (int) msg.wParam;

}


