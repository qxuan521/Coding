#pragma once
#include "y_command.h"

class YCdCommand : public YCommand
{
public:
	YCdCommand(const std::string& szName);
	virtual ~YCdCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
	virtual YErrorCode		toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList);
private:

};
