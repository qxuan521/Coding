#pragma once

#include "y_command.h"

class YMklinkCommand : public YCommand
{
public:
	YMklinkCommand(const std::string& szCommandName);
	virtual ~YMklinkCommand();


	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
private:

};

