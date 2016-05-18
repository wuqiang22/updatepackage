#include "Tool.h"
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>

void convertPathtoUnixStyle(wxString& path)
{
	for (size_t i = 0; i < path.length(); i++)
	{
		if (path.at(i) == '\\')
		{
			path.at(i) = '/';
		}
	}
}

void  checkPath(wxString& path)
{
	size_t length = path.length();
	char ch = path.at(length - 1);
	if (ch == '/' || ch == '\\')
	{
		return;
	}
	path += '/';
}

bool isAbsolutePath(wxString path)
{
	if (path.length() < 2)
	{
		return false;
	}
	return(path.at(0) >= 'A' && path.at(0) < 'Z' || path.at(0) >= 'a' && path.at(0) < 'z') && path.at(1) == ':';
}

std::vector<wxString> getWxFiles(wxString dir)
{
	std::vector<wxString> ret;
	if (wxDir::Exists(dir))
	{
		wxDir wxdir(dir);
		wxString name;
		bool exist = wxdir.GetFirst(&name);
		while (exist)
		{
			ret.push_back(name);
			exist = wxdir.GetNext(&name);
		}
	}
	return std::move(ret);
}

wxString getUpDir(wxString path)
{
	if (path.length()<=0)
	{
		return "";
	}
	bool isDir = wxDir::Exists(path);
	convertPathtoUnixStyle(path);
	
	if (isDir)
	{
		size_t  i = 0;
		for (i = path.length()-1; i >= 0; i--)
		{
			if (path.at(i)!='/')
			{
				break;
			}
		}
		path = path.substr(0, i+1);
		size_t lastIndex = path.find_last_of('/');
		return path.substr(0, lastIndex+1);
	}
	else{
		size_t  count = 0;
		size_t lastIndex = -1;
		for (size_t i = path.length()-1; i >= 0;i--)
		{
			if (path.at(i) == '/' && i!=lastIndex-1)
			{
				count++;
				lastIndex = i;
			}
			if (count == 2)
			{
				return path.substr(0, i+1);
			}
		}
	}
	return path;
}

wxString getCurDir(wxString path)
{
	if (wxDir::Exists(path))
	{
		return path;
	}
	
	wxFileName wxfilename(path);
	wxString name = wxfilename.GetFullName();
	size_t  index = path.find(name);
	return path.substr(0, index);
}


void tryMakeDir(wxString path)
{

	for (size_t i = 0; i < path.length();i++)
	{
		if (path.at(i) == '/' || path.at(i) == '\\')
		{
			wxString  dir = path.substr(0, i+1);
			if (!wxDir::Exists(dir))
			{
				wxDir::Make(dir);
			}
		}
	}
}

wxString getLastNameByPath(wxString path)
{

	if (path.at(path.length() - 1) == '/' || path.at(path.length() - 1) =='\\')
	{
		for (size_t i = path.length() - 1; i >= 0; i--)
		{
			if (path.at(i) != '/' && path.at(i) != '\\')
			{
				path = path.substr(0, i+1);
				break;
			}
		}
	}

	for (size_t i = path.length()-1; i >=0; i--)
	{
		if (path.at(i) == '/' || path.at(i) == '\\')
		{
			wxString temp = path.substr(i + 1, path.length() - i - 1);
			return temp;

		}
	}
	return path;
}

void removeDir(wxString path)
{
	if (!wxDir::Exists(path))
	{
		return;
	}
	checkPath(path);
	wxDir wxdir(path);
	if (wxdir.IsOpened())
	{
		std::vector<wxString> filenames = getWxFiles(path);
		for (wxString name : filenames)
		{
			wxString fullpath = path + name;
			if (wxDir::Exists(fullpath))
			{
				removeDir(fullpath);
			}
			else{
				wxRemoveFile(fullpath.c_str().AsChar());
			}
		}
	}
	wxRmdir(path);
}

wxString  readData(wxString path)
{
	wxString ret("");
	FILE* fp = NULL;
	do 
	{
		if (!wxFile::Exists(path))
		{
			break; 
		}
		fp = fopen(path.c_str(), "rb");
		if (!fp)
		{
			break;
		}
		char temp[512] = { 0 };
		while (!feof(fp))
		{
			memset(temp, 0, 512);
			size_t readSize = fread(temp, sizeof(char), 512, fp);
			ret.append(temp, readSize);
		}
	} while (0);
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
	return ret;
}