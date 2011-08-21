#include <iostream>
#include <fstream>

int CreateConfig()
{
	std::ofstream Editor;
	Editor.open("Editor.conf");

	Editor << "[expansion_option]\n"
		"############################################"
		"## Choose Expansion                       ##"
		"##			default:                       ##"
		"##             1 = The Burning Crusade    ##"
		"##             2 = Wrath of the Lich King ##"
		"##             3 = Cataclysm              ##"
		"############################################"
		"expansion = 2";

	Editor.close();
	return 0;
}