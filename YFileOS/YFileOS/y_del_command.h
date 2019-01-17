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
	//ɾ�������������ļ� ���ݹ�;
	YErrorCode		queryOneLevelFile(std::vector<std::string>& rDelPathArr);
	YErrorCode		queryAllChildFile(std::vector<std::string>& rDelPathArr);
};

