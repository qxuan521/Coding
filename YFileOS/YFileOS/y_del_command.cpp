#include "y_del_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YDelCommand::YDelCommand(const std::string& szName)
	: YCommand(szName, 1)
{
	m_rTypeArg.insert(std::pair<std::string, bool>("/s", false));
}
YDelCommand::~YDelCommand()
{

}
YErrorCode YDelCommand::excultCommand(YCommandInfo & rCommandInfo)
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
	queryOneLevelFile(rDelPathArr);
	//将路径全路径（有可能包含通配符）转换成节点数组 !!!!去掉通配符去掉文件夹 只留文件
	std::vector<YIFile*> rQueryResult;
	if (m_rTypeArg["/s"])
	{//删除所有节点
		queryAllChildFile(rDelPathArr);
	}
	if (rDelPathArr.empty())
	{
		return errorPrint(YERROR_FILE_IS_EXIST);
	}
	for (int index = (int)rDelPathArr.size() -1; index >=0 ;--index)
	{
		g_pDiskOperator->deleteNode(rDelPathArr[index]);
	}
	return Y_COPY_SUCCEED;
}

bool YDelCommand::checkPathValidation()
{
	for (size_t index = 0; index < m_rArgList.size();++index)
	{
		if (!wildCardOnlyLastLevel(m_rArgList[index]))
			return false;
	}
	return true;
}

YErrorCode YDelCommand::queryOneLevelFile(std::vector<std::string>& rDelPathArr)
{
	YErrorCode rResultCode;
	std::vector<YIFile*> rtempQueryResult;
	std::vector<YIFile*> rQueryResult;
	for (size_t index = 0; index < m_rArgList.size(); index++)
	{
	/*	if (equalOrLowerWithCurPath(m_szCurWorkPath, m_rArgList[index]))
		{
			return errorPrint(YERROR_PATH_ILLEGAL);
		}*/
		std::string szPath = m_rArgList[index];
		rResultCode = g_pDiskOperator->queryFileNode(m_rArgList[index], rtempQueryResult);
		if (rResultCode != Y_OPERAT_SUCCEED)
		{
			errorPrint(rResultCode, m_rArgList[index]);
		}
		if (rtempQueryResult.empty())
		{
			std::vector<YIFile*> rFolderQueryResult;
			rResultCode = g_pDiskOperator->queryFolderNode(m_rArgList[index], rFolderQueryResult);
			if (!rFolderQueryResult.empty())
			{
				std::vector<YIFile*> rChildren;
				g_pDiskOperator->getChildren(rFolderQueryResult[0], rChildren);
				for (size_t index = 0; index < rChildren.size();++index)
				{
					if (rChildren[index]->IsFile())
					{
						rtempQueryResult.push_back(rChildren[index]);
					}
				}
			}
		}
		rQueryResult.insert(rQueryResult.end(), rtempQueryResult.begin(), rtempQueryResult.end());
	}
	for (size_t index = 0; index < rQueryResult.size();++index)
	{
		if (nullptr == rQueryResult[index])
		{
			continue;
		}
		rDelPathArr.push_back(g_pDiskOperator->getFullPath(rQueryResult[index]));
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDelCommand::queryAllChildFile(std::vector<std::string>& rDelPathArr)
{
	YErrorCode rResultCode;
	std::vector<YIFile*> rQueryResult;
	std::vector<std::string> rParentArr;
	std::vector<YIFile*> rResult;
	for (size_t index = 0; index < m_rArgList.size(); index++)
	{
		rResultCode = g_pDiskOperator->queryFolderNode(m_rArgList[index], rQueryResult);
		if (rResultCode != Y_OPERAT_SUCCEED)
		{
			errorPrint(rResultCode, m_rArgList[index]);
		}
	}
	for (size_t index = 0; index < rQueryResult.size();++index)
	{
		if (rQueryResult[index]->IsRealFolder())
		{
			rParentArr.push_back(g_pDiskOperator->getFullPath(rQueryResult[index]));
		}
	}
	for (size_t index = 0 ;index < rParentArr.size();++index)
	{
		g_pDiskOperator->queryAllChildFolder(rParentArr[index], rResult);
	}
	for (size_t index = 0; index < rResult.size();++index)
	{
		std::string szFullPath = g_pDiskOperator->getFullPath(rResult[index]);
		if (userAsk(szFullPath, " has exist.Do you want to Delete file?<y/n>:"))
		{
			std::vector<YIFile*> rChildrenReult;
			g_pDiskOperator->getChildren(rResult[index], rChildrenReult);
			for (size_t nLoopCount = 0; nLoopCount < rChildrenReult.size();++nLoopCount)
			{
				std::string szChildFullPath = g_pDiskOperator->getFullPath(rChildrenReult[nLoopCount]);
				rDelPathArr.push_back(szChildFullPath);
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

