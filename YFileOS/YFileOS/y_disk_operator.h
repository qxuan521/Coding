#pragma once
#include <vector>
#include <regex>
#include <functional>
#include <set>
#include "y_error_code.h"
class YDisk;
class YIFile;
class YFile;
class YDiskOperator
{
public:
	YDiskOperator();
	~YDiskOperator();

	YErrorCode createNewFile(const std::string& szPathArr,YIFile*& pResult);
	YErrorCode queryFolderNode(const std::string& szPath, std::vector<YIFile*>& rResultArr);
//About File
	YErrorCode getChildren(YIFile* pFile, std::vector<YIFile*>& rResult);

//path
	std::string getFullPath(YIFile* pFile);
	std::string getCurWorkingPath();
	void		setCurWorkingPath(const std::string& szCurWokingPath);
//Verify
	bool	isRootName(const std::string& szName);
	bool	isPathExist(const std::string& szPath);
private:
	std::regex			makeRegexByPath(const std::string& szPath);
	void				queryHelper(YFile* pNode, std::function<bool(YFile*)>& rPredicate,std::vector<YIFile*>& rResult, std::set<YFile*>& rHistorySet);
private:

	YDisk*		m_pDisk;
	std::string m_szCurWorkingPath;
};

extern YDiskOperator*  g_pDiskOperator;