#ifndef OPENGL_H
#define OPENGL_H

#include <vector>
#include <string>
#include "Appstate.h"
#include "Font.h"
#include "FreeType.h"
/// XXX this really needs to be refactored into a singleton class

#define APP_TITLE "World of Warcraft Editor"

#define APP_VERSION "v0.1"


extern std::vector<AppState*> gStates;
extern bool gPop;

void gLog(char *str, ...);
extern Font *f16, *f24, *f32;
extern freetype::font_data arialn13,arial12,arial14,arial16,morpheus40;

extern float gFPS;

float frand();
float randfloat(float lower, float upper);
int randint(int lower, int upper);
void fixname(std::string &name);
void fixnamen(char *name, size_t len);

extern int fullscreen;
// Area database
class AreaDB;
extern AreaDB gAreaDB;

#endif