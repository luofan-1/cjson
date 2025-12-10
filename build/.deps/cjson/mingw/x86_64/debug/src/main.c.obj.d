{
    values = {
        [[C:\msys64\ucrt64\bin\gcc.exe]],
        {
            "-m64",
            "-g",
            "-O0"
        }
    },
    depfiles_format = "gcc",
    files = {
        [[src\main.c]]
    },
    depfiles = "main.o: src\\main.c src\\cjson.h\
"
}