#pragma once
#include "y_command.h"
class YClsCommand : public YCommand
{
public:
	YClsCommand(const std::string& szName);
	virtual ~YClsCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);

private:

};

