//ZIP工具类

#ifndef  _ZIP_H
#define  _ZIP_H

#include <wx/string.h>
#include "wx/wfstream.h"
#include "wx/zipstrm.h"
#include "wx/txtstrm.h"

class Zip{
public:
	static Zip* getInstance();

	bool  zipComparessDir(wxString dir, wxString zipPath, wxString zipname);  //zip压缩目录
	bool zipComparessFile(wxString sourcePath, wxString zipPath, wxString zipname); //zip压缩文件

	bool deComparessDir(wxString zipPath, wxString dirPath, wxString dirname);   //解压缩zip成目录
	bool deComparessFile(wxString zipPath, wxString filePath, wxString filename); //解压缩zip中的第一个文件
};

#endif