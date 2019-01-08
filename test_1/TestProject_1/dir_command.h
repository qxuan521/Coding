#pragma once
#include "command.h"
#include "dir_tool.h"

class Dircomman: public Command
{
public:
	Dircomman(std::vector<std::string>& ArgList);
	virtual ~Dircomman();
	virtual ErrorCode	Run();
	ErrorCode			ResolveArg();
	void				PrintResult();
private:
	DirSeacher*			m_Seacher;
	std::vector<std::string>			m_PathList;
	std::vector<std::string>	m_FileTypeStd;
};