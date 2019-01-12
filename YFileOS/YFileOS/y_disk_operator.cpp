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

YErrorCode YDiskOperator::queryFolderNode(const std::string & szPath, std::vector<YIFile*>& rResultArr)
{
	if (szPath.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	std::regex rPathRegex = makeRegexByPath(szPath);
	std::function<bool(YFile*)> rMatchFunc([=](YFile* pFile)->bool
	{
		std::string szNodePath;
		if (Y_OPERAT_SUCCEED != m_pDisk->getFileFullPath(pFile, szNodePath))
		{
			return  false;
		}
		if (szNodePath.empty())
		{
			return false;
		}
		return (pFile->IsFolder() && std::regex_match(szNodePath, rPathRegex));
	});

	std::vector<YFile*> rRootArr = m_pDisk->getRootArr();
	if (rRootArr.empty())
	{
		return TERROR_DISK_ERROR;
	}
	std::set<YFile*> rMatchHistory;
	for (size_t index = 0; index < rRootArr.size(); ++index )
	{
		queryHelper(rRootArr[index], rMatchFunc, rResultArr, rMatchHistory);
	}
	return Y_OPERAT_SUCCEED;
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

void YDiskOperator::setCurWorkingPath(const std::string & szCurWokingPath)
{
	m_szCurWorkingPath = szCurWokingPath;
}


bool YDiskOperator::isRootName(const std::string& szName)
{
	return m_pDisk->isRootName(szName);
}

bool YDiskOperator::isPathExist(const std::string& szPath)
{
	return nullptr != m_pDisk->queryFileNode(szPath);
}

std::regex YDiskOperator::makeRegexByPath(const std::string & szPath)
{
	std::string szRegexStr;
	for (size_t index = 0;index < szPath.size();++index)
	{
		if ('*' == szPath[index])
		{
			szRegexStr.append("[\\w_\\.]*");
		}
		else if ('?' == szPath[index])
		{
			szRegexStr.append("[\\w_\\.]");
		}
		else
		{
			szRegexStr+= szPath[index];
		}
	}
	return std::regex(szRegexStr);
}

void YDiskOperator::queryHelper(YFile * pNode, std::function<bool(YFile*)>& rPredicate, std::vector<YIFile*>& rResult, std::set<YFile*>& rHistorySet)
{
	if (nullptr == pNode)
		return;
	rHistorySet.insert(pNode);
	std::vector<YFile*> rChildren = pNode->getChildren();
	if (rPredicate(pNode))
	{
		rResult.push_back(pNode);
	}
	for (size_t index = 0; index < rChildren.size();++index)
	{
		if (rHistorySet.find(rChildren[index]) != rHistorySet.end())
		{
			queryHelper(rChildren[index], rPredicate, rResult, rHistorySet);
		}
	}
}


