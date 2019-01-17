#pragma once

#include "y_command.h"

class YCopyCommand : public YCommand
{
public:
	YCopyCommand(const std::string& szName);
	virtual ~YCopyCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
	virtual void			resetCommand();
private:
	//����Դ·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode		handleSrcToNoWildCard(const std::string& szSrc);
	//����Ŀ��·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode		handleDstToNoWildCard(const std::string& szDst);
	bool			sameCheck();
	YErrorCode		checkSameNAsk();
	YErrorCode					checkSrcReal();
	YErrorCode					checkDstReal();
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

