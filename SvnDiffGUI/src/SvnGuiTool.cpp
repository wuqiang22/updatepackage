#include "SvnGuiTool.h"

#include <tchar.h>

#include "Conversion.h"

SvnGuiTool* SvnGuiTool::getInstance()
{
	static SvnGuiTool instance;
	return &instance;
}


void SvnGuiTool::tracevector(std::vector<std::string> vec)
{
	if (vec.size() < 0)
	{
		return;
	}
	for (std::string str : vec)
	{
		printf("%s \n", str.c_str());
	}
}

void SvnGuiTool::splitLineStr(std::string str, std::vector<std::string>& vec_str)
{
	if (str.length() <= 0)
	{
		return;
	}

	vec_str.clear();

	std::string curLine;
	std::size_t index = 0;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '\n' && str[i - 1] == '\r')
		{
			curLine = str.substr(index, i - index - 1);
			if (curLine.length() > 0)
			{
				vec_str.push_back(curLine);
			}

			index = i + 1;
		}
	}
	return;
}

void SvnGuiTool::executeSvnCommand(std::string command, std::vector<std::string>& vec)
{
	wchar_t* mywstring = nullptr;
	do
	{
		if (command.length() <= 0)
		{
			break;
		}
		vec.clear();
		std::string ret = "";
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead, hWrite;

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			break;
		}
		char cmdcommand[1024];    //长达1K的命令行，够用了吧 
		strcpy(cmdcommand, "Cmd.exe /C ");
		strcat(cmdcommand, command.c_str());
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;            //把创建进程的标准错误输出重定向到管道输入 
		si.hStdOutput = hWrite;           //把创建进程的标准输出重定向到管道输入 
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		std::string str(cmdcommand);
		mywstring = string2wchar_t_star(str);

		//关键步骤，CreateProcess函数参数意义请查阅MSDN 
		if (!CreateProcess(NULL, mywstring, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			CloseHandle(hWrite);
			CloseHandle(hRead);
			break;
		}
		CloseHandle(hWrite);

		//char buffer[4096] = { 0 };          //用4K的空间来存储输出的内容，只要不是显示文件内容，一般情况下是够用了。 
		DWORD bytesRead;
		while (true)
		{
			char buffer[4096] = { 0 };
			if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
				break;
			ret.append(buffer);
		}
		CloseHandle(hRead);
		splitLineStr(ret, vec);
	} while (false);
	
	if (mywstring)
	{
		delete mywstring;
		mywstring = nullptr;
	}
}

/*
std::size_t getLastestVersion(std::string serverpath);
std::vector<std::string> getDiffPathBeginVersion(std::string svnpath, std::size_t version1, std::size_t version2);
*/

std::size_t SvnGuiTool::getLastestVersion(std::string serverpath)
{
	std::vector<std::string> vec;
	std::string svncommand = "svn info ";
	svncommand.append(serverpath);

	executeSvnCommand(svncommand, vec);

	for (std::string line : vec)
	{
		if (line.substr(0, 6) == "版本: ")
		{
			std::string str = line.substr(6);
			return atoi(str.c_str());
		}
	}
	return 0;
}

std::vector<std::string> SvnGuiTool::getDiffPathBeginVersion(std::string svnpath, std::size_t version1, std::size_t version2)
{
	std::vector<std::string> ret;

	std::string svncommand = " svn diff ";
	svncommand.append(svnpath);

	char params[20] = { 0 };
	sprintf(params, " -r %i:%i", version1, version2);
	svncommand.append(params);

	std::vector<std::string> lines;
	executeSvnCommand(svncommand, lines);

	for (auto it = lines.begin(); it != lines.end(); it++)
	{
		std::string line = *it;
		if (line.substr(0, 7) == "Index: ")
		{
			ret.push_back(line.substr(7));
		}
	}
	return ret;
}