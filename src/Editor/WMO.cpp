#include "WMO.h"
#include "World.h"
#include "Liquid.h"

using namespace std;

WMO::WMO(std::string name): ManagedItem(name)
{
	MPQFile f(name.c_str());
	ok = !f.isEof();
	if (!ok) {
		gLog("Error loading WMO %s\n", name.c_str());
		return;
	}
	Reloaded=false;
	reloadWMO=0;

	if(!f.isExternal())
		gLog("    Loading WMO from MPQ %s\n", name.c_str());
	else
		gLog("    Loading WMO from File %s\n", name.c_str());

	char fourcc[5];
	size_t size;
	float ff[3];

	char *ddnames;
	char *groupnames;

	skybox = 0;

	char *texbuf=0;

	while (!f.isEof()) {
		f.read(fourcc,4);
		f.read(&size, 4);

		flipcc(fourcc);
		fourcc[4] = 0;

		size_t nextpos = f.getPos() + size;

		if (!strcmp(fourcc,"MOHD")) {
			unsigned int col;
			// header
			f.read(&nTextures, 4);
			f.read(&nGroups, 4);
			f.read(&nP, 4);
			f.read(&nLights, 4);
			f.read(&nModels, 4);
			f.read(&nDoodads, 4);
			f.read(&nDoodadSets, 4);
			f.read(&col, 4);
			f.read(&nX, 4);
			f.read(ff,12);
			extents[0] = Vec3D(ff[0],ff[1],ff[2]);
			f.read(ff,12);
			extents[1] = Vec3D(ff[0],ff[1],ff[2]);

			groups = new WMOGroup[nGroups];
			mat = new WMOMaterial[nTextures];

		}
		else if (!strcmp(fourcc,"MOTX")) {
			// textures
			texbuf = new char[size];
			f.read(texbuf, size);
		}
		else if (!strcmp(fourcc,"MOMT")) {
			// materials
			//WMOMaterialBlock bl;

			for (int i=0; i<nTextures; i++) {
				WMOMaterial *m = &mat[i];
				f.read(m, 0x40);

				string texpath(texbuf+m->nameStart);
				fixname(texpath);

				m->tex = video.textures.add(texpath);
				textures.push_back(texpath);

				/*
				// material logging
				gLog("Material %d:\t%d\t%d\t%d\t%X\t%d\t%X\t%d\t%f\t%f",
					i, m->flags, m->d1, m->transparent, m->col1, m->d3, m->col2, m->d4, m->f1, m->f2);
				for (int j=0; j<5; j++) gLog("\t%d", m->dx[j]);
				gLog("\t - %s\n", texpath.c_str());
				*/
				
			}
		}
		else if (!strcmp(fourcc,"MOGN")) {
			groupnames = (char*)f.getPointer();
		}
		else if (!strcmp(fourcc,"MOGI")) {
			// group info - important information! ^_^
			for (int i=0; i<nGroups; i++) {
				groups[i].init(this, f, i, groupnames);

			}
		}
		else if (!strcmp(fourcc,"MOLT")) {
			// Lights?
			for (int i=0; i<nLights; i++) {
				WMOLight l;
				l.init(f);
				lights.push_back(l);
			}
		}
		else if (!strcmp(fourcc,"MODN")) {
			// models ...
			// MMID would be relative offsets for MMDX filenames
			if (size) {

				ddnames = f.getPointer();
				fixnamen(ddnames, size);

				char *p=ddnames,*end=p+size;
				int t=0;
				while (p<end) {
					string path(p);
					p+=strlen(p)+1;
					while ((p<end) && (*p==0)) p++;

					gWorld->modelmanager.add(path);
					models.push_back(path);
				}
				f.seekRelative((int)size);
			}
		}
		else if (!strcmp(fourcc,"MODS")) {
			for (int i=0; i<nDoodadSets; i++) {
				WMODoodadSet dds;
				f.read(&dds, 32);
				doodadsets.push_back(dds);
			}
		}
		else if (!strcmp(fourcc,"MODD")) {
			nModels = (int)size / 0x28;
			for (int i=0; i<nModels; i++) {
				int ofs;
				f.read(&ofs,4);
				Model *m = (Model*)gWorld->modelmanager.items[gWorld->modelmanager.get(ddnames + ofs)];
				ModelInstance mi;
				mi.init2(m,f);
				modelis.push_back(mi);
			}

		}
		else if (!strcmp(fourcc,"MOSB")) {
			if (size>4) {
				string path = f.getPointer();
				fixname(path);
				if (path.length()) {
					gLog("SKYBOX:\n");

					sbid = gWorld->modelmanager.add(path);
					skybox = (Model*)gWorld->modelmanager.items[sbid];

					if (!skybox->ok) {
						gWorld->modelmanager.del(sbid);
						skybox = 0;
					}
				}
			}
		}
		else if (!strcmp(fourcc,"MOPV")) {
			WMOPV p;
			for (int i=0; i<nP; i++) {
				f.read(ff,12);
				p.a = Vec3D(ff[0],ff[2],-ff[1]);
				f.read(ff,12);
				p.b = Vec3D(ff[0],ff[2],-ff[1]);
				f.read(ff,12);
				p.c = Vec3D(ff[0],ff[2],-ff[1]);
				f.read(ff,12);
				p.d = Vec3D(ff[0],ff[2],-ff[1]);
				pvs.push_back(p);
			}
		}
		else if (!strcmp(fourcc,"MOPR")) {
			int nn = (int)size / 8;
			WMOPR *pr = (WMOPR*)f.getPointer();
			for (int i=0; i<nn; i++) {
				prs.push_back(*pr++);
			}
		}
		else if (!strcmp(fourcc,"MFOG")) {
			int nfogs = (int)size / 0x30;
			for (int i=0; i<nfogs; i++) {
				WMOFog fog;
				fog.init(f);
				fogs.push_back(fog);
			}
		}

		f.seek((int)nextpos);
	}

	f.close();
	delete[] texbuf;
	Reloaded=false;
	reloadWMO=0;

	for (int i=0; i<nGroups; i++) groups[i].initDisplayList();
	Reloaded=false;
}

WMO::~WMO()
{
	if (ok) {
		gLog("Unloading WMO %s\n", name.c_str());
		delete[] groups;

		for (vector<string>::iterator it = textures.begin(); it != textures.end(); ++it) {
            video.textures.delbyname(*it);
		}

		for (vector<string>::iterator it = models.begin(); it != models.end(); ++it) {
			gWorld->modelmanager.delbyname(*it);
		}

		delete[] mat;

		if (skybox) {
			//delete skybox;
			gWorld->modelmanager.del(sbid);
		}
		if((Reloaded)&&((unsigned int)reloadWMO!=0xbaadf00d))
			delete reloadWMO;
	}
}

void WMO::draw(int doodadset, const Vec3D &ofs, const float rot)
{
	if((Reloaded)&&((unsigned int)reloadWMO!=0xbaadf00d))
	{
		reloadWMO->draw(doodadset,ofs,rot);
		return;
	}
	if (!ok) return;
	
	for (int i=0; i<nGroups; i++) {
		groups[i].draw(ofs, rot);
	}

	if (gWorld->drawdoodads) {
		for (int i=0; i<nGroups; i++) {
			groups[i].drawDoodads(doodadset, ofs, rot);
		}
	}

	for (int i=0; i<nGroups; i++) {
		groups[i].drawLiquid();
	}

	/*
	// draw light placeholders
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nLights; i++) {
		glColor4fv(lights[i].fcolor);
		glVertex3fv(lights[i].pos);
		glVertex3fv(lights[i].pos + Vec3D(-0.5f,1,0));
		glVertex3fv(lights[i].pos + Vec3D(0.5f,1,0));
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	*/

	/*
	// draw fog positions..?
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (size_t i=0; i<fogs.size(); i++) {
		WMOFog &fog = fogs[i];
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
		glVertex3fv(fog.pos);
		glVertex3fv(fog.pos + Vec3D(fog.rad1, 5, -fog.rad2));
		glVertex3fv(fog.pos + Vec3D(fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + Vec3D(-fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + Vec3D(-fog.rad1, 5, -fog.rad2));
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	*/

	/*
	// draw group boundingboxes
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (int i=0; i<nGroups; i++) {
		WMOGroup &g = groups[i];
		float fc[2] = {1,0};
		glColor4f(fc[i%2],fc[(i/2)%2],fc[(i/3)%2],1);
		glBegin(GL_LINE_LOOP);

		glVertex3f(g.b1.x, g.b1.y, g.b1.z);
		glVertex3f(g.b1.x, g.b2.y, g.b1.z);
		glVertex3f(g.b2.x, g.b2.y, g.b1.z);
		glVertex3f(g.b2.x, g.b1.y, g.b1.z);

		glVertex3f(g.b2.x, g.b1.y, g.b2.z);
		glVertex3f(g.b2.x, g.b2.y, g.b2.z);
		glVertex3f(g.b1.x, g.b2.y, g.b2.z);
		glVertex3f(g.b1.x, g.b1.y, g.b2.z);

		glEnd();
	}
	// draw portal relations
	glBegin(GL_LINES);
	for (size_t i=0; i<prs.size(); i++) {
		WMOPR &pr = prs[i];
		WMOPV &pv = pvs[pr.portal];
		if (pr.dir>0) glColor4f(1,0,0,1);
		else glColor4f(0,0,1,1);
		Vec3D pc = (pv.a+pv.b+pv.c+pv.d)*0.25f;
		Vec3D gc = (groups[pr.group].b1 + groups[pr.group].b2)*0.5f;
		glVertex3fv(pc);
		glVertex3fv(gc);
	}
	glEnd();
	glColor4f(1,1,1,1);
	// draw portals
	for (int i=0; i<nP; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3fv(pvs[i].d);
		glVertex3fv(pvs[i].c);
		glVertex3fv(pvs[i].b);
		glVertex3fv(pvs[i].a);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	*/
}

void WMO::drawSelect(int doodadset, const Vec3D &ofs, const float rot)
{
	if((Reloaded)&&((unsigned int)reloadWMO!=0xbaadf00d))
	{
		reloadWMO->drawSelect(doodadset,ofs,rot);
		return;
	}
	if (!ok) return;
	
	for (int i=0; i<nGroups; i++) {
		groups[i].draw(ofs, rot);
	}

	if (gWorld->drawdoodads) {
		for (int i=0; i<nGroups; i++) {
			groups[i].drawDoodadsSelect(doodadset, ofs, rot);
		}
	}

	for (int i=0; i<nGroups; i++) {
		groups[i].drawLiquid();
	}

	/*
	// draw light placeholders
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nLights; i++) {
		glColor4fv(lights[i].fcolor);
		glVertex3fv(lights[i].pos);
		glVertex3fv(lights[i].pos + Vec3D(-0.5f,1,0));
		glVertex3fv(lights[i].pos + Vec3D(0.5f,1,0));
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	*/

	/*
	// draw fog positions..?
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (size_t i=0; i<fogs.size(); i++) {
		WMOFog &fog = fogs[i];
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
		glVertex3fv(fog.pos);
		glVertex3fv(fog.pos + Vec3D(fog.rad1, 5, -fog.rad2));
		glVertex3fv(fog.pos + Vec3D(fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + Vec3D(-fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + Vec3D(-fog.rad1, 5, -fog.rad2));
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	*/

	/*
	// draw group boundingboxes
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (int i=0; i<nGroups; i++) {
		WMOGroup &g = groups[i];
		float fc[2] = {1,0};
		glColor4f(fc[i%2],fc[(i/2)%2],fc[(i/3)%2],1);
		glBegin(GL_LINE_LOOP);

		glVertex3f(g.b1.x, g.b1.y, g.b1.z);
		glVertex3f(g.b1.x, g.b2.y, g.b1.z);
		glVertex3f(g.b2.x, g.b2.y, g.b1.z);
		glVertex3f(g.b2.x, g.b1.y, g.b1.z);

		glVertex3f(g.b2.x, g.b1.y, g.b2.z);
		glVertex3f(g.b2.x, g.b2.y, g.b2.z);
		glVertex3f(g.b1.x, g.b2.y, g.b2.z);
		glVertex3f(g.b1.x, g.b1.y, g.b2.z);

		glEnd();
	}
	// draw portal relations
	glBegin(GL_LINES);
	for (size_t i=0; i<prs.size(); i++) {
		WMOPR &pr = prs[i];
		WMOPV &pv = pvs[pr.portal];
		if (pr.dir>0) glColor4f(1,0,0,1);
		else glColor4f(0,0,1,1);
		Vec3D pc = (pv.a+pv.b+pv.c+pv.d)*0.25f;
		Vec3D gc = (groups[pr.group].b1 + groups[pr.group].b2)*0.5f;
		glVertex3fv(pc);
		glVertex3fv(gc);
	}
	glEnd();
	glColor4f(1,1,1,1);
	// draw portals
	for (int i=0; i<nP; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3fv(pvs[i].d);
		glVertex3fv(pvs[i].c);
		glVertex3fv(pvs[i].b);
		glVertex3fv(pvs[i].a);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	*/
}

void WMO::drawSkybox()
{
	if((Reloaded)&&((unsigned int)reloadWMO!=0xbaadf00d))
	{
		reloadWMO->drawSkybox();
		return;
	}

	if (skybox&&((unsigned int)skybox!=0xbaadf00d)) {
		// TODO: only draw sky if we are "inside" the WMO... ?

		// We need to clear the depth buffer, because the skybox model can (will?)
		// require it *. This is inefficient - is there a better way to do this?
		// * planets in front of "space" in Caverns of Time
		//glClear(GL_DEPTH_BUFFER_BIT);

		// update: skybox models seem to have an explicit renderop ordering!
		// that saves us the depth buffer clear and the depth testing, too

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		Vec3D o = gWorld->camera;
		glTranslatef(o.x, o.y, o.z);
		const float sc = 2.0f;
		glScalef(sc,sc,sc);
        skybox->draw();
		glPopMatrix();
		gWorld->hadSky = true;
		glEnable(GL_DEPTH_TEST);
	}
}

/*
void WMO::drawPortals()
{
	// not used ;)
	glBegin(GL_QUADS);
	for (int i=0; i<nP; i++) {
		glVertex3fv(pvs[i].d);
		glVertex3fv(pvs[i].c);
		glVertex3fv(pvs[i].b);
		glVertex3fv(pvs[i].a);
	}
	glEnd();
}
*/

void WMOLight::init(MPQFile &f)
{
	char type[4];
	f.read(&type,4);
	f.read(&color,4);
	f.read(pos, 12);
	f.read(&intensity, 4);
	f.read(unk, 4*5);
	f.read(&r,4);

	pos = Vec3D(pos.x, pos.z, -pos.y);

	// rgb? bgr? hm
	float fa = ((color & 0xff000000) >> 24) / 255.0f;
	float fr = ((color & 0x00ff0000) >> 16) / 255.0f;
	float fg = ((color & 0x0000ff00) >>  8) / 255.0f;
	float fb = ((color & 0x000000ff)      ) / 255.0f;

	fcolor = Vec4D(fr,fg,fb,fa);
	fcolor *= intensity;
	fcolor.w = 1.0f;

	/*
	// light logging
	gLog("Light %08x @ (%4.2f,%4.2f,%4.2f)\t %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f\t(%d,%d,%d,%d)\n",
		color, pos.x, pos.y, pos.z, intensity,
		unk[0], unk[1], unk[2], unk[3], unk[4], r,
		type[0], type[1], type[2], type[3]);
	*/
}

void WMOLight::setup(GLint light)
{
	// not used right now -_-

	GLfloat LightAmbient[] = {0, 0, 0, 1.0f};
	GLfloat LightPosition[] = {pos.x, pos.y, pos.z, 0.0f};

	glLightfv(light, GL_AMBIENT, LightAmbient);
	glLightfv(light, GL_DIFFUSE, fcolor);
	glLightfv(light, GL_POSITION,LightPosition);

	glEnable(light);
}

void WMOLight::setupOnce(GLint light, Vec3D dir, Vec3D lcol)
{
	Vec4D position(dir, 0);
	//Vec4D position(0,1,0,0);

	Vec4D ambient = Vec4D(lcol * 0.3f, 1);
	//Vec4D ambient = Vec4D(0.101961f, 0.062776f, 0, 1);
	Vec4D diffuse = Vec4D(lcol, 1);
	//Vec4D diffuse = Vec4D(0.439216f, 0.266667f, 0, 1);

	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_POSITION,position);
	
	glEnable(light);
}

void WMOGroup::init(WMO *wmo, MPQFile &f, int num, char *names)
{
	this->wmo = wmo;
	this->num = num;

	// extract group info from f
	f.read(&flags,4);
	float ff[3];
	f.read(ff,12);
	v1 = Vec3D(ff[0],ff[1],ff[2]);
	f.read(ff,12);
	v2 = Vec3D(ff[0],ff[1],ff[2]);
	int nameOfs;
	f.read(&nameOfs,4);

	// TODO: get proper name from group header and/or dbc?
	if (nameOfs > 0) {
        name = string(names + nameOfs);
	} else name = "(no name)";

	ddr = 0;
	nDoodads = 0;

	lq = 0;
}

struct WMOBatch {
	signed char bytes[12];
	unsigned int indexStart;
	unsigned short indexCount, vertexStart, vertexEnd;
	unsigned char flags, texture;
};

void setGLColor(unsigned int col)
{
	//glColor4ubv((GLubyte*)(&col));
	GLubyte r,g,b,a;
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
    glColor4ub(r,g,b,1);
}

struct WMOGroupHeader {
    int nameStart, nameStart2, flags;
	float box1[3], box2[3];
	short portalStart, portalCount;
	short batches[4];
	uint8 fogs[4];
	int32 unk1, id, unk2, unk3;
};

void WMOGroup::initDisplayList()
{
	Vec3D *vertices, *normals;
	Vec2D *texcoords;
	unsigned short *indices;
	unsigned short *materials;
	WMOBatch *batches;
	int nBatches;

	WMOGroupHeader gh;

	short *useLights = 0;
	int nLR = 0;

	// open group file
	char temp[256];
	strcpy_s(temp, wmo->name.c_str());
    temp[wmo->name.length()-4] = 0;
	
	char fname[256];
	sprintf_s(fname,"%s_%03d.wmo",temp, num);

	MPQFile gf(fname);
    ok = !gf.isEof();
	if (!ok) {
		gLog("Error loading WMO %s\n", fname);
		return;
	}
	
	if(!gf.isExternal())
		gLog("    Loading WMO from MPQ %s\n", fname);
	else
		gLog("    Loading WMO from File %s\n", fname);
	
	gf.seek(0x14);
	// read header
	gf.read(&gh, sizeof(WMOGroupHeader));
	WMOFog &wf = wmo->fogs[gh.fogs[0]];
	if (wf.r2 <= 0) fog = -1; // default outdoor fog..?
	else fog = gh.fogs[0];

	b1 = Vec3D(gh.box1[0], gh.box1[2], -gh.box1[1]);
	b2 = Vec3D(gh.box2[0], gh.box2[2], -gh.box2[1]);

	gf.seek(0x58); // first chunk
	char fourcc[5];
	size_t size;

	unsigned int *cv;
	hascv = false;

	while (!gf.isEof()) {
		gf.read(fourcc,4);
		gf.read(&size, 4);

		flipcc(fourcc);
		fourcc[4] = 0;

		size_t nextpos = gf.getPos() + size;

		// why copy stuff when I can just map it from memory ^_^
		
		if (!strcmp(fourcc,"MOPY")) {
			// materials per triangle
			nTriangles = (int)size / 2;
			materials = (unsigned short*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MOVI")) {
			// indices
			indices =  (unsigned short*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MOVT")) {
			nVertices = (int)size / 12;
			// let's hope it's padded to 12 bytes, not 16...
			vertices =  (Vec3D*)gf.getPointer();
			vmin = Vec3D( 9999999.0f, 9999999.0f, 9999999.0f);
			vmax = Vec3D(-9999999.0f,-9999999.0f,-9999999.0f);
			rad = 0;
			for (int i=0; i<nVertices; i++) {
				Vec3D v(vertices[i].x, vertices[i].z, -vertices[i].y);
				if (v.x < vmin.x) vmin.x = v.x;
				if (v.y < vmin.y) vmin.y = v.y;
				if (v.z < vmin.z) vmin.z = v.z;
				if (v.x > vmax.x) vmax.x = v.x;
				if (v.y > vmax.y) vmax.y = v.y;
				if (v.z > vmax.z) vmax.z = v.z;
			}
			center = (vmax + vmin) * 0.5f;
			rad = (vmax-center).length();
		}
		else if (!strcmp(fourcc,"MONR")) {
			normals =  (Vec3D*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MOTV")) {
			texcoords =  (Vec2D*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MOLR")) {
			nLR = (int)size / 2;
			useLights =  (short*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MODR")) {
			nDoodads = (int)size / 2;
			ddr = new short[nDoodads];
			gf.read(ddr,size);
		}
		else if (!strcmp(fourcc,"MOBA")) {
			nBatches = (int)size / 24;
			batches = (WMOBatch*)gf.getPointer();
			
			/*
			// batch logging
			gLog("\nWMO group #%d - %s\nVertices: %d\nTriangles: %d\nIndices: %d\nBatches: %d\n",
				this->num, this->name.c_str(), nVertices, nTriangles, nTriangles*3, nBatches);
			WMOBatch *ba = batches;
			for (int i=0; i<nBatches; i++) {
				gLog("Batch %d:\t", i);
				
				for (int j=0; j<12; j++) {
					if ((j%4)==0 && j!=0) gLog("| ");
					gLog("%d\t", ba[i].bytes[j]);
				}
				
				gLog("| %d\t%d\t| %d\t%d\t", ba[i].indexStart, ba[i].indexCount, ba[i].vertexStart, ba[i].vertexEnd);
				gLog("%d\t%d\t%s\n", ba[i].flags, ba[i].texture, wmo->textures[ba[i].texture].c_str());

			}
			int l = nBatches-1;
			gLog("Max index: %d\n", ba[l].indexStart + ba[l].indexCount);
			*/
			
		}
		else if (!strcmp(fourcc,"MOCV")) {
			//gLog("CV: %d\n", size);
			hascv = true;
			cv = (unsigned int*)gf.getPointer();
		}
		else if (!strcmp(fourcc,"MLIQ")) {
			// liquids
			WMOLiquidHeader hlq;
			gf.read(&hlq, 0x1E);

			//gLog("WMO Liquid: %dx%d, %dx%d, (%f,%f,%f) %d\n", hlq.X, hlq.Y, hlq.A, hlq.B, hlq.pos.x, hlq.pos.y, hlq.pos.z, hlq.type);

			lq = new Liquid(hlq.A, hlq.B, Vec3D(hlq.pos.x, hlq.pos.z, -hlq.pos.y));
			lq->initFromWMO(gf, wmo->mat[hlq.type], (flags&0x2000)!=0);
		}

		// TODO: figure out/use MFOG ?

 		gf.seek((int)nextpos);
	}

	// ok, make a display list

	indoor = (flags&8192)!=0;
	//gLog("Lighting: %s %X\n\n", indoor?"Indoor":"Outdoor", flags);

	initLighting(nLR,useLights);

	dl = glGenLists(2);
	glNewList(dl, GL_COMPILE);
	glDisable(GL_BLEND);

	glColor4f(1,1,1,1);

	/*
	float xr=0,xg=0,xb=0;
	if (flags & 0x0040) xr = 1;
	if (flags & 0x2000) xg = 1;
	if (flags & 0x8000) xb = 1;
	glColor4f(xr,xg,xb,1);
	*/

	// assume that texturing is on, for unit 1

	for (int b=0; b<nBatches; b++) {
		WMOBatch *batch = &batches[b];
		WMOMaterial *mat = &wmo->mat[batch->texture];

        // setup texture
		glPushName(mat->tex);
		glBindTexture(GL_TEXTURE_2D, mat->tex);

		bool atest = (mat->transparent) != 0;

		if (atest) {
			glEnable(GL_ALPHA_TEST);
			float aval = 0;
            if (mat->flags & 0x80) aval = 0.3f;
			if (mat->flags & 0x01) aval = 0.0f;
			glAlphaFunc(GL_GREATER, aval);
		}

		if (mat->flags & 0x04) glDisable(GL_CULL_FACE);
		else glEnable(GL_CULL_FACE);

		/*
		float fr,fg,fb;
		fr = rand()/(float)RAND_MAX;
		fg = rand()/(float)RAND_MAX;
		fb = rand()/(float)RAND_MAX;
		glColor4f(fr,fg,fb,1);
		*/

		bool overbright = ((mat->flags & 0x10) && !hascv);
		if (overbright) {
			// TODO: use emissive color from the WMO Material instead of 1,1,1,1
			GLfloat em[4] = {1,1,1,1};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}
		
		// render
		glBegin(GL_TRIANGLES);
		for (int t=0, i=batch->indexStart; t<batch->indexCount; t++,i++) {
			int a = indices[i];
			if (indoor && hascv) {
	            setGLColor(cv[a]);
			}
			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glTexCoord2fv(texcoords[a]);
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);
		}
		glEnd();

		if (overbright) {
			GLfloat em[4] = {0,0,0,1};
			glMaterialfv(GL_FRONT, GL_EMISSION, em);
		}

		if (atest) {
			glDisable(GL_ALPHA_TEST);
		}
		glPopName();
	}

	glColor4f(1,1,1,1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glEndList();

	glNewList(dl+1, GL_COMPILE);
	//glEnable(GL_BLEND);
	//glDepthMask(false);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glColor4f(1.0f,0.0f,0.0f,0.5f);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<nTriangles;i++)
	{
		/*float fr,fg,fb;
		fr = rand()/(float)RAND_MAX;
		fg = rand()/(float)RAND_MAX;
		fb = rand()/(float)RAND_MAX;
		glColor4f(fr,fg,fb,1.0f);*/

		int flags2=materials[i]>>8;
		/*float xr=0,xg=0,xb=0;
		if (flags2 & 0x0001) xr = 1;
		if (flags2 & 0x0002) xg = 1;
		if (flags2 & 0x0004) xb = 1;
		glColor4f(xr,xg,xb,1);*/

		if((flags2&1)||((flags2&0x10)==0))
		{
			int a = indices[i*3];
			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);

			a = indices[i*3+1];
			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);
			
			a = indices[i*3+2];
			glNormal3f(normals[a].x, normals[a].z, -normals[a].y);
			glVertex3f(vertices[a].x, vertices[a].z, -vertices[a].y);
		}
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glDepthMask(true);
	glEndList();

	gf.close();

	// hmm
	indoor = false;
}


void WMOGroup::initLighting(int nLR, short *useLights)
{
	if(!ok)
		return;
	dl_light = 0;
	// "real" lighting?
	if ((flags&0x2000) && hascv) {

		Vec3D dirmin(1,1,1);
		float lenmin;
		int lmin;

		for (int i=0; i<nDoodads; i++) {
			lenmin = 999999.0f*999999.0f;
			lmin = 0;
			ModelInstance &mi = wmo->modelis[ddr[i]];
			for (int j=0; j<wmo->nLights; j++) {
				WMOLight &l = wmo->lights[j];
				Vec3D dir = l.pos - mi.pos;
				float ll = dir.lengthSquared();
				if (ll < lenmin) {
					lenmin = ll;
					dirmin = dir;
					lmin = j;
				}
			}
			mi.light = lmin;
			mi.ldir = dirmin;
		}

		outdoorLights = false;
	} else {
		outdoorLights = true;
	}
}

void WMOGroup::draw(const Vec3D& ofs, const float rot)
{
	if(!ok)
		return;
	visible = false;
	// view frustum culling
	Vec3D pos = center + ofs;
	rotate(ofs.x,ofs.z,&pos.x,&pos.z,rot*PI/180.0f);
	if (!gWorld->frustum.intersectsSphere(pos,rad)) return;
	float dist = (pos - gWorld->camera).length() - rad;
	if (dist >= gWorld->culldistance) return;
	visible = true;
	
	if (hascv) {
		glDisable(GL_LIGHTING);
		gWorld->outdoorLights(false);
	} else {
		if (gWorld->lighting) {
			if (gWorld->skies->hasSkies()) {
				gWorld->outdoorLights(true);
			} else {
				// set up some kind of default outdoor light... ?
				glEnable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glLightfv(GL_LIGHT0, GL_AMBIENT, Vec4D(0.4f,0.4f,0.4f,1));
				glLightfv(GL_LIGHT0, GL_DIFFUSE, Vec4D(0.8f,0.8f,0.8f,1));
				glLightfv(GL_LIGHT0, GL_POSITION, Vec4D(1,1,1,0));
			}
		} else glDisable(GL_LIGHTING);
	}
	setupFog();

	glCallList(dl);
	//glCallList(dl+1);

	if (hascv) {
		if (gWorld->lighting) {
			glEnable(GL_LIGHTING);
			//glCallList(dl_light);
		}
	}


}

void WMOGroup::drawDoodads(int doodadset, const Vec3D& ofs, const float rot)
{
	if(!ok)
		return;
	if (!visible) return;
	if (nDoodads==0) return;

	gWorld->outdoorLights(outdoorLights);
	setupFog();

	/*
	float xr=0,xg=0,xb=0;
	if (flags & 0x0040) xr = 1;
	//if (flags & 0x0008) xg = 1;
	if (flags & 0x8000) xb = 1;
	glColor4f(xr,xg,xb,1);
	*/

	// draw doodads
	glColor4f(1,1,1,1);
	for (int i=0; i<nDoodads; i++) {
		short dd = ddr[i];
		if ((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start+wmo->doodadsets[doodadset].size))) {

			ModelInstance &mi = wmo->modelis[dd];

			if (!outdoorLights) {
				WMOLight::setupOnce(GL_LIGHT2, mi.ldir, mi.lcol);
			}
			setupFog();
			wmo->modelis[dd].draw2(ofs,rot);
		}
	}

	glDisable(GL_LIGHT2);

	glColor4f(1,1,1,1);

}

void WMOGroup::drawDoodadsSelect(int doodadset, const Vec3D& ofs, const float rot)
{
	if(!ok)
		return;
	if (!visible) return;
	if (nDoodads==0) return;

	gWorld->outdoorLights(outdoorLights);
	setupFog();

	/*
	float xr=0,xg=0,xb=0;
	if (flags & 0x0040) xr = 1;
	//if (flags & 0x0008) xg = 1;
	if (flags & 0x8000) xb = 1;
	glColor4f(xr,xg,xb,1);
	*/

	// draw doodads
	glColor4f(1,1,1,1);
	for (int i=0; i<nDoodads; i++) {
		short dd = ddr[i];
		if ((dd >= wmo->doodadsets[doodadset].start) && (dd < (wmo->doodadsets[doodadset].start+wmo->doodadsets[doodadset].size))) {

			ModelInstance &mi = wmo->modelis[dd];

			if (!outdoorLights) {
				WMOLight::setupOnce(GL_LIGHT2, mi.ldir, mi.lcol);
			}

			wmo->modelis[dd].draw2Select(ofs,rot);
		}
	}

	glDisable(GL_LIGHT2);

	glColor4f(1,1,1,1);
}

void WMOGroup::drawLiquid()
{
	if(!ok)
		return;
	if (!visible) return;

	// draw liquid
	// TODO: culling for liquid boundingbox or something
	if (lq) {
		setupFog();
		if (outdoorLights) {
			gWorld->outdoorLights(true);
		} else {
			// TODO: setup some kind of indoor lighting... ?
			gWorld->outdoorLights(false);
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, Vec4D(0.1f,0.1f,0.1f,1));
			glLightfv(GL_LIGHT2, GL_DIFFUSE, Vec4D(0.8f,0.8f,0.8f,1));
			glLightfv(GL_LIGHT2, GL_POSITION, Vec4D(0,1,0,0));
		}
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDepthMask(GL_TRUE);
		glColor4f(1,1,1,1);
		lq->draw();
		glDisable(GL_LIGHT2);
	}
}

void WMOGroup::setupFog()
{
	if(!ok)
		return;
	if (outdoorLights || fog==-1) {
		gWorld->setupFog();
	} else {
		wmo->fogs[fog].setup();
	}
}

WMOGroup::~WMOGroup()
{
	if (dl) glDeleteLists(dl, 1);
	if (dl_light) glDeleteLists(dl_light, 1);
	if (nDoodads) delete[] ddr;
	if (lq) delete lq;
}


void WMOFog::init(MPQFile &f)
{
	f.read(this, 0x30);
	color = Vec4D( ((color1 & 0x00FF0000) >> 16)/255.0f, ((color1 & 0x0000FF00) >> 8)/255.0f,
					(color1 & 0x000000FF)/255.0f, ((color1 & 0xFF000000) >> 24)/255.0f);
	float temp;
	temp = pos.y;
	pos.y = pos.z;
	pos.z = -temp;
	fogstart = fogstart * fogend * 1.5;
	fogend *= 1.5;
}

void WMOFog::setup()
{
	if (gWorld->drawfog) {
		glFogfv(GL_FOG_COLOR, color);
		glFogf(GL_FOG_START, fogstart);
		glFogf(GL_FOG_END, fogend);

		glEnable(GL_FOG);
	} else {
		glDisable(GL_FOG);
	}
}

int WMOManager::add(std::string name)
{
	int id;
	if (names.find(name) != names.end()) {
		id = names[name];
		items[id]->addref();
		//gLog("Loading WMO %s [already loaded]\n",name.c_str());
		return id;
	}

	// load new
	WMO *wmo = new WMO(name);
	id = nextID();
    do_add(name, id, wmo);
    return id;
}

void WMOManager::reload()
{
	gLog("Reloading WMO's\n");
	for (std::map<std::string, int>::iterator it = names.begin(); it != names.end(); ++it)
		((WMO*)items[(*it).second])->reload((*it).first);
}


WMOInstance::WMOInstance(WMO *wmo, MPQFile &f) : wmo (wmo)
{
	float ff[3];
	nameID=-1;
    f.read(&id, 4);
	f.read(ff,12);
	pos = Vec3D(ff[0],ff[1],ff[2]);
	f.read(ff,12);
	dir = Vec3D(ff[0],ff[1],ff[2]);
	f.read(ff,12);
	extents[0] = Vec3D(ff[0],ff[1],ff[2]);
	f.read(ff,12);
	extents[1] = Vec3D(ff[0],ff[1],ff[2]);
	f.read(&d2,4);
	f.read(&d3,4);
	
	doodadset = (d2 & 0xFFFF0000) >> 16;

	//gLog("WMO instance: %s (%d, %d)\n", wmo->name.c_str(), d2, d3);
}


WMOInstance::WMOInstance(WMO *wmo, MODF *d) : wmo (wmo)
{
	nameID=-1;
	id = d->uniqueID;
	pos = Vec3D(d->pos[0],d->pos[1],d->pos[2]);
	dir = Vec3D(d->rot[0],d->rot[1],d->rot[2]);
	extents[0] = Vec3D(d->extents[0][0],d->extents[0][1],d->extents[0][2]);
	extents[1] = Vec3D(d->extents[1][0],d->extents[1][1],d->extents[1][2]);
	d2= d->unknown+((uint32)d->doodadSet)<16;
	d3= d->nameSet;
	
	doodadset = d->doodadSet;

	//gLog("WMO instance: %s (%d, %d)\n", wmo->name.c_str(), d2, d3);
}

void WMOInstance::draw()
{
	if (ids.find(id) != ids.end()) return;
	ids.insert(id);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	float rot = -90.0f + dir.y;

	// TODO: replace this with a single transform matrix calculated at load time

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	if(nameID==-1)
		nameID=addNameWMO(this);
	glPushName(nameID);
	wmo->draw(doodadset,pos,-rot);
	glPopName();

	glPopMatrix();
}

void WMOInstance::drawSelect()
{
	if (ids.find(id) != ids.end()) return;
	ids.insert(id);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	float rot = -90.0f + dir.y;

	// TODO: replace this with a single transform matrix calculated at load time

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	if(nameID==-1)
		nameID=addNameWMO(this);
	glPushName(nameID);
	wmo->drawSelect(doodadset,pos,-rot);
	glPopName();

	glPopMatrix();
}

/*
void WMOInstance::drawPortals()
{
	if (ids.find(id) != ids.end()) return;
	ids.insert(id);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(dir.y - 90.0f, 0, 1, 0);
	glRotatef(-dir.x, 0, 0, 1);
	glRotatef(dir.z, 1, 0, 0);

	wmo->drawPortals();
	glPopMatrix();
}
*/

void WMOInstance::reset()
{
    ids.clear();
}

std::set<int> WMOInstance::ids;

WMOInstance::~WMOInstance()
{
	if(nameID!=-1)
	{
		deleteName(nameID);
		nameID=-1;
	}
}