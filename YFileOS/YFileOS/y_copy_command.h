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
	YErrorCode		real2VirtualCopy();
	YErrorCode		virtual2RealCopy();
	YErrorCode		virtual2VirtualCopy();
	//����Դ·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode		handleSrcToNoWildCard(const std::string& szSrc);
	//����Ŀ��·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode		handleDstToNoWildCard(const std::string& szDst);

	YErrorCode		checkSameNAsk();
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

