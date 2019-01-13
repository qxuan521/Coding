#pragma once
#include "y_command.h"

class YMkdiskCommand : public YCommand
{
public:
	YMkdiskCommand(const std::string& szName);
	~YMkdiskCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
private:

};
