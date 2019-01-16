#include "y_cls_command.h"

YClsCommand::YClsCommand(const std::string& szName)
	: YCommand(szName, 0)
{

}

YClsCommand::~YClsCommand()
{

}

YErrorCode YClsCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	if (!rCommandInfo.rPathList.empty())
	{
		return YERROR_COMMAND_ARG_ILLEGAL;
	}
	system("cls");
	return Y_OPERAT_SUCCEED;
}
