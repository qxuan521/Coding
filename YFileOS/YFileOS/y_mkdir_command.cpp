#include "y_mkdir_command.h"
#include "y_tool.h"
#include "y_disk_operator.h"
YMkdirCommand::YMkdirCommand(const std::string& szName)
	: YCommand(szName, 1)
{

}

YMkdirCommand::~YMkdirCommand()
{

}

YErrorCode YMkdirCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	YErrorCode rResultCode = toAbsolutePath(rCommandInfo.rArglist);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode); 
		return rResultCode;
	}
	for (size_t index = 0; index < m_rArgList.size(); ++index)
	{
		if (isRealPath(m_rArgList[index]) || !noWildCardPathValidation(m_rArgList[index]))
		{
			errorPrint(YErrorCode::YERROR_PATH_ILLEGAL, m_rArgList[index]);
			continue;
		}
		if (g_pDiskOperator->isPathExist(m_rArgList[index]))
		{
			errorPrint(YErrorCode::YERROR_FILE_IS_EXIST, m_rArgList[index]);
			continue;
		}
		YIFile* pNewFile = nullptr;
		//创建文件
		rResultCode = g_pDiskOperator->createNewFolder(m_rArgList[index], pNewFile);
		//输出创建结果
		errorPrint(rResultCode, m_rArgList[index]);
	}
	return YErrorCode();
}
