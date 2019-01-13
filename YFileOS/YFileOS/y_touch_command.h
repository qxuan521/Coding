#pragma once
#include <regex>
#include "y_command.h"

class YTouchCommand : public YCommand
{
public:
	YTouchCommand(const std::string& szCommandRegexStr);
	virtual ~YTouchCommand();

	virtual YErrorCode	excultCommand(YCommandInfo& rCommandInfo);

private:
};

