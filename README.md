# Data2Array
Tool to pack external files into an array

### Usage
```
Usage: Data2Array <opts> -o <output file> -i <Input file(s)>

    <opts>:
       -b - generate array for the unsigned char datatype [0x00-0xFF]
       -f - generate for ASCII characters only.
```

### Usage for CMake
```
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include(Data2Array)

add_templates(
    some_file_name.h 
    templ1.ext 
    templ2.ext)

set(SRCFILES 
    some_file_name.h
    main.cpp
)
add_executable(SomeTarget ${SRCFILES})

```
