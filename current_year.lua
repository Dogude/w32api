timeid = alloc(16)

GetSystemTime(timeid)

local year = fetch(timeid,0,'int') -- read wYear and wMonth field together
year = year & 0XFFFF -- zero out upper bits since year assumed to be 32 bit

MessageBoxA(tostring(year),'Current Yearer')

release(timeid)
