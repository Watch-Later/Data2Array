include(Data2Array)


add_templates_text(Outfile.h Test1.in Test2.in)

set(SRC 
    Main.cpp 
    OutFile.h
    Test1.in 
    Test2.in
)
add_executable(Test ${SRC})