#pragma once

#include "y_command.h"

class YDelCommand : public YCommand
{
public:
	YDelCommand(const std::string& szName);
	~YDelCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);

private:
	bool			checkPathValidation();
	//删除符合条件的文件 不递归;
	YErrorCode		queryOneLevelFile(std::vector<std::string>& rDelPathArr);
	YErrorCode		queryAllChildFile(std::vector<std::string>& rDelPathArr);
};

