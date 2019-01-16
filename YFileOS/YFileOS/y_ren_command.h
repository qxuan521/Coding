#pragma once

#include "y_command.h"

class YRenCommand : public YCommand
{
public:
	YRenCommand(const std::string& szName);
	virtual ~YRenCommand();


	virtual YErrorCode excultCommand(YCommandInfo& rCommandInfo);
	virtual YErrorCode toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList);
private:

};

