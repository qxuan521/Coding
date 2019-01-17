#include <functional>
#include "y_move_command.h"
#include "y_tool.h"
#include "y_disk_operator.h"

YMoveCommand::YMoveCommand(const std::string& szName)
	: YCommand(szName, 2)
{
	m_rTypeArg.insert(std::pair<std::string, bool>("/y", false));
}

YMoveCommand::~YMoveCommand()
{

}

YErrorCode YMoveCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	YErrorCode rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rResultCode)
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
		{//��֧�� ��ʵ����·�����ƶ�
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
				return rResultCode;
			}
			rResultCode = checkSrcReal();
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			std::vector<YIFile*> rResult;
			rResultCode = g_pDiskOperator->moveFileFromRealDisk(m_rSrcArgList, m_rDstArgList, rResult);
		}
	}
	else
	{//Դ·��������ʵ·��
		if (dstIsReal)
		{//Ŀ��·������ʵ·��
			//verify
			if (!noWildCardPathValidation(szDst) || !noWildCardPathValidation(szSrc))
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			//��ͨ���ƥ���·��
			rResultCode = handleSrcToNoWildCard(szSrc);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			if (m_rSrcArgList.size() > 1)
			{
				return errorPrint(YERROR_PATH_ILLEGAL);
			}
			m_rDstArgList.push_back(getPathFromRealPath(szDst));
			//��֤��Ŀ��·��
			rResultCode = checkDstReal();
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
			}
			std::vector<YIFile*> rResult;
			rResultCode = g_pDiskOperator->moveFileToRealDisk(m_rSrcArgList, m_rDstArgList, rResult);
		}
		else
		{//�����������������п���
			//����Դ�ļ����Ҫ�������ļ���·��
			rResultCode = handleSrcToNoWildCard(szSrc);
			if (Y_OPERAT_SUCCEED != rResultCode)
			{
				return errorPrint(rResultCode);
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
			if (!m_rTypeArg["/y"])
			{
				std::function<bool(std::string&)> rPredicate([=](std::string &szName)->bool 
				{
					return userAsk(szName, " has exist.Do you want to Delete file?<y/n>:");
				});
				rResultCode = g_pDiskOperator->moveFile(m_rSrcArgList, m_rDstArgList, rPredicate);
			}
			else
			{
				std::function<bool(std::string&)> rPredicate([=]( std::string &szName)->bool {return true; });
				rResultCode = g_pDiskOperator->moveFile(m_rSrcArgList, m_rDstArgList, rPredicate);
			}
		}
	}
	return errorPrint(rResultCode);
}

void YMoveCommand::resetCommand()
{
	YCommand::resetCommand();
	m_rDstArgList.clear();
	m_rSrcArgList.clear();
}

YErrorCode YMoveCommand::virtual2virtualMove()
{

	return YErrorCode();
}

YErrorCode YMoveCommand::handleSrcToNoWildCard(const std::string & szSrc)
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
	for (size_t index = 0; index < rQueryResult.size(); ++index)
	{
		m_rSrcArgList.push_back(g_pDiskOperator->getFullPath(rQueryResult[index]));
	}
	if (m_rSrcArgList.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YMoveCommand::handleDstToNoWildCard(const std::string & szDst)
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
			std::smatch szMatchSrcPathResult;
			std::string szSrcName = getParentPath(m_rSrcArgList[index]);
			if (szSrcName.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}

			std::string absDstName = std::regex_replace(szSrcName, rDstResgex, szRepaceString);
			if (szMatchSrcPathResult.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}
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
				if (rQueryResult.empty() || nullptr == rQueryResult[0] || !rQueryResult[0]->IsFolder())
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
				if (m_rSrcArgList.size() > 1)
				{
					return YERROR_PATH_ILLEGAL;
				}
				else
				{
					std::vector<YIFile*> rQuerySrcResult;
					YErrorCode rResultCode = g_pDiskOperator->queryAllNode(m_rSrcArgList[0], rQuerySrcResult);
					if (!isRealPath(m_rArgList[0]))
					{
						if (rQuerySrcResult[0]->IsRealFolder())
						{
							return YERROR_PATH_ILLEGAL;
						}
					}
					m_rDstArgList.push_back(szDst);
				}
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

bool YMoveCommand::sameCheck()
{
	if (m_rDstArgList.size() != m_rSrcArgList.size()
		|| m_rSrcArgList.empty() || m_rDstArgList.empty())
	{
		return false;
	}
	for (size_t index = 0; index< m_rDstArgList.size();++index)
	{
		if (m_rDstArgList[index] == m_rSrcArgList[index])
		{
			return false;
		}
	}
	return true;
}

YErrorCode YMoveCommand::checkSrcReal()
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
			bool isCover = userAsk(m_rDstArgList[0], " has exist.Do you want to Delete file?<y/n>:");
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

YErrorCode YMoveCommand::checkDstReal()
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
