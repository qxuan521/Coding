#pragma once

#include "y_command.h"

class YMoveCommand : public YCommand
{
public:
	YMoveCommand(const std::string& szName);
	~YMoveCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);

private:

};

