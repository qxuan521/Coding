#pragma once

#include "command.h"


class DelCommand : public Command
{
public:
	DelCommand(std::vector<std::string>& ArgList);
	virtual ~DelCommand();
	virtual ErrorCode Run();
private:
	ErrorCode DelAllChild(std::string Str);
	ErrorCode DelOneLevel(std::string Str);
private:
	std::vector<std::string>			m_PathList;
};

