#pragma once
#include "Include.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	static bool init(void);
	static void close(void);
	static void renderAll(void);
	static void manageCamera(void);

	static const int GFX_OFFSET;
private:
	static SDL_Rect viewport;
	static std::vector<Texture*> blockGFX;
	static Texture* playerGFX;
	static SDL_Color renderColor;
	static std::string rDir;
	static std::string rExt;
	static std::string blockPrefix;
	static std::string playerPrefix;
};

