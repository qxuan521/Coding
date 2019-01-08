#include "y_file.h"



//------YFile----------------------------------------------------------------//
YFile::YFile(YFileType rFileType)
	: YIFile()
	, m_eFileType(rFileType)
	, m_nFileDataSize(0)
	, m_szModifyDate("")
{
}

YFile::~YFile()
{
}

const std::int8_t * YFile::getFileData()
{
	if (m_pData.empty())
		return nullptr;
	return &m_pData[0];
}

const std::uint32_t YFile::getFileSize()
{
	return m_nFileDataSize;
}

const std::string & YFile::getModifyDate()
{
	return m_szModifyDate
}

const std::uint32_t YFile::getChildrenCount()
{
	return m_rChildrenArr.size();
}

std::vector<YFile*>& YFile::getChildren()
{
	return m_rChildrenArr;
}

const std::string & YFile::getName()
{
	return m_szName;
}

const std::string & YFile::getShowName()
{
	return getName();
}

void YFile::setFileData(int8_t * data, uint32_t size)
{
	if (nullptr == data || 0 <= size)
		return;
	m_pData.resize(size);
	memcpy(&m_pData[0], data, size);
}

void YFile::setModifyDate(const std::string & modifyDate)
{
	m_szModifyDate = modifyDate;
}

void YFile::addChild(YFile * child)
{
	if (nullptr == child)
		return;
	m_rChildrenArr.push_back(child);
}

void YFile::delChild(YFile * child)
{
	if (nullptr == child)
		return;
	for (auto rIter = m_rChildrenArr.begin(); rIter != m_rChildrenArr.end();)
	{
		if (child == *rIter)
		{
			m_rChildrenArr.erase(rIter);
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
