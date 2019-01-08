#pragma once
#include "y_ifile.h"


//brief
//�ļ��� ��ʾ��ͨ�����ļ����ļ����ļ�
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
	//���ӿ�
	virtual const std::int8_t*		getFileData();
	virtual const std::uint32_t		getFileSize();
	virtual const std::string&		getModifyDate();
	virtual const std::uint32_t		getChildrenCount();
	virtual std::vector<YFile*>&	getChildren();
	virtual const std::string&		getName();
	virtual const std::string&		getShowName();
	//д�ӿ�
	void	setFileData(int8_t* data,uint32_t size);
	void	setModifyDate(const std::string& modifyDate);
	void	addChild(YFile* child);
	void	delChild(YFile* child);
	void	setName(const std::string& name);


	//��֤�ļ�����..RealΪʵ�����ͣ�Isxxx�ӿ��п����Ƿ��������ļ�ָ��һ���ļ���
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
};

