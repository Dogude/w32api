# Creating a custom C struct in Python
#
#
#
#

from ctypes import WinDLL, c_char_p, c_void_p, c_int, c_uint, c_ulong
from ctypes import Structure, WINFUNCTYPE, POINTER , create_string_buffer

class Data(Structure):
    _fields_ = [
        ("style", c_uint),
        ("cbClsExtra", c_int), 
    ]


db = Data()
db.style = 990
print(db.style)
