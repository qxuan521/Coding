#include "y_mkdisk_command.h"
#include "y_tool.h"
#include "y_disk_operator.h"


YMkdiskCommand::YMkdiskCommand(const std::string& szName)
	: YCommand(szName, 1)
{

}

YMkdiskCommand::~YMkdiskCommand()
{
}

YErrorCode YMkdiskCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	m_rArgList.clear();
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rArglist);
	if (rResultCode != Y_OPERAT_SUCCEED)
	{
		return rResultCode;
	}
	for (size_t index = 0; index < m_rArgList.size(); ++index)
	{
		if (isRealPath(m_rArgList[index]) || !rootDiskPathValidation(m_rArgList[index]))
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
		rResultCode = g_pDiskOperator->createNewDisk(m_rArgList[index], pNewFile);
		//输出创建结果
		errorPrint(rResultCode, m_rArgList[index]);
	}
	return Y_OPERAT_SUCCEED;
}
