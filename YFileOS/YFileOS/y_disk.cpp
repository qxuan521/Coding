#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"
#include "y_symlnk_file.h"

YDisk::YDisk()
{
}

YDisk::~YDisk()
{
}

YErrorCode YDisk::addNode(YIFile * parent, YIFile * newNode)
{
	if (nullptr == parent || nullptr == newNode)
		return YERROR_POINTER_NULL;
	((YFile*)parent)->addChild((YFile*)newNode);
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

YErrorCode YDisk::createDataFile(YIFile *& newFileNode)
{
	newFileNode = new YFile(YFileType::Y_File);
	if (nullptr == newFileNode)
		return YERROR_POINTER_NULL;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createSymlnkFile(YIFile *& newFileNode, YIFile* pDstFile)
{
	newFileNode = new YSymlnkFile((YFile*)pDstFile);
	if (nullptr == newFileNode)
		return YERROR_POINTER_NULL;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::createFolderFile(YIFile *& newFileNode)
{
	newFileNode = new YFile(YFileType::Y_Folder);
	if (nullptr == newFileNode)
		return YERROR_POINTER_NULL;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::formatDisk()
{
	return YErrorCode();
}

YErrorCode YDisk::destroyFileNode(YIFile *& beDestroyFile)
{
	if (nullptr == beDestroyFile)
		return YERROR_POINTER_NULL;
	delete beDestroyFile;
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDisk::destroyAllFileNode()
{
	return YErrorCode();
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
