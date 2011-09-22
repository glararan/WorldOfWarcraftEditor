#ifndef MENU_H
#define MENU_H

#include "Appstate.h"
#include <string>
#include <vector>

#include "World.h"

struct Clickable
{
	int x0, y0, x1, y1;

	bool hit(int x, int y);
};

struct MapEntry: public Clickable
{
	unsigned int id;
	unsigned int instanceType;
	unsigned int expansion;
	
	string directory;
	string name;

	Font* font;
};

struct Bookmark: public Clickable
{
	string basename, name, label;
	Vec3D pos;
	float ah, av;
};

enum Commands
{
	CMD_SELECT,
	CMD_LOAD_WORLD,
	CMD_DO_LOAD_WORLD,
	CMD_BACK_TO_MENU,
};

class Menu : public AppState
{
	int sel, cmd, x, y, cz, cx;

	World* world;

	vector<MapEntry> maps;
	vector<Bookmark> bookmarks;

	bool setpos;
	float ah,av;

	Model* bg;
	float mt;

	int lastbg;
	bool darken;

public:
	Menu();
	~Menu();

	void tick(float t, float dt);
	void display(float t, float dt);

	void keypressed(SDL_KeyboardEvent* e);
	void mousemove(SDL_MouseMotionEvent* e);
	void mouseclick(SDL_MouseButtonEvent* e);

	void refreshBookmarks();

	void randBackground();

	void shprint(Font* f, int x, int y, char* text);
};

#endif