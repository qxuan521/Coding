#pragma once
#include "model_def.h"
class FileInfo
{
public:
	FileInfo(std::string FilePathStr, FileInfo* ParentPtr, FileType eFileType);
	virtual ~FileInfo();
	/*获得孩子列表*/
	virtual std::vector<FileInfo*>&  GetChildrenList();
	/*获得孩子个数*/
	virtual int						GetChildrenSize();
	/*检查是否有与当前子节点重名，重名返回true*/
	virtual bool					CheckSameNameWithChildren(const std::string& FileNameStr);
	virtual void					AddChild(FileInfo* ChildPtr);
	virtual void					DelChild(FileInfo* ChildPtr);
	/*获得文件内容大小*/
	long					GetFileSize();
	/*获得文件数据,worning：可能返回空使用需要非空验证*/
	char*					GetFileData();
	/*设置文件数据*/
	void					SetFileData(char* FileData, long DataSize);
	/*设置父节点*/
	void					SetParent(FileInfo* ParentPtr);
	/*获得父节点，如果是链接文件是目标的父节点*/
	virtual FileInfo*				GetParent();
	/*获得父节点，无论是不是链接文件都是当前文件的父节点*/
	virtual FileInfo*				GetRealParent();
	/*获得文件类型，如果是链接文件则是目标文件的类型*/
	virtual FileType				GetType()const;
	/*获得文件类型，无论是不是链接文件都是当前文件的真实类型*/
	virtual FileType				GetRealType()const;

	//文件名文件路径
	virtual const std::string&			GetFileName();
	virtual void						SetFileName(std::string& FileNameStr);
	virtual std::string					GetFilePath();
	virtual std::string&				GetRealPath();
	//文件名end

	//时间相关
	const std::string&		GetModifyDate();
	void					SetModifyDate(const std::string& ModifyDateStr);
	const std::string&		GetCreateDate()const;
	void					SetCreateDate(const std::string& CreateDateStr);

	int						GetSize();
	//时间相关 end
private:
	void					SetFilePath(std::string& FilePath);
	//更新文件路径属性  当更改名字 更改parent的时候都应对文件路径进行更新
	void					UpdateFilePath();
private:
	std::vector<FileInfo*>					m_ChildrenList;		//孩子列表
	FileInfo*								m_ParentPtr;		//父节点指针
	std::string								m_CreateDate;		//创建时间
	std::string								m_ModifyDate;		//最后修改时间
	std::string								m_FileName;			//文件名
	std::string								m_FilePath;			//文件全路径
	long									m_FileSize;			//文件数据大小
	const FileType							m_FileType;			//文件类型
	char*									m_FileData;			//文件数据
};
