#include "Include.h"

int main(int argc, char** argv)
{
	std::map<std::string, std::string> smap;
	std::string sstring = "Damage = 5; Target = Player;";
	const char* cstring = sstring.c_str();
	std::stringstream kstring;
	std::stringstream vstring;
	kstring.str("");
	vstring.str("");
	char korv = 'k';
	for (int i = 0; i < strlen(cstring); i++)
	{
		if (cstring[i] == '=')
		{
			korv = 'v';
		}
		else if (cstring[i] == ';')
		{
			korv = 'k';
			smap[kstring.str()] = vstring.str();
			std::cout << kstring.str() << " " << vstring.str() << std::endl;
			kstring.str("");
			vstring.str("");
		}
		else if (cstring[i] == ' ')
			continue;
		else
		{
			if (korv == 'k')
				kstring << cstring[i];
			else if (korv == 'v')
				vstring << cstring[i];
		}
	}
	std::string test = smap["Damage"];
	std::cout << test << std::endl;
	test = smap["Target"];
	std::cout << test << std::endl;

	std::cout << "hey the game works" << std::endl;
	SDL_Delay(5000);
	return 0;
}