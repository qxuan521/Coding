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

	YErrorCode  createDataFile(YIFile*& newFileNode);
	YErrorCode  createSymlnkFile(YIFile*& newFileNode, YIFile* pDstFile);
	YErrorCode  createFolderFile(YIFile*& newFileNode);


	YErrorCode  destroyFileNode(YIFile*& beDestroyFile);

	YErrorCode  destroyAllFileNode();

	YIFile*		queryFileNode(const std::string& szPath);

	YErrorCode  clear();

private:
	YFile*		queryFileHelper(YFile* pParent, std::vector<std::string>& rNameArr, size_t nPathindex);
private:
	YFile*		m_DataRoot;
};

