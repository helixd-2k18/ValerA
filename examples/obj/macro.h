#pragma once
#pragma warning(disable : 4996)

#include <string>

inline const wchar_t* STDStrToWCHAR(const std::string& str)
{
    static std::wstring temp = L"";
    wchar_t* wnstr = new wchar_t[str.length() + 1];
    mbstowcs(wnstr, str.c_str(), str.length() + 1);
    temp = wnstr;
    delete[] wnstr;
    return temp.c_str();
};

inline const wchar_t* GetWC(const char* c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);
    return wc;
};
