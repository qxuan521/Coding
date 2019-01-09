#pragma once
#include <string>
// extern "C"
// {
struct YInputInfo;
// }

class YCommandSyster
{
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																
public:
	YCommandSyster();
	~YCommandSyster();

	void runSystem();
private:
	

	void resolveInput(const std::string& szInput, YInputInfo& rResult);
private:
};
