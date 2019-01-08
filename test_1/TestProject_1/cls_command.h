#pragma once
#include "command.h"

class ClsCommand : public Command
{
public:
	ClsCommand(std::vector<std::string>& ArgList);
	virtual ~ClsCommand();
	ErrorCode Run();
private:

};

