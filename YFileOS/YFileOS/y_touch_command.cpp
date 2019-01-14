#include "y_touch_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YTouchCommand::YTouchCommand(const std::string& szCommandRegexStr)
	: YCommand(szCommandRegexStr, 1)
{

}

YTouchCommand::~YTouchCommand()
{
}

YErrorCode YTouchCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	m_rArgList.clear();
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode);
		return rResultCode;
	}
	rResultCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode);
		return rResultCode;
	}
	for (size_t index = 0; index < m_rArgList.size();++index)
	{
		if (isRealPath(m_rArgList[index])|| !noWildCardPathValidation(m_rArgList[index]))
		{
			errorPrint(YErrorCode::YERROR_PATH_ILLEGAL, m_rArgList[index]);
			continue;
		}
		if (g_pDiskOperator->isPathExist(m_rArgList[index]))
		{
			errorPrint(YErrorCode::YERROR_FILE_IS_EXIST,m_rArgList[index]);
			continue;
		}
		YIFile* pNewFile = nullptr;
		//创建文件
		rResultCode = g_pDiskOperator->createNewFile(m_rArgList[index], pNewFile);
		//输出创建结果
		errorPrint(rResultCode, m_rArgList[index]);
	}
	return Y_OPERAT_SUCCEED;
}
