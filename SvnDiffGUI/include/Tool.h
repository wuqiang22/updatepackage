// ������,һЩȫ�ַ���

#ifndef _TOOL_H_  
#define _TOOL_H_

#include <wx/string.h>
#include <wx/dir.h>
#include <vector>

void convertPathtoUnixStyle(wxString& path);  /*   ('\\'---ת��------>'/')        */

void  checkPath(wxString& path);  //���һλ����'/'

std::vector<wxString> getWxFiles(wxString dir);  //��ȡĿ¼dir�µ������ļ�/�ļ���

bool isAbsolutePath(wxString path);  //�Ƿ��Ǿ���·��

wxString getUpDir(wxString path); //��ȡ�ϼ�Ŀ¼�ֶ�

wxString getCurDir(wxString path); //��ȡ��ǰĿ¼�ֶ�

wxString getLastNameByPath(wxString path);  //��ȡ·�����һ������

void tryMakeDir(wxString path);  //����Ŀ¼ wxDir��Make����ֻ�ܴ���һ��Ŀ¼

void removeDir(wxString path);  //ɾ��һ��Ŀ¼

wxString  readData(wxString path);  //��ȡ�ļ�����

#endif