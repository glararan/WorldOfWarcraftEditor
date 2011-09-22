#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include "Appstate.h"
#include "Video.h"

#include "World.h"
#include "UI.h"

class MapViewer : public AppState
{
	GLuint tex;

	float ah, av, moving, strafing, updown, mousedir, movespd;
	bool look;
	bool mapmode;
	bool hud;

	World* world;

	frame tileFrames;

	float lastBrushUpdate;

	void doSelection();

public:

	MapViewer(World* w, float ah0 = -90.0f, float av0 = -30.0f);
	~MapViewer();

	void tick(float t, float dt);
	void display(float t, float dt);

	void keypressed(SDL_KeyboardEvent* e);
	void mousemove(SDL_MouseMotionEvent* e);
	void mouseclick(SDL_MouseButtonEvent* e);
};

#endif