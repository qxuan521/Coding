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
	//处理源路径 成为没有通配符的文件列表
	YErrorCode		handleSrcToNoWildCard(const std::string& szSrc);
	//处理目标路径 成为没有通配符的文件列表
	YErrorCode		handleDstToNoWildCard(const std::string& szDst);
	bool			sameCheck();
	YErrorCode		checkSameNAsk();
	YErrorCode					checkSrcReal();
	YErrorCode					checkDstReal();
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

