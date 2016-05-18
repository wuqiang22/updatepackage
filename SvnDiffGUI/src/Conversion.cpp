#include "Conversion.h"


wchar_t* string2wchar_t_star(std::string from)
{
	std::size_t len = from.length() + 1;
	wchar_t* ret = new wchar_t[len];
	memset(ret, 0, sizeof(wchar_t)*len);
	MultiByteToWideChar(CP_ACP, 0, from.c_str(), -1, ret, len);
	return ret;
}


char* wchar_t_star2char_star(wchar_t* from)
{
	int len = WideCharToMultiByte(CP_ACP, 0, from, wcslen(from), NULL, 0, NULL, NULL);
	char* ret = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, from, wcslen(from), ret, len, NULL, NULL);
	ret[len] = '\0';
	return ret;
}


std::string cstring2string(CString from)
{

#ifdef UNICODE
	LPWSTR lpwstr =  from.GetBuffer(from.GetLength());
	char* temp = wchar_t_star2char_star(lpwstr);
	std::string ret(temp);
	if (temp)
	{
		delete(temp);
		temp = nullptr;
	}
	return ret;
#else
	return std::string(from.GetBuffer(from.GetLength()));
#endif // !UNICODE

}



CString string2cstring(std::string from)
{
	return CString(from.c_str());
}


CString int2cstring(std::size_t from)
{
	CString ret;
	ret.Format(_T("%d"), from);
	return ret;
}

CString float2cstring(float from)
{
	CString ret;
	ret.Format(_T("%f"), from);
	return ret;
}

int cstring2int(CString from)
{
	return _ttoi(from);
}

float cstring2float(CString from)
{
	return _ttof(from);
}

