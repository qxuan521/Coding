#include "y_load_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"

YLoadCommand::YLoadCommand(const std::string& szName)
	:YCommand(szName, 1)
{
}

YLoadCommand::~YLoadCommand()
{

}

YErrorCode YLoadCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	if (m_rArgList.size() != 1)
	{
		return errorPrint(YERROR_COMMAND_ARG_NUM_ERROR);
	}
	YErrorCode rErrorCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rErrorCode)
	{
		return errorPrint(rErrorCode);
	}
	rErrorCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rErrorCode)
	{
		return errorPrint(rErrorCode);
	}
	std::string szPath = getPathFromRealPath(m_rArgList[0]);
	if (szPath.empty())
	{
		return errorPrint(YERROR_PATH_ILLEGAL);
	}
	rErrorCode = g_pDiskOperator->loadData(szPath);
	if (Y_OPERAT_SUCCEED != rErrorCode)
	{
		return errorPrint(rErrorCode);
	}
	return rErrorCode;
}
