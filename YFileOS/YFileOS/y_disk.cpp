#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"
#include "y_symlnk_file.h"
#include <time.h>

std::unique_ptr<YDisk> g_pDiskPtr = std::make_unique<YDisk>();

YDisk::YDisk()
{
}

YDisk::~YDisk()
{
	destroyAllFileNode();
}

YErrorCode YDisk::addNode(YIFile * parent, YIFile * newNode)
{
	if (nullptr == parent || nullptr == newNode)
		return YERROR_POINTER_NULL;
	((YFile*)parent)->addChild((YFile*)newNode);
	((YFile*)newNode)->setParent((YFile*)parent);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::takeNode(YIFile * parent, YIFile * beTokenNode)
{
	if (nullptr == parent || nullptr == beTokenNode)
		return YERROR_POINTER_NULL;
	std::vector<YFile*>& Children = ((YFile*)parent)->getChildren();
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

YErrorCode YDisk::createDataFile(YIFile *& newFileNode ,const std::string& szName)
{
	YFile* newFile = new YFile(YFileType::Y_File);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createSymlnkFile(YIFile *& newFileNode, YIFile* pDstFile, const std::string& szName)
{
	YFile* newFile = new YSymlnkFile((YFile*)pDstFile);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createFolderFile(YIFile *& newFileNode,const std::string& szName)
{
	YFile* newFile = new YFile(YFileType::Y_Folder);
	if (nullptr == newFile)
		return YERROR_POINTER_NULL;
	newFile->setName(szName);
	newFile->setModifyDate(getDate());
	newFileNode = newFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::getFileFullPath(YIFile * pFile, std::string & fullPath)
{
	if (pFile == nullptr)
		return YERROR_POINTER_NULL;
	fullPathHelper((YFile*)pFile, fullPath);
}

YErrorCode YDisk::formatDisk()
{
	if (Y_OPERAT_FAILD == destroyAllFileNode())
		return Y_OPERAT_FAILD;
	m_DataRoot = new YFile(Y_Folder);
	m_DataRoot->setModifyDate(getDate());
	m_DataRoot->setName("root");
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::destroyFileNode(YIFile *& beDestroyFile)
{
	if (nullptr == beDestroyFile)
		return YERROR_POINTER_NULL;
	delete beDestroyFile;
	return Y_OPERAT_SUCCEED;
}

std::string & YDisk::getRootName()
{
	std::string curPath("");
	if (nullptr == m_DataRoot)
		return curPath;
	curPath = m_DataRoot->getName();
	return curPath;
}

YErrorCode YDisk::destroyAllFileNode()
{
	destroyHelper(m_DataRoot);
	if (nullptr != m_DataRoot)
		return Y_OPERAT_FAILD;
	return Y_OPERAT_SUCCEED;
}

YIFile* YDisk::queryFileNode(const std::string & szPath)
{
	if (szPath == m_DataRoot->getName())
		return m_DataRoot;
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath,'/');
	if (rSplitResult.empty())
		return nullptr;
	return queryFileHelper(m_DataRoot, rSplitResult, 1);
}

YErrorCode YDisk::clear()
{
	return YErrorCode();
}

YErrorCode YDisk::renameFile(YIFile * pFileNode, const std::string & szName)
{
	if (nullptr == pFileNode)
		return YERROR_POINTER_NULL;
	((YFile*)pFileNode)->setName(szName);
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
		if (!rChildren.empty());
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
	if (nullptr == pFile->getParent())
	{
		subPath.append(pFile->getName());
		return;
	}
	fullPathHelper(pFile->getParent(), subPath);
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
