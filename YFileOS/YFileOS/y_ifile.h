#pragma once
#include <vector>

class YIFile
{
public:
	virtual ~YIFile();
	YIFile();
	//基本信息获得接口
	virtual const std::int8_t*		getFileData() = 0;
	virtual const std::uint32_t		getFileSize() = 0;
	virtual const std::string&		getModifyDate() = 0;
	virtual const std::uint32_t		getChildrenCount() = 0;
	virtual const std::string&		getName() = 0;


	//验证文件类型..Real为实际类型，Isxxx接口有可能是符号链接文件指向一个文件夹
	virtual bool IsFile() = 0;
	virtual bool IsFolder() = 0;
	virtual bool IsRealSymLnk() = 0;
	virtual bool IsRealFolder() = 0;
	virtual bool IsRealFile() = 0;



private:

};