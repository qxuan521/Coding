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
	YErrorCode rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if(Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	rResultCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	if (!pathCanbeRealValid())
	{
		return errorPrint(YERROR_PATH_ILLEGAL);
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
			//error
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
		{//虚拟磁盘向虚拟磁盘中拷贝
			//处理源文件获得要拷贝的文件的路径
			rResultCode = handleSrcToNoWildCard(szSrc);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			if (m_rSrcArgList.empty())
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			rResultCode = handleDstToNoWildCard(szDst);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			//生成目标路径
			std::vector<YIFile*> rCopyResultArr;
			rResultCode = g_pDiskOperator->copyFileNode(m_rSrcArgList, m_rDstArgList, rCopyResultArr);
			return errorPrint(rResultCode);
		}
	}
	return Y_OPERAT_SUCCEED;
}
YErrorCode YCopyCommand::handleSrcToNoWildCard(const std::string & szSrc)
{
	std::vector<YIFile*> rQueryResult;
	YErrorCode rResultCode = g_pDiskOperator->queryAllNode(szSrc, rQueryResult);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	if (rQueryResult.empty())
	{
		return errorPrint(YERROR_PATH_NOT_EXIST);
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

YErrorCode YCopyCommand::handleDstToNoWildCard(const std::string & szDst)
{
	if (isHaveWildCard(szDst))
	{//目标路径存在通配符
		std::regex rDstResgex = makeRegexByPath(getNameFromFullPath(szDst));
		m_rDstArgList.resize(m_rSrcArgList.size());
		std::string szDstParent = getParentPath(szDst);
		for (size_t index = 0; index < m_rSrcArgList.size(); index++)
		{
			std::smatch szMatchSrcPathResult;
			std::string szSrcName = getParentPath(m_rSrcArgList[index]);
			if (szSrcName.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}
			std::regex_match(szSrcName, szMatchSrcPathResult, rDstResgex);
			if (szMatchSrcPathResult.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}
			m_rDstArgList[index].append(szDstParent);
			m_rDstArgList[index].append("/");
			m_rDstArgList[index].append(szMatchSrcPathResult);
		}
	}
	else
	{//目标路径不存在通配符
		std::vector<YIFile*> rQueryResult;
		YIFile* pDstFile = nullptr;
		YErrorCode rResultCode = g_pDiskOperator->queryAllNode(szDst, rQueryResult);
		if (Y_OPERAT_SUCCEED != rResultCode)
		{
			return errorPrint(rResultCode);
		}
		if (rQueryResult.empty())
		{//目标路径不存在 此时 目标路径的上一级应该存在切是个文件夹 最后一级时新文件的名字 意味着只能拷贝一个文件
			if (m_rSrcArgList.size() > 1)
			{//源文件有多个
				return YERROR_PATH_ILLEGAL;
			}
			else
			{//源文件有一个
				std::string szDstParent = getParentPath(szDst);
				if (szDstParent.empty())
				{//目标路径上一级不存在
					return YERROR_PATH_ILLEGAL;
				}
				rResultCode = g_pDiskOperator->queryAllNode(szDstParent, rQueryResult);
				if (Y_OPERAT_SUCCEED != rResultCode)
				{//查找上一级时失败
					return errorPrint(rResultCode);
				}
				if (rQueryResult.empty() || nullptr == rQueryResult[0] ||!rQueryResult[0]->IsFolder())
				{//目标路径没有上一级或者 上一级不是文件夹
					return errorPrint(YERROR_PATH_ILLEGAL);
				}
				m_rDstArgList.push_back(szDst);
			}
		}
		else
		{//目标路径存在
			if (pDstFile->IsFolder())
			{//如果目标路径时一个文件夹 可以拷贝多个文件代表把源文件拷贝到当前文件夹下以源文件名命名
				std::string szDstParent = getParentPath(szDst);
				m_rDstArgList.resize(m_rSrcArgList.size());
				for (size_t index = 0; index < m_rSrcArgList.size(); index++)
				{
					std::string szSrcName = getParentPath(m_rSrcArgList[index]);
					if (szSrcName.empty())
					{
						return YERROR_PATH_ILLEGAL;
					}
					m_rDstArgList[index].append(szDstParent);
					m_rDstArgList[index].append("/");
					m_rDstArgList[index].append(szSrcName);
				}
			}
			else if (pDstFile->IsFile())
			{//目标路径时一个文件 可能要发起覆盖操作 意味着源文件只能有一个
				if (m_rSrcArgList.size()> 1)
				{
					return YERROR_PATH_ILLEGAL;
				}
				else
				{
					m_rDstArgList.push_back(szDst);
				}
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YCopyCommand::checkSameNAsk()
{
	if (m_rSrcArgList.empty() || m_rDstArgList.empty() || m_rDstArgList.size() != m_rSrcArgList.size())
	{
		return YERROR_PATH_ILLEGAL;
	}
	std::set<std::string> rExistSet;
	std::vector<YIFile*> rQueryArr;
	YErrorCode rResultCode;
	for (auto rSrcIter = m_rSrcArgList.begin(),rDstIter = m_rDstArgList.begin(); rSrcIter != m_rSrcArgList.end() && rDstIter != m_rDstArgList.end();)
	{
		rResultCode = g_pDiskOperator->queryFileNode(*rDstIter, rQueryArr);
		if (Y_OPERAT_SUCCEED != rResultCode)
		{
			return rResultCode;
		}
		if (rExistSet.end() == rExistSet.find(*rDstIter) && rQueryArr.empty())
		{//不存在重复路径直接拷贝
			rExistSet.insert(*rDstIter);
			++rSrcIter;
			++rDstIter;
		}
		else
		{//存在重复验证是否覆盖
			while (true)
			{
				std::cout << "\"" << *rDstIter << "\"" << " has exist.Do you want to overwrite file?<y/n> ";
				std::string InPut;
				std::cin.clear();
				std::cin.sync();
				getline(std::cin, InPut);
				if ("y" == InPut)
				{
					rExistSet.insert(*rDstIter);
					++rSrcIter;
					++rDstIter;
				}
				if ("n" == InPut)
				{
					rSrcIter = m_rSrcArgList.erase(rSrcIter);
					rDstIter = m_rDstArgList.erase(rDstIter);
				}
				std::cin.clear();
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
	return Y_COPY_SUCCEED;
}


