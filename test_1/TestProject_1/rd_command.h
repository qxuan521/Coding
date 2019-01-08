#pragma once
#include "command.h"

class RdCommand : public Command
{
public:
	RdCommand(std::vector<std::string>& ArgList);
	virtual ~RdCommand();
	virtual ErrorCode Run();
private:
	ErrorCode DelAllChild(std::string Str);
	ErrorCode DelOneLevel(std::string Str);
};
