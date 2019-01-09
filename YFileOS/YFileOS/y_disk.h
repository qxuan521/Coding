#pragma once
#include "y_error_code.h"
#include <string>
#include <vector>
class YFile;
class YIFile;

class YDisk
{
public:
	YDisk();
	~YDisk();

	YErrorCode	addNode(YIFile* parent, YIFile* newNode);
	YErrorCode  takeNode(YIFile* parent, YIFile* beTokenNode);
	YErrorCode	formatDisk();
	YErrorCode  destroyFileNode(YIFile*& beDestroyFile);
	YIFile*		queryFileNode(const std::string& szPath);
	YErrorCode  clear();



	YErrorCode	renameFile(YIFile* pFileNode,const std::string& szName);
	YErrorCode  createDataFile(YIFile*& newFileNode, const std::string& szName);
	YErrorCode  createSymlnkFile(YIFile*& newFileNode, YIFile* pDstFile, const std::string& szName);
	YErrorCode  createFolderFile(YIFile*& newFileNode, const std::string& szName);
	YErrorCode	getFileFullPath(YIFile* pFile, std::string& fullPath);


	std::string& getRootName();
private:
	YErrorCode  destroyAllFileNode();
	YFile*		queryFileHelper(YFile* pParent, std::vector<std::string>& rNameArr, size_t nPathindex);
	void		destroyHelper(YFile*& pFile);
	void		fullPathHelper(YFile* pFile, std::string& subPath);
	std::string	getDate();
private:
	YFile*		m_DataRoot;
};

extern std::unique_ptr<YDisk> g_pDiskPtr;