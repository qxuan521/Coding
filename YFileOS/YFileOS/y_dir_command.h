#pragma once
#include <functional>
#include <set>
#include "y_command.h"
struct DirSearchResult
{
	YIFile*					FilePtr = NULL;
	YIFile*					ParentPtr = NULL;
	std::vector<YIFile*>	CurLevelResult;
	int						FolderCount = 0;
	int						FileCount = 0;
	//裸初始化 什么验证都没有
	void initializeResult(YIFile* pFile);
};

class YDirCommand :public YCommand
{
public:
	YDirCommand(const std::string& szName);
	virtual ~YDirCommand();
	virtual YErrorCode	excultCommand(YCommandInfo& rCommandInfo) ;

	 
private:
	void			printResult(std::vector<DirSearchResult>& rResult);
	std::string		fileTypeString(YIFile* pFile);
	void			printSeftNParent(YIFile* pFile);
	YErrorCode		allChildSearch(std::vector<YIFile*>& rDirArr,std::vector<DirSearchResult>& rDirResult, std::regex& rMatchRegex);
	YErrorCode		allChildFolderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult, std::regex& rMatchRegex);
	YErrorCode		folderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult, std::regex& rMatchRegex);
	YErrorCode		normalSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult, std::regex& rMatchRegex);
	YErrorCode		searchHelpter(YIFile* pFile, std::vector<DirSearchResult>& rDirResultArr, std::set<YIFile*>& rHistorySet,std::function<bool(YIFile*)>& rPredicate);
	YErrorCode		handleWildCard(std::vector<YIFile*>& rResult);
private:
};