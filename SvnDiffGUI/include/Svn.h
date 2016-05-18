#ifndef _SVN_H_
#define _SVN_H_

#include <wx/string.h>
#include <vector>

class Svn
{
public:
	static wxString  logfile;   //log��ʱ�����ɵ��ļ�
	static wxString difffile;  //diffʱ�����ɵ��ļ�
	static wxString svnServerPath;  //svn��������ַ

	static Svn* getInstance();

	std::vector<wxString> getSubPaths();  //��Ҫ����ļ��汾������Ŀ¼

	void checkVersionDiffFiles(size_t versionBegin,size_t versionEnd);  //��⹤����Ҫ���µ�Ŀ¼�������汾֮��Ĳ����ļ�

	void getLastTwoVersionId(wxString path, size_t& version1, size_t& version2);  //�õ���������ΰ汾ID
	std::vector<wxString> getDiffFiles(wxString path, wxString subpath, size_t version1, size_t version2);  //�õ������汾֮��Ĳ����ļ�

	void setWorkPath(wxString svnPath);
	wxString getUpdatePath();
private:
	void executeLogCommand(wxString path);
	void executeDiffCommand(wxString path, wxString subpath,size_t version1, size_t version2);
	void executeUpdateCommand(wxString path, size_t version,wxString destPath);

	wxString m_svnpath,m_updatepath;
};
#endif