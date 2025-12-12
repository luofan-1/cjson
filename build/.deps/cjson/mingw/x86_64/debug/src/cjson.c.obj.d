{
    depfiles_format = "gcc",
    files = {
        [[src\cjson.c]]
    },
    values = {
        [[C:\msys64\ucrt64\bin\gcc.exe]],
        {
            "-m64",
            "-g",
            "-O0"
        }
    },
    depfiles = "cjson.o: src\\cjson.c src\\cjson.h\
"
}