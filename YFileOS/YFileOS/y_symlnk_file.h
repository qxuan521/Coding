#pragma once
#include "y_file.h"


class YSymlnkFile : public YFile
{
public:
	YSymlnkFile(YFile* pDstFile);
	virtual ~YSymlnkFile();
	virtual bool IsFile();
	virtual bool IsFolder();

	virtual const std::int8_t*		getFileData();
	virtual const std::uint32_t		getFileSize();
	virtual const std::uint32_t		getChildrenCount();
	virtual std::vector<YFile*>&	getChildren();
	virtual const std::string		getShowName();
	virtual const std::uint32_t		getChildrenFolderCount();
	virtual const std::uint32_t		getChildrenFileCount();
	virtual bool					isValid();
	void	setDstFile(YFile* pDstFile);
	YFile*	getDstFile();
private:
	YFile*			m_pDstFile;
	std::string		m_szShowName;
};
