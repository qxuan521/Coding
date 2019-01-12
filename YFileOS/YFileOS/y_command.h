#pragma once
#include <vector>
#include <regex>
#include <map>
#include "y_error_code.h"
#include "y_ifile.h"

class YCommand
{
public:
	YCommand(const std::string& szName,int nMstSize);
	virtual ~YCommand();
	
	virtual YErrorCode		excultCommand(std::vector<std::string> rArgArr) = 0;
	void					setCurWoringPath(const std::string& szCurPath);
	virtual void			errorPrint(YErrorCode rErrorType, std::string& szPath);
	//整理路径为绝对路径
	virtual YErrorCode		toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList);
	virtual bool			isThisCommand(std::string& szCommandStr);
	virtual void			resetCommand();
protected:
	void					resetTypeArg();
	std::vector<std::string>	m_rArgList;
	std::string					m_szCurWorkPath;
	std::map<std::string,bool>	m_rTypeArg;
	int							m_nMustSize;
private:
	std::regex					m_rRegex;
};

