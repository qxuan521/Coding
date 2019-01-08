#include "y_tool.h"


std::vector<std::string>& splitStrByCharacter(const std::string& srcStr, char spliter)
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

