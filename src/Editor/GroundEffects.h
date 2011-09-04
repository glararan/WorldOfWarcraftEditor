struct GroundEffectDoodad
{
	unsigned int	id;
	unsigned int	doodadID;
	const char		*Model;
};

struct GroundEffectTexture
{
	unsigned int	id;
	unsigned int	seed;
	unsigned int	unknown1;
	unsigned int	unknown2;
	const char		*texture;
	unsigned int	doodad[4];
	unsigned int	number;
	unsigned int	unknown3;
};

void InitGroundEffects();
const char* getGroundEffectDoodad(unsigned int effectID, int DoodadNum);
unsigned int findEffectID(const char *tex);