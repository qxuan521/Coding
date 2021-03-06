#pragma once
#include <vector>
#include <regex>
#include <map>
#include <string>
#include <iostream>
#include "y_error_code.h"
#include "y_ifile.h"

struct YCommandInfo
{
	std::string szCommandName;
	std::vector<std::string> rArgList;
	std::vector<std::string> rPathList;
};

class YCommand
{
public:
	YCommand(const std::string & szName,int nMstSize);
	virtual ~YCommand();
	
	virtual YErrorCode		excultCommand(YCommandInfo& rCommandInfo) = 0;
	void					setCurWoringPath(const std::string& szCurPath);
	virtual YErrorCode		errorPrint(YErrorCode rErrorType, std::string szPath = std::string(""));
	//整理路径为绝对路径
	virtual YErrorCode		toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList);
	virtual bool			isThisCommand(std::string& szCommandStr);
	virtual void			resetCommand();
	virtual YErrorCode		handleCommandArg(YCommandInfo& rCommandInfo);
protected:
	void					resetTypeArg();
	bool					pathValidation(const std::string& szPath);
	bool					noWildCardPathValidation(const std::string& szPath);
	bool					rootDiskPathValidation(const std::string& szPath);
	bool					wildCardOnlyLastLevel(const std::string& szPath);
	bool					mayHaveWildCard(const std::string& szPath);
	bool					pathCanbeRealValid();
	bool					userAsk(const std::string& szPath, const std::string& szAskContent);
	std::vector<std::string>	m_rArgList;
	std::string					m_szCurWorkPath;
	std::map<std::string,bool>	m_rTypeArg;
	int							m_nMustSize;
private:
	std::regex					m_rRegex;
};

