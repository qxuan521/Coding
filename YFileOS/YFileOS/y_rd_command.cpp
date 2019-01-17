#include "y_rd_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YRdCommand::YRdCommand(const std::string& szName)
	: YCommand(szName,1)
{
	m_rTypeArg.insert(std::pair<std::string, bool>("/s", false));
}

YRdCommand::~YRdCommand()
{

}

YErrorCode YRdCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	rResultCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	if (!checkPathValidation())
	{
		return errorPrint(YERROR_PATH_ILLEGAL);
	}
	std::vector<std::string> rDelPathArr;
	queryFolderOneLevel(rDelPathArr);
	for (size_t index = 0; index < rDelPathArr.size(); ++index)
	{
		std::vector<YIFile*> rQueryResult;
		rResultCode = g_pDiskOperator->queryFolderNode(m_rArgList[index], rQueryResult);
		if (rQueryResult.empty()|| rQueryResult[index] ==nullptr)
		{
			errorPrint(YERROR_POINTER_NULL, g_pDiskOperator->getFullPath(rQueryResult[index]));
			continue;
		}
		if (rQueryResult[index]->getChildrenCount() != 0 && !m_rTypeArg["/s"])
		{
			errorPrint(YERROR_FOLDER_IS_NOT_BE_EMPTY, g_pDiskOperator->getFullPath(rQueryResult[index]));
			continue;
		}
		g_pDiskOperator->deleteNode(rDelPathArr[index]);
	}
	return Y_OPERAT_SUCCEED;
}

bool YRdCommand::checkPathValidation()
{
	for (size_t index = 0; index < m_rArgList.size(); ++index)
	{
		if (!wildCardOnlyLastLevel(m_rArgList[index]))
			return false;
	}
	return true;
}

YErrorCode YRdCommand::queryFolderOneLevel(std::vector<std::string>& rWaitRemove)
{
	YErrorCode rResultCode;
	std::vector<YIFile*> rQueryResult;
	for (size_t index = 0; index < m_rArgList.size(); index++)
	{
		if (equalOrLowerWithCurPath(m_szCurWorkPath, m_rArgList[index]))
		{
			return errorPrint(YERROR_PATH_ILLEGAL);
		}
		rResultCode = g_pDiskOperator->queryFolderNode(m_rArgList[index], rQueryResult);
		if (rResultCode != Y_OPERAT_SUCCEED)
		{
			errorPrint(rResultCode, m_rArgList[index]);
		}
	}
	for (size_t index = 0; index < rQueryResult.size(); ++index)
	{
		if (nullptr == rQueryResult[index])
		{
			continue;
		}
		rWaitRemove.push_back(g_pDiskOperator->getFullPath(rQueryResult[index]));
	}
	return Y_OPERAT_SUCCEED;
}


