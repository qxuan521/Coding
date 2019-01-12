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
};

class YDirCommand :public YCommand
{
public:
	YDirCommand(const std::string& szName);
	virtual ~YDirCommand();
	virtual YErrorCode	excultCommand(std::vector<std::string> rArgArr) ;

	 
private:
	void			printResult(std::vector<DirSearchResult>& rResult);
	std::string		fileTypeString(YIFile* pFile);
	void			printSeftNParent(YIFile* pFile);
	YErrorCode		allChildSearch(std::vector<YIFile*>& rDirArr,std::vector<DirSearchResult>& rDirResult);
	YErrorCode		allChildFolderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult);
	YErrorCode		folderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult);
	YErrorCode		normalSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult);
	YErrorCode		searchHelpter(YIFile* pFile, std::vector<DirSearchResult>& rDirResultArr, std::set<YIFile*>& rHistorySet,std::function<bool(YIFile*)>& rPredicate);
private:

};