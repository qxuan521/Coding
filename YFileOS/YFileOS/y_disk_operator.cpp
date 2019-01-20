#include <fstream>
#include "y_disk_operator.h"
#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"
#include "y_symlnk_file.h"
#include "y_link_manager.h"
YDiskOperator* g_pDiskOperator = new YDiskOperator;

struct YDataSaveFileHead
{
	int32_t			nFileType;
	int32_t			nFilePathSize = 0;
	int32_t			nFileDataSize = 0;
	char			rModifyDate[25];
};

struct LinkFileData
{
	std::string RealPath;
	std::string DstPath;
	std::string ModifyDate;
};


YDiskOperator::YDiskOperator()
	: m_pDisk(new YDisk)
{
	m_szCurWorkingPath = "c:";
}

YDiskOperator::~YDiskOperator()
{

}

YErrorCode YDiskOperator::createNewFile(const std::string & szPath, YIFile *& pResult)
{
	std::string szParent = getParentPath(szPath);
	std::string szNamePart = getNameFromFullPath(szPath);
	if (szParent.empty())
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YFile* pNewFileParent  = m_pDisk->queryFileNode(szParent);
	YFile* pResultFile = nullptr;
	if (nullptr == pNewFileParent)
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YErrorCode rResultCode = m_pDisk->createDataFile(pResultFile, szNamePart);
	if (rResultCode != Y_OPERAT_SUCCEED)
	{
		return rResultCode;
	}
	pResult = (YIFile*)pResultFile;
	return m_pDisk->addNode(pNewFileParent, pResultFile);
}

YErrorCode YDiskOperator::createNewFolder(const std::string & szPath, YIFile *& pResult)
{
	std::string szParent = getParentPath(szPath);
	std::string szNamePart = getNameFromFullPath(szPath);
	if (szParent.empty())
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YFile* pNewFileParent = m_pDisk->queryFileNode(szParent);
	YFile* pResultFile = nullptr;
	if (nullptr == pNewFileParent)
	{
		return YERROR_PATH_NOT_EXIST;
	}
	if (pNewFileParent->IsRealSymLnk())
	{
		pNewFileParent = m_pDisk->queryFileNode(pNewFileParent->getShowName());
		if (nullptr == pNewFileParent)
		{
			return YERROR_PATH_NOT_EXIST;
		}
	}
	if (pNewFileParent->IsFile())
	{
		return Y_OPERAT_FAILD;
	}
	YErrorCode rResultCode = m_pDisk->createFolderFile(pResultFile, szNamePart);
	if (rResultCode != Y_OPERAT_SUCCEED)
	{
		return rResultCode;
	}
	pResult = (YIFile*)pResultFile;
	return m_pDisk->addNode(pNewFileParent, pResultFile);
}

YErrorCode YDiskOperator::createNewLnkFile(const std::string & szPath, const std::string & szDstFilePath, YIFile *& pResult)
{
	std::string szParent = getParentPath(szPath);
	std::string szNamePart = getNameFromFullPath(szPath);
	if (szParent.empty())
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YFile* pNewFileParent = m_pDisk->queryFileNode(szParent);
	YFile* pResultFile = nullptr;
	YFile* pDstFile = g_pDiskOperator->lnkDstFindHelper(szDstFilePath);
	if (nullptr == pNewFileParent || nullptr == pDstFile)
	{
		return YERROR_PATH_NOT_EXIST;
	}
	YErrorCode rResultCode = m_pDisk->createSymlnkFile(pResultFile, pDstFile, szNamePart);
	if (rResultCode != Y_OPERAT_SUCCEED)
	{
		return rResultCode;
	}
	pResult = (YIFile*)pResultFile;
	return m_pDisk->addNode(pNewFileParent, pResultFile);
}

YErrorCode YDiskOperator::createNewDisk(const std::string & szPath, YIFile *& pResult)
{
	YFile* pResultFile = (YFile*)pResult;
	if (g_pDiskOperator->isRootName(szPath))
	{
		return TERROR_DISK_ERROR;
	}
	return m_pDisk->createRootNode(szPath, pResultFile);
}

YErrorCode YDiskOperator::queryFolderNode(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	if (szPath.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath, '/');
	if (rSplitResult.empty())
		return YERROR_PATH_ILLEGAL;
	std::vector<YFile*> rRootArr = m_pDisk->getRootArr();
	if (rRootArr.empty())
	{
		return TERROR_DISK_ERROR;
	}
	std::function<bool(YFile*, size_t)> rMatchFunc([=](YFile* pFile,size_t index)->bool
	{
		std::regex rPathRegex = makeRegexByPath(rSplitResult[index]);
		std::string szNodePath = pFile->getName();
		if (szNodePath.empty())
		{
			return false;
		}
		return (pFile->IsFolder() && std::regex_match(szNodePath, rPathRegex));
	});
	std::function<bool(size_t)> rFinishPredicate([=](size_t index)->bool 
	{
		return rSplitResult.size() == index + 1;
	});
	std::set<YFile*> rMatchHistory;
	queryHelper(rRootArr,0, rMatchFunc, rFinishPredicate, rResultArr, rMatchHistory);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::queryFileNode(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	if (szPath.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath, '/');
	if (rSplitResult.empty())
		return YERROR_PATH_ILLEGAL;
	std::vector<YFile*> rRootArr = m_pDisk->getRootArr();
	if (rRootArr.empty())
	{
		return TERROR_DISK_ERROR;
	}
	std::function<bool(YFile*, size_t)> rMatchFunc([=](YFile* pFile, size_t index)->bool
	{
		std::regex rPathRegex = makeRegexByPath(rSplitResult[index]);
		std::string szNodePath = pFile->getName();
		if (szNodePath.empty())
		{
			return false;
		}
		bool bFileType = rSplitResult.size() == index + 1? pFile ->IsRealFile() : pFile->IsFolder();
		return (bFileType && std::regex_match(szNodePath, rPathRegex));
	});
	std::function<bool(size_t)> rFinishPredicate([=](size_t index)->bool
	{
		return rSplitResult.size() == index + 1;
	});
	std::set<YFile*> rMatchHistory;
	queryHelper(rRootArr, 0, rMatchFunc, rFinishPredicate, rResultArr, rMatchHistory);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::queryAllNode(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	if (szPath.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath, '/');
	if (rSplitResult.empty())
		return YERROR_PATH_ILLEGAL;
	std::vector<YFile*> rRootArr = m_pDisk->getRootArr();
	if (rRootArr.empty())
	{
		return TERROR_DISK_ERROR;
	}
	std::function<bool(YFile*, size_t)> rMatchFunc([=](YFile* pFile, size_t index)->bool
	{
		std::regex rPathRegex = makeRegexByPath(rSplitResult[index]);
		std::string szNodePath = pFile->getName();
		if (szNodePath.empty())
		{
			return false;
		}
		return (std::regex_match(szNodePath, rPathRegex));
	});
	std::function<bool(size_t)> rFinishPredicate([=](size_t index)->bool
	{
		return rSplitResult.size() == index + 1;
	});
	std::set<YFile*> rMatchHistory;
	queryHelper(rRootArr, 0, rMatchFunc, rFinishPredicate, rResultArr, rMatchHistory);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::queryAllChildFile(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	YFile* pRootFolder = m_pDisk->queryFileNode(szPath);
	std::set<YFile*> rHistorySet;
	std::function<bool(YFile*)> rPredicate([=](YIFile* pfile)->bool
	{
		return !pfile->IsFolder();
	});
	std::vector<YFile*> rChildren = pRootFolder->getChildren();
	queryHelper(rChildren, rPredicate, rResultArr, rHistorySet);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::queryAllChildFolder(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	YFile* pRootFolder = m_pDisk->queryFileNode(szPath);
	std::set<YFile*> rHistorySet;
	std::function<bool(YFile*)> rPredicate([=](YIFile* pfile)->bool
	{
		return pfile->IsFolder();
	});
	std::vector<YFile*> rChildren = pRootFolder->getChildren();
	queryHelper(rChildren, rPredicate, rResultArr, rHistorySet);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::copyFileNode(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult)
{
	std::string szDstParent = getParentPath(rDstPathArr[0]);
	YFile* pDstParent = m_pDisk->queryFileNode(szDstParent);
	if (nullptr == pDstParent)
	{
		return YERROR_POINTER_NULL;
	}
	for (size_t index = 0; index < rDstPathArr.size();++index)
	{
		YFile* pDstFile = m_pDisk->queryFileNode(rDstPathArr[index]);
		YFile* pSrcFile = m_pDisk->queryFileNode(rSrcPathArr[index]);
		YFile* pSrcParent = m_pDisk->queryFileNode(rSrcPathArr[index]);
		if (pSrcParent == nullptr || pSrcFile == nullptr)
		{
			continue;
		}
		std::string szDstName = getNameFromFullPath(rDstPathArr[index]);
		if (nullptr == pDstFile)
		{//没有重名文件
			if (szDstName.empty())
			{
				return YERROR_PATH_ILLEGAL;
			}
			copyFIleHelper(pSrcFile, pDstFile, szDstName);
			m_pDisk->addNode(pDstParent, pDstFile);
		}
		else
		{//存在重名文件
			YFile* pNewFile = nullptr;
			m_pDisk->takeNode(pDstParent, pDstFile);
			copyFIleHelper(pSrcFile, pNewFile, szDstName);
			m_pDisk->addNode(pDstParent, pNewFile);
			if (!pSrcFile->IsRealSymLnk())
			{
				g_pSymMananger->changeLnkDst(pDstFile, pNewFile);
				g_pSymMananger->delDstFile(pDstParent);
			}
			m_pDisk->destroyFileNode(pDstFile);
		}
	}
	return Y_COPY_SUCCEED;
}

YErrorCode YDiskOperator::copyFileFromRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult)
{
	std::fstream rFileReader(rSrcPathArr[0], std::ios::binary | std::ios::in);
	if (!rFileReader.is_open())
	{
		return YERROR_PATH_ILLEGAL;
	}
	YIFile* pfile = m_pDisk->queryFileNode(rDstPathArr[0]);
	YFile*	pDstParent = m_pDisk->queryFileNode(getParentPath(rDstPathArr[0]));
	if (nullptr != pfile)
	{
		YFile* pFileRef = (YFile*)pfile;
		m_pDisk->takeNode(pDstParent, pFileRef);
		m_pDisk->destroyFileNode(pFileRef);
		pfile = nullptr;
	}
	createNewFile(rDstPathArr[0], pfile);
	rCopyResult.push_back(pfile);
	std::vector<int8_t> rBuffer;
	rFileReader.seekg(0, std::ios::end);
	long fileSize = (long)rFileReader.tellg();
	rBuffer.resize(fileSize);
	int8_t* DataBuffer = &rBuffer[0];
	rFileReader.seekg(0, std::ios::beg);
	rFileReader.read((char*)DataBuffer, fileSize);
	((YFile*)pfile)->setFileData(&rBuffer[0], fileSize);
	rFileReader.close();
	return 	Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::copyFileToRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult)
{
	std::fstream rFileReader(rDstPathArr[0], std::ios::binary | std::ios::out | std::ios::trunc);
	if (!rFileReader.is_open())
	{
		return YERROR_PATH_ILLEGAL;
	}
	YFile* pSrcFile = m_pDisk->queryFileNode(rSrcPathArr[0]);
	if (nullptr == pSrcFile)
	{
		return YERROR_POINTER_NULL;
	}

	std::vector<int8_t> rBuffer;
	const int8_t* DataBuffer = pSrcFile->getFileData();
	int fileSize = pSrcFile->getFileSize();
	rFileReader.write((char*)DataBuffer, fileSize);
	rFileReader.close();
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::deleteNode(const std::string & szPath)
{
	YFile* pFile = m_pDisk->queryFileNode(szPath);
	std::string szParentPath = getParentPath(szPath);
	YFile* pParent = m_pDisk->queryFileNode(szParentPath);
	if (nullptr == pFile || nullptr == pParent)
	{
		return YERROR_POINTER_NULL;
	}
// 	YErrorCode rResultCode;
// 	rResultCode = m_pDisk->takeNode(pParent, pFile);
// 	if (Y_OPERAT_SUCCEED != rResultCode)
// 	{
// 		return rResultCode;
// 	}
	return m_pDisk->destroyAllChildFileNode(pFile);
}

YErrorCode YDiskOperator::changeName(const std::string & szSrcPath, const std::string & szName)
{
	YFile* pParent = m_pDisk->queryFileNode(getParentPath(szSrcPath));
	YFile* pFile = m_pDisk->queryFileNode(szSrcPath);
	if (nullptr == pFile || nullptr == pParent)
	{
		return YERROR_POINTER_NULL;
	}
	//验重
	std::vector<YFile*> rChildren = pParent->getChildren();
	for (size_t index  = 0 ;index < rChildren.size();++index)
	{
		if (rChildren[index]->getName() == szName)
		{
			return YERROR_FILE_IS_EXIST;
		}
	}
	pFile->setName(szName);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::loadData(const std::string & szSrcPath)
{

	std::vector<char> rBuffer;
	std::vector<LinkFileData> LinkFileQueue;
	std::fstream rFileReader(szSrcPath, std::ios::binary | std::ios::in);
	if (!rFileReader.is_open())
	{
		return YERROR_PATH_ILLEGAL;
	}
	//格式化磁盘
	m_pDisk->formatDisk();
	//开始读文件
	int32_t nDiskCount = 0, nFileCount = 0;
	rFileReader.read((char*)&nDiskCount, sizeof(int32_t));
	rFileReader.read((char*)&nFileCount, sizeof(int32_t));
	rBuffer.resize(nDiskCount);
	rFileReader.read(&rBuffer[0], nDiskCount);
	YErrorCode rResultCode = initializeRootDisk(rBuffer);
	rResultCode = initializeFileTree(nFileCount, rFileReader);
	rFileReader.close();
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::saveData(const std::string & szDstPath)
{
	std::fstream rFileReader(szDstPath, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!rFileReader.is_open())
	{
		return YERROR_PATH_ILLEGAL;
	}
	//写盘符
	uint32_t nRootSize =(uint32_t)m_pDisk->getRootArr().size();
	rFileReader.write((char*)&nRootSize ,4);
	rFileReader.write((char*)&nRootSize, 4);
	std::vector<YFile*> rRootArr = m_pDisk->getRootArr();
	for (size_t index = 0 ;index < rRootArr.size() ;++index )
	{
		rFileReader.write(&rRootArr[index]->getName()[0], 1);
	}
	int32_t nFileCount = 0;
	for (size_t index = 0;index < rRootArr.size();++index)
	{
		saveDataHelper(rRootArr[index], rFileReader, nFileCount);
	}
	rFileReader.seekg(4, std::ios::beg);
	rFileReader.write((char*)&nFileCount, sizeof(int32_t));
	rFileReader.close();
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::moveFile(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::function<bool(std::string&szPath)>& rPredicate)
{
	std::set<std::string> rHistorySet;
	for (size_t index = 0; index < rDstPathArr.size();++index)
	{
		YFile* pFile = m_pDisk->queryFileNode(rSrcPathArr[index]);
		if (nullptr == pFile)
		{
			continue;
		}
		if (!pFile->IsRealFolder())
		{
			fileMoveHelper(pFile, rDstPathArr[index], rPredicate,  rHistorySet);
		}
		else
		{
			folderMoveHelper(pFile, rDstPathArr[index], rPredicate, rHistorySet);
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::moveFileFromRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult)
{
	YErrorCode rResultCode = copyFileFromRealDisk(rSrcPathArr, rDstPathArr, rCopyResult);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	std::string szPath;
	szPath += "del ";
	szPath += rSrcPathArr[0];
	std::replace_if(szPath.begin(), szPath.end(), [](char in) {return in == '/'; }, '\\');
	system(szPath.c_str());
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::moveFileToRealDisk(std::vector<std::string>& rSrcPathArr, std::vector<std::string>& rDstPathArr, std::vector<YIFile*>& rCopyResult)
{
	YErrorCode rResultCode = copyFileToRealDisk(rSrcPathArr, rDstPathArr, rCopyResult);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	return deleteNode(rDstPathArr[0]);
}

YErrorCode YDiskOperator::getChildren(YIFile * pFile, std::vector<YIFile*>& rResult)
{
	if (nullptr == pFile)
	{
		return YERROR_POINTER_NULL;
	}
	std::vector<YFile*> rChildren = ((YFile*)pFile)->getChildren();
	for (size_t index = 0; index < rChildren.size(); ++index)
	{
		rResult.push_back(rChildren[index]);
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::getChild(YIFile * pFile, const std::string& szName ,YIFile*& pChild)
{
	std::vector<YFile*> rChildren = ((YFile*)pFile)->getChildren();
	for (size_t index = 0; index < rChildren.size(); ++index)
	{
		if (szName == rChildren[index]->getName())
		{
			pChild = rChildren[index];
			return Y_OPERAT_FAILD;
		}
	}
	pChild = nullptr;
	return Y_OPERAT_FAILD;
}

void YDiskOperator::formatDisk()
{
	m_pDisk->formatDisk();
	YFile* pRoot = nullptr;
	m_pDisk->createRootNode("c:", pRoot);
}

std::string YDiskOperator::getFullPath(YIFile* pFile)
{
	std::string rPath;
	m_pDisk->getFileFullPath((YFile*)pFile, rPath);
	return rPath;
}

std::string YDiskOperator::getCurWorkingPath()
{
	return m_szCurWorkingPath;
}

YErrorCode YDiskOperator::setCurWorkingPath(const std::string & szCurWokingPath)
{
	YFile* pFile = m_pDisk->queryFileNode(szCurWokingPath);
	if (nullptr == pFile || !pFile->IsFolder())
	{
		return YERROR_PATH_ILLEGAL;
	}
	m_szCurWorkingPath = szCurWokingPath;
	return Y_OPERAT_SUCCEED;
}


bool YDiskOperator::isRootName(const std::string& szName)
{
	return m_pDisk->isRootName(szName);
}

bool YDiskOperator::isPathExist(const std::string& szPath)
{
	return nullptr != m_pDisk->queryFileNode(szPath);
}
YFile * YDiskOperator::lnkDstFindHelper(const std::string & szPath)
{
	YFile* pDstFile = m_pDisk->queryFileNode(szPath);
	while (pDstFile != nullptr && pDstFile->IsRealSymLnk())
	{
		pDstFile = m_pDisk->queryFileNode(pDstFile->getShowName());
	}
	return pDstFile;
}


void YDiskOperator::queryHelper
(
	std::vector<YFile*>& pParentNodes, 
	size_t  nPathindex, 
	std::function<bool(YFile*, size_t)>& rPredicate, 
	std::function<bool(size_t)>& rFinishPredicate, 
	std::vector<YIFile*>& rResult, 
	std::set<YFile*>& rHistorySet
)
{
	if (pParentNodes.empty())
		return;
	if (rFinishPredicate(nPathindex - 1))
	{
		return;
	}
	for (size_t index = 0; index < pParentNodes.size(); ++index)
	{
		if (rHistorySet.find(pParentNodes[index]) == rHistorySet.end())
		{
			if (nullptr == pParentNodes[index])
			{
				continue;
			}
			else
			{
				if (rPredicate(pParentNodes[index], nPathindex))
				{
					if (rFinishPredicate(nPathindex))
					{
						rResult.push_back(pParentNodes[index]);
					}
					else
					{
						std::vector<YFile*> rChildren = pParentNodes[index]->getChildren();
						if (!rChildren.empty())
						{
							queryHelper(rChildren, nPathindex + 1, rPredicate, rFinishPredicate, rResult, rHistorySet);
						}
					}
				}
				rHistorySet.insert(pParentNodes[index]);
			}
			
		}
	}
}

void YDiskOperator::queryHelper(std::vector<YFile*>& rParentNodes, std::function<bool(YFile*)>& rPredicate, std::vector<YIFile*>& rResult, std::set<YFile*>& rHistorySet)
{
	for (size_t index = 0;index < rParentNodes.size();++index)
	{
		if (rPredicate(rParentNodes[index]))
		{
			rResult.push_back(rParentNodes[index]);
			rHistorySet.insert(rParentNodes[index]);
		}
		std::vector<YFile*> rChildren = rParentNodes[index]->getChildren();
		if (rParentNodes[index]->IsRealFolder())
		{
			queryHelper(rChildren, rPredicate, rResult, rHistorySet);
		}
	}
}

void YDiskOperator::copyFIleHelper(YFile *& pSrcFile, YFile *& pDstFile, std::string& szDstName)
{
	if (pSrcFile->IsRealFile())
	{//数据文件
		pDstFile = new YFile;
		pDstFile->setFileData(pSrcFile->getFileData(), pSrcFile->getFileSize());
	}
	else if(pSrcFile->IsRealSymLnk())
	{//符号链接文件
		pDstFile = new YSymlnkFile(((YSymlnkFile*)pSrcFile)->getDstFile());
	}
	else
	{
		pDstFile = new YFile(Y_Folder);
	}
	pDstFile->setName(szDstName);
	pDstFile->setModifyDate(pSrcFile->getModifyDate());
}

void YDiskOperator::saveDataHelper(YFile * pParentNode, std::fstream & rFile, int& nFileCount)
{
	if (pParentNode == nullptr)
	{
		return;
	}
	std::vector<YFile*> rChildren = pParentNode->getChildren();
	for (size_t index = 0; index < rChildren.size();++index)
	{
		YDataSaveFileHead rFileHeader;
		if (rChildren[index]->IsRealSymLnk())
		{
			std::string szFullPath = this->getFullPath(rChildren[index]);
			rFileHeader.nFileType = 2;
			rFileHeader.nFilePathSize = (int)szFullPath.size();
			rFileHeader.nFileDataSize =(int) ((YSymlnkFile*)rChildren[index])->getShowName().size();
			memcpy(rFileHeader.rModifyDate,rChildren[index]->getModifyDate().c_str(), 25);
			rFile.write((char*)(&rFileHeader), sizeof(rFileHeader));
			rFile.write(this->getFullPath(rChildren[index]).c_str(), rFileHeader.nFilePathSize);
			rFile.write(((YSymlnkFile*)rChildren[index])->getShowName().c_str(), rFileHeader.nFileDataSize);
			++nFileCount;
		}
		else 
		{
			if (rChildren[index]->IsRealFile())
			{
				std::string szFullPath = this->getFullPath(rChildren[index]);
				rFileHeader.nFileType = 0;
				rFileHeader.nFileDataSize = rChildren[index]->getFileSize();
				rFileHeader.nFilePathSize = (int)szFullPath.size();
				memcpy(rFileHeader.rModifyDate, rChildren[index]->getModifyDate().c_str(), 25);
				rFile.write((char*)(&rFileHeader), sizeof(rFileHeader));
				rFile.write(this->getFullPath(rChildren[index]).c_str(), this->getFullPath(rChildren[index]).size());
				rFile.write((char*)(rChildren[index]->getFileData()), rChildren[index]->getFileSize());
				++nFileCount;
			}
			else
			{
				std::string szFullPath = this->getFullPath(rChildren[index]);
				rFileHeader.nFileType = 1;
				rFileHeader.nFileDataSize = rChildren[index]->getFileSize();
				rFileHeader.nFilePathSize =(int) szFullPath.size();
				memcpy(rFileHeader.rModifyDate, rChildren[index]->getModifyDate().c_str(), 25);
				rFile.write((char*)(&rFileHeader), sizeof(rFileHeader));
				rFile.write(this->getFullPath(rChildren[index]).c_str(), this->getFullPath(rChildren[index]).size());
				++nFileCount;

				saveDataHelper(rChildren[index], rFile, nFileCount);
			}
		}
	}
}

YErrorCode YDiskOperator::initializeRootDisk(std::vector<char>& rRootArr)
{
	if (rRootArr.empty())
	{
		return TERROR_DISK_ERROR;
	}
	for (size_t index = 0; index < rRootArr.size();++index)
	{
		std::string szRootName;
		szRootName += (rRootArr[index]);
		szRootName += ':';
		YFile* pResultFile = nullptr;
		if (g_pDiskOperator->isRootName(szRootName))
		{
			return TERROR_DISK_ERROR;
		}
		YErrorCode rResult = m_pDisk->createRootNode(szRootName, pResultFile);
		if (Y_OPERAT_SUCCEED != rResult)
		{
			return rResult;
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::initializeFileTree(int32_t nFileCount, std::fstream & rFileStream)
{
	if (0 == nFileCount)
	{
		return Y_OPERAT_SUCCEED;
	}
	int32_t nCount = 1;
	std::vector<char> rBuffer;
	std::vector<LinkFileData> rLnkArr;
	//初始化文件
	for ( ;nCount <= nFileCount && !rFileStream.eof(); ++nCount )
	{
		YDataSaveFileHead rHeader;
		rFileStream.read((char*)&rHeader, sizeof(rHeader));
		if (rHeader.nFileType == Y_SymLnk)
		{//符号链接文件处理
			LinkFileData rLnkData;
			bufferResetByDataSize(rBuffer, rHeader.nFilePathSize);
			rFileStream.read(&rBuffer[0], rHeader.nFilePathSize);
			rLnkData.RealPath = makeStringFromBuffer(rBuffer, rHeader.nFilePathSize);
			bufferResetByDataSize(rBuffer, rHeader.nFileDataSize);
			rFileStream.read(&rBuffer[0], rHeader.nFileDataSize);
			rLnkData.DstPath = makeStringFromBuffer(rBuffer, rHeader.nFileDataSize);
			rHeader.rModifyDate[24] = '\0';
			rLnkData.ModifyDate = rHeader.rModifyDate;
			rLnkArr.push_back(rLnkData);

		}
		else
		{//普通文件文件夹
			YIFile* pFile = nullptr;
			if (rHeader.nFileType == Y_File)
			{
				std::string szFilePath;
				bufferResetByDataSize(rBuffer, rHeader.nFilePathSize);
				rFileStream.read(&rBuffer[0], rHeader.nFilePathSize);
				szFilePath = makeStringFromBuffer(rBuffer, rHeader.nFilePathSize);
				createNewFile(szFilePath, pFile);

				if (nullptr != pFile)
				{
					bufferResetByDataSize(rBuffer, rHeader.nFileDataSize);
					rFileStream.read(&rBuffer[0], rHeader.nFileDataSize);
					((YFile*)pFile)->setFileData((int8_t*)&rBuffer[0], rHeader.nFileDataSize);
				}
				else
				{
					break;
				}
			}
			else
			{
				std::string szFilePath;
				bufferResetByDataSize(rBuffer, rHeader.nFilePathSize);
				rFileStream.read(&rBuffer[0], rHeader.nFilePathSize);
				szFilePath = makeStringFromBuffer(rBuffer, rHeader.nFilePathSize);
				createNewFolder(szFilePath, pFile);
				
			}
			if (nullptr != pFile)
			{
				rHeader.rModifyDate[24] = '\0';
				((YFile*)pFile)->setModifyDate(rHeader.rModifyDate);
			}
			else
			{
				break;
			}
		}
	}

	if (nCount  -1!= nFileCount)
	{
		formatDisk();
		return TERROR_DISK_ERROR;
	}
	YErrorCode rResultCode;
	for (size_t index = 0; index < rLnkArr.size(); index++)
	{
		YIFile* pResult = nullptr;
		rResultCode = createNewLnkFile(rLnkArr[index].RealPath, rLnkArr[index].DstPath, pResult);
		if (rResultCode != Y_OPERAT_SUCCEED || nullptr == pResult)
		{
			formatDisk();
			return TERROR_DISK_ERROR;
		}
		((YFile*)pResult)->setModifyDate(rLnkArr[index].ModifyDate);
	}
	return Y_COPY_SUCCEED;
}

void YDiskOperator::bufferResetByDataSize(std::vector<char>& rBuffer, int size)
{
	if ((int)rBuffer.size() < size)
	{
		rBuffer.resize(size);
	}
}

YErrorCode YDiskOperator::folderMoveHelper(YFile * rSrcRootNode, std::string & szDstPath, std::function<bool(std::string&szPath)>& rPredicate, std::set<std::string>& rHistorySet)
{
	YFile* pDstFileNode = m_pDisk->queryFileNode(szDstPath);
	YFile* pParentNode = m_pDisk->queryFileNode(getParentPath(szDstPath));
	if (nullptr == pParentNode)
	{
		return YERROR_POINTER_NULL;
	}
	if (nullptr == pDstFileNode)
	{//没有重名 直接移动
		std::string szSrcPath = getFullPath(rSrcRootNode);
		YFile* pSrcParent = m_pDisk->queryFileNode(getParentPath(szSrcPath));
		if (nullptr == pSrcParent)
		{
			return YERROR_POINTER_NULL;
		}
		g_pSymMananger->delDstFile(rSrcRootNode);
		m_pDisk->takeNode(pSrcParent, rSrcRootNode);
		m_pDisk->addNode(pParentNode, rSrcRootNode);
		g_pSymMananger->changeLnkDst(pDstFileNode, rSrcRootNode);
	}
	else
	{//存在重名 hebing 
		std::string szSrcPath = getFullPath(rSrcRootNode);
		YFile* pSrcParent = m_pDisk->queryFileNode(getParentPath(szSrcPath));
		if (nullptr == pSrcParent)
		{
			return YERROR_POINTER_NULL;
		}
		if (rPredicate(szSrcPath))
		{
			YFile* pResultParent = nullptr;
			m_pDisk->createFolderFile(pResultParent, "result");
			FolderMoveCoverHelper(rSrcRootNode, pSrcParent, pDstFileNode, pParentNode,rPredicate, pResultParent);
			std::vector<YFile*> rChildren = pResultParent->getChildren();
			if (rChildren.empty())
			{
				return TERROR_DISK_ERROR;
			}
			YFile* pResult = rChildren[0];
			m_pDisk->takeNode(pResultParent, pResult);
			m_pDisk->takeNode(pParentNode, pDstFileNode);
			m_pDisk->addNode(pParentNode,pResult);
			m_pDisk->destroyAllChildFileNode(pDstFileNode);
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::fileMoveHelper(YFile * rSrcRootNode, std::string & szDstPath, std::function<bool(std::string&szPath)>& rPredicate, std::set<std::string>& rHistorySet)
{
	YFile* pDstFileNode = m_pDisk->queryFileNode(szDstPath);
	YFile* pParentNode = m_pDisk->queryFileNode(getParentPath(szDstPath));
	std::string szName = getNameFromFullPath(szDstPath);
	if (nullptr == pParentNode)
	{
		return YERROR_POINTER_NULL;
	}
	if (nullptr == pDstFileNode)
	{//没有重名 直接移动
		std::string szSrcPath = getFullPath(rSrcRootNode);
		YFile* pSrcParent = m_pDisk->queryFileNode(getParentPath(szSrcPath));
		if (nullptr == pSrcParent)
		{
			return YERROR_POINTER_NULL;
		}
		g_pSymMananger->delDstFile(rSrcRootNode);
		m_pDisk->takeNode(pSrcParent, rSrcRootNode);
		m_pDisk->addNode(pParentNode, rSrcRootNode);
	}
	else
	{//存在重名
		std::string szSrcPath = getFullPath(rSrcRootNode);
		YFile* pSrcParent = m_pDisk->queryFileNode(getParentPath(szSrcPath));
		if (nullptr == pSrcParent)
		{
			return YERROR_POINTER_NULL;
		}
		if (rPredicate(szSrcPath))
		{
			g_pSymMananger->delDstFile(rSrcRootNode);
			g_pSymMananger->changeLnkDst(pDstFileNode, rSrcRootNode);
			m_pDisk->takeNode(pParentNode, pDstFileNode);
			m_pDisk->takeNode(pSrcParent, rSrcRootNode);
			m_pDisk->addNode(pParentNode, rSrcRootNode);
			m_pDisk->destroyFileNode(pDstFileNode);
		}
	}
	rSrcRootNode->setName(szName);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDiskOperator::FolderMoveCoverHelper(
	YFile * pSrcRootNode, 
	YFile * pSrcParentNode,
	YFile * pDstRootNode,
	YFile * pDstParentNode, 
	std::function<bool(std::string&szPath)>& rPredicate,
	YFile*& pResultRoot)
{
	if (nullptr == pSrcRootNode || nullptr == pDstParentNode || nullptr == pSrcParentNode)
	{
		return YERROR_POINTER_NULL;
	}
	std::string szSrcPath = getFullPath(pSrcRootNode);
	std::string szNamePart = getNameFromFullPath(szSrcPath);
	if (nullptr == pDstRootNode)
	{
		g_pSymMananger->delDstFile(pSrcRootNode);
		m_pDisk->takeNode(pSrcParentNode, pSrcRootNode);
		m_pDisk->addNode(pResultRoot, pSrcRootNode);
	}
	else
	{
		if (pSrcRootNode->IsRealFolder())
		{
			if (rPredicate(szSrcPath))
			{
				YFile* pCopyFolder = nullptr;
				copyFIleHelper(pSrcRootNode, pCopyFolder, szNamePart);
				m_pDisk->addNode(pResultRoot, pCopyFolder);
				g_pSymMananger->changeLnkDst(pDstRootNode, pCopyFolder);
				std::vector<YFile*> rChildren = pSrcRootNode->getChildren();
				for (size_t index = 0; index < rChildren.size();++index)
				{
					std::string szChildName = getNameFromFullPath(getFullPath(rChildren[index]));
					YIFile* pDstChild = nullptr;
					getChild(pDstRootNode, szChildName, pDstChild);
					FolderMoveCoverHelper(pSrcRootNode, rChildren[index], (YFile*)pDstChild, pDstRootNode, rPredicate, pCopyFolder);
				}
				if (0 == pSrcRootNode->getChildrenCount())
				{
					m_pDisk->takeNode(pSrcParentNode, pSrcRootNode);
					m_pDisk->destroyAllChildFileNode(pSrcRootNode);
				}
			}
		}
		else
		{
			if (rPredicate(szSrcPath))
			{
				m_pDisk->takeNode(pDstParentNode, pDstRootNode);
				m_pDisk->takeNode(pSrcParentNode, pSrcRootNode);
				m_pDisk->addNode(pResultRoot, pSrcRootNode);
				g_pSymMananger->delDstFile(pSrcRootNode);
				g_pSymMananger->changeLnkDst(pDstRootNode, pSrcRootNode);
			}
		}
	}
	
	return Y_OPERAT_SUCCEED;
}

