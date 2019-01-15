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
	DelOneLevel(rDelPathArr);
	//��·��ȫ·�����п��ܰ���ͨ�����ת���ɽڵ����� !!!!ȥ��ͨ���ȥ���ļ��� ֻ���ļ�
	std::vector<YIFile*> rQueryResult;
	if (m_rTypeArg["/s"])
	{//ɾ�����нڵ�
		for (size_t index = 0; index < m_rArgList.size(); index++)
		{
			rResultCode = g_pDiskOperator->queryAllNode(m_rArgList[index], rQueryResult);
			if (rResultCode != Y_OPERAT_SUCCEED)
			{
				errorPrint(rResultCode, m_rArgList[index]);
			}
		}
	}


	return YErrorCode();
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

YErrorCode YDelCommand::DelOneLevel(std::vector<std::string>& rDelPathArr)
{
	YErrorCode rResultCode;
	std::vector<YIFile*> rQueryResult;
	for (size_t index = 0; index < m_rArgList.size(); index++)
	{
		rResultCode = g_pDiskOperator->queryFileNode(m_rArgList[index], rQueryResult);
		if (rResultCode != Y_OPERAT_SUCCEED)
		{
			errorPrint(rResultCode, m_rArgList[index]);
		}
	}
}