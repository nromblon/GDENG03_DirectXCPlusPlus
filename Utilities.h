#pragma once
#include <string>
#include <Windows.h>

class Utilities
{
public:
	static std::string GetErrorStr();
	static void PrintHResult(HRESULT hres);
};

