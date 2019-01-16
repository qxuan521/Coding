#pragma once

#include "y_command.h"

class YLoadCommand :public YCommand
{
public:
	YLoadCommand(const std::string& szName);
	virtual ~YLoadCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);


private:

};
