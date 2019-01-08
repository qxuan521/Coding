#include "y_symlnk_file.h"


YSymlnkFile::YSymlnkFile(YFile* pDstFile)
	: YFile(Y_SymLnk)
	, m_pDstFile(pDstFile)
{
	
}

YSymlnkFile::~YSymlnkFile()
{
}

bool YSymlnkFile::IsFile()
{
	return m_pDstFile->IsFile();
}

bool YSymlnkFile::IsFolder()
{
	return m_pDstFile->IsFolder();
}

const std::int8_t * YSymlnkFile::getFileData()
{
	return m_pDstFile->getFileData();
}

const std::uint32_t YSymlnkFile::getFileSize()
{
	return std::uint32_t();
}


const std::uint32_t YSymlnkFile::getChildrenCount()
{
	return m_pDstFile->getChildrenCount();
}

std::vector<YFile*>& YSymlnkFile::getChildren()
{
	return m_pDstFile->getChildren();
}

const std::string & YSymlnkFile::getShowName()
{
	return m_szShowName = getName() + "[" + m_pDstFile->getName() + "]";
}
