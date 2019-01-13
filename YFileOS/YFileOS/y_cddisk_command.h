#pragma once

#include "y_command.h"

class YCddiskCommand : public YCommand
{
public:
	YCddiskCommand(const std::string& szName);
	virtual ~YCddiskCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
private:

};

