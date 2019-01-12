#include "y_tool.h"
#include <regex>

namespace YPathRegex
{
	std::regex rRealPathRegex("^[@]");
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
	int ParentNameEnd = szPath.find_last_of("/");
	if (std::string::npos == ParentNameEnd)
		return std::string("");
	std::string DstParent = szPath.substr(0, ParentNameEnd);
	return DstParent;
}

std::string getNameFromFullPath(const std::string & szPath)
{
	int nNameIndex = szPath.find_last_of('/');
	if (std::string::npos == nNameIndex)
		return szPath;
	else
		return szPath.substr(nNameIndex + 1, szPath.size() - nNameIndex - 1);
}

bool isRealPath(const std::string & szPath)
{
	return std::regex_match(szPath, YPathRegex::rRealPathRegex);
}

