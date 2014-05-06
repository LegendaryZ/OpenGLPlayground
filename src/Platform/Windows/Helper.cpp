#include "../Helper.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

/**
 * Convert a LPWSTR to a std::string
 *
 * @param wstr	the string to convert
 *
 * @return strTo	the converted string
 **/
inline std::string wstrtostr(const std::wstring &wstr)
{
    std::string strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

/**
 * Get the names of all the files in a directory
 *
 * @param d		The directory to search in
 * @param f		The vector to insert filenames into
 **/
void getFilesInDir(const char* d, std::vector<std::string> & f)
{
    WIN32_FIND_DATA ffd;

	const int dirLen = strlen(d)+1;
	wchar_t* wtext = new wchar_t[dirLen];
	mbstowcs(wtext, d, strlen(d)+1);//Plus null
	LPWSTR dirName = wtext;

    HANDLE hFind = FindFirstFile(dirName, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        std::cerr << _T("Invalid handle value.") << GetLastError() << std::endl;
        return;
    }

    bool finished = false;
    while (!finished)
    {
		if(ffd.cFileName[0] != '.')
			f.push_back(wstrtostr(ffd.cFileName));

        if (!FindNextFile(hFind, &ffd))
            finished = true;
    }

	delete[] wtext;
}
