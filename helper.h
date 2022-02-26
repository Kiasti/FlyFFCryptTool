#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Todo: Alternate way to avoid windows header include.
namespace helper
{
	inline int WideCharToMultiByteEx(const unsigned int CodePage, const unsigned long dwFlags, const wchar_t* lpWideCharStr, const int cchWideChar, char* lpMultiByteStr, 
		const int cchMultiByte, const char* lpDefaultChar, int* lpUsedDefaultChar)
	{
		return WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cchMultiByte, lpDefaultChar, lpUsedDefaultChar);
	}

	inline int MultiByteToWideCharEx(const unsigned int CodePage, const unsigned long dwFlags, const char* lpMultiByteStr, const int cchMultiByte, wchar_t* lpWideCharStr, 
		const int cchWideChar)
	{
		return MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cchMultiByte, lpWideCharStr, cchWideChar);
	}
}