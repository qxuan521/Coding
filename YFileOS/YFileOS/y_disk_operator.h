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
//command support
	//create
	YErrorCode createNewFile(const std::string& szPath, YIFile*& pResult);
	YErrorCode createNewFolder(const std::string& szPath, YIFile*& pResult);
	YErrorCode createNewLnkFile(const std::string& szPath, const std::string & szDstFilePath, YIFile*& pResult);
	YErrorCode createNewDisk(const std::string& szPath, YIFile*& pResult);
	//query
	YErrorCode queryFolderNode(const std::string& szPath, std::vector<YIFile*>& rResultArr);
	YErrorCode queryFileNode(const std::string& szPath, std::vector<YIFile*>& rResultArr);
	YErrorCode queryAllNode(const std::string& szPath, std::vector<YIFile*>& rResultArr);
	//copy
	YErrorCode copyFileNode(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	YErrorCode copyFileFromRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	YErrorCode copyFileToRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
//About File
	YErrorCode getChildren(YIFile* pFile, std::vector<YIFile*>& rResult);

//path
	std::string getFullPath(YIFile* pFile);
	std::string getCurWorkingPath();
	YErrorCode	setCurWorkingPath(const std::string& szCurWokingPath);
//Verify
	bool	isRootName(const std::string& szName);
	bool	isPathExist(const std::string& szPath);
private:
	YFile*	lnkDstFindHelper(const std::string& szPath);
	void	queryHelper
	(
		std::vector<YFile*>& pParentNodes,
		size_t nPathindex ,
		std::function<bool(YFile*, size_t)>& rPredicate,
		std::function<bool(size_t)>& rFinishPredicate,
		std::vector<YIFile*>& rResult, 
		std::set<YFile*>& rHistorySet
	);
	void copyFIleHelper(YFile*& pSrcFile, YFile*& pDstFile,std::string& szDstName);
private:

	YDisk*		m_pDisk;
	std::string m_szCurWorkingPath;
};

extern YDiskOperator*  g_pDiskOperator;