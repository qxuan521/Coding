#include "y_copy_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"

YCopyCommand::YCopyCommand(const std::string& szName)
	: YCommand(szName, 1)
{
}

YCopyCommand::~YCopyCommand()
{

}

YErrorCode YCopyCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	YErrorCode rResultCode = toAbsolutePath(rCommandInfo.rArglist);
	if(Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode);
		return rResultCode;
	}
	if (!pathCanbeRealValid())
	{
		errorPrint(YERROR_PATH_ILLEGAL);
		return YERROR_PATH_ILLEGAL;
	}
	bool srcIsReal = false, dstIsReal = false;
	std::string szSrc = m_rArgList[0];
	std::string szDst = m_rArgList[1];
	srcIsReal = isRealPath(szSrc);
	dstIsReal = isRealPath(szDst);
	if (srcIsReal)
	{//src real
		if (dstIsReal)
		{

		}
		else
		{

		}
	}
	else
	{
		if (dstIsReal)
		{
			
		}
		else
		{
			rResultCode = handleSrcToNoWildCard(szSrc);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				errorPrint(rResultCode);
				return rResultCode;
			}
			//生成目标路径
			rResultCode = virtual2VirtualCopy();
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				errorPrint(rResultCode);
				return rResultCode;
			}
		}
	}
	return YErrorCode();
}
YErrorCode YCopyCommand::handleSrcToNoWildCard(const std::string & szSrc)
{
	std::vector<YIFile*> rQueryResult;
	YErrorCode rResultCode = g_pDiskOperator->queryAllNode(szSrc, rQueryResult);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode);
		return rResultCode;
	}
	if (rQueryResult.empty())
	{
		errorPrint(YERROR_PATH_NOT_EXIST);
		return YERROR_PATH_NOT_EXIST);
	}

	if (isHaveWildCard(szSrc))
	{
		for (size_t index = 0; index < rQueryResult.size(); ++index)
		{
			if (rQueryResult[index]->IsFile())
			{
				m_rSrcArgList.push_back(g_pDiskOperator->getFullPath(rQueryResult[index]));
			}
		}
	}
	else
	{
		if (rQueryResult[0]->IsFile())
		{
			m_rSrcArgList.push_back(g_pDiskOperator->getFullPath(rQueryResult[0]));
		}
		else if(rQueryResult[0]->IsFolder())
		{
			std::vector<YIFile*> rChildren;
			g_pDiskOperator->getChildren(rQueryResult[0], rChildren);
			for (size_t index = 0; index < rChildren.size(); index++)
			{
				if (rQueryResult[index]->IsFile())
				{
					m_rSrcArgList.push_back(g_pDiskOperator->getFullPath(rChildren[index]));
				}
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YCopyCommand::real2VirtualCopy()
{
	return YErrorCode();
}

YErrorCode YCopyCommand::virtual2RealCopy()
{
	return YErrorCode();
}

YErrorCode YCopyCommand::virtual2VirtualCopy()
{
	return YErrorCode();
}


