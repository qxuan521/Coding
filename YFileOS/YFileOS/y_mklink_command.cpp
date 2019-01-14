#include "y_mklink_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"

YMklinkCommand::YMklinkCommand(const std::string& szCommandName)
	: YCommand(szCommandName, 2)
{

}

YMklinkCommand::~YMklinkCommand()
{

}

YErrorCode YMklinkCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	m_rArgList.clear();
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (rResultCode != Y_OPERAT_SUCCEED)
	{
		return rResultCode;
	}
	rResultCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	for (size_t index = 0; index < m_rArgList.size(); ++index)
	{
		if (isRealPath(m_rArgList[index]) || !noWildCardPathValidation(m_rArgList[index]))
		{
			errorPrint(YErrorCode::YERROR_PATH_ILLEGAL, m_rArgList[index]);
			continue;
		}
	}
	if (g_pDiskOperator->isPathExist(m_rArgList[0]))
	{
		return errorPrint(YErrorCode::YERROR_FILE_IS_EXIST, m_rArgList[0]);
	}
	YIFile* pNewFile = nullptr;
	//创建文件
	rResultCode = g_pDiskOperator->createNewLnkFile(m_rArgList[0], m_rArgList[1],pNewFile);
	//输出创建结果
	return errorPrint(rResultCode);
}
