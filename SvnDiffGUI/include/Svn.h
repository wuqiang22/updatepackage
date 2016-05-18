#ifndef _SVN_H_
#define _SVN_H_

#include <wx/string.h>
#include <vector>

class Svn
{
public:
	static wxString  logfile;   //log的时候生成的文件
	static wxString difffile;  //diff时候生成的文件
	static wxString svnServerPath;  //svn服务器地址

	static Svn* getInstance();

	std::vector<wxString> getSubPaths();  //需要检查文件版本的所有目录

	void checkVersionDiffFiles(size_t versionBegin,size_t versionEnd);  //检测工程需要更新的目录在两个版本之间的差异文件

	void getLastTwoVersionId(wxString path, size_t& version1, size_t& version2);  //拿到最近的两次版本ID
	std::vector<wxString> getDiffFiles(wxString path, wxString subpath, size_t version1, size_t version2);  //拿到两个版本之间的差异文件

	void setWorkPath(wxString svnPath);
	wxString getUpdatePath();
private:
	void executeLogCommand(wxString path);
	void executeDiffCommand(wxString path, wxString subpath,size_t version1, size_t version2);
	void executeUpdateCommand(wxString path, size_t version,wxString destPath);

	wxString m_svnpath,m_updatepath;
};
#endif