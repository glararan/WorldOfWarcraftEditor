#ifndef MODEL_H
#define MODEL_H

#include "Vec3D.h"

class Model;
class Bone;
Vec3D fixCoordSystem(Vec3D v);

#include "Manager.h"
#include "MPQ.h"
#include "Video.h"

#include "ModelHeaders.h"
#include "Quaternion.h"
#include "Matrix.h"

#include <vector>

#include "Animated.h"
#include "Particle.h"
#include "MapHeaders.h"

class Bone
{
	Animated<Vec3D> trans;
	Animated<Quaternion> rot;
	Animated<Vec3D> scale;

public:
	Vec3D pivot;
	int parent;

	bool billboard;
	Matrix mat;
	Matrix mrot;

	bool calc;
	void calcMatrix(Bone* allbones, int anim, int time);
	void init(MPQFile &f, ModelBoneDef &b, int *global);
};

class TextureAnim
{
	Animated<Vec3D> trans, rot, scale;

public:
	Vec3D tval, rval, sval;

	void calc(int anim, int time);
	void init(MPQFile &f, ModelTexAnimDef &mta, int *global);
	void setup();
};

struct ModelColor
{
	Animated<Vec3D> color;
	AnimatedShort opacity;

	void init(MPQFile &f, ModelColorDef &mcd, int *global);
};

struct ModelTransparency
{
	AnimatedShort trans;

	void init(MPQFile &f, ModelTransDef &mtd, int *global);
};

// copied from the .mdl docs? this might be completely wrong
enum BlendModes
{
	BM_OPAQUE,
	BM_TRANSPARENT,
	BM_ALPHA_BLEND,
	BM_ADDITIVE,
	BM_ADDITIVE_ALPHA,
	BM_MODULATE
};

struct ModelRenderPass
{
	uint16 indexStart, indexCount, vertexStart, vertexEnd;
	TextureID texture, texture2;
	bool usetex2, useenvmap, cull, trans, unlit, nozwrite;
	float p;
	
	int16 texanim, color, opacity, blendmode;
	int16 order;

	bool init(Model *m);
	void deinit();

	bool operator< (const ModelRenderPass &m) const
	{
		//return !trans;
		if (order<m.order) return true;
		else if (order>m.order) return false;
		else return blendmode == m.blendmode ? (p<m.p) : blendmode < m.blendmode;
	}
};

struct ModelCamera
{
	bool ok;

	Vec3D pos, target;
	float nearclip, farclip, fov;
	Animated<Vec3D> tPos, tTarget;
	Animated<float> rot;

	void init(MPQFile &f, ModelCameraDef &mcd, int *global);
	void setup(int time=0);

	ModelCamera():ok(false) {}
};

struct ModelLight
{
	int type, parent;
	Vec3D pos, tpos, dir, tdir;
	Animated<Vec3D> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;

	void init(MPQFile &f, ModelLightDef &mld, int *global);
	void setup(int time, GLuint l);
};

class Model: public ManagedItem
{
	GLuint dlist;
	GLuint slist;
	GLuint ghlist;
	GLuint rhlist;
	GLuint tmlist;
	GLuint vbuf, nbuf, tbuf;
	size_t vbufsize;
	bool animated;
	bool animGeometry,animTextures,animBones;

	bool forceAnim;

	void init(MPQFile &f);

	TextureAnim *texanims;
	ModelAnimation *anims;
	int *globalSequences;
	ModelColor *colors;
	ModelTransparency *transparency;
	ModelLight *lights;
	ParticleSystem *particleSystems;
	RibbonEmitter *ribbons;

	void drawModel();
	void drawModelTileMode();
	
	void drawModelHighlight();
	void drawModelSelect();
	
	void initCommon(MPQFile &f);
	bool isAnimated(MPQFile &f);
	void initAnimated(MPQFile &f);
	void initStatic(MPQFile &f);

	ModelVertex *origVertices;
	Vec3D *vertices, *normals;
	uint16 *indices;
	size_t nIndices;
	std::vector<ModelRenderPass> passes;

	void animate(int anim);
	void calcBones(int anim, int time);

	void lightsOn(GLuint lbase);
	void lightsOff(GLuint lbase);

	bool Reloaded;
	Model	*reloadModel;

public:
	ModelCamera cam;
	Bone *bones;
	TextureID *textures;
	ModelHeader header;

	bool ok;
	bool ind;

	float rad;
	float trans;
	bool animcalc;
	int anim, animtime;

	Model(std::string name, bool forceAnim=false);	
	~Model();
	void reload(std::string name);
	void draw();
	void drawTileMode();
	void drawHighlight();
	void drawSelect();
	void updateEmitters(float dt);

	friend struct ModelRenderPass;
};

class ModelManager: public SimpleManager
{
public:
	int add(std::string name);

	ModelManager() : v(0) {}

	int v;

	void resetAnim();
	void updateEmitters(float dt);
	void reload();
};

class ModelInstance
{
public:
	Model *model;
	
	int id;
	unsigned int nameID;
	unsigned int modelID;

	Vec3D pos, dir;
	unsigned int d1, scale;

	float frot,w,sc;

	int light;
	Vec3D ldir;
	Vec3D lcol;

	~ModelInstance();
	ModelInstance();
	ModelInstance(Model *m, MPQFile &f);
	ModelInstance(Model *m, MDDF *d);
    void init2(Model *m, MPQFile &f);
	void draw();
	void drawMapTile();
	void drawHighlight();
	void drawSelect();
	void draw2(const Vec3D& ofs, const float rot);
	void draw2Select(const Vec3D& ofs, const float rot);
};
void ResetUniques();
int	GetUniques();

int addModelToList(Model *m, MPQFile &f);
void removeModelFromList(int id);
void drawModelList();
void drawModelListSelect();

#endif