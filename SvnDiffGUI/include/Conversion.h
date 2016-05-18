#ifndef _CONVERSION_H
#define _CONVERSION_H

#include "stdlib.h"
#include "string"

#include <tchar.h>

#include "afxwin.h"


#define MAX_LENGTH 1024

//记得释放内存啊
//char*----------->wchar_t*
wchar_t* string2wchar_t_star(std::string from);


//记得释放内存啊
//wchar_t*----------->char*
char* wchar_t_star2char_star(wchar_t* from);

//CString------------>string
std::string cstring2string(CString from);


//string------------>CString
CString string2cstring(std::string from);

//int----------->CString
CString int2cstring(std::size_t from);

//float---------->CString
CString float2cstring(float from);


//CString------------>int
int cstring2int(CString from); 


//CString----------->float
float cstring2float(CString from);
#endif