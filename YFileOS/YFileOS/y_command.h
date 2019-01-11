#pragma once
#include <vector>
#include <regex>
#include <set>
#include "y_error_code.h"

class YCommand
{
public:
	YCommand(const std::string& szName,int nMstSize);
	virtual ~YCommand();
	
	virtual YErrorCode		excultCommand(std::vector<std::string> rArgArr) = 0;
	void					setCurWoringPath(const std::string& szCurPath);
	virtual void			errorPrint(YErrorCode rErrorType, std::string& szPath);
	//整理路径为绝对路径
	virtual YErrorCode		toAbsolutePath();
	virtual bool			isThisCommand(std::string& szCommandStr);
protected:
	std::vector<std::string>	m_rArgList;
	std::string					m_szCurWorkPath;
	std::set<std::string>		m_rTypeArg;
	int							m_nMustSize;
private:
	std::regex					m_rRegex;
};

