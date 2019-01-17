#pragma once
#include <string>
#include "y_error_code.h"
class YSystem
{
public:
	YSystem();
	~YSystem();

	void runingLoop();
	//百合测试支持
	YErrorCode runCommand(const std::string& szCmd);
private:

};

