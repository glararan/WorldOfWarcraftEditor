#define _CRT_SECURE_NO_WARNINGS
#include "Test.h"
#include "OpenGL.h"
#include "AreaDB.h"

#include <cmath>
#include <string>
#include "GroundEffects.h"
#include "TexturingUI.h"
using namespace std;

#define XSENS 15.0f
#define YSENS 15.0f
#define SPEED 66.6f

int MouseX;
int MouseY;
float mh,mv,rh,rv;

bool MoveObj;

Vec3D ObjMove;
Vec3D ObjRot;

bool Selecting=false;
bool TestSelection=false;
bool DetailSelection=true;
nameEntry *Selection;
nameEntry *CurSelection;
unsigned int Sel;

bool LShiftDown=false;
bool RShiftDown=false;
bool LAltDown=false;
bool RAltDown=false;
bool LCtrlDown=false;
bool RCtrlDown=false;

bool leftMouse=false;
bool leftClicked=false;
bool TileMode=false;
bool painting=false;

float BrushRadius=10.0f;
int BrushType=2;
float brushPressure=0.9f;
float brushLevel=255.0f;

int terrainMode=0;

brush textureBrush;

bool Saving=false;

frame *LastClicked;

void setTextureBrushHardness(float f)
{
	textureBrush.setHardness(f);
}

void setTextureBrushRadius(float f)
{
	textureBrush.setRadius(f);
}

void setTextureBrushPressure(float f)
{
	brushPressure=f;
}

void setTextureBrushLevel(float f)
{
	brushLevel=(1.0f-f)*255.0f;
}

// why the hell is this class called Test, anyway
// I should rename it to MapViewer or something when I'm not lazy
Test::Test(World *w, float ah0, float av0): world(w), ah(ah0), av(av0)
{
	LastClicked=0;
	moving = strafing = updown = 0;

	mousedir = 1.0f;

	movespd = SPEED;

	look = false;
	mapmode = false;
	hud = true;

	world->thirdperson = false;
	world->lighting = true;
	world->drawmodels = true;
	world->drawdoodads = true;
	world->drawterrain = true;
	world->drawlines = false;
	world->drawwmo = true;
	world->drawhighres = true;
	world->drawfog = true; // should this be on or off by default..? :(

	// in the wow client, fog distance is stored in wtf\config.wtf as "farclip"
	// minimum is 357, maximum is 777
	world->fogdistance = 512.0f;

	world->l_const = 0.0f;
	world->l_linear = 0.7f;
	world->l_quadratic = 0.03f;

	tileFrames.x=0;
	tileFrames.y=0;
	tileFrames.height=(float)video.yres;
	tileFrames.width=(float)video.xres;

	window *W1;
	W1=new window(video.xres-182.0f,2.0f,180.0f,100.0f);
	W1->movable=true;
	
	tileFrames.addChild(W1);

	textUI *T1;
	T1=new textUI(6.0f+145.0f/2,2,"Brush Settings");
	T1->setFont(&arial14);
	T1->setJustify(UI_JUSTIFY_CENTER);
	W1->addChild(T1);
	
	gradient *G1;
	G1=new gradient;	
	G1->width=20.0f;
	G1->x=W1->width-4-G1->width;
	G1->y=4.0f;
	G1->height=92.0f;
	G1->setMaxColor(1.0f,1.0f,1.0f,1.0f);
	G1->setMinColor(0.0f,0.0f,0.0f,1.0f);
	G1->horiz=false;
	G1->setClickColor(1.0f,0.0f,0.0f,1.0f);
	G1->setClickFunc(setTextureBrushLevel);
	G1->setValue(0.0f);
	
	W1->addChild(G1);

	slider	*S1;
	S1=new slider(6.0f,33.0f,145.0f,1.0f,0.0f);
	S1->setFunc(setTextureBrushHardness);
	S1->setValue(0.5f);
	S1->setText("Hardness: %.2f");
	W1->addChild(S1);

	S1=new slider(6.0f,59.0f,145.0f,49.0f,1.0f);
	S1->setFunc(setTextureBrushRadius);
	S1->setValue(0.5f);
	S1->setText("Radius: %.1f");
	W1->addChild(S1);

	S1=new slider(6.0f,85.0f,145.0f,0.99f,0.01f);
	S1->setFunc(setTextureBrushPressure);
	S1->setValue(0.90f);
	S1->setText("Pressure: %.2f");
	W1->addChild(S1);

	tileFrames.addChild(CreateTexturePalette());
	tileFrames.addChild(CreateSelectedTexture());
	tileFrames.addChild(CreateTilesetLoader());
	tileFrames.addChild(CreateTextureFilter());
	//tileFrames.addChild(createMapChunkWindow());
	
	lastBrushUpdate=0;
	textureBrush.init();
}

Test::~Test()
{
	delete world;
}

Vec3D	ObjPos;
extern ManagedItem	*selectedTexture;
void Test::tick(float t, float dt)
{
	world->onTheFlyLoading();
	if(dt>1)
		dt=1;
	if(SDL_GetAppState()&SDL_APPINPUTFOCUS)
	{
		if(world->LowerTerrain<0)
		{
			world->LowerTerrain+=5.0f*dt;
			if(world->LowerTerrain>0)
				world->LowerTerrain=0.0f;
		}

		Vec3D dir(1,0,0);
		Vec3D dirUp(1,0,0);
		Vec3D dirRight(0,0,1);
		rotate(0,0, &dir.x,&dir.y, av*PI/180.0f);
		rotate(0,0, &dir.x,&dir.z, ah*PI/180.0f);

		if(LShiftDown||RShiftDown)
		{
			dirUp.x=0;
			dirUp.y=1;
			dirRight*=0;
		}
		else if(LCtrlDown||RCtrlDown)
		{
			dirUp.x=0;
			dirUp.y=1;
			rotate(0,0, &dirUp.x,&dirUp.y, av*PI/180.0f);
			rotate(0,0, &dirRight.x,&dirRight.y, av*PI/180.0f);
			rotate(0,0, &dirUp.x,&dirUp.z, ah*PI/180.0f);
			rotate(0,0, &dirRight.x,&dirRight.z, ah*PI/180.0f);
			
		}
		else
		{
			rotate(0,0, &dirUp.x,&dirUp.z, ah*PI/180.0f);
			rotate(0,0, &dirRight.x,&dirRight.z, ah*PI/180.0f);
		}

		
		if(CurSelection!=0)
		{
			ObjPos=CurSelection->data.model->pos-world->camera;
				rotate(0,0, &ObjPos.x,&ObjPos.y, av*PI/180.0f);
				rotate(0,0, &ObjPos.x,&ObjPos.z, ah*PI/180.0f);
				ObjPos.x=abs(ObjPos.x);
		}

		if(MoveObj&&(CurSelection!=0)&&(CurSelection->type==ENTRY_MODEL))
		{
			if(LAltDown||RAltDown)
			{
				float ScaleAmount;
				ScaleAmount=pow(2.0f,mv*4.0f);
				CurSelection->data.model->sc*=ScaleAmount;
				if(CurSelection->data.model->sc>63.9)
					CurSelection->data.model->sc=63.9f;
				else if (CurSelection->data.model->sc<0.00098)
					CurSelection->data.model->sc=0.00098f;
			}
			else
			{
				ObjPos.x=40.0f;
				CurSelection->data.model->pos+=mv*dirUp*ObjPos.x;
				CurSelection->data.model->pos-=mh*dirRight*ObjPos.x;
			}
		}

		if(look&&(CurSelection!=0))
		{
			if((LShiftDown||RShiftDown)&&(CurSelection->type==ENTRY_MODEL))
			{
				CurSelection->data.model->dir.y+=rh+rv;

				if(CurSelection->data.model->dir.y>360.0f)
					CurSelection->data.model->dir.y-=360.0f;
				else if(CurSelection->data.model->dir.y<-360.0f)
					CurSelection->data.model->dir.y+=360.0f;			
			}
			else if((LCtrlDown||RCtrlDown)&&(CurSelection->type==ENTRY_MODEL))
			{
				CurSelection->data.model->dir.x+=rh+rv;

				if(CurSelection->data.model->dir.x>360.0f)
					CurSelection->data.model->dir.x-=360.0f;
				else if(CurSelection->data.model->dir.x<-360.0f)
					CurSelection->data.model->dir.x+=360.0f;
			}
			else if((LAltDown||RAltDown)&&(CurSelection->type==ENTRY_MODEL))
			{
				CurSelection->data.model->dir.z+=rh+rv;

				if(CurSelection->data.model->dir.z>360.0f)
					CurSelection->data.model->dir.z-=360.0f;
				else if(CurSelection->data.model->dir.z<-360.0f)
					CurSelection->data.model->dir.z+=360.0f;
			}
		}

		mh=0;
		mv=0;
		rh=0;
		rv=0;

		if(leftMouse&&Selection&&Selection->type==ENTRY_MAPCHUNK)
		{
			float xPos,yPos,zPos;
			if(terrainMode==0)
			{
				if(LShiftDown||RShiftDown)
				{
					Selection->data.mapchunk->getSelectionCoord(&xPos,&zPos);
					world->changeTerrain(xPos,zPos,7.5f*dt,BrushRadius,BrushType);		
				}
				if(LCtrlDown||RCtrlDown)
				{
					Selection->data.mapchunk->getSelectionCoord(&xPos,&zPos);
					world->changeTerrain(xPos,zPos,-7.5f*dt,BrushRadius,BrushType);
				}
			}
			else
			{
				if(LShiftDown||RShiftDown)
				{
					Selection->data.mapchunk->getSelectionCoord(&xPos,&zPos);
					yPos=Selection->data.mapchunk->getSelectionHeight();
					world->flattenTerrain(xPos,zPos,yPos,pow(0.2f,dt),BrushRadius,BrushType);		
				}
				if(LCtrlDown||RCtrlDown)
				{
					Selection->data.mapchunk->getSelectionCoord(&xPos,&zPos);
					world->blurTerrain(xPos,zPos,pow(0.2f,dt),min(BrushRadius,30.0f),BrushType);
				}
			}
		}
		
		if(TileMode==false)
		{
			if (moving != 0) world->camera += dir * dt * movespd * moving;
			if (strafing != 0) {
				Vec3D right = dir % Vec3D(0,1,0);
				right.normalize();
				world->camera += right * dt * movespd * strafing;
			}
			if (updown != 0) world->camera += Vec3D(0, dt * movespd * updown, 0);
			world->lookat = world->camera + dir;
		}
		else
		{
			if (moving != 0) world->camera.z -= dt*movespd*moving/(world->zoom*1.5f);
			if (strafing != 0) world->camera.x+= dt*movespd * strafing/(world->zoom*1.5f);
			if (updown != 0) world->zoom *= pow(2.0f,dt*updown*4.0f);
			if(world->zoom>2)
				world->zoom=2;
			else if(world->zoom<0.1)
				world->zoom=0.1f;

			if(leftMouse&&!LastClicked&&selectedTexture!=0)
			{
				float mX,mY;
				mX=CHUNKSIZE*4.0f*video.ratio*((float)MouseX/(float)video.xres-0.5f)/world->zoom+world->camera.x;
				mY=CHUNKSIZE*4.0f*((float)MouseY/(float)video.yres-0.5f)/world->zoom+world->camera.z;

				if(LCtrlDown||RCtrlDown)
					world->eraseTextures(mX,mY);
				else
				{		
					if(textureBrush.needUpdate())
						textureBrush.GenerateTexture();
					
					world->paintTexture(mX,mY,&textureBrush,brushLevel,1.0f-pow(1.0f-brushPressure,dt*10),video.textures.add(selectedTexture->name.c_str()));
				}
			}
		}
	}
	else
	{
		leftMouse=false;
		look = false;
		MoveObj=false;
		moving=0;
		strafing=0;
		updown=0;
	}

	if((t-lastBrushUpdate)>0.1&&textureBrush.needUpdate())
		textureBrush.GenerateTexture();

	world->time += (world->modelmanager.v * /*360.0f*/ 90.0f * dt);
	world->animtime += dt * 1000.0f;
	globalTime = (int)world->animtime;

	world->tick(dt);
}

void Test::doSelection()
{
	world->drawSelection(MouseX,MouseY);
	Sel=world->getSelection();
	Selection=getSelect(Sel);

	if(Selection==0)
		return;
	int ErrorNum=glGetError();
	if(ErrorNum)
		gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n",ErrorNum);

	if(Selection->type==ENTRY_MAPCHUNK)
	{
		world->drawSelectionChunk(MouseX,MouseY);
		Sel=world->getSelection();
		Selection=getSelect(Sel);
	}
}

void Test::display(float t, float dt)
{
	if(Saving)
	{		
		video.setTileMode();
		world->saveMap();
		Saving=false;
	}
	
	if (mapmode && world->minimap) {
		// show map
        // TODO: try to use a real map from WoW? either the large map or the minimap would be nice
		video.clearScreen();
		video.set2D();

		const int len = 768;
		const int basex = 200;
		const int basey = 0;
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, world->minimap);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2i(basex,basey);
		glTexCoord2f(1,0);
		glVertex2i(basex+len,basey);
		glTexCoord2f(1,1);
		glVertex2i(basex+len,basey+len);
		glTexCoord2f(0,1);
		glVertex2i(basex,basey+len);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		float fx, fz;
		fx = basex + world->camera.x / TILESIZE * 12.0f;
		fz = basey + world->camera.z / TILESIZE * 12.0f;
		glVertex2f(fx, fz);
		glColor4f(1,1,1,0);
		glVertex2f(fx + 10.0f*cosf(ah/180.0f*PI), fz + 10.0f*sinf(ah/180.0f*PI));
		glEnd();
	}
	else if(TileMode)
	{
		ResetUniques();
		video.setTileMode();
		world->drawTileMode(ah);
		//world->saveMap();

		float mX,mY,tRadius;
		mX=4.0f*video.ratio*((float)MouseX/(float)video.xres-0.5f);
		mY=4.0f*((float)MouseY/(float)video.yres-0.5f);

		mX=CHUNKSIZE*4.0f*video.ratio*((float)MouseX/(float)video.xres-0.5f)/world->zoom+world->camera.x;
		mY=CHUNKSIZE*4.0f*((float)MouseY/(float)video.yres-0.5f)/world->zoom+world->camera.z;

		mX=mX/CHUNKSIZE;
		mY=mY/CHUNKSIZE;

		glPushMatrix();
		glScalef(world->zoom,world->zoom,1.0f);
		glTranslatef(-world->camera.x/CHUNKSIZE,-world->camera.z/CHUNKSIZE,0);

		glColor4f(1.0f,1.0f,1.0f,0.5f);
		glActiveTextureARB(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, textureBrush.getTexture());
		tRadius=textureBrush.getRadius()/CHUNKSIZE;//*world->zoom;
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(mX-tRadius,mY+tRadius,0);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(mX+tRadius,mY+tRadius,0);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(mX+tRadius,mY-tRadius,0);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(mX-tRadius,mY-tRadius,0);
		glEnd();

		glPopMatrix();

		video.set2D();
		
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTextureARB(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glColor4f(1,1,1,1);

		glActiveTextureARB(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		tileFrames.render();
		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		
		glColor4f(1,1,1,1);

		if (hud) {
			f16->shprint(5,0,"%.2f fps", gFPS);

			//char *sn = world->skies->getSkyName();
			//if (sn)	f16->print(5,60,"%s", sn);

			// TODO: look up WMO names/group names as well from some client db?
			unsigned int areaID = world->getAreaID();
			unsigned int regionID = 0;
			/// Look up area
			try {
				AreaDB::Record rec = gAreaDB.getByAreaID(areaID);
				std::string areaName = rec.getString(AreaDB::Name);
				regionID = rec.getUInt(AreaDB::Region);
				f16->shprint(5,20,"%s", areaName.c_str());
			} catch(AreaDB::NotFound)
			{
				/// Not found, unknown area
				//f16->print(5,20,"Unknown [%i]", areaID);
			}
			if (regionID != 0) {
				/// Look up region
				try {
					AreaDB::Record rec = gAreaDB.getByAreaID(regionID);
					std::string regionName = rec.getString(AreaDB::Name);
					f16->shprint(5,40,"%s", regionName.c_str());
				} catch(AreaDB::NotFound)
				{
					//f16->print(5,40,"Unknown [%i]", regionID);
				}
			}
		}

		if (world->loading) {
			const char* loadstr = "Loading...";
			const char* oobstr = "Out of bounds";

			f16->shprint(video.xres/2 - f16->textwidth(loadstr)/2, /*video.yres/2-8*/ 0, world->oob?oobstr:loadstr);
		}
	}	
	else
	{
		ResetUniques();
		// draw 3D view
		video.set3D();
		int ErrorNum=glGetError();
		if(ErrorNum)
			gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n",ErrorNum);
		//glEnable(GL_LIGHTING);
		int start,end;
		if(Selecting)
			doSelection();
		ErrorNum=glGetError();
		if(ErrorNum)
			gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n",ErrorNum);

		video.set3D();

		glActiveTextureARB(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		world->draw();

		ResetUniques();
		video.set3D();
		
		if(hud)
		{
			video.set2D();
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glActiveTextureARB(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glColor4f(1,1,1,1);

			f16->shprint(5,0,"%.2f fps", gFPS);

			//char *sn = world->skies->getSkyName();
			//if (sn)	f16->print(5,60,"%s", sn);

			// TODO: look up WMO names/group names as well from some client db?
			unsigned int areaID = world->getAreaID();
			unsigned int regionID = 0;
			/// Look up area
			try {
				AreaDB::Record rec = gAreaDB.getByAreaID(areaID);
				std::string areaName = rec.getString(AreaDB::Name);
				regionID = rec.getUInt(AreaDB::Region);
				f16->shprint(5,20,"%s", areaName.c_str());
			} catch(AreaDB::NotFound)
			{
				/// Not found, unknown area
				//f16->print(5,20,"Unknown [%i]", areaID);
			}
			if (regionID != 0) {
				/// Look up region
				try {
					AreaDB::Record rec = gAreaDB.getByAreaID(regionID);
					std::string regionName = rec.getString(AreaDB::Name);
					f16->shprint(5,40,"%s", regionName.c_str());
				} catch(AreaDB::NotFound)
				{
					//f16->print(5,40,"Unknown [%i]", regionID);
				}
			}

			//f16->print(5,60,"%d", world->modelmanager.v);

			int time = ((int)world->time)%2880;
			int hh,mm;

            hh = time / 120;
			mm = (time % 120) / 2;
			
			//f16->print(5, 60, "%02d:%02d", hh,mm);
			f16->shprint(video.xres - 50, 60, "%02d:%02d", hh,mm);
			if(BrushType==0)
				f16->shprint(video.xres - 250, 20, "Brush Type: Flat");
			else if(BrushType==1)
				f16->shprint(video.xres - 250, 20, "Brush Type: Linear");
			else if(BrushType==2)
				f16->shprint(video.xres - 250, 20, "Brush Type: Smooth");
			
			if(terrainMode==0)
			{
				f16->shprint(video.xres - 250, 0, "Terrain Mode: Raise/Lower");
				f16->shprint(video.xres - 250, 40, "Brush Radius: %.2f",BrushRadius);
			}
			else
			{
				f16->shprint(video.xres - 250, 0, "Terrain Mode: Flatten/Blur");
				if(BrushRadius>30)
					f16->shprint(video.xres - 250, 40, "Brush Radius: %.2f (Blur 30)",BrushRadius);
				else
					f16->shprint(video.xres - 250, 40, "Brush Radius: %.2f",BrushRadius);
			}
			//f16->shprint(video.xres - 250, 40, "Object Pos (%.2f, %.2f, %.2f)",ObjPos.x,ObjPos.y,ObjPos.z);

			if(CurSelection!=0)
			{
				if(DetailSelection)
				{
					if(CurSelection->type==ENTRY_MODEL)
					{
						f16->shprint(5,63,"%s",CurSelection->returnName());
						f16->shprint(10,83,"%UniqueID: %d",CurSelection->data.model->d1);
						f16->shprint(10,103,"Pos: (%.2f, %.2f, %.2f)",CurSelection->data.model->pos.x,CurSelection->data.model->pos.y,CurSelection->data.model->pos.z);
						f16->shprint(10,123,"Rot: (%.2f, %.2f, %.2f)",CurSelection->data.model->dir.x,CurSelection->data.model->dir.y,CurSelection->data.model->dir.z);
						f16->shprint(10,143,"Scale: %d",(int)(CurSelection->data.model->sc*1024));
						f16->shprint(10,163,"Textures Used: %d",CurSelection->data.model->model->header.nTextures);
						for(unsigned int j=0;j<CurSelection->data.model->model->header.nTextures;j++)
							f16->shprint(15,183+20*j,"%d - %s",j,video.textures.items[CurSelection->data.model->model->textures[j]]->name.c_str());
						
					}
					else if(CurSelection->type==ENTRY_WMO)
					{
						f16->shprint(5,63,"%s",CurSelection->returnName());
						f16->shprint(10,83,"%UniqueID: %d",CurSelection->data.wmo->id);
						f16->shprint(10,103,"Pos: (%.2f, %.2f, %.2f)",CurSelection->data.wmo->pos.x,CurSelection->data.wmo->pos.y,CurSelection->data.wmo->pos.z);
						f16->shprint(10,123,"Rot: (%.2f, %.2f, %.2f)",CurSelection->data.wmo->dir.x,CurSelection->data.wmo->dir.y,CurSelection->data.wmo->dir.z);
						f16->shprint(10,143,"Lower Extents: (%.2f, %.2f, %.2f)",CurSelection->data.wmo->extents[0].x,CurSelection->data.wmo->extents[0].y,CurSelection->data.wmo->extents[0].z);
						f16->shprint(10,163,"Upper Extents: (%.2f, %.2f, %.2f)",CurSelection->data.wmo->extents[1].x,CurSelection->data.wmo->extents[1].y,CurSelection->data.wmo->extents[1].z);
						f16->shprint(10,183,"Textures Used: %d",CurSelection->data.wmo->wmo->nTextures);
						for(int j=0;j<CurSelection->data.wmo->wmo->nTextures;j++)
						{
							if(j<25)
								f16->shprint(15,203+20*j,"%d - %s",j,CurSelection->data.wmo->wmo->textures[j].c_str());
							else
								f16->shprint((video.xres-15)/2,203+20*(j-25),"%d - %s",j,CurSelection->data.wmo->wmo->textures[j].c_str());
						}
					}
					else if(CurSelection->type==ENTRY_MAPCHUNK)
					{
						int TextOffset=0;
						f16->shprint(5,63,"Map Chunk %d, %d from Tile %d, %d",CurSelection->data.mapchunk->px,CurSelection->data.mapchunk->py,CurSelection->data.mapchunk->mt->x,CurSelection->data.mapchunk->mt->z);
						f16->shprint(10,83,"Flags %x",CurSelection->data.mapchunk->Flags);
						if(CurSelection->data.mapchunk->Flags&FLAG_SHADOW)
						{
							f16->shprint(15,103+TextOffset,"Shadows Enabled");
							TextOffset+=20;
						}
						if(CurSelection->data.mapchunk->Flags&FLAG_IMPASS)
						{
							f16->shprint(15,103+TextOffset,"Impassible Chunk Enabled");
							TextOffset+=20;
						}
						if(CurSelection->data.mapchunk->Flags&FLAG_LQ_RIVER)
						{
							f16->shprint(15,103+TextOffset,"River Enabled");
							TextOffset+=20;
						}
						if(CurSelection->data.mapchunk->Flags&FLAG_LQ_OCEAN)
						{
							f16->shprint(15,103+TextOffset,"Ocean Enabled");
							TextOffset+=20;
						}
						if(CurSelection->data.mapchunk->Flags&FLAG_LQ_MAGMA)
						{
							f16->shprint(15,103+TextOffset,"Lava Enabled");
							TextOffset+=20;
						}
						f16->shprint(10,103+TextOffset,"Textures %d",CurSelection->data.mapchunk->nTextures);
						TextOffset+=20;
						for(int q=0;q<CurSelection->data.mapchunk->nTextures;q++)
						{
							const char *EffectModel;
							f16->shprint(20,103+TextOffset,"%d - %s  Flags - %x Effect ID - %d",q,video.textures.items[CurSelection->data.mapchunk->textures[q]]->name.c_str(),CurSelection->data.mapchunk->texFlags[q],CurSelection->data.mapchunk->effectID[q]);
							TextOffset+=20;
							if(Selection->data.mapchunk->effectID[q]!=0)
							for(int r=0;r<4;r++)
							{
								EffectModel=getGroundEffectDoodad(CurSelection->data.mapchunk->effectID[q],r);
								if(EffectModel!=0)
								{
										f16->shprint(30,103+TextOffset,"%d - World\\NoDXT\\%s",r,EffectModel);
										TextOffset+=20;
								}
							}
							TextOffset+=5;
						}
						
					}
				}
			}
			
			if((Selection!=0)&&(Selection->type!=ENTRY_FAKE))
			{
				unsigned int SelTex;
				SelTex=world->getSelectTex();
				int TextOffset=0;

				if((Selection->type!=ENTRY_MAPCHUNK)&&(SelTex!=0xFFFFFFFF)&&(video.textures.items.find(SelTex)!=video.textures.items.end()))
				{
					TextOffset=-20;
					f16->shprint(5,video.yres-20,"%s",video.textures.items[SelTex]->name.c_str());
				}
				
				f16->shprint(5,video.yres-20+TextOffset,"%s",Selection->returnName());
				f16->shprint(5, video.yres-40+TextOffset, "(%.0f, %.0f, %.0f) Tile %.0f %.0f (%.0f, %.0f)", 
				-(world->camera.x - ZEROPOINT), 
				-(world->camera.z - ZEROPOINT),
				world->camera.y,
				(-(TILESIZE/2))/TILESIZE,
				(world->camera.z-(TILESIZE/2))/TILESIZE,
				world->camera.x,
				world->camera.z);

				if(DetailSelection)
				{
					if(CurSelection==0)
					{
						if(Selection->type==ENTRY_MODEL)
						{
							f16->shprint(5,63,"%s",Selection->returnName());
							f16->shprint(10,83,"%UniqueID: %d",Selection->data.model->d1);
							f16->shprint(10,103,"Pos: (%.2f, %.2f, %.2f)",Selection->data.model->pos.x,Selection->data.model->pos.y,Selection->data.model->pos.z);
							f16->shprint(10,123,"Rot: (%.2f, %.2f, %.2f)",Selection->data.model->dir.x,Selection->data.model->dir.y,Selection->data.model->dir.z);
							f16->shprint(10,143,"Scale: %d",(int)(Selection->data.model->sc*1024));
							f16->shprint(10,163,"Textures Used: %d",Selection->data.model->model->header.nTextures);
							for(unsigned int j=0;j<Selection->data.model->model->header.nTextures;j++)
								f16->shprint(15,183+20*j,"%d - %s",j,video.textures.items[Selection->data.model->model->textures[j]]->name.c_str());
							
						}
						else if(Selection->type==ENTRY_WMO)
						{
							f16->shprint(5,63,"%s",Selection->returnName());
							f16->shprint(10,83,"%UniqueID: %d",Selection->data.wmo->id);
							f16->shprint(10,103,"Pos: (%.2f, %.2f, %.2f)",Selection->data.wmo->pos.x,Selection->data.wmo->pos.y,Selection->data.wmo->pos.z);
							f16->shprint(10,123,"Rot: (%.2f, %.2f, %.2f)",Selection->data.wmo->dir.x,Selection->data.wmo->dir.y,Selection->data.wmo->dir.z);
							f16->shprint(10,143,"Lower Extents: (%.2f, %.2f, %.2f)",Selection->data.wmo->extents[0].x,Selection->data.wmo->extents[0].y,Selection->data.wmo->extents[0].z);
							f16->shprint(10,163,"Upper Extents: (%.2f, %.2f, %.2f)",Selection->data.wmo->extents[1].x,Selection->data.wmo->extents[1].y,Selection->data.wmo->extents[1].z);
							f16->shprint(10,183,"Textures Used: %d",Selection->data.wmo->wmo->nTextures);
							for(int j=0;j<Selection->data.wmo->wmo->nTextures;j++)
							{
								if(j<25)
									f16->shprint(15,203+20*j,"%d - %s",j,Selection->data.wmo->wmo->textures[j].c_str());
								else
									f16->shprint((video.xres-15)/2,203+20*(j-25),"%d - %s",j,Selection->data.wmo->wmo->textures[j].c_str());
							}
						}
						else if(Selection->type==ENTRY_MAPCHUNK)
						{
							int TextOffset=0;
							f16->shprint(5,63,"Map Chunk %d, %d from Tile %d, %d",Selection->data.mapchunk->px,Selection->data.mapchunk->py,Selection->data.mapchunk->mt->x,Selection->data.mapchunk->mt->z);
							f16->shprint(10,83,"Flags %x",Selection->data.mapchunk->Flags);
							if(Selection->data.mapchunk->Flags&FLAG_SHADOW)
							{
								f16->shprint(15,103+TextOffset,"Shadows Enabled");
								TextOffset+=20;
							}
							if(Selection->data.mapchunk->Flags&FLAG_IMPASS)
							{
								f16->shprint(15,103+TextOffset,"Impassible Chunk Enabled");
								TextOffset+=20;
							}
							if(Selection->data.mapchunk->Flags&FLAG_LQ_RIVER)
							{
								f16->shprint(15,103+TextOffset,"River Enabled");
								TextOffset+=20;
							}
							if(Selection->data.mapchunk->Flags&FLAG_LQ_OCEAN)
							{
								f16->shprint(15,103+TextOffset,"Ocean Enabled");
								TextOffset+=20;
							}
							if(Selection->data.mapchunk->Flags&FLAG_LQ_MAGMA)
							{
								f16->shprint(15,103+TextOffset,"Lava Enabled");
								TextOffset+=20;
							}
							f16->shprint(10,103+TextOffset,"Textures %d",Selection->data.mapchunk->nTextures);
							TextOffset+=20;
							for(int q=0;q<Selection->data.mapchunk->nTextures;q++)
							{
								const char *EffectModel;
								f16->shprint(20,103+TextOffset,"%d - %s  Flags - %x Effect ID - %d",q,video.textures.items[Selection->data.mapchunk->textures[q]]->name.c_str(),Selection->data.mapchunk->texFlags[q],Selection->data.mapchunk->effectID[q]);
								TextOffset+=20;
								if(Selection->data.mapchunk->effectID[q]!=0)
								for(int r=0;r<4;r++)
								{
									EffectModel=getGroundEffectDoodad(Selection->data.mapchunk->effectID[q],r);
									if(EffectModel!=0)
									{
											f16->shprint(30,103+TextOffset,"%d - World\\NoDXT\\%s",r,EffectModel);
											TextOffset+=20;
									}
								}
								TextOffset+=5;								
							}
							setChunkWindow(Selection->data.mapchunk);
						}
					}
				}
			}
			else if((CurSelection==0)||(DetailSelection==false))
			{
				f16->shprint(5, video.yres-22, "(%.0f, %.0f, %.0f) Tile %.0f %.0f (%.0f, %.0f)", 
				-(world->camera.x - ZEROPOINT), 
				-(world->camera.z - ZEROPOINT),
				world->camera.y,
				(world->camera.x-(TILESIZE/2))/TILESIZE,
				(world->camera.z-(TILESIZE/2))/TILESIZE,
				world->camera.x,
				world->camera.z);
			}
			//f16->shprint(video.xres-300,video.yres/2,"(%.2f, %.2f, %.2f)",ObjPos.x,ObjPos.y,ObjPos.z);
		}

		if (world->loading) {
			const char* loadstr = "Loading...";
			const char* oobstr = "Out of bounds";

			video.set2D();
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glActiveTextureARB(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTextureARB(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glColor4f(1,1,1,1);

			f16->shprint(video.xres/2 - f16->textwidth(loadstr)/2, /*video.yres/2-8*/ 0, world->oob?oobstr:loadstr);
		}

		/*
		f16->print(5,20,"C: %.1f", world->l_const);
		f16->print(5,40,"L: %.2f", world->l_linear);
		f16->print(5,60,"Q: %.3f", world->l_quadratic);
		*/
		ErrorNum=glGetError();
		if(ErrorNum)
			gLog("[World of Warcraft Studio - Editor] - OpenGL Error %d\n",ErrorNum);
	}

};

extern bool DrawMapContour;
extern bool drawFlags;

void Test::keypressed(SDL_KeyboardEvent *e)
{

	if (e->type == SDL_KEYDOWN) {
		if((LastClicked)&&(LastClicked->processKey(e->keysym.sym,(e->keysym.mod&KMOD_SHIFT)!=0,(e->keysym.mod&KMOD_CTRL)!=0,(e->keysym.mod&KMOD_ALT)!=0)))
			return;
		// key DOWN

		// quit
		if (e->keysym.sym == SDLK_ESCAPE) {
		    gPop = true;
		}

		// movement
		if (e->keysym.sym == SDLK_w) {
			moving = 1.0f;
		}

		if (e->keysym.sym == SDLK_s) {
			moving = -1.0f;
		}

		if (e->keysym.sym == SDLK_a) {
			strafing = -1.0f;
		}

		if (e->keysym.sym == SDLK_d) {
			strafing = 1.0f;
		}

		if (e->keysym.sym == SDLK_e) {
			updown = -1.0f;
		}

		if (e->keysym.sym == SDLK_q) {
			updown = 1.0f;
		}

		// invertmouse
		if (e->keysym.sym == SDLK_i) {
			mousedir *= -1.0f;
		}

		// move speed
		if (e->keysym.sym == SDLK_p) {
			if((LCtrlDown||RCtrlDown)&&(LShiftDown||RShiftDown))
				Saving=true;
			else
				movespd *= 2.0f;
		}

		if (e->keysym.sym == SDLK_o) {
			movespd *= 0.5f;
		}

		// turn around
		if (e->keysym.sym == SDLK_r) {
			ah += 180.0f;
		}

		// testing
		if (e->keysym.sym == SDLK_n) {
			world->modelmanager.v++;
		}

		if (e->keysym.sym == SDLK_b) {
			world->modelmanager.v--;
			if (world->modelmanager.v<0) world->modelmanager.v = 0;
		}

		// toggles
		if (e->keysym.sym == SDLK_t) {
			terrainMode++;
			if(terrainMode>1)
				terrainMode=0;
		}

		if (e->keysym.sym == SDLK_l) {
			world->lighting = !world->lighting;
		}

		if (e->keysym.sym == SDLK_F1) {
			world->drawmodels = !world->drawmodels;
		}

		if (e->keysym.sym == SDLK_F2) {
			world->drawdoodads = !world->drawdoodads;
		}

		if (e->keysym.sym == SDLK_F3) {
			world->drawterrain = !world->drawterrain;
		}

		if (e->keysym.sym == SDLK_F4) {
			if(LShiftDown||RShiftDown)
				Selecting=!Selecting;
			else
				hud = !hud;			
		}

		if (e->keysym.sym == SDLK_F7) {
			world->drawlines = !world->drawlines;
		}

		if (e->keysym.sym == SDLK_F6) {
			world->drawwmo = !world->drawwmo;
		}

		if (e->keysym.sym == SDLK_F8) {
			DetailSelection = !DetailSelection;
		}

		if (e->keysym.sym == SDLK_F9) {
			DrawMapContour = !DrawMapContour;
		}

		if (e->keysym.sym == SDLK_h) {
			world->drawhighres = !world->drawhighres;
		}

		if (e->keysym.sym == SDLK_f) {
			world->drawfog = !world->drawfog;
		}

		if (e->keysym.sym == SDLK_j) {
			world->reloadTile(((int)world->camera.x)/TILESIZE,((int)world->camera.z)/TILESIZE);
		}

		if (e->keysym.sym == SDLK_k) {
			world->saveTile(((int)world->camera.x)/TILESIZE,((int)world->camera.z)/TILESIZE);
		}

		if (e->keysym.sym == SDLK_F10) {
			video.textures.reload();
		}

		if (e->keysym.sym == SDLK_F11) {
			world->modelmanager.reload();
		}

		if (e->keysym.sym == SDLK_F12) {
			world->wmomanager.reload();
		}

		if (e->keysym.sym == SDLK_KP_PLUS || e->keysym.sym == SDLK_PLUS) {
			world->fogdistance += 60.0f;
		}

		if (e->keysym.sym == SDLK_KP_MINUS || e->keysym.sym == SDLK_MINUS) {
			world->fogdistance -= 60.0f;
		}

		// minimap
		if (e->keysym.sym == SDLK_m) {
			mapmode = !mapmode;
		}

		if(e->keysym.sym == SDLK_v)
		{
			TestSelection=!TestSelection;
			
		}

		if(e->keysym.sym == SDLK_y)
		{
			BrushType++;
			if(BrushType>2)
				BrushType=0;
		}

		if(e->keysym.sym == SDLK_g)
			drawFlags=!drawFlags;

		if(e->keysym.sym == SDLK_u)
		{
			TileMode=!TileMode;
			Selection=0;
			CurSelection=0;
		}

		if(e->keysym.sym == SDLK_0)
			world->LowerTerrain=-120.0f;
		/*
		// lighting
		if (e->keysym.sym == SDLK_1) {
			world->l_const -= 0.1f;
			if (world->l_const <= 0) world->l_const = 0.0f;
		}
		if (e->keysym.sym == SDLK_2) {
			world->l_const += 0.1f;
		}
		if (e->keysym.sym == SDLK_3) {
			world->l_linear -= 0.01f;
			if (world->l_linear <= 0) world->l_linear = 0.0f;
		}
		if (e->keysym.sym == SDLK_4) {
			world->l_linear += 0.01f;
		}
		if (e->keysym.sym == SDLK_5) {
			world->l_quadratic -= 0.001f;
			if (world->l_quadratic <= 0) world->l_quadratic = 0.0f;
		}
		if (e->keysym.sym == SDLK_6) {
			world->l_quadratic += 0.001f;
		}
		*/

		if (e->keysym.sym == SDLK_F5) {
			FILE *bf = fopen("bookmarks.txt","a");
			// copied from above: retreive area name for bookmarks, too
			unsigned int areaID = world->getAreaID();
			unsigned int regionID = 0;
			std::string areaName = "";
			try {
				AreaDB::Record rec = gAreaDB.getByAreaID(areaID);
				areaName = rec.getString(AreaDB::Name);
				//regionID = rec.getUInt(AreaDB::Region);
			} catch(AreaDB::NotFound)
			{
				if (world->gnWMO==0) areaName = "Unknown location";
			}
			if (regionID != 0) {
				/// Look up region
				try {
					AreaDB::Record rec = gAreaDB.getByAreaID(regionID);
					areaName = rec.getString(AreaDB::Name);
				} catch(AreaDB::NotFound)
				{
					// do nothing
				}
			}
			fprintf(bf, "%s %f %f %f  %f %f  %s\n", world->basename.c_str(), world->camera.x, world->camera.y, world->camera.z, ah, av, areaName.c_str());
			fclose(bf);
		}

		if (e->keysym.sym==SDLK_LSHIFT)
			LShiftDown=true;
		if (e->keysym.sym==SDLK_RSHIFT)
			RShiftDown=true;
		if (e->keysym.sym==SDLK_LALT)
			LAltDown=true;
		if (e->keysym.sym==SDLK_RALT)
			RAltDown=true;
		if (e->keysym.sym==SDLK_LCTRL)
			LCtrlDown=true;
		if (e->keysym.sym==SDLK_RCTRL)
			RCtrlDown=true;

	} else {
		// key UP

		if (e->keysym.sym == SDLK_w) {
			if (moving > 0) moving = 0;
		}
		if (e->keysym.sym == SDLK_s) {
			if (moving < 0) moving = 0;
		}
		if (e->keysym.sym == SDLK_d) {
			if (strafing > 0) strafing = 0;
		}
		if (e->keysym.sym == SDLK_a) {
			if (strafing < 0) strafing = 0;
		}
		if (e->keysym.sym == SDLK_q) {
			if (updown > 0) updown = 0;
		}
		if (e->keysym.sym == SDLK_e) {
			if (updown < 0) updown = 0;
		}
		if (e->keysym.sym == SDLK_KP8) {
			ObjMove.z=1;
		}
		if (e->keysym.sym == SDLK_KP2) {
			ObjMove.z=-1;
		}
		if (e->keysym.sym == SDLK_KP6) {
			ObjMove.x=1;
		}
		if (e->keysym.sym == SDLK_KP4) {
			ObjMove.x=-1;
		}
		if (e->keysym.sym == SDLK_KP9) {
			ObjMove.y=1;
		}
		if (e->keysym.sym == SDLK_KP3) {
			ObjMove.y=-1;
		}
		if (e->keysym.sym == SDLK_KP7) {
			ObjRot.y=1;
		}
		if (e->keysym.sym == SDLK_KP1) {
			ObjRot.y=-1;
		}
		if (e->keysym.sym==SDLK_LSHIFT)
			LShiftDown=false;
		if (e->keysym.sym==SDLK_RSHIFT)
			RShiftDown=false;
		if (e->keysym.sym==SDLK_LCTRL)
			LCtrlDown=false;
		if (e->keysym.sym==SDLK_RCTRL)
			RCtrlDown=false;
		if (e->keysym.sym==SDLK_LALT)
			LAltDown=false;
		if (e->keysym.sym==SDLK_RALT)
			RAltDown=false;
	}
}

void Test::mousemove(SDL_MouseMotionEvent *e)
{
	if ((look && !(LShiftDown||RShiftDown||LCtrlDown||RCtrlDown||LAltDown||RAltDown)) || fullscreen) {
		ah += e->xrel / XSENS;
		av += mousedir * e->yrel / YSENS;
		if (av < -80) av = -80;
		else if (av > 80) av = 80;
	}
	
	if (MoveObj)
	{
		mh = -video.ratio*e->xrel /float(video.xres);
		mv = -e->yrel /float(video.yres);
	}
	else
	{
		mh=0;
		mv=0;
	}

	if (LShiftDown||RShiftDown||LCtrlDown||RCtrlDown||LAltDown||RAltDown)
	{
		rh = e->xrel / XSENS * 5.0f;
		rv = e->yrel / YSENS * 5.0f;
	}

	if(leftMouse&&(LAltDown||RAltDown))
	{
		BrushRadius+=e->xrel /( XSENS);
		if(BrushRadius>100.0f)
			BrushRadius=100.0f;
		else if(BrushRadius<1.0f)
			BrushRadius=1.0f;
	}

	if(TileMode)
	{
		if(leftMouse&&LastClicked)
			LastClicked->processLeftDrag(e->x-4,e->y-4,e->xrel,e->yrel);
	}

	MouseX=e->x;
	MouseY=e->y;

}

void Test::mouseclick(SDL_MouseButtonEvent *e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		if(e->button==SDL_BUTTON_RIGHT)
			look = true;
		if(e->button==SDL_BUTTON_LEFT)
		{
			if(TileMode)
				LastClicked=tileFrames.processLeftClick((float)MouseX,(float)MouseY);
			else
			{
				if(!Selecting)
					doSelection();
				if((Selection!=0)&&(Selection->type==ENTRY_MODEL))
					CurSelection=Selection;
				else
					CurSelection=0;
			}
			leftMouse=true;	
		}
		if(e->button==SDL_BUTTON_MIDDLE)
			MoveObj=true;

	} else if (e->type == SDL_MOUSEBUTTONUP) {
		if(e->button==SDL_BUTTON_LEFT)
		{
			leftMouse=false;
			if(LastClicked!=0)
				LastClicked->processUnclick();
		}
		if(e->button==SDL_BUTTON_RIGHT)
			look = false;
		if(e->button==SDL_BUTTON_MIDDLE)
			MoveObj=false;
	}
}