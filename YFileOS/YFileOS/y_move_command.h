#pragma once

#include "y_command.h"

class YMoveCommand : public YCommand
{
public:
	YMoveCommand(const std::string& szName);
	~YMoveCommand();
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);

private:
	YErrorCode					virtual2virtualMove();
	//����Դ·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode					handleSrcToNoWildCard(const std::string& szSrc);
	//����Ŀ��·�� ��Ϊû��ͨ������ļ��б�
	YErrorCode					handleDstToNoWildCard(const std::string& szDst);
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

