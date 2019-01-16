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
	//处理源路径 成为没有通配符的文件列表
	YErrorCode		handleSrcToNoWildCard(const std::string& szSrc);
	//处理目标路径 成为没有通配符的文件列表
	YErrorCode		handleDstToNoWildCard(const std::string& szDst);

	YErrorCode		checkSameNAsk();
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

