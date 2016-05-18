#include "Zip.h"
#include <wx/dir.h>
#include "Tool.h"
#include <wx/file.h>

const wxString ZIP_SIGN = "wuqiang";

Zip* Zip::getInstance()
{
	static Zip instance;
	return &instance; 
}

static bool readDir(wxString dir, wxZipOutputStream& zipout, wxString entry)
{
	convertPathtoUnixStyle(dir);
	checkPath(dir);

	if (!wxDir::Exists(dir))
	{
		return false;
	}

	wxDir wxdir(dir);
	if (wxdir.IsOpened())
	{
		std::vector<wxString> filenames = getWxFiles(dir);
		for (wxString name : filenames)
		{
			wxString temppath = dir + name;
			wxString tempentry = entry;
			tempentry += ("/" + name);

			if (wxDir::Exists(temppath))
			{
				if (!readDir(temppath, zipout, tempentry)){
					printf("%s  dir zip fail \n", temppath);
					return false;
				}
			}
			else{
				zipout.PutNextEntry(tempentry);
				wxFileInputStream inFile(temppath);
				if (!inFile.IsOk())
				{
					printf("%s read data fail \n", temppath);
					continue;
				}
				inFile.Read(zipout);
			}
		}
		return true;
	}

	return false;
}

bool Zip::zipComparessDir(wxString dir, wxString zipPath, wxString zipname)
{
	convertPathtoUnixStyle(dir);
	checkPath(dir);

	tryMakeDir(dir);


	if (zipPath.length()<0 || !isAbsolutePath(zipPath))
	{
		wxString curDir = getUpDir(dir);
		checkPath(curDir);
		zipPath = curDir + zipPath;
	}

	if (zipname.length()<=0)
	{
		zipname = getLastNameByPath(dir)+".zip";
	}

	convertPathtoUnixStyle(zipPath);
	checkPath(zipPath);

	wxString finalzippath = zipPath + zipname;
	tryMakeDir(finalzippath);

	wxFileOutputStream outFile(finalzippath);
	if (!outFile.IsOk())
	{
		printf("open file %s fail \n", finalzippath.c_str().AsChar());
		return false;
	}
	wxZipOutputStream zipout(outFile);
	if (!readDir(dir, zipout, ""))
	{
		return false;
	}

	zipout.Close();
	outFile.Close();

	return true;
}

bool Zip::zipComparessFile(wxString sourcePath, wxString zipPath, wxString zipname)
{
	if (!wxFile::Exists(sourcePath))
	{
		return false;
	}
	
	if (zipPath.length()<=0)
	{
		
		zipPath = getCurDir(sourcePath);
	}
	checkPath(zipPath);

	wxFileName wxfilename(sourcePath);
	if (zipname.length()<=0)
	{
		zipname = wxfilename.GetName();
	}

	wxFileOutputStream outFile(zipPath + zipname+".zip");
	if (!outFile.IsOk()) return false;
	wxZipOutputStream zipout(outFile);

	zipout.PutNextEntry(wxfilename.GetFullName());

	wxFileInputStream inFile(sourcePath);
	if (!inFile.IsOk()) return false;
	inFile.Read(zipout);

	zipout.Close();
	outFile.Close();

	return true;
}

bool Zip::deComparessDir(wxString zipPath, wxString dirPath,wxString dirname)
{
	wxFileName wxfilename(zipPath);
	if (!wxFile::Exists(zipPath))
	{
		return false;
	}

	wxString fileext = wxfilename.GetExt();
	if (fileext.compare("zip")!=0)
	{
		return false;
	}

	if (dirPath.length() <=0)
	{
		dirPath = getCurDir(zipPath);
	}
	checkPath(dirPath);

	if (dirname.length()<=0)
	{
		dirname = wxfilename.GetName();
	}
	
	wxString temppath = dirPath + dirname +"/";

	wxFileInputStream inFile(zipPath);
	wxZipInputStream zipIn(inFile);

	wxZipEntry* entry;
	while ((entry = zipIn.GetNextEntry()))
	{
		wxString name = entry->GetName(wxPATH_UNIX);
		wxString finalPath = temppath + name;
		wxString finalDir = getCurDir(finalPath);
		tryMakeDir(finalDir);
		wxFileOutputStream outFile(finalPath);
		if (outFile.IsOk())
		{
			zipIn.Read(outFile);
		}

		delete entry;
	}

	return false;
}

bool Zip::deComparessFile(wxString zipPath, wxString filePath,wxString filename)
{
	if (!wxFile::Exists(zipPath))
	{
		return false;
	}

	wxFileName wxfilename(zipPath);

	wxString fileext = wxfilename.GetExt();
	if (fileext.compare("zip") != 0)
	{
		return false;
	}
	
	if (filename.length() <= 0)
	{
		filename = wxfilename.GetName();
	}

	if (filePath.length()<=0)
	{
		filePath = getCurDir(zipPath);
	}

	wxString tempOutFilePath = filePath + filename + ".temp";

	wxFileInputStream inFile(zipPath);
	wxZipInputStream zipIn(inFile);

	wxFileOutputStream outFile(tempOutFilePath);
	wxZipEntry* entry = zipIn.GetNextEntry();
	wxString  suffix = "";
	if (entry)
	{
		outFile.Write(zipIn);
		wxString intername = entry->GetName(wxPATH_UNIX);
		size_t dotIndex = intername.find(".");
		if (dotIndex!=wxString::npos)
		{
			suffix ="."+ intername.substr(dotIndex + 1);
		}
		delete entry;
		zipIn.CloseEntry();
	}

	outFile.Close();

	if (suffix.length()>0)
	{
		remove(filePath + wxfilename.GetName() + suffix);
		rename(tempOutFilePath, filePath + wxfilename.GetName() + suffix);
	}
	return true;
}