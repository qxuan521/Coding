#pragma once
#include "command.h"


class CDCommand : public Command
{
public:
	CDCommand(std::vector<std::string>& ArgList);
	virtual ~CDCommand();
	virtual ErrorCode	Run();
};