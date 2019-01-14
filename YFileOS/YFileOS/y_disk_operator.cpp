#include "y_disk_operator.h"
#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"
#include "y_symlnk_file.h"
YDiskOperator* g_pDiskOperator = new YDiskOperator;

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
	YFile* pResultFile = nullptr;
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
			m_pDisk->takeNode(pDstParent, pDstFile);
			m_pDisk->destroyFileNode(pDstFile);
			copyFIleHelper(pSrcFile, pDstFile, szDstName);
		}
	}
	return Y_COPY_SUCCEED;
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

void YDiskOperator::copyFIleHelper(YFile *& pSrcFile, YFile *& pDstFile, std::string& szDstName)
{
	if (pSrcFile->IsRealFile())
	{//数据文件
		pDstFile = new YFile;
		pDstFile->setFileData(pSrcFile->getFileData(), pSrcFile->getFileSize());
	}
	else
	{//符号链接文件
		pDstFile = new YSymlnkFile(((YSymlnkFile*)pSrcFile)->getDstFile());
	}
	pDstFile->setName(szDstName);
	pDstFile->setModifyDate(pSrcFile->getModifyDate());
}

