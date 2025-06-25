Add-Type @"
using System;
using System.Runtime.InteropServices;

public class NativeMethods {

    [DllImport("user32.dll" , CharSet = CharSet.Ansi)]
    public static extern int MessageBoxA(IntPtr hWnd, string text, string caption, uint type);

    [StructLayout(LayoutKind.Sequential)]
    public struct SYSTEMTIME {
        public ushort wYear;
        public ushort wMonth;
        public ushort wDayOfWeek;
        public ushort wDay;
        public ushort wHour;
        public ushort wMinute;
        public ushort wSecond;
        public ushort wMilliseconds;
    }

    [DllImport("kernel32.dll")]
    public static extern uint GetCurrentProcessId();
    [DllImport("kernel32.dll")]
    public static extern void GetSystemTime(out SYSTEMTIME time);

}
"@

$time = New-Object NativeMethods+SYSTEMTIME
[NativeMethods]::GetSystemTime([ref]$time)
Write-Host "Day : $($time.wDay)"
