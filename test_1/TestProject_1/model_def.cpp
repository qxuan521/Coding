#include "model_def.h"

std::vector<std::string> SplitStr(const std::string & str, const char & Spliter)
{
	int StartIndex = 0;
	std::vector<std::string> Result;
	for (int index = 0; index < (int)str.size(); ++index)
	{
		if (Spliter == str[index])
		{
			Result.push_back(str.substr(StartIndex, index - StartIndex));
			if ((int)str.size() > index + 1)
				StartIndex = index + 1;
		}
	}
	Result.push_back(str.substr(StartIndex, str.size() - StartIndex));
	return Result;
}
