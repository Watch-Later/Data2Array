/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <stdio.h>


#define MAX_PRINT_PER_LINE 20

typedef std::string String;
typedef std::vector<String> StringArray;


class Compiler
{
public:
    Compiler();
    ~Compiler() {}

    bool bin;
    bool java;
    bool cs;
    bool filter;

    String      m_output;
    StringArray m_input;

    void writeSource(FILE* fp, const String& filename);

private:

    void upper(String& str);
    void split(StringArray& dest, const String& spl, const String& expr);
    void replace(String& in, const String& from, const String& to);
    void getAsBuffer(const String& in, char** buffer, int& fileSize);

    String base(const String& in);
    String extension(const String& in);
};






int main(int argc, char** argv)
{
    if (argc <= 3)
    {
        std::cout << "Usage: " << argv[0] << " <opts>";
        std::cout << "-o <output file> -i <Input file(s)>\n";
        std::cout << " <opts>\n";
        std::cout << "    -b - generate array for the unsigned char datatype [0x00-0xFF]\n";
        std::cout << "    -f - generate for ASCII characters only. \n";
        return 1;
    }
    int i = 1;

    Compiler c;
    for (i = 1; i < argc; ++i)
    {
        String opt = argv[i];
        std::transform(opt.begin(), opt.end(), opt.begin(), std::tolower);

        printf("%s ", opt.c_str());
        if (opt == "-b")
            c.bin = true;
        else if (opt == "-f")
            c.filter = true;
        else if (opt == "-o")
        {
            if (i + 1 < argc)
            {
                printf("%s ", argv[i+1]);
                c.m_output = argv[++i];
            }
        }
        else if (opt == "-i")
        {
            ++i;
            while (i < argc)
            {
                printf("%s ", argv[i]);
                c.m_input.push_back(argv[i++]);

            }
        }
    }

#ifdef _WIN32
    FILE* fp = fopen(c.m_output.c_str(), "w");
#else
    FILE* fp = fopen(c.m_output.c_str(), "wb");
#endif
    if (!fp)
    {
        printf("Failed to load file %s\n", argv[1]);
        return -1;
    }


    StringArray::iterator it = c.m_input.begin();
    while (it != c.m_input.end())
        c.writeSource(fp, (*it++).c_str());

    fclose(fp);
    return 0;
}




Compiler::Compiler() :
    bin(false),
    java(false),
    cs(false),
    filter(false),
    m_output("")
{
}

void Compiler::upper(String& str)
{
    std::transform(str.begin(), str.end(), str.begin(), toupper);
}


void Compiler::split(StringArray& rval, const String& spl, const String& expr)
{
    String string = spl;
    rval.reserve(32);
    for (;;)
    {
        size_t pos = string.find_first_of(expr);
        if (pos != String::npos)
        {
            if (pos == 0)
                pos = pos + 1;
            String sub = string.substr(0, pos);
            if (!sub.empty() && expr.find(sub) == String::npos)
                rval.push_back(sub);
            string.erase(0, pos);
        }
        else
        {
            if (!string.empty())
                rval.push_back(string);
            break;
        }
    }
}

void Compiler::replace(String& in, const String& from, const String& to)
{
    if (!from.empty() && from != to)
    {
        if (to.empty())
        {
            size_t pos = 0;
            while (pos != String::npos)
            {
                pos = in.find(from);
                if (pos != String::npos)
                    in.erase(pos, from.size());
            }
        }
        else
        {
            size_t pos = 0;
            while (pos != String::npos)
            {
                pos = in.find(from);
                if (pos != String::npos)
                {
                    in.erase(pos, from.size());
                    in.insert(pos, to);
                }
            }
        }
    }
}

void Compiler::writeSource(FILE* fp, const String& fileName)
{

    int fileSize = 0;
    char* data = 0;
    getAsBuffer(fileName, &data, fileSize);

    if (data != 0 && fileSize > 0)
    {
        String name = base(fileName);
        replace(name, extension(fileName), "");
        replace(name, ".", "_");
        upper(name);

        if (filter)
            fprintf(fp, "static char %s[%i]={\n", name.c_str(), fileSize+1);
        else
            fprintf(fp, "static unsigned char %s[%i]={\n", name.c_str(), fileSize+1);

        fprintf(fp, "    ");
        for (int i = 0; i < fileSize; ++i)
        {
            unsigned char cp = data[i];
            if (filter)
            {
                if (cp < 9 && cp > 127)
                    continue;
            }
            fprintf(fp, "0x%02X,", ((int)cp));
            if (i % (MAX_PRINT_PER_LINE) == (MAX_PRINT_PER_LINE - 1))
                fprintf(fp, "\n    ");
        }
        fprintf(fp, "0x00");

        fprintf(fp, "\n};// %s\n", name.c_str());
        fprintf(fp, "static unsigned int %s_SIZE=%i;\n\n", name.c_str(), fileSize);

        delete[]data;
    }
}

void Compiler::getAsBuffer(const String& in, char** buffer, int& fileSize)
{
    if (!buffer)
        return;

    FILE* fp = fopen(in.c_str(), "rb");
    if (!fp)
        return;

    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    if (fileSize > 0)
    {
        char* data = new char[fileSize + 1];
        fileSize = fread(data, 1, fileSize, fp);
        data[fileSize] = 0;
        (*buffer) = data;
        (*buffer)[fileSize] = 0;
    }

    fclose(fp);
}


String Compiler::base(const String& in)
{
    String path = in;
    replace(path, "\\", "/");

    std::vector<String> arr;
    split(arr, path, "/");

    if (arr.empty())
        return "";

    return arr.at(arr.size() - 1);
}


String Compiler::extension(const String& in)
{
    String name = base(in);
    if (name.empty())
        return "";

    std::vector<String> arr;
    split(arr, name, ".");

    if (arr.empty())
        return "";

    return String(".") + arr.at(arr.size() - 1);
}
