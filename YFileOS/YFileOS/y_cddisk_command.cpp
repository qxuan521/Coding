#include "y_cddisk_command.h"
#include "y_disk_operator.h"
YCddiskCommand::YCddiskCommand(const std::string& szName)
	: YCommand(szName, 1)
{

}

YCddiskCommand::~YCddiskCommand()
{

}

YErrorCode YCddiskCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	m_rArgList.push_back(rCommandInfo.szCommandName);
	if (m_rArgList.empty() || m_rArgList.size() != 1)
	{
		return YERROR_COMMAND_ARG_NUM_ERROR;
	}
	if (!rootDiskPathValidation(m_rArgList[0]))
	{
		return YERROR_PATH_ILLEGAL;
	}
	if (!g_pDiskOperator->isRootName(m_rArgList[0]))
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YErrorCode rResultCode = g_pDiskOperator->setCurWorkingPath(m_rArgList[0]);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	return Y_OPERAT_SUCCEED;
}
