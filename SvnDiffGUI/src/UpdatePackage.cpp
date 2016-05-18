#include "UpdatePackage.h"
#include "Svn.h"
#include "Zip.h"
#include "LuaCompiler.h"
#include "Tool.h"
#include "MD5ChecksumDefines.h"
#include "CMD5Checksum.h"
#include "ResourceEncrypt.h"

UpdatePackage::UpdatePackage() :
m_isNeedLuac(false), 
m_versionBegin(0), 
m_versionEnd(0), 
m_thisversion(""),
m_isNeedEncrypt(false)
{
	m_updatepath = wxGetCwd() + "/update/";
	m_svnpath = wxGetCwd() + "/svn/";

	ResourceEncrypt::getInstance()->setIsEncrypt(m_isNeedEncrypt);
}

UpdatePackage* UpdatePackage::getInstance()
{
	static UpdatePackage instance;
	return &instance;
}

void UpdatePackage::setNeedEncrypt(bool needEncrypt)
{
	m_isNeedEncrypt = needEncrypt;
	ResourceEncrypt::getInstance()->setIsEncrypt(m_isNeedEncrypt);
}

void UpdatePackage::setUpdatePackagePath(wxString path)
{
	m_workplace = path;
	m_updatepath = m_workplace + "/update/";
	m_svnpath = m_workplace + "/svn/";

	tryMakeDir(path);
	tryMakeDir(m_updatepath);
	tryMakeDir(m_svnpath);

	Svn::getInstance()->setWorkPath(m_svnpath);
	m_manifestManager.setWorkPath(m_updatepath.c_str().AsChar());
}

void UpdatePackage::setRemoteInfo(std::string remoteVersion, std::string remoteManifest)
{
	m_manifestManager.setRemoteInfo(remoteVersion, remoteManifest);
}

void UpdatePackage::setNeedLuac(bool isNeed)
{
	m_isNeedLuac = isNeed;
}


void UpdatePackage::setVersion(size_t versionBegin, size_t versionEnd)
{
	m_versionBegin = versionBegin;
	m_versionEnd = versionEnd;
	
}

void UpdatePackage::setPackageUrl(std::string packageUrl)
{
	m_manifestManager.setPackageUrl(packageUrl);
}

void UpdatePackage::setThisVersion(wxString  version, wxString fixVersion)
{
	m_thisversion = version;
	m_manifestManager.setFixVersion(fixVersion.c_str().AsChar());
	m_manifestManager.setVersion(version.c_str().AsChar());

	m_manifestManager.setFixVersion(fixVersion.c_str().AsChar());
}

bool UpdatePackage::getVersion()
{
	int temp1 = (int)&m_versionBegin;
	int temp2 = (int)&m_versionEnd;
	Svn::getInstance()->getLastTwoVersionId("", m_versionBegin, m_versionEnd);//3664196
	printf("\n#########################\n");
	printf("版本差：%i------>%i\n\n", m_versionBegin, m_versionEnd);
	return true;
}

bool UpdatePackage::updateVersion()
{
	Svn::getInstance()->checkVersionDiffFiles(m_versionBegin, m_versionEnd); 
	return true;
}

bool UpdatePackage::package(std::string& reason)
{
	bool ret = true;
	do 
	{
		size_t group_size;
		m_manifestManager.updateGroupSize(group_size);
		group_size++;

		wxString tempSvnUpdatePath = Svn::getInstance()->getUpdatePath();
		wxString zipName = wxString::Format("%s.zip", m_thisversion.c_str().AsChar());
		if (!Zip::getInstance()->zipComparessDir(tempSvnUpdatePath, m_updatepath, zipName))
		{
			reason = "压缩文件失败";
			ret = false; 
			break;
		}

		wxString tempZipFullPath = m_updatepath + zipName;
		size_t  zipSize = 0;
		if (wxFile::Exists(tempZipFullPath))
		{
			wxFile zipFile(tempZipFullPath);
			if (zipFile.IsOpened()) zipSize = zipFile.Length();
		}

		wxString groupName = wxString::Format("%i", group_size);
		if (!m_manifestManager.addGroupVersion(groupName.c_str().AsChar(), m_thisversion.c_str().AsChar(), zipSize))
		{
			reason = "添加版本配置文件失败";
			ret = false;
			break;
		}

		wxString assetName = wxString::Format("update%i", group_size);
		std::string md5 = CMD5Checksum::md5(zipName.c_str().AsChar());
		if (!m_manifestManager.addAsset(assetName.c_str().AsChar(), zipName.c_str().AsChar(), md5, true))
		{
			reason = "添加资源文件失败";
			ret = false;
			break;
		}

		if (!m_manifestManager.save())
		{
			reason = "保存配置文件失败";
			ret = false;
			break;
		}

	} while (0);
	
	return ret;
}

bool UpdatePackage::luac()
{
	if (!m_isNeedLuac)
	{
		return true;
	}
	return LuaCompiler::getInstance()->compileDir(m_svnpath); 
}

void UpdatePackage::encrypt(wxString path, bool isRoot)
{

	checkPath(path);

	wxDir dir(path);
	if (dir.IsOpened())
	{
		wxString  filename;
		bool cont = dir.GetFirst(&filename);
		std::vector<std::string> filenames;
		while (cont)
		{
			filenames.push_back(filename.c_str().AsChar());
			cont = dir.GetNext(&filename);
		}

		for (std::string name : filenames)
		{
			wxString fullpath = path + name;
			if (wxDir::Exists(fullpath))
			{
				encrypt(fullpath.c_str().AsChar(), false);
			}
			else
			{
				ResourceEncrypt::getInstance()->encryptFile(fullpath.c_str());
			}

			if (!(isRoot && (name == "src" || name == "res")))
			{
				std::string outname;
				ResourceEncrypt::getInstance()->encryptName(name, outname);
				std::string temppath = path + outname;
				rename(fullpath.c_str().AsChar(), temppath.c_str());
			}
		}
	}
	else{
		printf("open path:%s failed", path);
	}
}

bool UpdatePackage::startWork(std::string& reason)
{
	bool ret = true;
	do 
	{

		if (!updateVersion())
		{
			reason = "更新文件出现错误";
			printf("update version fail");
			ret = false;
			break;
		}
		if (!luac())
		{
			reason = "lua 编译出现错误";
			printf("luac fail");
			ret = false;
			break;
		}
		if (m_isNeedEncrypt)
		{
			encrypt(Svn::getInstance()->getUpdatePath(), true);
		}
		
		if (!package(reason))
		{
			printf("package fail");
			ret = false;
			break;
		}
	} while (0);

	return ret;
}

size_t UpdatePackage::getNewVersionBegin()
{
	return m_versionBegin;
}

size_t UpdatePackage::getNewVersionEnd()
{
	return m_versionEnd;
}