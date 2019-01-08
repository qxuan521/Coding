#pragma once
#include "model_def.h"
class FileInfo
{
public:
	FileInfo(std::string FilePathStr, FileInfo* ParentPtr, FileType eFileType);
	virtual ~FileInfo();
	/*��ú����б�*/
	virtual std::vector<FileInfo*>&  GetChildrenList();
	/*��ú��Ӹ���*/
	virtual int						GetChildrenSize();
	/*����Ƿ����뵱ǰ�ӽڵ���������������true*/
	virtual bool					CheckSameNameWithChildren(const std::string& FileNameStr);
	virtual void					AddChild(FileInfo* ChildPtr);
	virtual void					DelChild(FileInfo* ChildPtr);
	/*����ļ����ݴ�С*/
	long					GetFileSize();
	/*����ļ�����,worning�����ܷ��ؿ�ʹ����Ҫ�ǿ���֤*/
	char*					GetFileData();
	/*�����ļ�����*/
	void					SetFileData(char* FileData, long DataSize);
	/*���ø��ڵ�*/
	void					SetParent(FileInfo* ParentPtr);
	/*��ø��ڵ㣬����������ļ���Ŀ��ĸ��ڵ�*/
	virtual FileInfo*				GetParent();
	/*��ø��ڵ㣬�����ǲ��������ļ����ǵ�ǰ�ļ��ĸ��ڵ�*/
	virtual FileInfo*				GetRealParent();
	/*����ļ����ͣ�����������ļ�����Ŀ���ļ�������*/
	virtual FileType				GetType()const;
	/*����ļ����ͣ������ǲ��������ļ����ǵ�ǰ�ļ�����ʵ����*/
	virtual FileType				GetRealType()const;

	//�ļ����ļ�·��
	virtual const std::string&			GetFileName();
	virtual void						SetFileName(std::string& FileNameStr);
	virtual std::string					GetFilePath();
	virtual std::string&				GetRealPath();
	//�ļ���end

	//ʱ�����
	const std::string&		GetModifyDate();
	void					SetModifyDate(const std::string& ModifyDateStr);
	const std::string&		GetCreateDate()const;
	void					SetCreateDate(const std::string& CreateDateStr);

	int						GetSize();
	//ʱ����� end
private:
	void					SetFilePath(std::string& FilePath);
	//�����ļ�·������  ���������� ����parent��ʱ��Ӧ���ļ�·�����и���
	void					UpdateFilePath();
private:
	std::vector<FileInfo*>					m_ChildrenList;		//�����б�
	FileInfo*								m_ParentPtr;		//���ڵ�ָ��
	std::string								m_CreateDate;		//����ʱ��
	std::string								m_ModifyDate;		//����޸�ʱ��
	std::string								m_FileName;			//�ļ���
	std::string								m_FilePath;			//�ļ�ȫ·��
	long									m_FileSize;			//�ļ����ݴ�С
	const FileType							m_FileType;			//�ļ�����
	char*									m_FileData;			//�ļ�����
};
