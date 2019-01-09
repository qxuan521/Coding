#include "y_command.h"

YCommand::YCommand(const std::string& szName)
	: m_szName(szName)
{
}

YCommand::~YCommand()
{
}

const std::string & YCommand::getCommandName()
{
	return m_szName;
}
