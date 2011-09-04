#include "Model.h"
#include "World.h"
#include <cassert>
#include <algorithm>
#include <map>

int globalTime = 0;

#define	MAX_CHECK 4096
unsigned int UniqueUsed[MAX_CHECK];
int	Uniques;
int HColor;

bool CheckUniques(unsigned int Num)
{
	for(int i=0;i<Uniques;i++)
		if(Num==UniqueUsed[i])
			return true;
	if(Uniques==MAX_CHECK)
		return false;
	UniqueUsed[Uniques]=Num;
	Uniques++;
	return false;
}

void ResetUniques()
{
	Uniques=0;
}

int	GetUniques()
{
	return Uniques;
}

Model::Model(std::string name, bool forceAnim) : ManagedItem(name), forceAnim(forceAnim)
{
	// replace .MDX with .M2
	char tempname[256];
	strcpy_s(tempname,name.c_str());
	tempname[name.length()-2] = '2';
	tempname[name.length()-1] = 0;
	Reloaded=false;

	MPQFile f(tempname);
	ok = !f.isEof();

	if (!ok)
	{
		gLog("[World of Warcraft Studio - Editor] - Error loading model [%s]\n", tempname);
		return;
	}

	memcpy(&header, f.getBuffer(), sizeof(ModelHeader));

	// HACK: these particle systems are way too active and cause horrible horrible slowdowns
	// I'm removing them until I can fix emission speed so it doesn't get this crazy
	if (false
	|| name=="World\\Kalimdor\\Orgrimmar\\Passivedoodads\\Orgrimmarbonfire\\Orgrimmarsmokeemitter.Mdx"	
	//|| name=="World\\Kalimdor\\Orgrimmar\\Passivedoodads\\Orgrimmarbonfire\\Orgrimmarbonfire01.Mdx"	
	)
	{
        header.nParticleEmitters = 0;
	}

	animated = isAnimated(f) || forceAnim;  // isAnimated will set animGeometry and animTextures

	if(!f.isExternal())
		gLog("[World of Warcraft Studio - Editor] - Loading model from MPQ %s%s\n", tempname, animated ? " (animated)" : "");
	else
		gLog("[World of Warcraft Studio - Editor] - Loading model from File %s%s\n", tempname, animated ? " (animated)" : "");

	trans = 1.0f;

	vbuf = nbuf = tbuf = 0;

	globalSequences = 0;
	animtime = 0;
	anim = 0;
	colors = 0;
	lights = 0;
	transparency = 0;
	particleSystems = 0;
	ribbons = 0;
	if (header.nGlobalSequences)
	{
		globalSequences = new int[header.nGlobalSequences];
		memcpy(globalSequences, (f.getBuffer() + header.ofsGlobalSequences), header.nGlobalSequences * 4);
	}

	if (animated) initAnimated(f);
	else initStatic(f);

	f.close();
	Reloaded=false;
}

Model::~Model()
{
	if (ok) {
		//gLog("[World of Warcraft Studio - Editor] - Unloading model %s\n", name.c_str());

		if (header.nTextures)
		{
			for (size_t i=0; i<header.nTextures; i++)
			{
				if (textures[i]!=0)
				{
					//Texture *tex = (Texture*)video.textures.items[textures[i]];
					video.textures.del(textures[i]);
				}
			}
			delete[] textures;
		}

		delete[] globalSequences;

		if (animated)
		{
			// unload all sorts of crap
			//delete[] vertices;
			//delete[] normals;
			delete[] indices;
			delete[] anims;
			delete[] origVertices;
			if (animBones) delete[] bones;
			if (!animGeometry)
			{
				glDeleteBuffersARB(1, &nbuf);
			}
			glDeleteBuffersARB(1, &vbuf);
			glDeleteBuffersARB(1, &tbuf);

			if (animTextures) delete[] texanims;
			if (colors) delete[] colors;
			if (transparency) delete[] transparency;
			if (lights) delete[] lights;

			if (particleSystems) delete[] particleSystems;
			if (ribbons) delete[] ribbons;

		}
		else
		{
			glDeleteLists(dlist, 1);
		}
		if(Reloaded)
			delete reloadModel;
	}
}

void Model::reload(std::string name)
{
	Reloaded=true;
	reloadModel=new Model(name);
}

bool Model::isAnimated(MPQFile &f)
{
	// see if we have any animated bones
	ModelBoneDef *bo = (ModelBoneDef*)(f.getBuffer() + header.ofsBones);

	animGeometry = false;
	animBones = false;
	ind = false;

	ModelVertex *verts = (ModelVertex*)(f.getBuffer() + header.ofsVertices);
	for (size_t i=0; i<header.nVertices && !animGeometry; i++)
	{
		for (size_t b=0; b<4; b++)
		{
			if (verts[i].weights[b]>0)
			{
				ModelBoneDef &bb = bo[verts[i].bones[b]];
				if (bb.translation.type || bb.rotation.type || bb.scaling.type || (bb.flags&8))
				{
					if (bb.flags&8)
					{
						// if we have billboarding, the model will need per-instance animation
						ind = true;
					}
					animGeometry = true;
					break;
				}
			}
		}
	}

	if (animGeometry)
		animBones = true;
	else
	{
		for (size_t i=0; i<header.nBones; i++)
		{
			ModelBoneDef &bb = bo[i];
			if (bb.translation.type || bb.rotation.type || bb.scaling.type)
			{
				animBones = true;
				break;
			}
		}
	}

	animTextures = header.nTexAnims > 0;

	bool animMisc = header.nCameras>0 || // why waste time, pretty much all models with cameras need animation
					header.nLights>0 || // same here
					header.nParticleEmitters>0 ||
					header.nRibbonEmitters>0;

	if (animMisc) animBones = true;

	// animated colors
	if (header.nColors)
	{
		ModelColorDef *cols = (ModelColorDef*)(f.getBuffer() + header.ofsColors);
		for (size_t i=0; i<header.nColors; i++)
		{
			if (cols[i].color.type!=0 || cols[i].opacity.type!=0)
			{
				animMisc = true;
				break;
			}
		}
	}

	// animated opacity
	if (header.nTransparency && !animMisc)
	{
		ModelTransDef *trs = (ModelTransDef*)(f.getBuffer() + header.ofsTransparency);
		for (size_t i=0; i<header.nTransparency; i++)
		{
			if (trs[i].trans.type!=0)
			{
				animMisc = true;
				break;
			}
		}
	}

	// guess not...
	return animGeometry || animTextures || animMisc;
}

Vec3D fixCoordSystem(Vec3D v)
{
	return Vec3D(v.x, v.z, -v.y);
}

Vec3D fixCoordSystem2(Vec3D v)
{
	return Vec3D(v.x, v.z, v.y);
}

Quaternion fixCoordSystemQuat(Quaternion v)
{
	return Quaternion(-v.x, -v.z, v.y, v.w);
}


void Model::initCommon(MPQFile &f)
{
	// assume: origVertices already set
	if (!animGeometry) {
		vertices = new Vec3D[header.nVertices];
		normals = new Vec3D[header.nVertices];
	}

	//Vec3D vmin = Vec3D( 9999999.0f, 9999999.0f, 9999999.0f);
	//Vec3D vmax = Vec3D(-9999999.0f,-9999999.0f,-9999999.0f);
	// vertices, normals
	for (size_t i=0; i<header.nVertices; i++)
	{
		origVertices[i].pos = fixCoordSystem(origVertices[i].pos);
		origVertices[i].normal = fixCoordSystem(origVertices[i].normal);

		if (!animGeometry)
		{
			vertices[i] = origVertices[i].pos;
			normals[i] = origVertices[i].normal.normalize();
		}

		float len = origVertices[i].pos.lengthSquared();
		if (len > rad)
		{ 
			rad = len;
		}
		/*
		if (origVertices[i].pos.x < vmin.x) vmin.x = origVertices[i].pos.x;
		if (origVertices[i].pos.y < vmin.y) vmin.y = origVertices[i].pos.y;
		if (origVertices[i].pos.z < vmin.z) vmin.z = origVertices[i].pos.z;
		if (origVertices[i].pos.x > vmax.x) vmax.x = origVertices[i].pos.x;
		if (origVertices[i].pos.y > vmax.y) vmax.y = origVertices[i].pos.y;
		if (origVertices[i].pos.z > vmax.z) vmax.z = origVertices[i].pos.z;
		*/
	}
	rad = sqrtf(rad);
	//rad = std::max(vmin.length(),vmax.length());

	// textures
	ModelTextureDef *texdef = (ModelTextureDef*)(f.getBuffer() + header.ofsTextures);
	if (header.nTextures)
	{
		textures = new TextureID[header.nTextures];
		for (size_t i=0; i<header.nTextures; i++)
		{
			char texname[256];
			if (texdef[i].type == 0)
			{
				strncpy_s(texname, f.getBuffer() + texdef[i].nameOfs, texdef[i].nameLen);
				texname[texdef[i].nameLen] = 0;
				std::string path(texname);
				fixname(path);
				textures[i] = video.textures.add(texname);
			}
			else
			{
				// special texture - only on characters and such...
                textures[i] = 0;
			}
		}
	}

	// init colors
	if (header.nColors)
	{
		colors = new ModelColor[header.nColors];
		ModelColorDef *colorDefs = (ModelColorDef*)(f.getBuffer() + header.ofsColors);
		for (size_t i=0; i<header.nColors; i++) colors[i].init(f, colorDefs[i], globalSequences);
	}
	// init transparency
	int16 *transLookup = (int16*)(f.getBuffer() + header.ofsTransparencyLookup);
	if (header.nTransparency)
	{
		transparency = new ModelTransparency[header.nTransparency];
		ModelTransDef *trDefs = (ModelTransDef*)(f.getBuffer() + header.ofsTransparency);
		for (size_t i=0; i<header.nTransparency; i++) transparency[i].init(f, trDefs[i], globalSequences);
	}

	// just use the first LOD/view

	// indices - allocate space, too
	ModelView *view = (ModelView*)(f.getBuffer() + header.ofsViews);

	uint16 *indexLookup = (uint16*)(f.getBuffer() + view->ofsIndex);
	uint16 *triangles = (uint16*)(f.getBuffer() + view->ofsTris);
	nIndices = view->nTris;
	indices = new uint16[nIndices];
	for (size_t i = 0; i<nIndices; i++)
	{
        indices[i] = indexLookup[triangles[i]];
	}

	// render ops
	ModelGeoset *ops = (ModelGeoset*)(f.getBuffer() + view->ofsSub);
	ModelTexUnit *tex = (ModelTexUnit*)(f.getBuffer() + view->ofsTex);
	ModelRenderFlags *renderFlags = (ModelRenderFlags*)(f.getBuffer() + header.ofsTexFlags);
	uint16 *texlookup = (uint16*)(f.getBuffer() + header.ofsTexLookup);
	uint16 *texanimlookup = (uint16*)(f.getBuffer() + header.ofsTexAnimLookup);
	int16 *texunitlookup = (int16*)(f.getBuffer() + header.ofsTexUnitLookup);

	/*
	for (size_t i = 0; i<view->nSub; i++)
	{
		ModelRenderPass pass;
		pass.usetex2 = false;
		pass.indexStart = ops[i].istart;
		pass.indexCount = ops[i].icount;

		// textures
		for (size_t j = 0; j<view->nTex; j++)
		{
			if (tex[j].op==i)
			{

				TextureID texid = textures[texlookup[tex[j].textureid]];

				if (tex[j].texunit==0)
				{
					pass.texture = texid;
					
					// TODO: figure out these flags properly -_-
					ModelRenderFlags &rf = renderFlags[tex[j].flagsIndex];
					
					//pass.useenvmap = (rf.flags2 & 6)==6;
					//pass.useenvmap = rf.blend == 6; // ???
					pass.useenvmap = texunitlookup[tex[j].texunit] == -1;

					pass.blendmode = rf.blend;
					pass.color = tex[j].colorIndex;
					pass.opacity = transLookup[tex[j].transid];

					pass.cull = (rf.flags & 4)==0 && rf.blend==0;
					pass.unlit = (rf.flags & 3)!=0;

					pass.nozwrite = pass.blendmode >= 2; //(rf.flags & 16)!=0;

					pass.trans = pass.blendmode != 0;

					pass.p = ops[i].v.x;


					if (animTextures)
					{
						if (tex[j].flags & 16)
						{
							pass.texanim = -1; // no texture animation
						}
						else
						{
							pass.texanim = texanimlookup[tex[j].texanimid];
						}
					}
					else
					{
						pass.texanim = -1; // no texture animation
					}
				}
				else if (tex[j].texunit==1)
				{
					pass.texture2 = texid;
					//pass.usetex2 = true;
				}
			}
		}

        passes.push_back(pass);
	}
	*/
	for (size_t j = 0; j<view->nTex; j++)
	{
		ModelRenderPass pass;
		pass.usetex2 = false;
		pass.texture2 = 0;
		size_t geoset = tex[j].op;
		pass.indexStart = ops[geoset].istart;
		pass.indexCount = ops[geoset].icount;
		pass.vertexStart = ops[geoset].vstart;
		pass.vertexEnd = pass.vertexStart + ops[geoset].vcount;

		pass.order = tex[j].order;

		TextureID texid = textures[texlookup[tex[j].textureid]];

		pass.texture = texid;
		
		// TODO: figure out these flags properly -_-
		ModelRenderFlags &rf = renderFlags[tex[j].flagsIndex];
		
		pass.useenvmap = texunitlookup[tex[j].texunit] == -1;

		pass.blendmode = rf.blend;
		pass.color = tex[j].colorIndex;
		pass.opacity = transLookup[tex[j].transid];

		pass.cull = (rf.flags & 4)==0 && rf.blend==0;
		pass.unlit = (rf.flags & 3)!=0;

		pass.nozwrite = pass.blendmode >= 2; //(rf.flags & 16)!=0;

		pass.trans = pass.blendmode != 0;

		pass.p = ops[geoset].v.x;

		if (animTextures)
		{
			if (tex[j].flags & 16)
			{
				pass.texanim = -1; // no texture animation
			}
			else
			{
				pass.texanim = texanimlookup[tex[j].texanimid];
			}
		}
		else
		{
			pass.texanim = -1; // no texture animation
		}

        passes.push_back(pass);
	}

	// transparent parts come later
	std::sort(passes.begin(), passes.end());

	// zomg done
}

void Model::initStatic(MPQFile &f)
{
	origVertices = (ModelVertex*)(f.getBuffer() + header.ofsVertices);

	initCommon(f);

	dlist = glGenLists(1);
	glNewList(dlist, GL_COMPILE);

    drawModel();

	glEndList();

	slist = glGenLists(1);
	glNewList(slist, GL_COMPILE);

    drawModelSelect();

	glEndList();

	tmlist = glGenLists(1);
	glNewList(tmlist, GL_COMPILE);

    drawModelTileMode();

	glEndList();

	rhlist = glGenLists(1);
	glNewList(rhlist, GL_COMPILE);

	HColor=1;
    drawModelHighlight();

	glEndList();

	ghlist = glGenLists(1);
	glNewList(ghlist, GL_COMPILE);

	HColor=0;
    drawModelHighlight();

	glEndList();

	// clean up vertices, indices etc
/*	delete[] vertices;
	delete[] normals;
	delete[] indices;

	if (colors) delete[] colors;
	if (transparency) delete[] transparency;*/
}

void Model::initAnimated(MPQFile &f)
{
	origVertices = new ModelVertex[header.nVertices];
	memcpy(origVertices, f.getBuffer() + header.ofsVertices, header.nVertices * sizeof(ModelVertex));

	glGenBuffersARB(1,&vbuf);
	glGenBuffersARB(1,&tbuf);
	const size_t size = header.nVertices * sizeof(float);
	vbufsize = 3 * size;

	initCommon(f);

	if (animBones)
	{
		// init bones...
		bones = new Bone[header.nBones];
		ModelBoneDef *mb = (ModelBoneDef*)(f.getBuffer() + header.ofsBones);
		for (size_t i=0; i<header.nBones; i++)
		{
			bones[i].init(f, mb[i], globalSequences);
		}
	}

	if (!animGeometry)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vbufsize, vertices, GL_STATIC_DRAW_ARB);
		glGenBuffersARB(1,&nbuf);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vbufsize, normals, GL_STATIC_DRAW_ARB);
		//delete[] vertices;
		delete[] normals;
	}
	Vec2D *texcoords = new Vec2D[header.nVertices];
	for (size_t i=0; i<header.nVertices; i++) texcoords[i] = origVertices[i].texcoords;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*size, texcoords, GL_STATIC_DRAW_ARB);
	delete[] texcoords;

	if (animTextures)
	{
		texanims = new TextureAnim[header.nTexAnims];
		ModelTexAnimDef *ta = (ModelTexAnimDef*)(f.getBuffer() + header.ofsTexAnims);
		for (size_t i=0; i<header.nTexAnims; i++)
		{
			texanims[i].init(f, ta[i], globalSequences);
		}
	}

	// particle systems
	if (header.nParticleEmitters)
	{
		ModelParticleEmitterDef *pdefs = (ModelParticleEmitterDef *)(f.getBuffer() + header.ofsParticleEmitters);
		particleSystems = new ParticleSystem[header.nParticleEmitters];
		for (size_t i=0; i<header.nParticleEmitters; i++) {
			particleSystems[i].model = this;
			particleSystems[i].init(f, pdefs[i], globalSequences);
		}
	}

	// ribbons
	if (header.nRibbonEmitters)
	{
		ModelRibbonEmitterDef *rdefs = (ModelRibbonEmitterDef *)(f.getBuffer() + header.ofsRibbonEmitters);
		ribbons = new RibbonEmitter[header.nRibbonEmitters];
		for (size_t i=0; i<header.nRibbonEmitters; i++)
		{
			ribbons[i].model = this;
			ribbons[i].init(f, rdefs[i], globalSequences);
		}
	}

	// just use the first camera, meh
	if (header.nCameras>0)
	{
		ModelCameraDef *camDefs = (ModelCameraDef*)(f.getBuffer() + header.ofsCameras);
		cam.init(f, camDefs[0], globalSequences);
	}

	// init lights
	if (header.nLights)
	{
		lights = new ModelLight[header.nLights];
		ModelLightDef *lDefs = (ModelLightDef*)(f.getBuffer() + header.ofsLights);
		for (size_t i=0; i<header.nLights; i++) lights[i].init(f, lDefs[i], globalSequences);
	}

	anims = new ModelAnimation[header.nAnimations];
	memcpy(anims, f.getBuffer() + header.ofsAnimations, header.nAnimations * sizeof(ModelAnimation));

	animcalc = false;
}

void Model::calcBones(int anim, int time)
{
	for (size_t i=0; i<header.nBones; i++)
	{
		bones[i].calc = false;
	}

	for (size_t i=0; i<header.nBones; i++)
	{
		bones[i].calcMatrix(bones, anim, time);
	}
}

void Model::animate(int anim)
{
	ModelAnimation &a = anims[anim];
	int t = globalTime; //(int)(gWorld->animtime /* / a.playSpeed*/);
	int tmax = (a.timeEnd-a.timeStart);
	t %= tmax;
	t += a.timeStart;
	animtime = t;
	this->anim = anim;

	if (animBones)
	{
		calcBones(anim, t);
	}

	if (animGeometry)
	{

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*vbufsize, NULL, GL_STREAM_DRAW_ARB);
		vertices = (Vec3D*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);

		// transform vertices
		ModelVertex *ov = origVertices;
		for (size_t i=0,k=0; i<header.nVertices; ++i,++ov)
		{
			Vec3D v(0,0,0), n(0,0,0);

			for (size_t b=0; b<4; b++)
			{
				if (ov->weights[b]>0)
				{
					Vec3D tv = bones[ov->bones[b]].mat * ov->pos;
					Vec3D tn = bones[ov->bones[b]].mrot * ov->normal;
					v += tv * ((float)ov->weights[b] / 255.0f);
					n += tn * ((float)ov->weights[b] / 255.0f);
				}
			}
			/*
			vertices[i] = v;
			normals[i] = n;
			*/
			vertices[i] = v;
			vertices[header.nVertices + i] = n.normalize(); // shouldn't these be normal by default?
		}

        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	}

	for (size_t i=0; i<header.nLights; i++)
	{
		if (lights[i].parent>=0) {
			lights[i].tpos = bones[lights[i].parent].mat * lights[i].pos;
			lights[i].tdir = bones[lights[i].parent].mrot * lights[i].dir;
		}
	}

	for (size_t i=0; i<header.nParticleEmitters; i++)
	{
		// random time distribution for teh win ..?
		int pt = a.timeStart + (t + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(anim, pt);
	}

	for (size_t i=0; i<header.nRibbonEmitters; i++)
	{
		ribbons[i].setup(anim, t);
	}

	if (animTextures)
	{
		for (size_t i=0; i<header.nTexAnims; i++)
		{
			texanims[i].calc(anim, t);
		}
	}
}

bool ModelRenderPass::init(Model *m)
{
	// blend mode
	switch (blendmode)
	{
	case BM_OPAQUE:	// 0
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		break;
	case BM_TRANSPARENT: // 1
		glDisable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		break;
	case BM_ALPHA_BLEND: // 2
		glDisable(GL_ALPHA_TEST);
 		glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // default blend func
		break;
	case BM_ADDITIVE: // 3
		glDisable(GL_ALPHA_TEST);
 		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE);
		break;
	case BM_ADDITIVE_ALPHA: // 4
		glDisable(GL_ALPHA_TEST);
 		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	default:
		// ???
		glDisable(GL_ALPHA_TEST);
 		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
	}

	if (nozwrite)
	{
		glDepthMask(GL_FALSE);
	}

	if (cull)
	{
        glEnable(GL_CULL_FACE);
	} else {
        glDisable(GL_CULL_FACE);
	}
	
	glPushName(texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (usetex2)
	{
		glActiveTextureARB(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture2);
	}

	if (unlit)
	{
		glDisable(GL_LIGHTING);
		// unfogged = unlit?
		if((blendmode==3)||(blendmode==4))
			glDisable(GL_FOG);
	}

	if (useenvmap)
	{
		// env mapping
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		const GLint maptype = GL_SPHERE_MAP;
		//const GLint maptype = GL_REFLECTION_MAP_ARB;

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, maptype);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, maptype);
	}

	if (texanim!=-1)
	{
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();

		m->texanims[texanim].setup();
		glMatrixMode(GL_MODELVIEW);
	}

	Vec4D ocol(1,1,1,m->trans);
	Vec4D ecol(0,0,0,0);

	// emissive colors
	if (color!=-1)
	{
		Vec3D c = m->colors[color].color.getValue(m->anim,m->animtime);
		ocol.w *= m->colors[color].opacity.getValue(m->anim,m->animtime);
		if (unlit)
		{
			ocol.x = c.x; ocol.y = c.y; ocol.z = c.z;
		}
		else
		{
			ocol.x = ocol.y = ocol.z = 0;
		}
		ecol = Vec4D(c, 1.0f);
	}
	glMaterialfv(GL_FRONT, GL_EMISSION, ecol);

	// opacity
	if (opacity!=-1)
	{
		ocol.w *= m->transparency[opacity].trans.getValue(m->anim,m->animtime);
	}

	// color
	glColor4fv(ocol);

	if (blendmode<=1 && ocol.w!=1.0f) glEnable(GL_BLEND);

	return (ocol.w > 0) || (ecol.lengthSquared() > 0);
}

void ModelRenderPass::deinit()
{
	switch (blendmode)
	{
	case BM_OPAQUE:
		break;
	case BM_TRANSPARENT:
		break;
	case BM_ALPHA_BLEND:
		//glDepthMask(GL_TRUE);
		break;
	default:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // default blend func
	}
	if (nozwrite)
	{
		glDepthMask(GL_TRUE);
	}
	if (texanim!=-1)
	{
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	if (unlit)
	{
		glEnable(GL_LIGHTING);
		//if (gWorld && gWorld->drawfog) glEnable(GL_FOG);
		if((blendmode==3)||(blendmode==4))
			glEnable(GL_FOG);
	}
	if (useenvmap)
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	if (usetex2)
	{
		glActiveTextureARB(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
	}
	glPopName();
	//glColor4f(1,1,1,1); //???
}

void Model::drawModel()
{
	// assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY

	if (animated)
	{

		if (animGeometry)
		{

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);

			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(vbufsize));

		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		//glTexCoordPointer(2, GL_FLOAT, sizeof(ModelVertex), &origVertices[0].texcoords);
	}

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc (GL_GREATER, 0.3f);

	for (size_t i=0; i<passes.size(); i++)
	{
		ModelRenderPass &p = passes[i];
		int result=(int)strstr(this->name.c_str(),"Ghostlandsweb");

		if (p.init(this)&&(result==0))
		{
			// we don't want to render completely transparent parts
			// render
			if (animated)
			{
				//glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
				// a GDC OpenGL Performace Tuning paper recommended glDrawRangeElements over glDrawElements
				// I can't notice a difference but I guess it can't hurt
				glDrawRangeElements(GL_TRIANGLES, p.vertexStart, p.vertexEnd, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
			} else {
				glBegin(GL_TRIANGLES);
				for (size_t k = 0, b=p.indexStart; k<p.indexCount; k++,b++)
				{
					uint16 a = indices[b];
					glNormal3fv(normals[a]);
					glTexCoord2fv(origVertices[a].texcoords);
					glVertex3fv(vertices[a]);
				}
				glEnd();
			}		
		}

		p.deinit();

	}
	// done with all render ops

	glAlphaFunc (GL_GREATER, 0.0f);
	glDisable (GL_ALPHA_TEST);

	GLfloat czero[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT, GL_EMISSION, czero);
	glColor4f(1,1,1,1);
	glDepthMask(GL_TRUE);
}

void Model::drawModelTileMode()
{
	// assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY

	if (animated)
	{
		if (animGeometry)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);

			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(vbufsize));

		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		//glTexCoordPointer(2, GL_FLOAT, sizeof(ModelVertex), &origVertices[0].texcoords);
	}

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc (GL_GREATER, 0.3f);

	for (size_t i=0; i<passes.size(); i++)
	{
		ModelRenderPass &p = passes[i];

		p.unlit=true;
		if (p.init(this))
		{
			// we don't want to render completely transparent parts
			
			// render
			if (animated)
			{
				//glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
				// a GDC OpenGL Performace Tuning paper recommended glDrawRangeElements over glDrawElements
				// I can't notice a difference but I guess it can't hurt
				glDrawRangeElements(GL_TRIANGLES, p.vertexStart, p.vertexEnd, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
			}
			else
			{
				glBegin(GL_TRIANGLES);
				for (size_t k = 0, b=p.indexStart; k<p.indexCount; k++,b++)
				{
					uint16 a = indices[b];
					//glNormal3fv(normals[a]);
					glTexCoord2fv(origVertices[a].texcoords);
					glVertex3fv(vertices[a]);
				}
				glEnd();
			}
			
		}

		p.deinit();
	}
	// done with all render ops

	glAlphaFunc (GL_GREATER, 0.0f);
	glDisable (GL_ALPHA_TEST);

	GLfloat czero[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT, GL_EMISSION, czero);
	glColor4f(1,1,1,1);
	glDepthMask(GL_TRUE);
}

void ModelHighlight()
{
	Vec4D	RedColor(1.0,0.25,0.25,0.5);
	Vec4D	GreenColor(0.25,1.0,0.25,0.5);
	float Highlight=0.5;

	glDisable(GL_ALPHA_TEST);
 	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glActiveTextureARB(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	if(HColor==0)
	{
        glColor4fv(GreenColor);
		glMaterialfv(GL_FRONT, GL_EMISSION, GreenColor);
	}
	else
	{
		glColor4fv(RedColor);
		glMaterialfv(GL_FRONT, GL_EMISSION, RedColor);
	}
	
	//glDisable(GL_FOG);
	glDepthMask(GL_FALSE);
}

void ModelUnhighlight()
{
	float Highlight=(float)(abs(10-globalTime%20))/10.0f;

	glEnable(GL_ALPHA_TEST);
 	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	Vec4D ocol(1,1,1,1.0);

	glColor4fv(ocol);
	//glEnable(GL_FOG);
	glDepthMask(GL_TRUE);
}

void Model::drawModelHighlight()
{
	// assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY

	if (animated)
	{

		if (animGeometry)
		{

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);

			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(vbufsize));

		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		//glTexCoordPointer(2, GL_FLOAT, sizeof(ModelVertex), &origVertices[0].texcoords);
	}

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc (GL_GREATER, 0.3f);

	for (size_t i=0; i<passes.size(); i++)
	{
		ModelRenderPass &p = passes[i];

		if (p.init(this))
		{
			// we don't want to render completely transparent parts
			ModelHighlight();
			// render
			if (animated)
			{
				//glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
				// a GDC OpenGL Performace Tuning paper recommended glDrawRangeElements over glDrawElements
				// I can't notice a difference but I guess it can't hurt
				glDrawRangeElements(GL_TRIANGLES, p.vertexStart, p.vertexEnd, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
			}
			else
			{
				glBegin(GL_TRIANGLES);
				for (size_t k = 0, b=p.indexStart; k<p.indexCount; k++,b++)
				{
					uint16 a = indices[b];
					//glNormal3fv(normals[a]);
					//glTexCoord2fv(origVertices[a].texcoords);
					glVertex3fv(vertices[a]);
				}
				glEnd();
			}
			ModelUnhighlight();
		}

		p.deinit();
	}
	// done with all render ops

	glAlphaFunc (GL_GREATER, 0.0f);
	glDisable (GL_ALPHA_TEST);

	GLfloat czero[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT, GL_EMISSION, czero);
	glColor4f(1,1,1,1);
	glDepthMask(GL_TRUE);
}

void Model::drawModelSelect()
{
	// assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY

	/*if (animated)
	{

		if (animGeometry)
		{

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);

			glVertexPointer(3, GL_FLOAT, 0, 0);
			//glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(vbufsize));

		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			//glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
			//glNormalPointer(GL_FLOAT, 0, 0);
		}

		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
		//glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		//glTexCoordPointer(2, GL_FLOAT, sizeof(ModelVertex), &origVertices[0].texcoords);
	}*/

	for (size_t i=0; i<passes.size(); i++)
	{
		ModelRenderPass &p = passes[i];

			// render
			glPushName(p.texture);
			glBegin(GL_TRIANGLES);
			for (size_t k = 0, b=p.indexStart; k<p.indexCount; k++,b++)
			{
				uint16 a = indices[b];
				glVertex3fv(vertices[a]);
			}
			glEnd();
			glPopName();
	}
}

void TextureAnim::calc(int anim, int time)
{
	if (trans.used)
	{
		tval = trans.getValue(anim, time);
	}
	if (rot.used)
	{
        rval = rot.getValue(anim, time);
	}
	if (scale.used)
	{
        sval = scale.getValue(anim, time);
	}
}

void TextureAnim::setup()
{
	glLoadIdentity();
	if (trans.used)
	{
		glTranslatef(tval.x, tval.y, tval.z);
	}
	if (rot.used)
	{
		glRotatef(rval.x, 0, 0, 1); // this is wrong, I have no idea what I'm doing here ;)
	}
	if (scale.used)
	{
		glScalef(sval.x, sval.y, sval.z);
	}
}

void ModelCamera::init(MPQFile &f, ModelCameraDef &mcd, int *global)
{
	ok = true;
    nearclip = mcd.nearclip;
	farclip = mcd.farclip;
	fov = mcd.fov;
	pos = fixCoordSystem(mcd.pos);
	target = fixCoordSystem(mcd.target);
	tPos.init(mcd.transPos, f, global);
	tTarget.init(mcd.transTarget, f, global);
	rot.init(mcd.rot, f, global);
	tPos.fix(fixCoordSystem);
	tTarget.fix(fixCoordSystem);
}

void ModelCamera::setup(int time)
{
	if (!ok) return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov * 34.5f, (GLfloat)video.xres/(GLfloat)video.yres, nearclip, farclip);

	Vec3D p = pos + tPos.getValue(0, time);
	Vec3D t = target + tTarget.getValue(0, time);

	Vec3D u(0,1,0);
	float roll = rot.getValue(0, time) / PI * 180.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(p.x, p.y, p.z, t.x, t.y, t.z, u.x, u.y, u.z);
	//glRotatef(roll, 0, 0, 1);
}

void ModelColor::init(MPQFile &f, ModelColorDef &mcd, int *global)
{
	color.init(mcd.color, f, global);
	opacity.init(mcd.opacity, f, global);
}

void ModelTransparency::init(MPQFile &f, ModelTransDef &mcd, int *global)
{
	trans.init(mcd.trans, f, global);
}

void ModelLight::init(MPQFile &f, ModelLightDef &mld, int *global)
{
	tpos = pos = fixCoordSystem(mld.pos);
	tdir = dir = Vec3D(0,1,0); // no idea
	type = mld.type;
	parent = mld.bone;
	ambColor.init(mld.ambColor, f, global);
	ambIntensity.init(mld.ambIntensity, f, global);
	diffColor.init(mld.color, f, global);
	diffIntensity.init(mld.intensity, f, global);
}

void ModelLight::setup(int time, GLuint l)
{
	Vec4D ambcol(ambColor.getValue(0, time) * ambIntensity.getValue(0, time), 1.0f);
	Vec4D diffcol(diffColor.getValue(0, time) * diffIntensity.getValue(0, time), 1.0f);
	Vec4D p;
	if (type==0)
	{
		// directional
		p = Vec4D(tdir, 0.0f);
	}
	else
	{
		// point
		p = Vec4D(tpos, 1.0f);
	}
	//gLog("[World of Warcraft Studio - Editor] - Light %d (%f,%f,%f) (%f,%f,%f) [%f,%f,%f]\n", l-GL_LIGHT4, ambcol.x, ambcol.y, ambcol.z, diffcol.x, diffcol.y, diffcol.z, p.x, p.y, p.z);
	glLightfv(l, GL_POSITION, p);
	glLightfv(l, GL_DIFFUSE, diffcol);
	glLightfv(l, GL_AMBIENT, ambcol);
	glEnable(l);
}

void TextureAnim::init(MPQFile &f, ModelTexAnimDef &mta, int *global)
{
	trans.init(mta.trans, f, global);
	rot.init(mta.rot, f, global);
	scale.init(mta.scale, f, global);
}

void Bone::init(MPQFile &f, ModelBoneDef &b, int *global)
{
	parent = b.parent;
	pivot = fixCoordSystem(b.pivot);
	billboard = (b.flags & 8) != 0;

	trans.init(b.translation, f, global);
	rot.init(b.rotation, f, global);
	scale.init(b.scaling, f, global);
	trans.fix(fixCoordSystem);
	rot.fix(fixCoordSystemQuat);
	scale.fix(fixCoordSystem2);
}

void Bone::calcMatrix(Bone *allbones, int anim, int time)
{
	if (calc) return;
	Matrix m;
	Quaternion q;

	bool tr = rot.used || scale.used || trans.used || billboard;
	if (tr)
	{
		m.translation(pivot);
		
		if (trans.used)
		{
			Vec3D tr = trans.getValue(anim, time);
			m *= Matrix::newTranslation(tr);
		}
		if (rot.used)
		{
			q = rot.getValue(anim, time);
			m *= Matrix::newQuatRotate(q);
		}
		if (scale.used)
		{
			Vec3D sc = scale.getValue(anim, time);
			m *= Matrix::newScale(sc);
		}
		if (billboard)
		{
			Matrix mtrans;
			glGetFloatv(GL_MODELVIEW_MATRIX, &(mtrans.m[0][0]));
			mtrans.transpose();
			mtrans.invert();
			Vec3D camera = mtrans * Vec3D(0,0,0);
			Vec3D look = (camera - pivot).normalize();
			//Vec3D up(0,1,0);
			Vec3D up = ((mtrans * Vec3D(0,1,0)) - camera).normalize();
			// these should be normalized by default but fp inaccuracy kicks in when looking down :(
			Vec3D right = (up % look).normalize();
			up = (look % right).normalize();

			// calculate a billboard matrix
			Matrix mbb;
			mbb.unit();
			mbb.m[0][2] = right.x;
			mbb.m[1][2] = right.y;
			mbb.m[2][2] = right.z;
			mbb.m[0][1] = up.x;
			mbb.m[1][1] = up.y;
			mbb.m[2][1] = up.z;
			mbb.m[0][0] = look.x;
			mbb.m[1][0] = look.y;
			mbb.m[2][0] = look.z;
			/*
			mbb.m[0][1] = right.x;
			mbb.m[1][1] = right.y;
			mbb.m[2][1] = right.z;
			mbb.m[0][2] = up.x;
			mbb.m[1][2] = up.y;
			mbb.m[2][2] = up.z;
			mbb.m[0][0] = look.x;
			mbb.m[1][0] = look.y;
			mbb.m[2][0] = look.z;
			*/
			m *= mbb;
		}

		m *= Matrix::newTranslation(pivot*-1.0f);
	}
	else
		m.unit();

	if (parent>=0)
	{
		allbones[parent].calcMatrix(allbones, anim, time);
		mat = allbones[parent].mat * m;
	} else mat = m;

	// transform matrix for normal vectors ... ??
	if (rot.used)
	{
		if (parent>=0)
		{
			mrot = allbones[parent].mrot * Matrix::newQuatRotate(q);
		}
		else
			mrot = Matrix::newQuatRotate(q);
	}
	else
		mrot.unit();

	calc = true;
}

void Model::draw()
{
	if(Reloaded)
	{
		reloadModel->draw();
		return;
	}
	if (!ok) return;

	if (!animated)
	{
		glCallList(dlist);
	}
	else
	{
		if (ind) animate(0);
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}
		lightsOn(GL_LIGHT4);
        drawModel();
		lightsOff(GL_LIGHT4);

		// effects are unfogged..?
		glDisable(GL_FOG);

		// draw particle systems
		for (size_t i=0; i<header.nParticleEmitters; i++)
		{
			//particleSystems[i].draw();
		}

		// draw ribbons
		for (size_t i=0; i<header.nRibbonEmitters; i++)
		{
			ribbons[i].draw();
		}

		if (gWorld && gWorld->drawfog) glEnable(GL_FOG);
	}
}

void Model::drawTileMode()
{
	if(Reloaded)
	{
		reloadModel->draw();
		return;
	}
	if (!ok) return;

	if (!animated)
	{
		glCallList(tmlist);
	}
	else
	{
		if (ind) animate(0);
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}
        drawModelTileMode();


		// draw particle systems
		for (size_t i=0; i<header.nParticleEmitters; i++)
		{
			particleSystems[i].draw();
		}

		// draw ribbons
		for (size_t i=0; i<header.nRibbonEmitters; i++)
		{
			ribbons[i].draw();
		}
	}
}

void Model::drawHighlight()
{
	if(Reloaded)
	{
		reloadModel->drawHighlight();
		return;
	}
	if (!ok) return;

	if (!animated)
	{
		glCallList(dlist);
		if(HColor==0)
			glCallList(ghlist);
		else
			glCallList(rhlist);
	}
	else
	{
		if (ind)
			animate(0);
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}
		// effects are unfogged..?
		
		lightsOn(GL_LIGHT4);
        drawModel();
		lightsOff(GL_LIGHT4);

		glDisable(GL_FOG);

		// draw particle systems
		for (size_t i=0; i<header.nParticleEmitters; i++)
		{
			particleSystems[i].draw();
		}

		// draw ribbons
		for (size_t i=0; i<header.nRibbonEmitters; i++)
		{
			ribbons[i].draw();
		}

		//Highlighting
		// draw particle systems
		for (size_t i=0; i<header.nParticleEmitters; i++)
		{
			particleSystems[i].drawHighlight();
		}
		drawModelHighlight();

		if (gWorld && gWorld->drawfog) glEnable(GL_FOG);
	}
	if (gWorld && gWorld->drawfog)
		glEnable(GL_FOG);
}

void startTimer();
int stopTimer2();

int	modelTimes[15];

void initModelTimer()
{
	for(int i=0;i<15;i++)
		modelTimes[i]=0;
}

void stopModelTimer(int i)
{
	modelTimes[i]+=stopTimer2();
}

void reportModelTimes()
{
	gLog("[World of Warcraft Studio - Editor] - Total Model Time %d ms\n",modelTimes[0]);
	gLog("[World of Warcraft Studio - Editor] - Reload Time %d ms\n",modelTimes[1]);
	gLog("[World of Warcraft Studio - Editor] - Total Model Drawing Time %d ms\n",modelTimes[2]);
	gLog("[World of Warcraft Studio - Editor] - Calling Display Lists Time %d ms\n",modelTimes[3]);
	gLog("[World of Warcraft Studio - Editor] - Model Drawing Time %d ms\n",modelTimes[4]);
	gLog("[World of Warcraft Studio - Editor] - Particle Time %d ms\n",modelTimes[6]);
	gLog("[World of Warcraft Studio - Editor] - Ribbon Time %d ms\n",modelTimes[7]);
	gLog("[World of Warcraft Studio - Editor] - Reinit Time %d ms\n",modelTimes[5]);
	initModelTimer();
}

void Model::drawSelect()
{
	//startTimer();
	//startTimer();
	if(Reloaded)
	{
		reloadModel->drawSelect();
		return;
	}
	//stopModelTimer(1);
	if (!ok)
		return;
	
	//startTimer();
	if (!animated)
	{
		//startTimer();
		glCallList(slist);
		//drawModelSelect();
		//stopModelTimer(3);
	}
	else
	{
		if (ind) animate(0);
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}
		//startTimer();
        drawModelSelect();
		//stopModelTimer(4);

		//startTimer();

		// effects are unfogged..?
		glDisable(GL_FOG);

		// draw particle systems
		/*for (size_t i=0; i<header.nParticleEmitters; i++)
		{
			particleSystems[i].draw();
		}*/
		//stopModelTimer(6);


		//startTimer();
		// draw ribbons
		for (size_t i=0; i<header.nRibbonEmitters; i++)
		{
			ribbons[i].draw();
		}
		//stopModelTimer(7);

		//startTimer();
		if (gWorld && gWorld->drawfog)
			glEnable(GL_FOG);
		//stopModelTimer(5);
	}
	//stopModelTimer(2);
	//stopModelTimer(0);
}

void Model::lightsOn(GLuint lbase)
{
	// setup lights
	for (unsigned int i=0, l=lbase; i<header.nLights; i++) lights[i].setup(animtime, l++);
}

void Model::lightsOff(GLuint lbase)
{
	for (unsigned int i=0, l=lbase; i<header.nLights; i++) glDisable(l++);
}

void Model::updateEmitters(float dt)
{
	if(Reloaded)
	{
		reloadModel->updateEmitters(dt);
		return;
	}
	if (!ok) return;
	for (size_t i=0; i<header.nParticleEmitters; i++)
	{
		particleSystems[i].update(dt);
	}
}

int ModelManager::add(std::string name)
{
	int id;
	if (names.find(name) != names.end())
	{
		id = names[name];
		items[id]->addref();

		return id;
	}
	// load new
	Model *model = new Model(name);
	id = nextID();
    do_add(name, id, model);

    return id;
}

void ModelManager::reload()
{
	gLog("[World of Warcraft Studio - Editor] - Reloading Models\n");
	for (std::map<std::string, int>::iterator it = names.begin(); it != names.end(); ++it)
		((Model*)items[(*it).second])->reload((*it).first);
}

void ModelManager::resetAnim()
{
	for (std::map<int, ManagedItem*>::iterator it = items.begin(); it != items.end(); ++it)
	{
		((Model*)it->second)->animcalc = false;
	}
}

void ModelManager::updateEmitters(float dt)
{
	for (std::map<int, ManagedItem*>::iterator it = items.begin(); it != items.end(); ++it)
	{
		((Model*)it->second)->updateEmitters(dt);
	}
}

ModelInstance::ModelInstance()
{
	//nameID=addNameModel(this);
}

ModelInstance::ModelInstance(Model *m, MPQFile &f) : model (m)
{
	float ff[3];
	
    f.read(&d1, 4);
	f.read(ff,12);
	pos = Vec3D(ff[0],ff[1],ff[2]);
	f.read(ff,12);
	dir = Vec3D(ff[0],ff[1],ff[2]);
	f.read(&scale,4);
	// scale factor - divide by 1024. blizzard devs must be on crack, why not just use a float?
	sc = scale / 1024.0f;
	nameID=-1;
	modelID=d1;
}

ModelInstance::ModelInstance(Model *m, MDDF *d) : model (m)
{
	d1 = d->uniqueID;
	pos = Vec3D(d->pos[0],d->pos[1],d->pos[2]);
	dir = Vec3D(d->rot[0],d->rot[1],d->rot[2]);
	scale=d->scale;
	// scale factor - divide by 1024. blizzard devs must be on crack, why not just use a float?
	sc = scale / 1024.0f;
	nameID=-1;
	modelID=d1;
}

void ModelInstance::init2(Model *m, MPQFile &f)
{
	nameID=-1;
	model = m;
	addNameModel(this);
	float ff[3],temp;
	f.read(ff,12);
	pos = Vec3D(ff[0],ff[1],ff[2]);
	temp = pos.z;
	pos.z = -pos.y;
	pos.y = temp;
	f.read(&w,4);
	f.read(ff,12);
	dir = Vec3D(ff[0],ff[1],ff[2]);
	f.read(&sc,4);
	f.read(&d1,4);
	lcol = Vec3D(((d1&0xff0000)>>16) / 255.0f, ((d1&0x00ff00)>>8) / 255.0f, (d1&0x0000ff) / 255.0f);
}

extern nameEntry *Selection;
extern nameEntry *CurSelection;

void ModelInstance::draw()
{
	//if ((pos - gWorld->camera).lengthSquared() > (gWorld->modeldrawdistance2+(model->rad*model->rad*sc))) return;
	float dist = (pos - gWorld->camera).length() - model->rad;
	if (dist > 2*gWorld->modeldrawdistance) return;
	if (!gWorld->frustum.intersectsSphere(pos, model->rad*sc)) return;
	if(CheckUniques(d1)) return;
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	glScalef(sc,sc,sc);

	if(nameID==-1)
		nameID=addNameModel(this);
	glPushName(nameID);
	if((CurSelection!=0)&&(CurSelection->type==ENTRY_MODEL)&&(CurSelection->data.model->d1==d1))
	{
		HColor=0;
		model->drawHighlight();
	}
	else if((Selection!=0)&&(Selection->type==ENTRY_MODEL)&&(Selection->data.model->d1==d1))
	{
		HColor=1;		
		model->drawHighlight();
	}
	else
		model->draw();

	glPopName();
	glPopMatrix();
}

void ModelInstance::drawMapTile()
{
	//if ((pos - gWorld->camera).lengthSquared() > (gWorld->modeldrawdistance2+(model->rad*model->rad*sc))) return;
	if(CheckUniques(d1)) return;
	glPushMatrix();
	glTranslatef(pos.x/CHUNKSIZE, pos.z/CHUNKSIZE, pos.y);

	glRotatef(-90.0f, 1, 0, 0);

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	glScalef(1/CHUNKSIZE,1/CHUNKSIZE,1/CHUNKSIZE);
	glScalef(sc,sc,sc);

	if(nameID==-1)
		nameID=addNameModel(this);

	glPushName(nameID);
	model->draw();
	glPopName();
	glPopMatrix();
}

void ModelInstance::drawHighlight()
{
	//if ((pos - gWorld->camera).lengthSquared() > (gWorld->modeldrawdistance2+(model->rad*model->rad*sc))) return;
	float dist = (pos - gWorld->camera).length() - model->rad;
	if (dist > gWorld->modeldrawdistance) return;
	if (!gWorld->frustum.intersectsSphere(pos, model->rad*sc))
		return;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	glScalef(sc,sc,sc);

	if(nameID==-1)
		nameID=addNameModel(this);

	glPushName(nameID);
	model->drawHighlight();
	glPopName();
	glPopMatrix();
}

void ModelInstance::drawSelect()
{
	//if ((pos - gWorld->camera).lengthSquared() > (gWorld->modeldrawdistance2+(model->rad*model->rad*sc))) return;
	float dist = (pos - gWorld->camera).length() - model->rad;
	if (dist > gWorld->modeldrawdistance)
		return;
	if (!gWorld->frustum.intersectsSphere(pos, model->rad*sc))
		return;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	glScalef(sc,sc,sc);

	if(nameID==-1)
		nameID=addNameModel(this);

	glPushName(nameID);
	model->drawSelect();
	glPopName();
	glPopMatrix();
}

ModelInstance::~ModelInstance()
{
	if(nameID!=-1)
	{
		deleteName(nameID);
		nameID=-1;
	}
}

void glQuaternionRotate(const Vec3D& vdir, float w)
{
	Matrix m;
	Quaternion q(vdir, w);
	m.quaternionRotate(q);
	glMultMatrixf(m);
}

void ModelInstance::draw2(const Vec3D& ofs, const float rot)
{
	Vec3D tpos(ofs + pos);
	rotate(ofs.x,ofs.z,&tpos.x,&tpos.z,rot*PI/180.0f);
	//if ( (tpos - gWorld->camera).lengthSquared() > (gWorld->doodaddrawdistance2*model->rad*sc) ) return;
	if (!gWorld->frustum.intersectsSphere(tpos, model->rad*sc))
		return;

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	Vec3D vdir(-dir.z,dir.x,dir.y);
	glQuaternionRotate(vdir,w);
	glScalef(sc,-sc,-sc);

	model->draw();
	glPopMatrix();
}

void ModelInstance::draw2Select(const Vec3D& ofs, const float rot)
{
	Vec3D tpos(ofs + pos);
	rotate(ofs.x,ofs.z,&tpos.x,&tpos.z,rot*PI/180.0f);
	if ( (tpos - gWorld->camera).lengthSquared() > (gWorld->doodaddrawdistance2*model->rad*sc) )
		return;
	if (!gWorld->frustum.intersectsSphere(tpos, model->rad*sc))
		return;

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	Vec3D vdir(-dir.z,dir.x,dir.y);
	glQuaternionRotate(vdir,w);
	glScalef(sc,-sc,-sc);

	model->drawSelect();
	glPopMatrix();
}

struct ltint
{
  bool operator()(int i1, int i2) const
  {
    return i1<i2;
  }
};

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

struct ModelEntry
{
	int				count;
	ModelInstance	*instance;
};

std::map<int, ModelEntry, ltint>	ModelList;

int addModelToList(Model *m, MPQFile &f)
{
	int id;
	f.read(&id,4);
	f.seekRelative(-4);

	if(ModelList[id].count==0)
		ModelList[id].instance=new ModelInstance(m,f);
	else
		f.seekRelative(32);
	ModelList[id].count++;

	return id;
}

void removeModels(int id)
{
	ModelList[id].count--;
	if(ModelList[id].count==0)
		ModelList.erase(ModelList.find(id));
}

void drawModelList()
{
	for(std::map<int, ModelEntry, ltint>::iterator i=ModelList.begin();i!=ModelList.end();i++)
		(*i).second.instance->draw();
}

void drawModelListSelect()
{
	for(std::map<int, ModelEntry, ltint>::iterator i=ModelList.begin();i!=ModelList.end();i++)
		(*i).second.instance->drawSelect();
}