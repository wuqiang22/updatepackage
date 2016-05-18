#ifndef _UPDATEPACKAGE_H_
#define _UPDATEPACKAGE_H_

#include "ManifestManager.h"
#include <wx/string.h>

class UpdatePackage
{
public:
	UpdatePackage();
	static UpdatePackage* getInstance();

	bool getVersion(); //获取最新的两个版本

	void setVersion(size_t versionBegin, size_t versionEnd); //设置更新的两个版本
	void setThisVersion(wxString  version,wxString fixVersion);

	bool updateVersion();  //更新两个版本

	bool package(std::string& reason);//打包版本
	
	bool luac(); //luac编译获取写lua文件

	bool startWork(std::string& reason);

	void setNeedLuac(bool isNeed);

	void setUpdatePackagePath(wxString path);  //设置生成目录

	void encrypt(wxString  path, bool isRoot); //加密

	void setNeedEncrypt(bool needEncrypt);

	size_t getNewVersionBegin();
	size_t getNewVersionEnd();
	void setPackageUrl(std::string packageUrl);

	void setRemoteInfo(std::string remoteVersion, std::string remoteManifest);
private:
	bool m_isNeedLuac,m_isNeedEncrypt;
	size_t  m_versionBegin, m_versionEnd;
	ManifestManager m_manifestManager;

	wxString m_updatepath, m_svnpath, m_thisversion, m_workplace;

};

#endif