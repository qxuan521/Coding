#pragma once
#include "y_error_code.h"
#include <string>
#include <vector>
class YFile;
class YDisk
{
public:
	YDisk();
	~YDisk();

	YErrorCode	addNode(YFile* parent, YFile* newNode);
	YErrorCode  takeNode(YFile* parent, YFile* beTokenNode);
	YErrorCode	formatDisk();
	YErrorCode  destroyFileNode(YFile*& beDestroYFile);
	YFile*		queryFileNode(const std::string& szPath);
	YErrorCode  clear();



	YErrorCode	renameFile(YFile* pFileNode,const std::string& szName);
	YErrorCode  createDataFile(YFile*& newFileNode, const std::string& szName);
	YErrorCode  createSymlnkFile(YFile*& newFileNode, YFile* pDstFile, const std::string& szName);
	YErrorCode  createFolderFile(YFile*& newFileNode, const std::string& szName);
	YErrorCode	getFileFullPath(YFile* pFile, std::string& fullPath);

	//Verify
	bool		isRootName(const std::string& szName);
private:
	YErrorCode  destroyAllFileNode();
	YFile*		queryFileHelper(YFile* pParent, std::vector<std::string>& rNameArr, size_t nPathindex);
	void		destroyHelper(YFile*& pFile);
	void		fullPathHelper(YFile* pFile, std::string& subPath);
	std::string	getDate();
private:
	std::vector<YFile*>		m_rRootArr;
};

extern std::unique_ptr<YDisk> g_pDiskPtr;