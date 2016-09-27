//#include "utils.h"
#include "stdafx.h"
#include <stdio.h>

bool isFile(const std::string& path)
{

	//return PathFileExistsA(path.c_str());
	
	FILE *fp;
	fpos_t fsize = 0;

	if (!fopen_s(&fp, path.c_str(), "r"))
	{
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &fsize);
		fclose(fp);
	}

	return fsize > 0;
	//struct stat buf;
	//stat(path.c_str(), &buf);

//	return (S_ISREG(buf.st_mode));
}

bool dirOrFileExists(const std::string& path)
{
	DWORD dwAttrib = GetFileAttributesA(path.c_str());

	auto bisDir = (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

	auto bisFile = isFile(path.c_str());

	return (bisDir || bisFile);


	//struct stat buf;
	//stat(path.c_str(), &buf);

	//return (S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode));

	//if (s.st_mode & S_IFDIR)
	//DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	//if (ftyp == INVALID_FILE_ATTRIBUTES)
	//	return false;  //something is wrong with your path!

	//if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	//	return true;   // this is a directory!

	//if (ftyp & FILE_ATTRIBUTE_)
	//	return true;   // this is a directory!


	//return false;    // this is not a directory!
}

int StringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}

std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{

		return "";

	}

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
	}

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

bool setClipBoardString(string source)
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.size() + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(source.c_str()));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
		return true;
	}
	return false;
}

std::vector<std::string> split(const char *str, char c /*= ' '*/)
{
	vector<string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

std::string strip(string source)
{
	source.erase(std::remove(source.begin(), source.end(), '\r'), source.end());
	source.erase(std::remove(source.begin(), source.end(), '\n'), source.end());
	return source;
}
