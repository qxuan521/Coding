#pragma once
#include <string>
#include <vector>


class FileInfo;
class LinkFileInfo;

enum FileType
{
	DATA_FILE = 0,		//普通数据文件
	FOLDER_FILE = 1,	//文件夹文件
	LINK_FILE = 2		//符号链接文件
};
enum ErrorCode
{
	NULL_CODE = 0,
	SUCCESS_CODE = 1,
	ERROR_DST_PATH_CODE = 2,
	ERROR_DATA_DISK_UNKNOW = 3,
	ERROR_SRC_PATH_CODE = 4,
	ERROR_THE_NAME_EXIST_CODE = 5,
	ERROR_ARG_COUNT_CODE = 6,
	ERROR_COMMAND_USE_CODE = 7,
	ERROR_USER_STOP_CODE = 8,
	ERROR_LNK_CREATE_CODE = 9,
	ERROR_PATH_N_TYPE = 10,
	ERROR_FOLDER_NOT_NULL = 11,
	ERROR_DATA_DISK_NULL = 12,
	ERROR_DISK_READ_CODE = 13,
	ERROR_NEW_NAME = 14,

};

//分隔路径每一级 返回字符串数组
std::vector<std::string>			SplitStr(const std::string& str, const char& Spliter);