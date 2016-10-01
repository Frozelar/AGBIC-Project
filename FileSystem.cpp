#include "FileSystem.h"
#include "Window.h"
#include "Audio.h"

std::string FileSystem::cfg = "Resources/config.cfg";

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

void FileSystem::saveSettings()
{
	std::ofstream file;
	file.open(cfg);
	
	file << Window::isFullscreen() << " ";
	file << Window::isVSync() << " ";
	file << Audio::getVolume('m') << " ";
	file << Audio::getVolume('s') << " ";

	file.close();
}

void FileSystem::loadSettings()
{
	std::ifstream file;
	int setting = 0;

	if (std::ifstream(cfg))
	{
		file.open(cfg);

		file >> setting;
		if (setting != Window::isFullscreen())
			Window::toggleFullscreen();
		file >> setting;
		if (setting != Window::isVSync())
			Window::toggleVSync();
		file >> setting;
		Audio::setVolume(setting, 'm');
		file >> setting;
		Audio::setVolume(setting, 's');

		file.close();
	}
}