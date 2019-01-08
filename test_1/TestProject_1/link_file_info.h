#pragma once
#include "file_info.h"

class LinkFileInfo : public FileInfo
{
public: 
	/*ʹ��ʱ��Ҫ���յ�FileInfo*�����ļ�������û��ָ���������ۺ�ʱFileInfo*������Ϊ�� */
	LinkFileInfo(std::string& FilePathStr, FileInfo* ParentPtr, FileInfo* DstFilePtr);
	~LinkFileInfo();

	std::vector<FileInfo*>&  GetChildrenList();
	/*��ú��Ӹ���*/
	int						GetChildrenSize();
	/*����Ƿ����뵱ǰ�ӽڵ���������������true*/
	bool					CheckSameNameWithChildren(const std::string& FileNameStr);
	void					AddChild(FileInfo* ChildPtr);
	void					DelChild(FileInfo* ChildPtr);
	/*����ļ����ݴ�С*/
	int						GetFileSize();
	/*����ļ�����,warning�����ܷ��ؿ�ʹ����Ҫ�ǿ���֤*/
	char*					GetFileData();
	/*����ļ�·��*/
	std::string				GetFilePath();
	/*����ļ����ͣ�����������ļ�����Ŀ���ļ�������*/
	FileType				GetType()const;
	/*��ø��ڵ㣬����������ļ���Ŀ��ĸ��ڵ�*/
	FileInfo*				GetParent();
	FileInfo*				GetDstFile();
	void					SetDstFile(FileInfo* DstFile);
private:
	FileInfo*		m_DstFilePtr;
};