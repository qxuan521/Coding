#pragma once
#include "command.h"


class SaveCommand : public Command
{
public:
	SaveCommand(std::vector<std::string>& ArgList);
	virtual ~SaveCommand();
	virtual ErrorCode Run();
private:

};

