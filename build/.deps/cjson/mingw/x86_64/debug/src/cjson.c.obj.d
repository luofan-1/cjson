{
    files = {
        [[src\cjson.c]]
    },
    depfiles_format = "gcc",
    depfiles = "cjson.o: src\\cjson.c src\\cjson.h\
",
    values = {
        [[C:\msys64\ucrt64\bin\gcc.exe]],
        {
            "-m64",
            "-g",
            "-O0"
        }
    }
}