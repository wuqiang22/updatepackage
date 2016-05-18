#ifndef _ManifestManager_H_
#define _ManifestManager_H_

#include "Manifest.h"
#include <vector>

class ManifestManager{
public:

	ManifestManager();
	
	static std::string version_config_path;
	static std::string project_config_path;

	static std::string  temp_version_config_path;
	static std::string temp_project_config_path;

	void setVersion(std::string version);
	void setFixVersion(std::string fixversion);
	void setPackageUrl(std::string packageUrl);
	
	bool addAsset(std::string name, std::string path, std::string md5, bool isCompress);
	bool addGroupVersion(std::string name, std::string vername, size_t versize);
	void updateGroupSize(size_t & total_size);

	void loadData();

	bool save();

	void setWorkPath(std::string path);

	void setRemoteInfo(std::string remoteVersion, std::string remoteManifest);

private :
	Manifest m_version;
	Manifest m_project;

	std::string fixversion,packageUrl, version,m_workpath;
};

#endif