#include "Encrypt.h"
#include "ResourceEncrypt.h"
#include "Tool.h"

Encrypt* Encrypt::getInstance()
{
	Encrypt encrypt;
	return &encrypt;
}

bool Encrypt::encryptDir(wxString path, bool isRoot)
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
				encryptDir(fullpath.c_str().AsChar(), false);
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
	return true;
}

bool Encrypt::decryptDir(wxString path, bool isRoot)
{
	checkPath(path);
	wxDir dir(path);
	if (dir.IsOpened())
	{
		std::vector<wxString> filenames = getWxFiles(path);
		for (wxString name : filenames)
		{
			wxString fullpath = path + name;
			if (wxDir::Exists(fullpath))
			{
				decryptDir(fullpath,false);
			}
			else{
				ResourceEncrypt::getInstance()->decryptFile(fullpath.c_str().AsChar());
			}

			if (!(isRoot && (name == "src" || name == "res")))
			{
				std::string outname;
				ResourceEncrypt::getInstance()->decryptName(name.c_str().AsChar(), outname);
				std::string temppath = path + outname;
				rename(fullpath.c_str().AsChar(), temppath.c_str());
			}
		}
	}


	return true;
}