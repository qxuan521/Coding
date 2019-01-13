#pragma once
#include <string>
#include <vector>
#include "y_command.h"
class YInputResolve
{
public:
	YInputResolve();
	~YInputResolve();

	YCommandInfo resolve(const std::string& szInput);

private:

};

