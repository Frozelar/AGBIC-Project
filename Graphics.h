#pragma once
#include "Include.h"

enum Background {
	MOUNTAINS
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	static bool init(void);
	static void close(void);
	static void renderAll(void);
	static void manageCamera(void);
	static void loadBG(int);
	static void closeBG(void);
	static void manageBG(void);

	static const int GFX_OFFSET;
	static const int BG_ALPHA_BASE;
private:
	static SDL_Rect viewport;
	static std::vector<Texture*> blockGFX;
	static std::vector<std::pair<Texture*, int>> bgObjects;
	static Texture* bg;
	static Texture* playerGFX;
	static SDL_Color renderColor;
	static std::string rDir;
	static std::string rExt;
	static std::string blockPrefix;
	static std::string playerPrefix;
	static std::string bgPrefix;
};

