// SvnDiff.cpp : 定义控制台应用程序的入口点。
//

#include "windows.h"
#include "UpdatePackage.h"
#include "ResourceEncrypt.h"

int main(int argc, char* argv[])
{
	
	wxString savePath = "E:/UpdatePackage/";
	UpdatePackage update;
	update.setUpdatePackagePath(savePath);
	update.startWork("1.0.0.2");
	
//	ResourceEncrypt::getInstance()->encryptFile("E:/svnworkspace/projectmax/thelastday/projects/TheLastDay/trunk/TheLastDay/res/version/project.manifest");

	system("pause");
	return 1;
}

