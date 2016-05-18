#include "LuaCompiler.h"
#include <stdio.h>
#include <windows.h>
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/dir.h"

#include <vector>

#include "Tool.h"


LuaCompiler* LuaCompiler::getInstance()
{
	static LuaCompiler compiler;
	return &compiler;
}

bool LuaCompiler::compileFile(wxString sourcepath)
{
	wxFileName file = sourcepath;
	if (file.IsDir())
	{
		return false;
	}
	wxString ext = file.GetExt();
	if (ext.compare("lua") == 0)
	{
		wxString dest = sourcepath + "c";
		wxString command = wxString::Format("luajit.exe -b %s %s", sourcepath.c_str().AsChar(), dest.c_str().AsChar());
		system(command.c_str().AsChar());
		remove(sourcepath.c_str().AsChar());
		printf("%s has been luac \n",sourcepath.c_str().AsChar());
	}
	else{
		printf("%s do not need luac \n", sourcepath.c_str().AsChar());
	}
	return true;
}

bool LuaCompiler::compileDir(wxString sdir)
{
	convertPathtoUnixStyle(sdir);
	checkPath(sdir);

	bool ret = false;
	do 
	{
		if (wxDir::Exists(sdir))
		{
			wxDir dir(sdir);
			if (dir.IsOpened())
			{
				wxString  filename;
				bool cont = dir.GetFirst(&filename);
				std::vector<wxString> filenames;
				while (cont)
				{
					filenames.push_back(filename.c_str().AsChar());
					cont = dir.GetNext(&filename);
				}
				for (wxString name : filenames)
				{
					wxString path = sdir + name;

					if (wxDir::Exists(path))
					{
						if (!compileDir(path))
						{
							return false;
						}
					}
					else{
						if (!compileFile(path))
						{
							return false;
						}
					}
				}
				ret = true;
			}
		}
		else{
			printf("can not find dir:%s\n", sdir.c_str().AsChar());
		}
	} while (0);
	
	return ret;
}