#include "y_symlnk_file.h"
#include "y_disk_operator.h"

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

const std::string  YSymlnkFile::getShowName()
{
	std::string szDstFullPath;
	if (nullptr != m_pDstFile)
	{
		szDstFullPath = g_pDiskOperator->getFullPath(m_pDstFile);
	}
	return szDstFullPath;
}

const std::uint32_t YSymlnkFile::getChildrenFolderCount()
{
	return m_pDstFile->getChildrenFolderCount();
}

const std::uint32_t YSymlnkFile::getChildrenFileCount()
{
	return m_pDstFile->getChildrenFileCount();
}

bool YSymlnkFile::isValid()
{
	return m_pDstFile != nullptr;
}

void YSymlnkFile::setDstFile(YFile * pDstFile)
{
	m_pDstFile = pDstFile;
}

YFile * YSymlnkFile::getDstFile()
{
	return m_pDstFile;
}
