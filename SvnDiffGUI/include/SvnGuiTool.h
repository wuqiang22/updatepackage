#ifndef _SVN_GUI_TOOL_H
#define _SVN_GUI_TOOL_H


#include<stdlib.h>
#include<vector>
#include<string>

class SvnGuiTool
{
public:
	static SvnGuiTool* getInstance();

	std::size_t getLastestVersion(std::string serverpath);
	std::vector<std::string> getDiffPathBeginVersion(std::string svnpath, std::size_t version1, std::size_t version2);


	void tracevector(std::vector<std::string> vec);
private:
	void splitLineStr(std::string str, std::vector<std::string>& vec_str);

	void executeSvnCommand(std::string command, std::vector<std::string>& vec);
};

#endif