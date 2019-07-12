#include <string>
#include <iostream>
#include "OutFile.h"




int main(int arcg, char **argv)
{
    std::string s1 = std::string((char*)TEST1, TEST1_SIZE);
    std::string s2 = std::string((char*)TEST2, TEST2_SIZE);


    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    return 0;
}

