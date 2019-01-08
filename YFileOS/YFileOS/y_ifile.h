#pragma once
#include <vector>

class YIFile
{
public:
	virtual ~YIFile();
	YIFile();
	//������Ϣ��ýӿ�
	virtual const std::int8_t*		getFileData() = 0;
	virtual const std::uint32_t		getFileSize() = 0;
	virtual const std::string&		getModifyDate() = 0;
	virtual const std::uint32_t		getChildrenCount() = 0;
	virtual const std::string&		getName() = 0;


	//��֤�ļ�����..RealΪʵ�����ͣ�Isxxx�ӿ��п����Ƿ��������ļ�ָ��һ���ļ���
	virtual bool IsFile() = 0;
	virtual bool IsFolder() = 0;
	virtual bool IsRealSymLnk() = 0;
	virtual bool IsRealFolder() = 0;
	virtual bool IsRealFile() = 0;



private:

};