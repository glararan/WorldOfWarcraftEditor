#include "Video.h"
#include "MPQ.h"
#include "OpenGL.h"
#include <string>
#include <algorithm>
#include <cctype> 

struct ToLower
{
    char operator() (char c) const  { return std::tolower(c); }
};

/////// EXTENSIONS

#if defined(_WIN32) || defined(DEFINE_ARB_MULTITEX)
// multitex
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;
#endif
// compression
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB	= NULL;
// vbo
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;

PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;

PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;

//0 - GL_ALPHA_TEST
//1 - GL_BLEND
//2 - GL_COLOR_MATERIAL
//3 - GL_CULL_FACE
//4 - GL_DEPTH_TEST
//5 - GL_FOG
//6 - GL_FRAGMENT_PROGRAM_ARB
//7 - GL_LIGHTING
//8 - GL_LINE_SMOOTH
//9/10 - GL_TEXTURE_2D
//11 - GL_TEXTURE_GEN_S
//12 - GL_TEXTURE_GEN_T
GLboolean	GLSettings[13];
void SaveGLSettings()
{
	GLSettings[0]=glIsEnabled(GL_ALPHA_TEST);
	GLSettings[1]=glIsEnabled(GL_BLEND);
	GLSettings[2]=glIsEnabled(GL_COLOR_MATERIAL);
	GLSettings[3]=glIsEnabled(GL_CULL_FACE);
	GLSettings[4]=glIsEnabled(GL_DEPTH_TEST);
	GLSettings[5]=glIsEnabled(GL_FOG);
	GLSettings[6]=glIsEnabled(GL_FRAGMENT_PROGRAM_ARB);
	GLSettings[7]=glIsEnabled(GL_LIGHTING);
	GLSettings[8]=glIsEnabled(GL_LINE_SMOOTH);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	GLSettings[10]=glIsEnabled(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	GLSettings[9]=glIsEnabled(GL_TEXTURE_2D);
	GLSettings[11]=glIsEnabled(GL_TEXTURE_GEN_S);
	GLSettings[12]=glIsEnabled(GL_TEXTURE_GEN_T);
}

void compareSetting(GLenum Setting, GLboolean Cur)
{
	if(glIsEnabled(Setting)!=Cur)
	{
		if(Cur==GL_TRUE)
			glEnable(Setting);
		else
			glDisable(Setting);
	}
}

void LoadGLSettings()
{
	compareSetting(GL_ALPHA_TEST,GLSettings[0]);
	compareSetting(GL_BLEND,GLSettings[1]);
	compareSetting(GL_COLOR_MATERIAL,GLSettings[2]);
	compareSetting(GL_CULL_FACE,GLSettings[3]);
	compareSetting(GL_DEPTH_TEST,GLSettings[4]);
	compareSetting(GL_FOG,GLSettings[5]);
	compareSetting(GL_FRAGMENT_PROGRAM_ARB,GLSettings[6]);
	compareSetting(GL_LIGHTING,GLSettings[7]);
	compareSetting(GL_LINE_SMOOTH,GLSettings[8]);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	compareSetting(GL_TEXTURE_2D,GLSettings[10]);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	compareSetting(GL_TEXTURE_2D,GLSettings[9]);
	compareSetting(GL_TEXTURE_GEN_S,GLSettings[11]);
	compareSetting(GL_TEXTURE_GEN_T,GLSettings[12]);
}



////// VIDEO CLASS


Video video;

Video::Video()
{
}

Video::~Video()
{
}

void Video::resize(int xres, int yres)
{
	SDL_Rect Rect;

	this->xres = xres;
	this->yres = yres;
	this->ratio = ((float)xres)/((float)yres);
	
	//I shouldn't be changing these according to the documentation but it works
	primary->w=xres;
	primary->h=yres;

	Rect.h=yres;
	Rect.w=xres;
	Rect.x=0;
	Rect.y=0;
	SDL_SetClipRect(primary,&Rect);


	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void Video::init(int xres, int yres, bool fullscreen)
{
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	flags = SDL_OPENGL | SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_RESIZABLE;
	if (fullscreen)
		flags |= SDL_FULLSCREEN;
	//else 
//		flags |= SDL_RESIZABLE;
	// 32 bits ffs
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
#ifdef _WIN32
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	primary = SDL_SetVideoMode(xres, yres, 32, flags);
#else
	//nvidia dont support 32bpp on my linux :(
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	primary = SDL_SetVideoMode(xres, yres, 24, flags);
#endif
	if (!primary) {
		printf("SDL Error: %s\n",SDL_GetError());
		exit(1);
	}

	initExtensions();

	origX=xres;
	origY=yres;

	this->xres = xres;
	this->yres = yres;
	this->ratio = ((float)xres)/((float)yres);

	glViewport(0,0,xres,yres);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 0.01f, 1024.0f);
	gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 1.0f, 1024.0f);


	// hmmm...
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Video::close()
{
	//Crashes if I don't do this, so prolly why they say don't change those
	primary->w=origX;
	primary->h=origY;
	SDL_Quit();
}



void Video::initExtensions()
{
#if defined(_WIN32) || defined(DEFINE_ARB_MULTITEX)
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		SDL_GL_GetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)		SDL_GL_GetProcAddress("glClientActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
#endif
	glCompressedTexImage2DARB	= (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)		SDL_GL_GetProcAddress("glCompressedTexImage2DARB");
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) SDL_GL_GetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) SDL_GL_GetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) SDL_GL_GetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) SDL_GL_GetProcAddress("glDeleteBuffersARB");

	glMapBufferARB = (PFNGLMAPBUFFERARBPROC) SDL_GL_GetProcAddress("glMapBufferARB");
	glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) SDL_GL_GetProcAddress("glUnmapBufferARB");

	glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) SDL_GL_GetProcAddress("glDrawRangeElements");
}


void Video::flip()
{
	SDL_GL_SwapBuffers();
}

void Video::clearScreen()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
extern int MouseX;
extern int MouseY;

void Video::set3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(SelectionMode)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		gluPickMatrix(MouseX,viewport[3]-MouseY,
			7,7,viewport);
	}

	gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Video::set3D_select()
{
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, (GLfloat)xres/(GLfloat)yres, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Video::set2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, xres, yres, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Video::setTileMode()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2*ratio, 2*ratio, 2.0, -2.0, -100.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//////// TEXTURE MANAGER

GLuint TextureManager::get(std::string name)
{
	std::transform (name.begin(), name.end(), name.begin(), ToLower());
	return names[name];
}

GLuint TextureManager::add(std::string name)
{
	bool ext;
	GLuint id;
	std::transform (name.begin(), name.end(), name.begin(), ToLower());
	if (names.find(name) != names.end()) {
		id = names[name];
		items[id]->addref();
		return id;
	}
	
	glGenTextures(1,&id);

	Texture *tex= new Texture(name);
	tex->id = id;
	ext=LoadBLP(id, tex);

	if(!ext)
		gLog("      Loading Texture from MPQ %s\n",name.c_str());
	else
		gLog("      Loading Texture from File %s\n",name.c_str());

	do_add(name, id, tex);

	return id;
}

void TextureManager::reload()
{
	gLog("Reloading Textures\n");
	for (std::map<std::string, GLuint>::iterator it = names.begin(); it != names.end(); ++it)
	{
		bool ext;
		GLuint id=(*it).second;
		Texture *tex=(Texture*)items[(*it).second];

		ext=LoadBLP(id, tex);

		if(!ext)
			gLog(" Reloading Texture from MPQ %s\n",(*it).first.c_str());
		else
			gLog(" Reloading Texture from File %s\n",(*it).first.c_str());
	}
	gLog("Finish Reloading Textures\n\n");
}

bool TextureManager::LoadBLP(GLuint id, Texture *tex)
{
	// load BLP texture
	glBindTexture(GL_TEXTURE_2D, id);

	int offsets[16],sizes[16],w,h;
	GLint format;

	char attr[4];

	MPQFile f(tex->name.c_str());
	if (f.isEof()) {
		tex->id = 0;
		return false;
	}

	f.seek(8);
	f.read(attr,4);
	f.read(&w,4);
	f.read(&h,4);
	f.read(offsets,4*16);
	f.read(sizes,4*16);

	tex->w = w;
	tex->h = h;

	if (attr[0] == 2) {
		// compressed

		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		int blocksize = 8;

		// guesswork here :(
		if (attr[1]==8) {
			// dxt3 or 5
			//if (attr[2]) format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			//else format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

			blocksize = 16;

		} else {
			if (!attr[3]) format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		}

		char *buf = new char[sizes[0]];

		// do every mipmap level
		for (int i=0; i<16; i++) {
			if (w==0) w = 1;
			if (h==0) h = 1;
			if (offsets[i] && sizes[i]) {
				f.seek(offsets[i]);
				f.read(buf,sizes[i]);

				int size = ((w+3)/4) * ((h+3)/4) * blocksize;

				glCompressedTexImage2DARB(GL_TEXTURE_2D, i, format, w, h, 0, size, buf);

			} else break;
			w >>= 1;
			h >>= 1;
		}

		delete[] buf;
	}
	else if (attr[0]==1) {
		// uncompressed
		unsigned int pal[256];
		f.read(pal,1024);

		unsigned char *buf = new unsigned char[sizes[0]];
		unsigned int *buf2 = new unsigned int[w*h];
		unsigned int *p;
		unsigned char *c, *a;

		int alphabits = attr[1];
		bool hasalpha = alphabits!=0;

		for (int i=0; i<16; i++) {
			if (w==0) w = 1;
			if (h==0) h = 1;
			if (offsets[i] && sizes[i]) {
				f.seek(offsets[i]);
				f.read(buf,sizes[i]);

				int cnt = 0;
				p = buf2;
				c = buf;
				a = buf + w*h;
				for (int y=0; y<h; y++) {
					for (int x=0; x<w; x++) {
						unsigned int k = pal[*c++];
						k = ((k&0x00FF0000)>>16) | ((k&0x0000FF00)) | ((k& 0x000000FF)<<16);
						int alpha;
						if (hasalpha) {
							if (alphabits == 8) {
								alpha = (*a++);
							} else if (alphabits == 1) {
								alpha = (*a & (1 << cnt++)) ? 0xff : 0;
								if (cnt == 8) {
									cnt = 0;
									a++;
								}
							}
						} else alpha = 0xff;

						k |= alpha << 24;
						*p++ = k;
					}
				}

				glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf2);

			} else break;
			w >>= 1;
			h >>= 1;
		}

		delete[] buf2;
		delete[] buf;
	}

	f.close();

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return f.isExternal();
}


void TextureManager::doDelete(GLuint id)
{
	glDeleteTextures(1, &id);
}



#pragma pack(push,1)
struct TGAHeader {
   char  idlength;
   char  colourmaptype;
   char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short width;
   short height;
   char  bitsperpixel;
   char  imagedescriptor;
};
#pragma pack(pop)


GLuint loadTGA(const char *filename, bool mipmaps)
{
	FILE *f = fopen(filename,"rb");
	TGAHeader h;
	fread(&h,18,1,f);
	if (h.datatypecode != 2) return 0;
	size_t s = h.width * h.height;
	GLint bppformat;
	GLint format;
	int bypp = h.bitsperpixel / 8;
	if (h.bitsperpixel == 24) {
		s *= 3;
		format = GL_RGB;
		bppformat = GL_RGB8;
	} else if (h.bitsperpixel == 32) {
		s *= 4;
		format = GL_RGBA;
		bppformat = GL_RGBA8;
	} else return 0;

	unsigned char *buf = new unsigned char[s], *buf2;
	//unsigned char *buf2 = new unsigned char[s];
	fread(buf,s,1,f);
	fclose(f);

	buf2 = buf;

	GLuint t;
	glGenTextures(1,&t);
	glBindTexture(GL_TEXTURE_2D, t);

	if (mipmaps) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps (GL_TEXTURE_2D, bppformat, h.width, h.height, format, GL_UNSIGNED_BYTE, buf2);
	} else {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, bppformat, h.width, h.height, 0, format, GL_UNSIGNED_BYTE, buf2);
	}
    delete[] buf;
	//delete[] buf2;
	return t;
}