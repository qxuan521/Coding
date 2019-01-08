#pragma once
#include "file_info.h"

class LinkFileInfo : public FileInfo
{
public: 
	/*使用时不要传空的FileInfo*链接文件不允许没有指向所有无论何时FileInfo*不允许为空 */
	LinkFileInfo(std::string& FilePathStr, FileInfo* ParentPtr, FileInfo* DstFilePtr);
	~LinkFileInfo();

	std::vector<FileInfo*>&  GetChildrenList();
	/*获得孩子个数*/
	int						GetChildrenSize();
	/*检查是否有与当前子节点重名，重名返回true*/
	bool					CheckSameNameWithChildren(const std::string& FileNameStr);
	void					AddChild(FileInfo* ChildPtr);
	void					DelChild(FileInfo* ChildPtr);
	/*获得文件内容大小*/
	int						GetFileSize();
	/*获得文件数据,warning：可能返回空使用需要非空验证*/
	char*					GetFileData();
	/*获得文件路径*/
	std::string				GetFilePath();
	/*获得文件类型，如果是链接文件则是目标文件的类型*/
	FileType				GetType()const;
	/*获得父节点，如果是链接文件是目标的父节点*/
	FileInfo*				GetParent();
	FileInfo*				GetDstFile();
	void					SetDstFile(FileInfo* DstFile);
private:
	FileInfo*		m_DstFilePtr;
};