#ifndef VIDEO_H
#define VIDEO_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <map>

#ifndef _WIN32
#include <stdarg.h>
#endif

#include "Vec3D.h"

#include "Manager.h"
#include "Font.h"

#define PI 3.14159265358f

typedef GLuint TextureID;

// some versions of gl.h on linux have problems - they might or might not have some of this stuff defined
// this would look better in an autoconf script but I am lazy and nobody will use this on linux anyway ;|
//#define DEFINE_ARB_MULTITEX

////////// OPENGL EXTENSIONS
#if defined(_WIN32) || defined(DEFINE_ARB_MULTITEX)
// multitexture
extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB;
#endif
// compressed textures
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
// VBO
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;

extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;

extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;

#define GL_BUFFER_OFFSET(i) ((char *)(0) + (i))
//
void SaveGLSettings();
void LoadGLSettings();

////////// TEXTURE MANAGER

class Texture : public ManagedItem
{
public:
	int w,h;
	GLuint id;

	Texture(std::string name):ManagedItem(name), w(0), h(0) {}
};

class TextureManager : public Manager<GLuint>
{
	
	bool LoadBLP(GLuint id, Texture *tex);

public:
	void reload();
	virtual GLuint add(std::string name);
	void doDelete(GLuint id);
	GLuint get(std::string name);
};

////////// VIDEO CLASS

class Video
{
	int status;
	int	flags;

	SDL_Surface *primary;

	void initExtensions();

public:
	bool SelectionMode;

	Video();
	~Video();

	void init(int xres, int yres, bool fullscreen);

	void close();

	void flip();
	void clearScreen();
	void set3D();
	void set3D_select();
	void set2D();
	void setTileMode();
	void resize(int w, int h);
	void GraphicVersion();
	void GraphicSeller();
	void GraphicRender();

	TextureManager textures;
    
	int xres, yres;
	int origX, origY;
	float ratio;
};

extern Video video;

GLuint loadTGA(const char *filename, bool mipmaps);

#endif