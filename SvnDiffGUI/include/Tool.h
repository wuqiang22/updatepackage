// 工具类,一些全局方法

#ifndef _TOOL_H_  
#define _TOOL_H_

#include <wx/string.h>
#include <wx/dir.h>
#include <vector>

void convertPathtoUnixStyle(wxString& path);  /*   ('\\'---转换------>'/')        */

void  checkPath(wxString& path);  //最后一位加上'/'

std::vector<wxString> getWxFiles(wxString dir);  //获取目录dir下的所有文件/文件夹

bool isAbsolutePath(wxString path);  //是否是绝对路径

wxString getUpDir(wxString path); //获取上级目录字段

wxString getCurDir(wxString path); //获取当前目录字段

wxString getLastNameByPath(wxString path);  //获取路径最后一个名字

void tryMakeDir(wxString path);  //创建目录 wxDir的Make方法只能创建一个目录

void removeDir(wxString path);  //删除一个目录

wxString  readData(wxString path);  //读取文件内容

#endif