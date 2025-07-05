
from ctypes import WinDLL, c_char_p, c_void_p, c_int, c_uint, c_ulong
from ctypes import Structure, WINFUNCTYPE, POINTER , create_string_buffer


user32 = WinDLL("user32.dll")
kernel32 = WinDLL("kernel32.dll")


kernel32.GetSystemDirectoryA.argtypes = [c_char_p, c_uint]
kernel32.GetSystemDirectoryA.restype = c_uint

buffer_size = 260
buffer = create_string_buffer(buffer_size)

length = kernel32.GetSystemDirectoryA(buffer, buffer_size)

print("System PATH:", buffer.value.decode())
  
user32.MessageBoxA(
        None,
        buffer.value,
        b"from win32 python",
        3
)
