#include "cls_command.h"
ClsCommand::ClsCommand(std::vector<std::string>& ArgList)
	: Command(ArgList, 1)
{
}

ClsCommand::~ClsCommand()
{
}

ErrorCode ClsCommand::Run()
{
	if (m_ArgList.size() != m_MustSize)
		return ERROR_COMMAND_USE_CODE;
	system("cls");
	return SUCCESS_CODE;
}
