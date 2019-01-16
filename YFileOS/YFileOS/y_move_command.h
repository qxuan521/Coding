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
	//处理源路径 成为没有通配符的文件列表
	YErrorCode					handleSrcToNoWildCard(const std::string& szSrc);
	//处理目标路径 成为没有通配符的文件列表
	YErrorCode					handleDstToNoWildCard(const std::string& szDst);
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

