struct GroundEffectDoodad
{
	unsigned int id;
	const char	 *Model;
	unsigned int flags;
};

struct GroundEffectTexture
{
	unsigned int id;
	unsigned int doodad[4];
	unsigned int unknown1;
	unsigned int unknown2;
	unsigned int unknown3;
	unsigned int unknown4;
	int			 amount;
	unsigned int unknown5;
};

void InitGroundEffects();
const char* getGroundEffectDoodad(unsigned int effectDoodad, int amount);