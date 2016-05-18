#include "ManifestManager.h"
#include "wx/file.h"

#include "wx/url.h"
#include "wx/sstream.h"
#include "wx/wfstream.h"

#include "Tool.h"
#include "ResourceEncrypt.h"

std::string ManifestManager::temp_version_config_path = "manifest/version.manifest";
std::string ManifestManager::temp_project_config_path = "manifest/project.manifest";


ManifestManager::ManifestManager()
: packageUrl("E:/svnworkspace/projectmax/thelastday/projects/TheLastDay/trunk/TheLastDay/res/version/project.manifest"), 
  version("1.0.0.0"), 
  fixversion("1.0.0.0"),
  m_workpath("")
{
	loadData();
}

void ManifestManager::setFixVersion(std::string fixversion)
{
	this->fixversion = fixversion;
	m_version.setFixVersion(fixversion);
	m_project.setFixVersion(fixversion);
}

void ManifestManager::setVersion(std::string version)
{
	this->version = version;
	m_project.setVersion(version);
	m_project.setVersion(version);
}

void ManifestManager::setPackageUrl(std::string packageUrl)
{
	this->packageUrl = packageUrl;
	m_project.clear();
	m_version.clear();
	loadData();
	m_version.setPackageUrl(packageUrl);
	m_project.setPackageUrl(packageUrl);
}

void ManifestManager::setRemoteInfo(std::string remoteVersion, std::string remoteManifest)
{
	m_project.setManifestFileUrl(remoteManifest);
	m_project.setVersionFileUrl(remoteVersion);

	m_version.setManifestFileUrl(remoteManifest);
	m_version.setVersionFileUrl(remoteVersion);
}


void ManifestManager::loadData()
{
	wxURL urlTool(packageUrl);
	if (!urlTool.IsOk() && !wxFile::Exists(packageUrl))
	{
		return;
	}
	wxString content = "";
	if (urlTool.IsOk())
	{
		wxInputStream *input = urlTool.GetInputStream();
		if (input == nullptr || !input->IsOk()) return;

		wxStringOutputStream output;
		input->Read(output);

		content = output.GetString();
	}
	else if (wxFile::Exists(packageUrl))
	{
		wxFileInputStream input(packageUrl);
		if (!input.IsOk()) return;
		wxStringOutputStream output;
		input.Read(output);
		content = output.GetString();
	}

	char* source = (char*)content.c_str().AsChar();
	size_t insize = content.length();
	size_t outsize = insize;
	char* dest = source;

	size_t index = 0;
	ResourceEncrypt::getInstance()->checkDecrypt(source, insize, dest, outsize);
	if (outsize<insize)
	{
		source[outsize] = '\0';
	}

	m_version.parse(dest);
	m_project.parse(dest);
	m_version.removeAssets();

	printf("\n##########################\n", packageUrl.c_str());
	printf("%s ½âÎöÍê³É\n", packageUrl.c_str());
}

bool ManifestManager::addAsset(std::string name, std::string path, std::string md5, bool isCompress)
{
	return m_project.addAsset(name, path, md5, isCompress);
}

bool ManifestManager::addGroupVersion(std::string name, std::string vername, size_t versize)
{
	return m_version.addGroupVersion(name, vername, versize) && m_project.addGroupVersion(name, vername, versize);
}

void ManifestManager::updateGroupSize(size_t & total_size)
{
	if (m_version.isVersionLoaded())
	{
		total_size = m_version.getGroupVerions().size();
	}
}

bool ManifestManager::save()
{
	m_version.saveToFile(m_workpath + temp_version_config_path);
	ResourceEncrypt::getInstance()->encryptFile((m_workpath + temp_version_config_path).c_str());
	m_project.saveToFile(m_workpath + temp_project_config_path);
	ResourceEncrypt::getInstance()->encryptFile((m_workpath + temp_project_config_path).c_str());
	return true;
}

void ManifestManager::setWorkPath(std::string path)
{
	wxString temp(path);
	checkPath(temp);
	m_workpath = temp.c_str().AsChar();
}