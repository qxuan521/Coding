#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"
#include "y_symlnk_file.h"
#include <time.h>


YDisk::YDisk()
{
	createRootNode("c:");
}

YDisk::~YDisk()
{
	destroyAllFileNode();
}

YErrorCode YDisk::addNode(YFile * parent, YFile * newNode)
{
	if (nullptr == parent || nullptr == newNode)
		return YERROR_POINTER_NULL;
	parent->addChild(newNode);
	newNode->setParent(parent);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::takeNode(YFile * parent, YFile * beTokenNode)
{
	if (nullptr == parent || nullptr == beTokenNode)
		return YERROR_POINTER_NULL;
	std::vector<YFile*>& Children = parent->getChildren();
	for (std::vector<YFile*>::iterator rIter = Children.begin(); rIter != Children.end();)
	{
		if (beTokenNode == *rIter)
		{
			Children.erase(rIter);
			return Y_OPERAT_SUCCEED;
		}
		else
		{
			++rIter;
		}
	}
	return YERROR_NO_THIS_CHILD;
}

YErrorCode YDisk::createDataFile(YFile *& newFileNode ,const std::string& szName)
{
	YFile* newFile = new YFile(YFileType::Y_File);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createSymlnkFile(YFile *& newFileNode, YFile* pDstFile, const std::string& szName)
{
	YFile* newFile = new YSymlnkFile(pDstFile);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createFolderFile(YFile *& newFileNode,const std::string& szName)
{
	YFile* newFile = new YFile(YFileType::Y_Folder);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::getFileFullPath(YFile * pFile, std::string & fullPath)
{
	if (pFile == nullptr)
		return YERROR_POINTER_NULL;
	fullPathHelper((YFile*)pFile, fullPath);
	return Y_OPERAT_SUCCEED;
}

std::vector<YFile*>& YDisk::getRootArr()
{
	return m_rRootArr;
}

bool YDisk::isRootName(const std::string & szName)
{
	for (size_t index = 0; index < m_rRootArr.size();++index)
	{
		if (m_rRootArr[index]->getName() == szName)
		{
			return true;
		}
	}
	return false;
}

YErrorCode YDisk::formatDisk()
{
	if (Y_OPERAT_FAILD == destroyAllFileNode())
		return Y_OPERAT_FAILD;
	createRootNode("c:");
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::destroyFileNode(YFile *& beDestroYFile)
{
	if (nullptr == beDestroYFile)
		return YERROR_POINTER_NULL;
	delete beDestroYFile;
	return Y_OPERAT_SUCCEED;
}

//std::string & YDisk::getRootName()
//{
//	std::string curPath("");
//	if (nullptr == m_DataRoot)
//		return curPath;
//	curPath = m_DataRoot->getName();
//	return curPath;
//}

YErrorCode YDisk::destroyAllFileNode()
{
	for (size_t index = 0; index < m_rRootArr.size();++index)
	{
		destroyHelper(m_rRootArr[index]);
		if (nullptr != m_rRootArr[index])
			return Y_OPERAT_FAILD;
	}
	return Y_OPERAT_SUCCEED;
}

YFile* YDisk::queryFileNode(const std::string & szPath)
{
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath, '/');
	if (rSplitResult.empty())
		return nullptr;
	std::string& szRootName = rSplitResult[0];
	YFile* pRoot = nullptr;
	for (size_t index = 0; index < m_rRootArr.size(); ++index)
	{
		if (szPath == m_rRootArr[index]->getName())
		{
			return m_rRootArr[index];
		}
		else
		{
			if (szRootName == m_rRootArr[index]->getName())
			{
				pRoot = m_rRootArr[index];
			}
		}
	}
	if (nullptr == pRoot)
	{
		return nullptr;
	}
	return queryFileHelper(pRoot, rSplitResult, 1);
}

YErrorCode YDisk::clear()
{
	return YErrorCode();
}
//没有名字非空验证
YErrorCode YDisk::createRootNode(const std::string & szRootName)
{
	YFile* pRoot = new YFile(Y_Folder);
	m_rRootArr.push_back(pRoot);
	pRoot->setModifyDate(getDate());
	pRoot->setName(szRootName);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::renameFile(YFile * pFileNode, const std::string & szName)
{
	if (nullptr == pFileNode)
		return YERROR_POINTER_NULL;
	pFileNode->setName(szName);
	return Y_OPERAT_SUCCEED;
}

YFile * YDisk::queryFileHelper(YFile * pParent, std::vector<std::string>& rNameArr, size_t nPathindex)
{
	if(0 == pParent->getChildrenCount() ||nPathindex +1 >=rNameArr.size())
		return nullptr;
	std::vector<YFile*>& rChildren = pParent->getChildren();
	for (size_t index = 0; index < rChildren.size();++index)
	{
		if (rNameArr[nPathindex] == rChildren[index]->getName())
		{
			return queryFileHelper(rChildren[index], rNameArr,++nPathindex);
		}
	}
	return nullptr;
}

void YDisk::destroyHelper(YFile *& pFile)
{
	if (nullptr == pFile)
		return;
	if (pFile->IsRealFolder())
	{
		std::vector<YFile*> rChildren = pFile->getChildren();
		if (!rChildren.empty())
		{
			for (size_t index = 0; index < rChildren.size(); ++index)
			{
				destroyHelper(rChildren[index]);
			}
		}
	}
	delete pFile;
	pFile = nullptr;
}

void YDisk::fullPathHelper(YFile * pFile, std::string & subPath)
{
	if (nullptr == pFile->getUseParent())
	{
		subPath.append(pFile->getName());
		return;
	}
	fullPathHelper(pFile->getUseParent(), subPath);
	subPath.append(pFile->getName());
}

std::string YDisk::getDate()
{
	time_t timep;
	time(&timep);
	char stTmp[32];
	stTmp[24] = '\0';
	tm TempTime;
	gmtime_s(&TempTime, &timep);
	asctime_s(stTmp, &TempTime);
	return std::string(stTmp);
}
