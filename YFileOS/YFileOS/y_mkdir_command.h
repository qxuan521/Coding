#pragma once
#include "y_command.h"

class YMkdirCommand : public YCommand
{
public:
	YMkdirCommand(const std::string& szName);
	virtual ~YMkdirCommand();
	virtual YErrorCode	excultCommand(YCommandInfo& rCommandInfo);

private:

};

