#pragma once
#include "command.h"


class MklinkCommand : public Command
{
public:
	MklinkCommand(std::vector<std::string>& ArgList);
	virtual ~MklinkCommand();
	virtual ErrorCode Run();
private:

};

