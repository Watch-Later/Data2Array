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
#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
#define MAX_PRINT_PER_LINE 16

typedef vector<string> svec_t;

class Compiler
{
public:
    Compiler();

    bool   m_bin;
    bool   m_filter;
    string m_output;
    svec_t m_input;

    void writeSource(FILE* fp, const string& filename);

private:
    void upper(string& str) const;
    void split(svec_t& dest, const string& spl, const string& expr);
    void replace(string& in, const string& from, const string& to);
    void getAsBuffer(const string& in, char** buffer, int& fileSize);

    string base(const string& in);
    string extension(const string& in);
};

int main(int argc, char** argv)
{
    int      i;
    Compiler c;

    if (argc <= 3)
    {
        cout << "Usage: " << argv[0] << " <opts>";
        cout << "-o <output file> -i <Input file(s)>\n";
        cout << " <opts>\n";
        cout << "    -b - generate array for the unsigned char datatype [0x00-0xFF]\n";
        cout << "    -f - generate for ASCII characters only. \n";
        return 1;
    }

    for (i = 1; i < argc; ++i)
    {
        string opt = argv[i];
        transform(opt.begin(), opt.end(), opt.begin(), ptr_fun<int, int>(tolower));


        if (opt == "-b")
            c.m_bin = true;
        else if (opt == "-f")
            c.m_filter = true;
        else if (opt == "-o")
        {
            if (i + 1 < argc)
                c.m_output = argv[++i];
        }
        else if (opt == "-i")
        {
            ++i;
            while (i < argc)
                c.m_input.push_back(argv[i++]);
        }
        else
            cout << "Unknown option '" << opt << "'." << endl;
    }

#ifdef _WIN32
    FILE* fp = fopen(c.m_output.c_str(), "w");
#else
    FILE* fp = fopen(c.m_output.c_str(), "wb");
#endif

    if (!fp)
    {
        cout << "Failed to load file '" << c.m_output << "'." << endl;
        return -1;
    }


    auto it = c.m_input.begin();
    while (it != c.m_input.end())
        c.writeSource(fp, *it++);
    fclose(fp);
    return 0;
}



Compiler::Compiler() :
    m_bin(false),
    m_filter(false),
    m_output("")
{
}

void Compiler::upper(string& str) const
{
    transform(str.begin(),
              str.end(),
              str.begin(),
              ptr_fun<int, int>(toupper));
}


void Compiler::split(svec_t& rval, const string& spl, const string& expr)
{
    string str = spl;
    rval.reserve(32);
    for (;;)
    {
        size_t pos = str.find_first_of(expr);
        if (pos != string::npos)
        {
            if (pos == 0)
                pos = pos + 1;

            string ss = str.substr(0, pos);
            if (!ss.empty() && expr.find(ss) == string::npos)
                rval.push_back(ss);
            str.erase(0, pos);
        }
        else
        {
            if (!str.empty())
                rval.push_back(str);
            break;
        }
    }
}

void Compiler::replace(string& in, const string& from, const string& to)
{
    if (!from.empty() && from != to)
    {
        if (to.empty())
        {
            size_t pos = 0;
            while (pos != string::npos)
            {
                pos = in.find(from);
                if (pos != string::npos)
                    in.erase(pos, from.size());
            }
        }
        else
        {
            size_t pos = 0;
            while (pos != string::npos)
            {
                pos = in.find(from);
                if (pos != string::npos)
                {
                    in.erase(pos, from.size());
                    in.insert(pos, to);
                }
            }
        }
    }
}

void Compiler::writeSource(FILE* fp, const string& fileName)
{
    int   fileSize = 0, i, cp;
    char* data     = nullptr;
    getAsBuffer(fileName, &data, fileSize);

    if (data != nullptr && fileSize > 0)
    {
        string name = base(fileName);
        replace(name, extension(fileName), "");
        replace(name, ".", "_");
        replace(name, "-", "_");
        upper(name);

        if (m_filter)
            fprintf(fp, "const char %s[]={\n", name.c_str());
        else
            fprintf(fp, "const unsigned char %s[%i]={\n", name.c_str(), fileSize + 1);

        fprintf(fp, "    ");

        int acc = 0, wb = 0;
        for (i = 0; i < fileSize; ++i)
        {
            cp = (int)(unsigned char)data[i];
            if (m_filter)
            {
                if (cp > 9 && cp < 127)
                    acc++;
            }
            else
                ++acc;
            if (acc > wb)
            {
                fprintf(fp, "0x%02X, ", cp);
                if ((acc - 1) % (MAX_PRINT_PER_LINE) == (MAX_PRINT_PER_LINE - 1))
                    fprintf(fp, "\n    ");
                wb = acc;
            }
        }

        fprintf(fp, "0x00");
        fprintf(fp, "\n};// %s\n", name.c_str());
        fprintf(fp, "const unsigned int %s_SIZE=sizeof(%s);\n\n", name.c_str(), name.c_str());
        delete[] data;
    }
}


void Compiler::getAsBuffer(const string& in, char** buffer, int& fileSize)
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
        char* data = new char[(size_t)fileSize + 1];
        fread(data, 1, fileSize, fp);

        data[fileSize] = 0;
        *buffer        = data;
    }

    fclose(fp);
}


string Compiler::base(const string& in)
{
    string path = in;
    replace(path, "\\", "/");

    vector<string> arr;
    split(arr, path, "/");

    if (arr.empty())
        return "";

    return arr.at(arr.size() - 1);
}


string Compiler::extension(const string& in)
{
    string name = base(in);
    if (name.empty())
        return "";

    vector<string> arr;
    split(arr, name, ".");

    if (arr.empty())
        return "";

    return string(".") + arr.at(arr.size() - 1);
}
