#pragma once
#include "y_command.h"

class YRdCommand : public YCommand
{
public:
	YRdCommand(const std::string& szName);
	~YRdCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
	bool	checkPathValidation();
private:
	YErrorCode queryFolderOneLevel(std::vector<std::string>& rWaitRemove);

};

