#include "fm_pch.h"

std::wstring s2ws(const std::string& s)
{
	UINT32 len;
	UINT32 sLength = static_cast<UINT32>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, buf, len);
	std::wstring ret(buf);
	delete[] buf;
	return ret;
}

std::string ws2s(const std::wstring& s)
{
	UINT32 len;
	UINT32 sLength = static_cast<UINT32>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), sLength, 0, 0, 0, 0);
	std::string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), sLength, &r[0], len, 0, 0);
	return r;
}