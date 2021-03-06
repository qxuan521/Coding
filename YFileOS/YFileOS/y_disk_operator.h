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
	YErrorCode queryAllChildFile(const std::string& szPath, std::vector<YIFile*>& rResultArr);
	YErrorCode queryAllChildFolder(const std::string& szPath, std::vector<YIFile*>& rResultArr);
	//copy
	YErrorCode copyFileNode(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	YErrorCode copyFileFromRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	YErrorCode copyFileToRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	//delete
	YErrorCode deleteNode(const std::string& szPath);
	//changeName
	YErrorCode changeName(const std::string& szSrcPath, const std::string& szName);
	//load & save
	YErrorCode loadData(const std::string& szSrcPath);
	YErrorCode saveData(const std::string& szDstPath);
	// move
	YErrorCode moveFile(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::function<bool(std::string& szPath)>& rPredicate);
	YErrorCode moveFileFromRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
	YErrorCode moveFileToRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult);
//About File
	YErrorCode getChildren(YIFile* pFile, std::vector<YIFile*>& rResult);
	YErrorCode getChild(YIFile * pFile, const std::string& szName, YIFile*& pChild);
//Disk
	void	formatDisk();
//path
	std::string getFullPath(YIFile* pFile);
	std::string getCurWorkingPath();
	YErrorCode	setCurWorkingPath(const std::string& szCurWokingPath);

//Verify
	bool	isRootName(const std::string& szName);
	bool	isPathExist(const std::string& szPath);
	YFile*	lnkDstFindHelper(const std::string& szPath);
private:
	void	queryHelper
	(
		std::vector<YFile*>& pParentNodes,
		size_t nPathindex ,
		std::function<bool(YFile*, size_t)>& rPredicate,
		std::function<bool(size_t)>& rFinishPredicate,
		std::vector<YIFile*>& rResult, 
		std::set<YFile*>& rHistorySet
	);
	void queryHelper(std::vector<YFile*>& rParentNodes, std::function<bool(YFile*)>& rPredicate, std::vector<YIFile*>& rResult, std::set<YFile*>& rHistorySet);
	void copyFIleHelper(YFile*& pSrcFile, YFile*& pDstFile,std::string& szDstName);
	void saveDataHelper(YFile* pParentNode, std::fstream& rFile,int& nFileCount);
	YErrorCode initializeRootDisk(std::vector<char>& rRootArr);
	YErrorCode initializeFileTree(int32_t nFileCount, std::fstream& rFileStream);
	void bufferResetByDataSize(std::vector<char>& rBuffer, int size);
	YErrorCode folderMoveHelper(YFile* rSrcRootNode,std::string& szDstPath, std::function<bool(std::string& szPath)>& rPredicate, std::set<std::string>& rHistorySet);
	YErrorCode fileMoveHelper(YFile* rSrcRootNode, std::string& szDstPath, std::function<bool(std::string& szPath)>& rPredicate,  std::set<std::string>& rHistorySet);
	YErrorCode FolderMoveCoverHelper(
		YFile* pSrcRootNode, 
		YFile* pSrcParentNode, 
		YFile* pDstRootNode, 
		YFile* pDstParentNode, 
		std::function<bool(std::string& szPath)>& rPredicate,
		YFile*& pResultRoot
	);
private:

	YDisk*		m_pDisk;
	std::string m_szCurWorkingPath;
};

extern YDiskOperator*  g_pDiskOperator;