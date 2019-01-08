#pragma once
#include "command.h"


class RenCommand : public Command
{
public:
	RenCommand(std::vector<std::string>& ArgList);
	virtual ~RenCommand();
	virtual ErrorCode Run();
	virtual ErrorCode	ToAbsolutePath();

private:

};

