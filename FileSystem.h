#pragma once
#include "Include.h"

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	static void saveSettings(void);
	static void loadSettings(void);

	static std::string cfg;
};