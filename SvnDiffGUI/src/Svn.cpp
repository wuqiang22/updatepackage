#include "Svn.h"
#include "wx/wfstream.h"
#include "wx/zipstrm.h"
#include "wx/txtstrm.h"
#include <stdlib.h>
#include "Tool.h"
#include<direct.h>

wxString Svn::logfile = "log/log.txt";
wxString Svn::difffile = "log/diff_%s.txt";
wxString Svn::svnServerPath = "http://10.17.1.60:8080/svn/projectmax/thelastday/projects/TheLastDay/trunk/TheLastDay/";


#define  MAX_BUFF_SIZE 512

Svn* Svn::getInstance()
{
	static Svn svn;
	return &svn;
}

std::vector<wxString> Svn::getSubPaths()
{
	std::vector<wxString> ret;
	ret.push_back("src");
	ret.push_back("res");
	return ret;
}

void Svn::setWorkPath(wxString svnPath)
{
	m_svnpath = svnPath;
	m_updatepath = svnPath+"/update/";
}

void Svn::getLastTwoVersionId(wxString path, size_t& version1, size_t& version2)
{
	version2 = 0;
	version1 = 0;


	executeLogCommand(path);
	FILE* file = fopen(m_svnpath + logfile.c_str().AsChar(), "rb+");

	if (file)
	{
		char buff[MAX_BUFF_SIZE] = { 0 };
		std::string  temp;
		while (!feof(file))
		{
			memset(buff, 0, MAX_BUFF_SIZE);
			char* ret = fgets(buff, MAX_BUFF_SIZE, file);
			if (ret !=NULL)
			{
				temp = ret;

				if (temp.find('|') != std::string::npos)
				{
					std::string  first = temp.substr(1, temp.find_first_of('|') - 1);

					if (version2 == 0)
					{
						version2 = atoi(first.c_str());
						continue;
					}
					if (version1 == 0)
					{
						version1 = atoi(first.c_str());
						break;
					}
				}
			}
		}
		fclose(file);
	}
}

std::vector<wxString>  Svn::getDiffFiles(wxString path,wxString subpath, size_t version1, size_t version2)
{
	std::vector<wxString> ret;
	executeDiffCommand(path, subpath,version1, version2);

	wxString filename = wxString::Format(difffile, subpath);
	FILE* file = fopen(m_svnpath + filename.c_str().AsChar(), "rb");
	wxString  data;
	if (file)
	{
		char buff[MAX_BUFF_SIZE] = {0};
		while (!feof(file))
		{
			memset(buff, 0, MAX_BUFF_SIZE);
			char* retbuff = fgets(buff, MAX_BUFF_SIZE, file);
			if (retbuff != NULL)
			{
				wxString temp = retbuff;
				if (temp.find("Index: ")==0)
				{
					ret.push_back(path+"/" + temp.substr(7,temp.length()-9));
				}
			}
		}
		fclose(file);
	}
	
	return std::move(ret);
}

void Svn::executeDiffCommand(wxString path, wxString subpath,size_t version1, size_t version2)
{
	path = svnServerPath + path;
	wxString filename = m_svnpath + wxString::Format(difffile, subpath);
	remove(filename.c_str().AsChar());
	tryMakeDir(filename);
	wxString command = wxString::Format("svn diff -r %d:%d %s>>%s", version1, version2, path.c_str().AsChar(), filename.c_str().AsChar());
	system(command);
}

void Svn::executeLogCommand(wxString path)
{
	path = svnServerPath + path;
	wxString filename = m_svnpath + logfile;
	remove(filename.c_str().AsChar());
	tryMakeDir(filename);
	wxString command = wxString::Format("svn log %s>>%s", path.c_str().AsChar(), filename.c_str().AsChar());
	system(command.c_str().AsChar());
}

void Svn::executeUpdateCommand(wxString path, size_t version, wxString destPath)
{
	path = svnServerPath + path;
	wxString command = wxString::Format("svn cat -r %i %s>>%s", version, path, destPath);
	system(command.c_str().AsChar());
}

void Svn::checkVersionDiffFiles(size_t versionBegin, size_t versionEnd)
{
	removeDir(m_svnpath);
	checkPath(m_svnpath);
	std::vector<wxString> subpaths = getSubPaths();
	for (wxString path : subpaths)
	{
		std::vector<wxString> diffFiles = getDiffFiles(path, path,versionBegin, versionEnd);
		for (wxString file : diffFiles)
		{
			wxString destPath = m_updatepath + file;
			tryMakeDir(destPath);
			executeUpdateCommand(file, versionEnd, destPath);
			printf("更新成功:  %s\n", file.c_str().AsChar());
		}
	}
}

wxString Svn::getUpdatePath()
{
	return m_updatepath;
}