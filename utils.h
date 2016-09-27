#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <string>
#include <clocale>
#include <locale>
#include <iostream>
#include <codecvt>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <vector>
#include <Shlwapi.h>


using namespace std;
#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif


bool isFile(const std::string& path);

bool dirOrFileExists(const std::string& path);


int StringToWString(std::wstring &ws, const std::string &s);

wstring s2ws(const std::string& str);

string ws2s(const std::wstring& wstr);


std::string GetClipboardText();


bool setClipBoardString(string source);


vector<string> split(const char *str, char c = ' ');


std::string GetLastErrorAsString();