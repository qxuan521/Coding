#pragma once
#include "y_file.h"


class YSymlnkFile : public YFile
{
public:
	YSymlnkFile(YFile* pDstFile);
	~YSymlnkFile();
	virtual bool IsFile();
	virtual bool IsFolder();

	virtual const std::int8_t*		getFileData();
	virtual const std::uint32_t		getFileSize();
	virtual const std::uint32_t		getChildrenCount();
	virtual std::vector<YFile*>&	getChildren();
	virtual const std::string&		getShowName();
private:
	YFile*			m_pDstFile;
	std::string		m_szShowName;
};
