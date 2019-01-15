#pragma once

#include "y_command.h"

class YDelCommand : public YCommand
{
public:
	YDelCommand(const std::string& szName);
	~YDelCommand();

private:

};

YDelCommand::YDelCommand(const std::string& szName)
	: YCommand(szName,1)
{

}

YDelCommand::~YDelCommand()
{

}