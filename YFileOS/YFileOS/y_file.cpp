#include "y_file.h"
#include "y_link_manager.h"


//------YFile----------------------------------------------------------------//
YFile::YFile(YFileType rFileType)
	: YIFile()
	, m_eFileType(rFileType)
	, m_nFileDataSize(0)
	, m_szModifyDate("")
	, m_pParent(nullptr)
	, m_nChildFileCount(0)
	, m_nChildFolderCount(0)
{
}

YFile::~YFile()
{
	if (!IsRealSymLnk())
	{
		g_pSymMananger->delDstFile(this);
	}
}

const std::int8_t * YFile::getFileData()
{
	if (m_pData.empty())
		return nullptr;
	return &m_pData[0];
}

const std::uint32_t YFile::getFileSize()
{
	return (std::uint32_t)m_pData.size();
}

const std::string & YFile::getModifyDate()
{
	return m_szModifyDate;
}

const std::uint32_t YFile::getChildrenCount()
{
	return (std::uint32_t)m_rChildrenArr.size();
}

std::vector<YFile*>& YFile::getChildren()
{
	return m_rChildrenArr;
}

const std::string & YFile::getName()
{
	return m_szName;
}

const std::string  YFile::getShowName()
{
	return getName();
}

YFile * YFile::getUseParent()
{
	return m_pParent;
}

YIFile * YFile::getParent()
{
	return m_pParent;
}

bool YFile::isValid()
{
	return true;
}

const std::uint32_t YFile::getChildrenFolderCount()
{
	return m_nChildFolderCount;
}

const std::uint32_t YFile::getChildrenFileCount()
{
	return m_nChildFileCount;
}

void YFile::setFileData(const int8_t * data, const uint32_t size)
{
	if (nullptr == data/* || 0 <= size*/)
		return;
	if (size > 0)
	{
		m_pData.resize(size);
		memcpy(&m_pData[0], data, size);
	}
}

void YFile::setModifyDate(const std::string & modifyDate)
{
	m_szModifyDate = modifyDate;
}

void YFile::setModifyDate(char (*modifyDate)[25])
{
	m_szModifyDate.clear();
	for (size_t index = 0; index < 25;++index)
	{
		m_szModifyDate += modifyDate[index];
	}
}

void YFile::addChild(YFile * child)
{
	if (nullptr == child)
		return;
	m_rChildrenArr.push_back(child);
	if (child->IsRealFolder())
	{
		++m_nChildFolderCount;
	}
	else
	{
		++m_nChildFileCount;
	}
}

void YFile::delChild(YFile * child)
{
	if (nullptr == child)
		return;
	for (auto rIter = m_rChildrenArr.begin(); rIter != m_rChildrenArr.end();)
	{
		if (child == *rIter)
		{
			if ((*rIter)->IsRealFolder())
			{
				--m_nChildFolderCount;
			}
			else
			{
				--m_nChildFileCount;
			}
			rIter = m_rChildrenArr.erase(rIter);
		}
		else
		{ 
			++rIter;
		}
	}
}

void YFile::setName(const std::string & name)
{
	if (name.empty())
		return;
	m_szName = name;
}

void YFile::setParent(YFile * pParent)
{
	m_pParent = pParent;
}

bool YFile::IsFile()
{
	return Y_File == m_eFileType;
}

bool YFile::IsFolder()
{
	return Y_Folder == m_eFileType;
}

bool YFile::IsRealSymLnk()
{
	return Y_SymLnk == m_eFileType;
} 

bool YFile::IsRealFolder()
{
	return Y_Folder == m_eFileType;
}

bool YFile::IsRealFile()
{
	return Y_File == m_eFileType;
}
