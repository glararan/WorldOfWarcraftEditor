#ifndef TEST_H
#define TEST_H

#include "Appstate.h"
#include "Video.h"

#include "World.h"
#include "UI.h"

class Test :public AppState
{
	GLuint tex;

	float ah,av,moving,strafing,updown,mousedir,movespd;
	bool look;
	bool mapmode;
	bool hud;

	World *world;

	frame tileFrames;

	float lastBrushUpdate;

	void doSelection();

public:

	Test(World *w, float ah0 = -90.0f, float av0 = -30.0f);
	~Test();

	void tick(float t, float dt);
	void display(float t, float dt);

	void keypressed(SDL_KeyboardEvent *e);
	void mousemove(SDL_MouseMotionEvent *e);
	void mouseclick(SDL_MouseButtonEvent *e);
};

#endif