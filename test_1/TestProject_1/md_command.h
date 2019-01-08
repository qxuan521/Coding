#pragma once
#include "command.h"

class MdCommand : public Command
{
public:
	MdCommand(std::vector<std::string>& ArgList);
	virtual ~MdCommand();
	virtual ErrorCode Run();
private:
	std::vector<std::string>		m_PathArr;
};