#include "y_disk.h"
#include "y_tool.h"
#include "y_file.h"


YDisk::YDisk()
{
}

YDisk::~YDisk()
{
}

YErrorCode YDisk::addNode(YIFile * parent, YIFile * newNode)
{
	return YErrorCode();
}

YErrorCode YDisk::takeNode(YIFile * parent, YIFile * beTokenNode)
{
	return YErrorCode();
}

YErrorCode YDisk::createDataFile(YIFile *& newFileNode)
{
	return YErrorCode();
}

YErrorCode YDisk::createSymlnkFile(YIFile *& newFileNode)
{
	return YErrorCode();
}

YErrorCode YDisk::createFolderFile(YIFile *& newFileNode)
{
	return YErrorCode();
}

YErrorCode YDisk::destroyFileNode(YIFile *& beDestroyFile)
{
	return YErrorCode();
}

YErrorCode YDisk::destroyAllFileNode()
{
	return YErrorCode();
}

YIFile* YDisk::queryFileNode(const std::string & szPath)
{
	std::vector<std::string> rSplitResult = splitStrByCharacter(szPath,'/');
	if (rSplitResult.empty())
		return nullptr;
	YIFile* pResult = (YIFile*)m_DataRoot;
	for (size_t nIndex = 0; nIndex < rSplitResult.size(); ++nIndex)
	{

	}
}

YErrorCode YDisk::clear()
{
	return YErrorCode();
}

YFile * YDisk::queryFileHelper(YFile * pParent, std::vector<std::string>& rNameArr, size_t nPathindex)
{
	if(0 == pParent->getChildrenCount())
	return nullptr;
}
