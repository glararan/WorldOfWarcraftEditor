struct GroundEffectDoodad
{
	unsigned int	id;
	const char		*Model;
	unsigned int	flags;
};

struct GroundEffectTexture
{
	unsigned int	id;
	unsigned int	doodad[4];
	unsigned int	unknown1;
	unsigned int	unknown2;
	unsigned int	unknown3;
	unsigned int	weight[4];
	unsigned int	unknown4;
	unsigned int	unknown5;
	unsigned int	unknown6;
	int				amount;
	const char*		terraintype;
};

void InitGroundEffects();
const char* getGroundEffectDoodad(unsigned int effectID, int amount);
unsigned int findEffectID(const char *tex);