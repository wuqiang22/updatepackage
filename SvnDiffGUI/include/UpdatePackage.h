#ifndef _UPDATEPACKAGE_H_
#define _UPDATEPACKAGE_H_

#include "ManifestManager.h"
#include <wx/string.h>

class UpdatePackage
{
public:
	UpdatePackage();
	static UpdatePackage* getInstance();

	bool getVersion(); //��ȡ���µ������汾

	void setVersion(size_t versionBegin, size_t versionEnd); //���ø��µ������汾
	void setThisVersion(wxString  version,wxString fixVersion);

	bool updateVersion();  //���������汾

	bool package(std::string& reason);//����汾
	
	bool luac(); //luac�����ȡдlua�ļ�

	bool startWork(std::string& reason);

	void setNeedLuac(bool isNeed);

	void setUpdatePackagePath(wxString path);  //��������Ŀ¼

	void encrypt(wxString  path, bool isRoot); //����

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