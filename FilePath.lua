buffer = alloc(256)

re = GetModuleFileNameA(nil,buffer,256)

s = fetch(buffer,0,'str')

MessageBoxA(s,'File Path')

release(buffer)
