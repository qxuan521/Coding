#include "y_touch_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YTouchCommand::YTouchCommand(const std::string& szCommandRegexStr)
	: YCommand(szCommandRegexStr, 2)
{

}

YTouchCommand::~YTouchCommand()
{
}

YErrorCode YTouchCommand::excultCommand(std::vector<std::string> rArgArr)
{
	m_rArgList.clear();
	m_rArgList = rArgArr;
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath();
	for (size_t index = 0; index < rArgArr.size();++index)
	{
		if (isRealPath(m_rArgList[index]))
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
