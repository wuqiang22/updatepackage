//----  luajit ±‡“Îπ§æﬂ

#ifndef _LUA_COMPILER_H
#define  _LUA_COMPILER_H

#include <stdlib.h>
#include <string>
#include "wx/string.h"




class LuaCompiler{
public:
	static LuaCompiler* getInstance();
	bool compileFile(wxString sourcepath);
	bool compileDir(wxString dir);
};

#endif