#pragma once
#include<vector>
#include "y_error_code.h"

class YCommand
{
public:
	YCommand(const std::string& szName);
	~YCommand();
	
	virtual YErrorCode	excultCommand(std::vector<std::string> rArgArr) = 0;

	const std::string& getCommandName();
private:
	const std::string			m_szName;
};

