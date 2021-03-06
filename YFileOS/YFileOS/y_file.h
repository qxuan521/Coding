#pragma once
#include "y_ifile.h"


//brief
//文件类 表示普通数据文件、文件夹文件
enum YFileType
{
	Y_File,
	Y_Folder,
	Y_SymLnk
};

class YFile : public YIFile
{

public:
	YFile(YFileType rFileType = Y_File );
	virtual ~YFile();
	//父类 读接口 实现
	virtual const std::int8_t*		getFileData();
	virtual const std::uint32_t		getFileSize();
	virtual const std::string&		getModifyDate();
	virtual const std::uint32_t		getChildrenCount();
	virtual std::vector<YFile*>&	getChildren();
	virtual const std::string&		getName();
	virtual const std::string		getShowName();
	virtual	YFile*					getUseParent();
	virtual YIFile*					getParent();
	virtual bool					isValid();

	virtual const std::uint32_t		getChildrenFolderCount();
	virtual const std::uint32_t		getChildrenFileCount();
	//写接口
	void	setFileData(const int8_t* data, const uint32_t size);
	void	setModifyDate(const std::string& modifyDate);
	void	addChild(YFile* child);
	void	delChild(YFile* child);
	void	setName(const std::string& name);
	void	setParent(YFile* pParent);

	void	setModifyDate(char(*modifyDate)[25]);

	//验证文件类型..Real为实际类型，Isxxx接口有可能是符号链接文件指向一个文件夹
	virtual bool IsFile();
	virtual bool IsFolder();
	virtual bool IsRealSymLnk();
	virtual bool IsRealFolder();
	virtual bool IsRealFile();



private:
	std::vector<int8_t>			m_pData;
	std::string					m_szName;
	std::string					m_szModifyDate;
	const YFileType				m_eFileType;
	std::uint32_t				m_nFileDataSize;
	std::vector<YFile*>		    m_rChildrenArr;
	YFile*						m_pParent;
	std::uint32_t				m_nChildFolderCount;
	std::uint32_t				m_nChildFileCount;
};

