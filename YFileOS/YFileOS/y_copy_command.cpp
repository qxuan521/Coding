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
		{//��֧�� ��ʵ����·���俽��
			return errorPrint(YERROR_PATH_ILLEGAL);
		}
		else
		{//��ʵ·���������������ͨ���
			if (!noWildCardPathValidation(szDst) || !noWildCardPathValidation(szSrc))
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			m_rSrcArgList.push_back(getPathFromRealPath(szSrc));
			rResultCode = handleDstToNoWildCard(szDst);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			rResultCode = checkSrcReal();
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			std::vector<YIFile*> rCopyResultArr;
			rResultCode = g_pDiskOperator->copyFileFromRealDisk(m_rSrcArgList, m_rDstArgList, rCopyResultArr);
			return errorPrint(rResultCode);
		}
	}
	else
	{//Դ·��������ʵ·��
		if (dstIsReal)
		{//Ŀ��·������ʵ·��
			if (!noWildCardPathValidation(szDst) || !noWildCardPathValidation(szSrc))
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			rResultCode = handleSrcToNoWildCard(szSrc);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			if (m_rSrcArgList.empty() || m_rSrcArgList.size() > 1)
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			m_rDstArgList.push_back(getPathFromRealPath(szDst));
			rResultCode = checkDstReal();
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			std::vector<YIFile*> rCopyResultArr;
			rResultCode = g_pDiskOperator->copyFileToRealDisk(m_rSrcArgList, m_rDstArgList, rCopyResultArr);
			return errorPrint(rResultCode);
		}
		else
		{//�����������������п���
			//����Դ�ļ����Ҫ�������ļ���·��
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
			if (!sameCheck())
			{
				return YERROR_COMMAND_ARG_ILLEGAL;
			}
			//����Ŀ��·��
			std::vector<YIFile*> rCopyResultArr;
			rResultCode = g_pDiskOperator->copyFileNode(m_rSrcArgList, m_rDstArgList, rCopyResultArr);
			return errorPrint(rResultCode);
		}
	}
	return Y_OPERAT_SUCCEED;
}
void YCopyCommand::resetCommand()
{
	YCommand::resetCommand();
	m_rDstArgList.clear();
	m_rSrcArgList.clear();
}
YErrorCode YCopyCommand::handleSrcToNoWildCard(const std::string & szSrc)
{
	std::vector<YIFile*> rQueryResult;
	YErrorCode rResultCode = g_pDiskOperator->queryAllNode(szSrc, rQueryResult);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	if (rQueryResult.empty())
	{
		return YERROR_PATH_NOT_EXIST;
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
	{//Ŀ��·������ͨ���
		m_rDstArgList.resize(m_rSrcArgList.size());
		std::string szDstParent = getParentPath(szDst);
		std::string szDstName = getNameFromFullPath(szDst);
		std::string szRepaceString;
		std::regex rDstResgex = makeRepaceRegexByPath(getNameFromFullPath(szDst), szRepaceString);
		for (size_t index = 0; index < m_rSrcArgList.size(); index++)
		{
			std::string szSrcName = getNameFromFullPath(m_rSrcArgList[index]);
			if (szSrcName.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}
			
			std::string absDstName = std::regex_replace(szSrcName, rDstResgex, szRepaceString);
			m_rDstArgList[index].append(szDstParent);
			m_rDstArgList[index].append("/");
			m_rDstArgList[index].append(absDstName);
		}
	}
	else
	{//Ŀ��·��������ͨ���
		std::vector<YIFile*> rQueryResult;
		YIFile* pDstFile = nullptr;
		YErrorCode rResultCode = g_pDiskOperator->queryAllNode(szDst, rQueryResult);
		if (Y_OPERAT_SUCCEED != rResultCode)
		{
			return errorPrint(rResultCode);
		}
		if (rQueryResult.empty())
		{//Ŀ��·�������� ��ʱ Ŀ��·������һ��Ӧ�ô������Ǹ��ļ��� ���һ��ʱ���ļ������� ��ζ��ֻ�ܿ���һ���ļ�
			if (m_rSrcArgList.size() > 1)
			{//Դ�ļ��ж��
				return YERROR_PATH_ILLEGAL;
			}
			else
			{//Դ�ļ���һ��
				std::string szDstParent = getParentPath(szDst);
				if (szDstParent.empty())
				{//Ŀ��·����һ��������
					return YERROR_PATH_ILLEGAL;
				}
				rResultCode = g_pDiskOperator->queryAllNode(szDstParent, rQueryResult);
				if (Y_OPERAT_SUCCEED != rResultCode)
				{//������һ��ʱʧ��
					return errorPrint(rResultCode);
				}
				if (rQueryResult.empty() || nullptr == rQueryResult[0] ||!rQueryResult[0]->IsFolder())
				{//Ŀ��·��û����һ������ ��һ�������ļ���
					return errorPrint(YERROR_PATH_ILLEGAL);
				}
				m_rDstArgList.push_back(szDst);
			}
		}
		else
		{//Ŀ��·������
			pDstFile = rQueryResult[0];
			if (pDstFile->IsFolder())
			{//���Ŀ��·��ʱһ���ļ��� ���Կ�������ļ������Դ�ļ���������ǰ�ļ�������Դ�ļ�������
				m_rDstArgList.resize(m_rSrcArgList.size());
				for (size_t index = 0; index < m_rSrcArgList.size(); index++)
				{
					std::string szSrcName = getNameFromFullPath(m_rSrcArgList[index]);
					if (szSrcName.empty())
					{
						return YERROR_PATH_ILLEGAL;
					}
					m_rDstArgList[index].append(szDst);
					m_rDstArgList[index].append("/");
					m_rDstArgList[index].append(szSrcName);
				}
			}
			else if (pDstFile->IsFile())
			{//Ŀ��·��ʱһ���ļ� ����Ҫ���𸲸ǲ��� ��ζ��Դ�ļ�ֻ����һ��
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

bool YCopyCommand::sameCheck()
{
	if (m_rDstArgList.size() != m_rSrcArgList.size()
		|| m_rSrcArgList.empty() || m_rDstArgList.empty())
	{
		return false;
	}
	for (size_t index = 0; index < m_rDstArgList.size(); ++index)
	{
		if (m_rDstArgList[index] == m_rSrcArgList[index])
		{
			return false;
		}
	}
	return true;
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
		{//�������ظ�·��ֱ�ӿ���
			rExistSet.insert(*rDstIter);
			++rSrcIter;
			++rDstIter;
		}
		else
		{//�����ظ���֤�Ƿ񸲸�
			if (userAsk(*rDstIter, " has exist.Do you want to overwrite file?<y/n> "))
			{
				rExistSet.insert(*rDstIter);
				++rSrcIter;
				++rDstIter;
			}
			else
			{
				rSrcIter = m_rSrcArgList.erase(rSrcIter);
				rDstIter = m_rDstArgList.erase(rDstIter);
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YCopyCommand::checkSrcReal()
{
	YErrorCode rResultCode;
	std::vector<YIFile*> rDstNode;
	rResultCode = g_pDiskOperator->queryFileNode(m_rDstArgList[0], rDstNode);
	if (rDstNode.empty())
	{
		return Y_OPERAT_SUCCEED;
	}
	else
	{
		if (nullptr != rDstNode[0])
		{
			if (!rDstNode[0]->IsRealFile())
			{
				return YERROR_PATH_ILLEGAL;
			}
			bool isCover = userAsk(m_rDstArgList[0], " has exist.Do you want to overWrite file?<y/n>:");
			if (isCover)
			{
				return Y_OPERAT_SUCCEED;
			}
			else {
				return Y_OPERAT_FAILD;
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}
YErrorCode YCopyCommand::checkDstReal()
{
	YErrorCode rResultCode;
	rResultCode = handleDstToNoWildCard(m_rSrcArgList[0]);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	std::vector<YIFile*> rSrcNode;
	g_pDiskOperator->queryFileNode(m_rSrcArgList[0], rSrcNode);
	if (rSrcNode.size() != 1 || nullptr == rSrcNode[0] || !rSrcNode[0]->IsRealFile())
	{
		return YERROR_PATH_ILLEGAL;
	}
	return Y_OPERAT_SUCCEED;
}


