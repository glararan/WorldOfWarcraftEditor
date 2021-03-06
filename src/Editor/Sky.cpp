#include "Sky.h"
#include "World.h"
#include <algorithm>

const float skymul = 36.0f;

void SkyColor::init(int t, int col)
{
	time = t;
	color.z = ((col & 0x0000ff)) / 255.0f;
	color.y = ((col & 0x00ff00) >> 8) / 255.0f;
	color.x = ((col & 0xff0000) >> 16) / 255.0f;
}

struct lightinfo
{
    int ia, ib, ic;
	float fa, fb, fc, fd, fe;
	char name[32];
};

Sky::Sky(MPQFile &f)
{
	lightinfo li;
	f.read(&li, 64);

	pos = Vec3D(li.fa/skymul, li.fb/skymul, li.fc/skymul);
	r1 = li.fd / skymul;
	r2 = li.fe / skymul;

	strcpy_s(name, li.name);

	for(int i = 0; i < 36; i++)
		mmin[i] = -2;

	global = (li.ia == -1);
}

void Sky::init(MPQFile &f)
{
	int addr = (int)f.getPos();
	int ll[18];
	int buf[64];

	for(int k = 0; k < 4; k++)
	{
		if(k == 0 || k == 2)
		{
			f.seek(addr + k * 0x15f0);
			f.read(ll, 18 * 4);

			for(int i = 0; i < 18; i++)
			{
				f.read(buf, 64 * 4);
				int* p = buf;

				int idx = (k/2) * 18 + i;

				if(ll[i] == 0)
					mmin[idx] = -1;
				else
				{
					mmin[idx] = *p;
					for(int l = 0; l < ll[i]; l++)
					{
						SkyColor sc;
						sc.init(p[0], p[1]);
						p += 2;
						colorRows[idx].push_back(sc);
					}
				}
			}
		}
	}

	f.seek(addr + 4 * 0x15f0);
}

Vec3D Sky::colorFor(int r, int t) const
{
	if(mmin[r] < 0)
		return Vec3D(0, 0, 0);

	Vec3D c1, c2;
	int t1, t2;
	size_t last = colorRows[r].size() - 1;

	if(t < mmin[r])
	{
		// reverse interpolate
		c1 = colorRows[r][last].color;
		c2 = colorRows[r][0].color;
		t1 = colorRows[r][last].time;
		t2 = colorRows[r][0].time + 2880;
		t += 2880;
	}
	else
	{
		for(size_t i = last; i >= 0; i--)
		{
			if(colorRows[r][i].time <= t)
			{
				c1 = colorRows[r][i].color;
				t1 = colorRows[r][i].time;

				if(i == last)
				{
					c2 = colorRows[r][0].color;
					t2 = colorRows[r][0].time + 2880;
				}
				else
				{
					c2 = colorRows[r][i + 1].color;
					t2 = colorRows[r][i + 1].time;
				}
				break;
			}
		}
	}

	float tt = (float)(t - t1) / (float)(t2 - t1);
    return c1 * (1.0f - tt) + c2 * tt;
}

const float rad = 400.0f;

//.......................top....med....medh........horiz..........bottom
const float angles[] = {90.0f, 30.0f, 15.0f, 5.0f, 0.0f, -30.0f, -90.0f};
const int skycolors[] = {2,      3,      4,    5,    6,    7,     7};
const int cnum = 7;

const int hseg = 32;

void Skies::draw()
{
	// draw sky sphere?
	// TODO: do this as a vertex array and use glColorPointer? :|
	Vec3D basepos1[cnum], basepos2[cnum];
	glBegin(GL_QUADS);
	for(int h = 0; h < hseg; h++)
	{
		for(int i=0; i < cnum; i++)
		{
			basepos1[i] = basepos2[i] = Vec3D(cosf(angles[i] * PI/180.0f) * rad, sinf(angles[i] * PI/180.0f) * rad, 0);
			rotate(0, 0, &basepos1[i].x, &basepos1[i].z, PI * 2.0f/hseg * h);
			rotate(0, 0, &basepos2[i].x, &basepos2[i].z, PI * 2.0f/hseg * (h + 1));
		}

		for(int v = 0; v < cnum - 1; v++)
		{
			glColor3fv(colorSet[skycolors[v]]);
			glVertex3fv(basepos2[v]);
			glVertex3fv(basepos1[v]);
			glColor3fv(colorSet[skycolors[v + 1]]);
			glVertex3fv(basepos1[v + 1]);
			glVertex3fv(basepos2[v + 1]);
		}
	}
	glEnd();
}

Skies::Skies(const char* basename, bool force)
{
	char fn[256];
	sprintf_s(fn, "World\\Maps\\%s\\lights.lit", basename);

	numSkies = 0;
	cs = -1;
	stars = 0;
	bool succ = loadFrom(fn, false);
	if(!succ && force)
		loadFrom("World\\Maps\\Kalimdor\\lights.lit", true);
}

bool Skies::loadFrom(const char* fname, bool forced)
{
	MPQFile f(fname);
	if(f.isEof())
		return false;

	int ns;

	f.seek(4);
	f.read(&ns, 4);
	numSkies = ns;
	if(forced && numSkies > 1)
		numSkies = 1;

	for(int i = 0; i < numSkies; i++)
	{
		Sky s(f);
		skies.push_back(s);
	}
	// sanity seek
	f.seek(8 + ns * 64);

	for(int i = 0; i < numSkies; i++)
	{
		skies[i].init(f);
	}

	f.close();

	// sort skies from smallest to largest; global last.
	// smaller skies will have precedence when calculating weights to achieve smooth transitions etc.
	std::sort(skies.begin(), skies.end());

	stars = new Model("Environments\\Stars\\Stars.mdx", true); // will be replaced to .m2

	return true;
}

Skies::~Skies()
{
	delete stars;
}

void Skies::findSkyWeights(Vec3D pos)
{
	int maxsky = (int)skies.size() - 1;
	skies[maxsky].weight = 1.0f;
	cs = maxsky;
	for(int i = maxsky-1; i >= 0; i--)
	{
		Sky &s = skies[i];
        float dist = (pos - s.pos).length();
		if(dist < s.r1)
		{
			// we're in a sky, zero out the rest
			s.weight = 1.0f;
			cs = i;
			for(size_t j = i + 1; j < skies.size(); j++)
				skies[j].weight = 0.0f;
		}
		else if(dist < s.r2)
		{
			// we're in an outer area, scale down the other weights
			float r = (dist - s.r1)/(s.r2 - s.r1);
			s.weight = 1.0f - r;
			for(size_t j = i + 1; j < skies.size(); j++)
				skies[j].weight *= r;
		}
		else
			s.weight = 0.0f;
	}
	// weights are all normalized at this point :D
}

void Skies::initSky(Vec3D pos, int t)
{
	if(numSkies == 0)
		return;

	findSkyWeights(pos);

	for(int i = 0; i < 18; i++)
		colorSet[i] = Vec3D(0, 0, 0);

	// interpolation
	for(size_t j = 0; j < skies.size(); j++)
	{
		if(skies[j].weight > 0)
		{
			// now calculate the color rows
			for(int i = 0; i < 18; i++)
			{
				if((skies[j].colorFor(i, t).x > 1.0f) || (skies[j].colorFor(i, t).y > 1.0f) || (skies[j].colorFor(i, t).z > 1.0f))
				{
					gLog("[World of Warcraft Studio - Editor] - Sky %d %d is out of bounds!\n",j,i);
					continue;
				}

				colorSet[i] += skies[j].colorFor(i, t) * skies[j].weight;
			}
		}
	}
}

/*
void drawCircle(unsigned int *buf, int dim, float x, float y, float r, unsigned int col)
{
    float circ = 2*r*PI;
	for (int i=0; i<circ; i++)
	{
		float phi = 2*PI*i/circ;
		int px = (int)(x + r * cosf(phi));
		int py = (int)(y + r * sinf(phi));
		if (px>=0 && px<dim && py>=0 && py<dim)
		{
            buf[py*dim+px] = col;
		}
	}
}

void Skies::debugDraw(unsigned int *buf, int dim)
{
	float worldSize = 64.0f*533.333333f;
	for (size_t i=1; i<skies.size(); i++)
	{
		Sky &s = skies[i];
		float cx = dim * s.pos.x / worldSize;
		float cy = dim * s.pos.z / worldSize;
		float r1 = dim * s.r1 / worldSize;
		float r2 = dim * s.r2 / worldSize;
		drawCircle(buf, dim, cx, cy, r1, 0xFFFF0000);
		drawCircle(buf, dim, cx, cy, r2, 0xFFFFFF00);
	}
}
*/

bool Skies::drawSky(const Vec3D &pos)
{
	if(numSkies == 0)
		return false;

	// drawing the sky: we'll undo the camera translation
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	draw();

	// if it's night, draw the stars
	float ni = gWorld->outdoorLightStats.nightIntensity;
	if(ni > 0)
	{
		const float sc = 0.1f;
		glScalef(sc, sc, sc);
		glEnable(GL_TEXTURE_2D);
		stars->trans = ni;
        stars->draw();
	}

	glPopMatrix();

	return true;
}

char* Skies::getSkyName()
{
	if(cs == -1)
		return "[no sky]";
	else
		return skies[cs].name;
}

// TODO: figure out what dnc.db is _really_ used for
void OutdoorLightStats::init(MPQFile &f)
{
	float h, m;

	f.seekRelative(4);
	f.read(&h, 4);
	f.seekRelative(4);
	f.read(&m, 4);
	f.seekRelative(4);
	f.read(&dayIntensity, 4);
	
	f.seekRelative(4);
	f.read(&dayColor.x, 4);
	f.seekRelative(4);
	f.read(&dayColor.y, 4);
	f.seekRelative(4);
	f.read(&dayColor.z, 4);

	f.seekRelative(4);
	f.read(&dayDir.x, 4);
	f.seekRelative(4);
	f.read(&dayDir.y, 4);
	f.seekRelative(4);
	f.read(&dayDir.z, 4);

	f.seekRelative(4);
	f.read(&nightIntensity, 4);

	f.seekRelative(4);
	f.read(&nightColor.x, 4);
	f.seekRelative(4);
	f.read(&nightColor.y, 4);
	f.seekRelative(4);
	f.read(&nightColor.z, 4);

	f.seekRelative(4);
	f.read(&nightDir.x, 4);
	f.seekRelative(4);
	f.read(&nightDir.y, 4);
	f.seekRelative(4);
	f.read(&nightDir.z, 4);

	f.seekRelative(4);
	f.read(&ambientIntensity, 4);

	f.seekRelative(4);
	f.read(&ambientColor.x, 4);
	f.seekRelative(4);
	f.read(&ambientColor.y, 4);
	f.seekRelative(4);
	f.read(&ambientColor.z, 4);

	f.seekRelative(4);
	f.read(&fogDepth, 4);
	f.seekRelative(4);
	f.read(&fogIntensity, 4);

	f.seekRelative(4);
	f.read(&fogColor.x, 4);
	f.seekRelative(4);
	f.read(&fogColor.y, 4);
	f.seekRelative(4);
	f.read(&fogColor.z, 4);

    time = (int)h * 60 * 2 + (int)m * 2;

	// HACK: make day & night intensity exclusive; set day intensity to 1.0
	if(dayIntensity > 0)
	{
		dayIntensity = 1.0f;
		nightIntensity = 0.0f;
	}
}

void OutdoorLightStats::interpolate(OutdoorLightStats* a, OutdoorLightStats* b, float r)
{
	float ir = 1.0f - r;
	time = 0; // unused

	dayIntensity = a->dayIntensity * ir + b->dayIntensity * r;
	nightIntensity = a->nightIntensity * ir + b->nightIntensity * r;
	ambientIntensity = a->ambientIntensity * ir + b->ambientIntensity * r;
	fogIntensity = a->fogIntensity * ir + b->fogIntensity * r;
	fogDepth = a->fogDepth * ir + b->fogDepth * r;
	dayColor = a->dayColor * ir + b->dayColor * r;
	nightColor = a->nightColor * ir + b->nightColor * r;
	ambientColor = a->ambientColor * ir + b->ambientColor * r;
	fogColor = a->fogColor * ir + b->fogColor * r;
	dayDir = a->dayDir * ir + b->dayDir * r;
	nightDir = a->nightDir * ir + b->nightDir * r;
}

void OutdoorLightStats::setupLighting()
{
	GLfloat la[4];
	GLfloat ld[4];
	GLfloat lp[4];

	la[3] = 1.0f;
	ld[3] = 1.0f;
	lp[3] = 0.0f; // directional lights plz

	la[0] = la[1] = la[2] = 0.0f;
	
	if(dayIntensity > 0)
	{
		ld[0] = dayColor.x * dayIntensity;
		ld[1] = dayColor.y * dayIntensity;
		ld[2] = dayColor.z * dayIntensity;
		lp[0] = dayDir.x;
		lp[1] = dayDir.z;
		lp[2] = -dayDir.y;
	
		glLightfv(GL_LIGHT0, GL_AMBIENT, la);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
		glLightfv(GL_LIGHT0, GL_POSITION,lp);
		glEnable(GL_LIGHT0);
	}
	else
		glDisable(GL_LIGHT0);

	if(nightIntensity > 0)
	{
		ld[0] = nightColor.x * nightIntensity;
		ld[1] = nightColor.y * nightIntensity;
		ld[2] = nightColor.z * nightIntensity;
		lp[0] = nightDir.x;
		lp[1] = nightDir.z;
		lp[2] = -nightDir.y;

		glLightfv(GL_LIGHT1, GL_AMBIENT, la);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, ld);
		glLightfv(GL_LIGHT1, GL_POSITION,lp);
		glEnable(GL_LIGHT1);
	}
	else
		glDisable(GL_LIGHT1);

	// light 2 will be ambient -> max. 3 lights for outdoors...
	la[0] = ambientColor.x * ambientIntensity;
	la[1] = ambientColor.y * ambientIntensity;
	la[2] = ambientColor.z * ambientIntensity;

	/*
	ld[0] = ld[1] = ld[2] = 0.0f;
	lp[0] = lp[1] = lp[2] = 0.0f;
	glLightfv(GL_LIGHT2, GL_AMBIENT, la);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, ld);
	glLightfv(GL_LIGHT2, GL_POSITION,lp);
	glEnable(GL_LIGHT2);
	*/
	glDisable(GL_LIGHT2);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, la);

	// not using the rest
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);
	glDisable(GL_LIGHT6);
	glDisable(GL_LIGHT7);
	// should really loop to GL_MAX_LIGHTS lol
}

OutdoorLighting::OutdoorLighting(char* fname)
{
	MPQFile f(fname);
	unsigned int n, d;

	f.seekRelative(4);
	f.read(&n, 4); // it's the same thing twice? :|

	f.seekRelative(4);
	f.read(&d, 4); // d is now the final offset
	f.seek(8 + n * 8);

	while(f.getPos() < d)
	{
		OutdoorLightStats ols;
		ols.init(f);

		lightStats.push_back(ols);
	}

	f.close();
}

OutdoorLightStats OutdoorLighting::getLightStats(int time)
{
	// ASSUME: only 24 light info records, one for each whole hour
	// TODO: generalize this if the data file changes in the future

	OutdoorLightStats out;

	OutdoorLightStats* a, *b;
	int ta = time / 120;
	int tb = (ta + 1) % 24;
	float r = (time - (ta * 120)) / 120.0f;

	a = &lightStats[ta];
	b = &lightStats[tb];

    out.interpolate(a, b, r);

    return out;
}