#include "y_tool.h"
#include <regex>
#include<sstream>
namespace YPathRegex
{
	std::regex rRealPathRegex("^[@][. ]*");
};
std::vector<std::string> splitStrByCharacter(const std::string& srcStr, char spliter)
{
	size_t startIndex = 0;
	std::vector<std::string> resultArr;
	for (size_t index = 0; index < srcStr.size(); ++index)
	{
		if (spliter == srcStr[index])
		{
			resultArr.push_back(srcStr.substr(startIndex, index - startIndex));
			if (srcStr.size() > index + 1)
				startIndex = index + 1;
		}
	}
	resultArr.push_back(srcStr.substr(startIndex, srcStr.size() - startIndex));
	return resultArr;
}

std::string getParentPath(const std::string & szPath)
{
	size_t ParentNameEnd = szPath.find_last_of("/");
	if (std::string::npos == ParentNameEnd)
		return std::string("");
	std::string DstParent = szPath.substr(0, ParentNameEnd);
	return DstParent;
}

std::string getNameFromFullPath(const std::string & szPath)
{
	size_t nNameIndex = szPath.find_last_of('/');
	if (std::string::npos == nNameIndex)
		return szPath;
	else
		return szPath.substr(nNameIndex + 1, szPath.size() - nNameIndex - 1);
}

std::string getPathFromRealPath(const std::string & szPath)
{
	if (szPath.empty() || '@' != szPath[0])
	{
		return std::string();
	}
	return  szPath.substr(1, szPath.size() - 1);
}

bool isRealPath(const std::string & szPath)
{
	return !szPath.empty() && '@' == szPath[0];
}

bool isHaveWildCard(const std::string & szPath)
{
	return std::string::npos != szPath.find('?') || std::string::npos != szPath.find('*');
}

std::regex makeRegexByPath(const std::string & szPath)
{
	std::string szRegexStr;
	for (size_t index = 0; index < szPath.size(); ++index)
	{
		if ('*' == szPath[index])
		{
			szRegexStr.append("[\\w_\\.]*");
		}
		else if ('?' == szPath[index])
		{
			szRegexStr.append("[\\w_\\.]?");
		}
		else
		{
			szRegexStr += szPath[index];
		}
	}
	return std::regex(szRegexStr);
}


std::regex makeRepaceRegexByPath(const std::string& szPath, std::string& szRepaceStr)
{
	std::string szRegexStr;
	std::string szIndex;
	std::stringstream rStringStream;
	for (size_t index = 0; index < szPath.size(); ++index)
	{
		std::string szIndexBuffer;
		if ('*' == szPath[index])
		{
			szRegexStr.append("([\\w_\\.]*)");
			rStringStream << (index + 1);
			szRepaceStr.append("$");
			rStringStream >> szIndex;
			szRepaceStr.append(szIndex);
		}
		else if ('?' == szPath[index])
		{
			szRegexStr.append("([\\w_\\.]?)");
			rStringStream << (index + 1);
			szRepaceStr.append("$");
			rStringStream >> szIndex;
			szRepaceStr.append(szIndex);
		}
		else
		{
			if (index == 0)
			{
				szRegexStr.append("(^[\\w_\\.])");
			}
			else if (index == szPath.size() - 1)
			{
				szRegexStr.append("([\\w_\\.]$)");
			}
			else
			{
				szRegexStr.append("([\\w_\\.])");
			}
			szRepaceStr += szPath[index];
		}
	}
	return std::regex(szRegexStr);
}

std::string makeStringFromBuffer(std::vector<char>& rBuffer, int size)
{
	std::string szPath;
	for (size_t index = 0; index < rBuffer.size();++index)
	{
		szPath += rBuffer[index];
	}
	return szPath;
}

bool equalOrLowerWithCurPath(const std::string & szCurPath, const std::string & szPath)
{
	std::string szTempPath = szCurPath;
	while (!szTempPath.empty())
	{
		if (szPath == szTempPath)
		{
			return true;
		}
		szTempPath = getParentPath(szTempPath);
	}
	return false;
}
