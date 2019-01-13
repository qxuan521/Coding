#pragma once

#include "y_command.h"

class YCopyCommand : public YCommand
{
public:
	YCopyCommand(const std::string& szName);
	~YCopyCommand();

	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo);
private:
	YErrorCode		real2VirtualCopy();
	YErrorCode		virtual2RealCopy();
	YErrorCode		virtual2VirtualCopy();
	YErrorCode		handleSrcToNoWildCard(const std::string& szSrc);
	std::vector<std::string>	m_rSrcArgList;
	std::vector<std::string>	m_rDstArgList;
};

