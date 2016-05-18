//ZIP������

#ifndef  _ZIP_H
#define  _ZIP_H

#include <wx/string.h>
#include "wx/wfstream.h"
#include "wx/zipstrm.h"
#include "wx/txtstrm.h"

class Zip{
public:
	static Zip* getInstance();

	bool  zipComparessDir(wxString dir, wxString zipPath, wxString zipname);  //zipѹ��Ŀ¼
	bool zipComparessFile(wxString sourcePath, wxString zipPath, wxString zipname); //zipѹ���ļ�

	bool deComparessDir(wxString zipPath, wxString dirPath, wxString dirname);   //��ѹ��zip��Ŀ¼
	bool deComparessFile(wxString zipPath, wxString filePath, wxString filename); //��ѹ��zip�еĵ�һ���ļ�
};

#endif