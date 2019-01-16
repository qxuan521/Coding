#pragma once

#include "y_command.h"

class YSaveCommand : public YCommand
{
public:
	YSaveCommand(const std ::string& szName);
	virtual ~YSaveCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);

private:

};

