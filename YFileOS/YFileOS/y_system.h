#pragma once
#include <string>
#include "y_error_code.h"
class YSystem
{
public:
	YSystem();
	~YSystem();

	void runingLoop();
	//�ٺϲ���֧��
	YErrorCode runCommand(const std::string& szCmd);
private:

};

