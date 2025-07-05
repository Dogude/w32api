from ctypes import WinDLL, c_char_p, c_void_p, c_int, c_uint, c_ulong
from ctypes import Structure, WINFUNCTYPE, POINTER

user32 = WinDLL("user32.dll")

user32.MessageBoxA.argtypes = [
    c_void_p,  
    c_char_p,  
    c_char_p,  
    c_uint       
]

user32.MessageBoxA.restype = c_uint

hwnd = user32.MessageBoxA(
        None,
        b"hello",
        b"from win32 python",
        3

)

print(f" (HWND): {hwnd}")

WNDPROC = WINFUNCTYPE(c_int, c_void_p, c_uint, c_void_p, c_void_p)

class WNDCLASSA(Structure):
    _fields_ = [
        ("style", c_uint),
        ("lpfnWndProc", WNDPROC),
        ("cbClsExtra", c_int),
        ("cbWndExtra", c_int),
        ("hInstance", c_void_p),
        ("hIcon", c_void_p),
        ("hCursor", c_void_p),
        ("hbrBackground", c_void_p),
        ("lpszMenuName", c_char_p),
        ("lpszClassName", c_char_p),
    ]


user32.RegisterClassA.argtypes = [POINTER(WNDCLASSA)]
user32.RegisterClassA.restype = c_uint


def wnd_proc(hwnd, msg, wParam, lParam):
    if msg == 2: 
        user32.PostQuitMessage(0)
    return user32.DefWindowProcA(hwnd, msg, wParam, lParam)

wnd_proc_c = WNDPROC(wnd_proc)

wc = WNDCLASSA()
wc.style = 0
wc.lpfnWndProc = wnd_proc_c
wc.cbClsExtra = 0
wc.cbWndExtra = 0
wc.hInstance = None
wc.hIcon = None
wc.hCursor = None
wc.hbrBackground = None
wc.lpszMenuName = None
wc.lpszClassName = b"MyWindowClass"

atom = user32.RegisterClassA(wc)
print("is not zero", atom != 0)
