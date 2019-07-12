# Data2Array


```
 set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
  .. 
  include(Data2Array)

  add_templates(some_file_name.h templ1.ext templ2.ext ... etc)

  set(SRCFILES 
    some_file_name.h
    ...
  )
  add_executable(SomeTarget ${SRCFILES})
```
