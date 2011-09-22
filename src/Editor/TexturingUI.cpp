#include "OpenGL.h"
#include "Video.h"
#include "MapTile.h"
#include "UI.h"
#include "AreaDB.h"

//Texture Files
std::vector<std::string>	textureNames;
std::vector<std::string>		tilesetNames;
std::vector<std::string>		tilesetDirectories;
char	filters[24][255];
std::vector<std::string>		filterNames;

//Texture Palette Window
bool		texturePaletteCreated=false;
minWindowUI	*windowTexturePalette;
textureUI	*curTextures[8];
ManagedItem	*theTextures[8];
ManagedItem	*selectedTexture;
std::map<GLuint, ManagedItem*>::iterator startTexture;

char CurTextureName[256];

//Selected Texture Window
minWindowUI	*windowSelectedTexture;
textureUI	*textureSelected;
textUI		*textSelectedTexture;

//Texture Loading Window
closeWindowUI	*windowTilesetLoader;

//Texture Filter Window
closeWindowUI	*windowTextureFilter;
bool	fileFilter[25];
bool	tilesetFilter[48];

//Map Chunk Window
closeWindowUI	*windowMapChunk;
textUI			*chunkLocation;
textUI			*chunkAreaID;
textUI			*chunkFlags;
textureUI		*chunkHoles[16];
checkboxUI		*chunkFlagChecks[5];
textUI			*chunkEffectID;
textUI			*chunkNumEffects;
textUI			*chunkEffectModels[4];
textureUI		*chunkTexture[4];
textUI			*chunkTextureNames[4];
textUI			*chunkTextureFlags[4];
textUI			*chunkTextureEffectID[4];

void LoadTextureNames()
{
	char	Temp[512];
	char	*T2;
	std::string	tString;
	FILE *fid;

	fid=fopen("textures.txt","rt");
	while(!feof(fid))
	{
		fgets(Temp,512,fid);
		int i=0;
		while(Temp[i]!=0&&Temp[i]!=10)
			i++;
		Temp[i]=0;
		tString=Temp;
		textureNames.push_back(tString);
	}
	fclose(fid);

	fid=fopen("tilesets.txt","rt");
	while(!feof(fid))
	{
		int i=0;
		fgets(Temp,512,fid);
		while(Temp[i]!=9)
			i++;
		Temp[i]=0;		
		T2=Temp+i+1;

		i++;
		while(Temp[i]!=10&&Temp[i]!=0)
			i++;
		Temp[i]=0;
		tString=Temp;
		tilesetNames.push_back(tString);
		
		tString=T2;
		tilesetDirectories.push_back(tString);
	}
	fclose(fid);

	fid=fopen("filters.txt","rt");
	int j=0;
	while(!feof(fid))
	{
		int i=0;
		fgets(Temp,512,fid);
		while(Temp[i]!=9)
			i++;
		Temp[i]=0;		
		T2=Temp+i+1;

		i++;
		while(Temp[i]!=10&&Temp[i]!=0)
			i++;
		Temp[i]=0;
		tString=Temp;
		filterNames.push_back(tString);
		
		//tString=T2;
		//filters.push_back(tString);
		strcpy(filters[j],T2);
		j++;
	}
	fclose(fid);
}

int checkTileset(const char*Texture)
{
	std::vector<std::string>::iterator	i;
	int j=0;
	for(i=tilesetDirectories.begin();i!=tilesetDirectories.end();i++)
	{
		if(strstr(Texture,i->c_str())!=NULL)
			return j;
		j++;
	}
	return -1;
}

std::map<GLuint, ManagedItem*>::iterator FindCurTexture()
{
	//want to guarantee the texture is there
	video.textures.add(CurTextureName);

	//now find where it is
	for(std::map<GLuint, ManagedItem*>::iterator i = video.textures.items.begin(); i != video.textures.items.end(); i++)
		if(strcmp(CurTextureName,i->second->name.c_str()) == 0)
			return i;
}

inline bool TextureInPalette(const char *TexName)
{
	bool result,tilesetFiltering, filenameFilters[25],fFilter,filteringByFName;
	const char *FName;
	int i;
	
	result=(strstr(TexName,"tileset")!=NULL);

	if(!result)
		return false;

	i=0;
	while(TexName[i]!=0)
	{
		if(TexName[i]=='\\')
			FName=TexName+i+1;
		i++;
	}

	filenameFilters[24]=true;
	for(i=0;i<24;i++)
	{
		if(strstr(FName,filters[i])!=NULL)
		{
			filenameFilters[i]=true;
			filenameFilters[24]=false;
		}
		else
			filenameFilters[i]=false;
	}

	fFilter=false;
	filteringByFName=false;
	for(i=0;i<25;i++)
		if(fileFilter[i])
		{
			fFilter=fFilter||filenameFilters[i];
			filteringByFName=true;
		}

	if(filteringByFName)
		result=result&&fFilter;
	
	if(!result)
		return false;
	
	tilesetFiltering=false;
	for(i=0;i<48;i++)
		tilesetFiltering|=tilesetFilter[i];
	
	if(tilesetFiltering)
	{
		tilesetFiltering=false;
		for(i=0;i<48;i++)
			if(tilesetFilter[i])
				if(strstr(TexName,tilesetDirectories[i].c_str())!=NULL)
					tilesetFiltering=true;
		result=result&tilesetFiltering;
	}

	return result;//&&(strstr(TexName,"dungeons\\textures\\floor")==NULL);
}

void updateTextures()
{
	std::map<GLuint, ManagedItem*>::iterator t=FindCurTexture();
	for(int i=0;i<8;i++)
	{
CheckForEnd:
		if(t!=video.textures.items.end())
		{
			//See if the item has either of the below in it
			//tileset
			//dungeons\\textures\\floor
			const char *TexName;
			TexName=t->second->name.c_str();
			if(!TextureInPalette(TexName))
			{
				t++;
				goto CheckForEnd;
			}
			curTextures[i]->hidden=false;
			curTextures[i]->setTexture(t->first);
			theTextures[i]=t->second;
			if(theTextures[i]==selectedTexture)
				curTextures[i]->setHighlight(true);
			else
				curTextures[i]->setHighlight(false);
			t++;
		}
		else
			curTextures[i]->hidden=true;
	}
}

void incrementTextures(frame *button,int id)
{
	std::map<GLuint, ManagedItem*>::iterator t=FindCurTexture();
	for(int i=0;i<8;i++)
	{
CheckForEnd2:
		if(t==video.textures.items.end())
			return;
		if(!TextureInPalette(t->second->name.c_str()))
		{
			t++;
			goto CheckForEnd2;
		}
		/*if((strstr(TexName,"tileset")==NULL)&&(strstr(TexName,"dungeons\\textures\\floor")==NULL))
		{
			t++;
			goto CheckForEnd3;
		}*/
		t++;
	}
	strcpy(CurTextureName,t->second->name.c_str());
	updateTextures();
}

void decrementTextures(frame *button,int id)
{
	std::map<GLuint, ManagedItem*>::iterator t=FindCurTexture();
	for(int i=0;i<8;i++)
	{
CheckForBegin:
		if(t==video.textures.items.begin())
			return;
		if(!TextureInPalette(t->second->name.c_str()))
		{
			t--;
			goto CheckForBegin;
		}
		t--;
	}
	strcpy(CurTextureName,t->second->name.c_str());
	updateTextures();
}

void updatedSelectedTexture(const ManagedItem* T)
{
	textureSelected->setTexture(video.textures.add(T->name.c_str()));
	textSelectedTexture->setText(T->name.c_str());
}

void texturePaletteClick(frame *f, int id)
{
	if(curTextures[id]->hidden)
		return;
	selectedTexture=theTextures[id];
	if(textureSelected)
		updatedSelectedTexture(selectedTexture);
	for(int i=0;i<8;i++)
		if(i==id)
			curTextures[i]->setHighlight(true);
		else
			curTextures[i]->setHighlight(false);
}

void showTextureLoader(frame *button,int id)
{
	windowTilesetLoader->hidden=false;
}

void showTextureFilter(frame *button,int id)
{
	windowTextureFilter->hidden=false;
}

frame *CreateTexturePalette()
{
	windowTexturePalette=new minWindowUI(2.0f,video.yres-188.0f,284.0f,186.0f,"Texture Palette");
	windowTexturePalette->movable=true;

	for(int i=0;i<8;i++)
	{
		curTextures[i]=new textureUI(8.0f+(i%4)*68.0f,22.0f+(i/4)*68.0f,64.0f,64.0f,video.textures.add("tileset\\generic\\black.blp"));
		curTextures[i]->setClickFunc(texturePaletteClick,i);
		windowTexturePalette->addChild(curTextures[i]);
	}
	texturePaletteCreated=true;
	
	startTexture=video.textures.items.begin();
	strcpy(CurTextureName,startTexture->second->name.c_str());
	//incrementTextures(0,0);
	//decrementTextures(0,0);
	texturePaletteClick(0,0);
	updateTextures();

	float DistFromMiddle=54.0f;
	buttonUI	*B1;
	B1=new buttonUI(284.0f/2.0f+DistFromMiddle,2.0f,20.0f,20.0f,video.textures.add("Interface\\Buttons\\UI-SpellbookIcon-NextPage-Up.blp"),video.textures.add("Interface\\Buttons\\UI-SpellbookIcon-NextPage-Down.blp"));
	B1->setClickFunc(incrementTextures,0);
	windowTexturePalette->addChild(B1);

	B1=new buttonUI(284.0f/2.0f-DistFromMiddle-20.0f,2.0f,20.0f,20.0f,video.textures.add("Interface\\Buttons\\UI-SpellbookIcon-PrevPage-Up.blp"),video.textures.add("Interface\\Buttons\\UI-SpellbookIcon-PrevPage-Down.blp"));
	B1->setClickFunc(decrementTextures,0);
	windowTexturePalette->addChild(B1);

	B1=new buttonUI(145.0f,160.0f,132.0f,28.0f,video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"),video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Down.blp"));
	B1->setText("Load Textures");
	B1->setClickFunc(showTextureLoader,0);
	windowTexturePalette->addChild(B1);

	B1=new buttonUI(7.0f,160.0f,132.0f,28.0f,video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"),video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Down.blp"));
	B1->setText("Filter Textures");
	B1->setClickFunc(showTextureFilter,0);
	windowTexturePalette->addChild(B1);

	return windowTexturePalette;
}

frame *CreateSelectedTexture()
{
	windowSelectedTexture=new minWindowUI(video.xres-148.0f-128.0f,video.yres-162.0f-128.0f,274.0f,288.0f,"Current Texture");
	windowSelectedTexture->movable=true;

	if(selectedTexture!=0)
		textureSelected=new textureUI(9.0f,24.0f,256.0f,256.0f,video.textures.add(selectedTexture->name.c_str()));
	else
		textureSelected=new textureUI(9.0f,24.0f,256.0f,256.0f,video.textures.add("tileset\\generic\\black.blp"));
	windowSelectedTexture->addChild(textureSelected);

	if(selectedTexture!=0)
		textSelectedTexture=new textUI(274.0f/2.0f,264.0f,selectedTexture->name.c_str());
	else
		textSelectedTexture=new textUI(274.0f/2.0f,264.0f,"tileset\\generic\\black.blp");
	textSelectedTexture->setFont(&arialn13);
	textSelectedTexture->setJustify(UI_JUSTIFY_CENTER);
	textSelectedTexture->setBackground(0.0f,0.0f,0.0f,0.5f);
	windowSelectedTexture->addChild(textSelectedTexture);

	return windowSelectedTexture;
}

void LoadTileset(frame *button,int id)
{
	std::vector<std::string>::iterator i;
	for(i=textureNames.begin();i!=textureNames.end();i++)
	{
		if(checkTileset(i->c_str())==id)
			video.textures.add(i->c_str());
	}
	updateTextures();
}

frame *CreateTilesetLoader()
{	
	buttonUI		*name;
	LoadTextureNames();
	windowTilesetLoader=new closeWindowUI(video.xres/2.0f-308.0f,video.yres/2.0f-139.0f,616.0f,279.0f,"Tileset Loading");
	windowTilesetLoader->movable=true;

	for(int i=0;i<48;i++)
	{
		name=new buttonUI(5+152.0f*(i/12),22+21*(i%12)+1,150.0f,28.0f,video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"),video.textures.add("Interface\\Buttons\\UI-DialogBox-Button-Down.blp"));
		name->setText(tilesetNames[i].c_str());
		name->setClickFunc(LoadTileset,i);
		windowTilesetLoader->addChild(name);
	}
	windowTilesetLoader->hidden=true;

	return windowTilesetLoader;
}

void clickFilterTexture(bool value,int id)
{
	tilesetFilter[id]=value;
	updateTextures();
}

void clickFileFilterTexture(bool value,int id)
{
	fileFilter[id]=value;
	updateTextures();
}

frame *CreateTextureFilter()
{	
	checkboxUI		*check;
	textUI			*text;
	LoadTextureNames();
	windowTextureFilter=new closeWindowUI(video.xres/2.0f-308.0f,video.yres/2.0f-314.5f,616.0f,630.0f,"Texture Filtering");
	windowTextureFilter->movable=true;
	windowTextureFilter->hidden=true;

	//Filename Filters
	text=new textUI(308.0f,26.0f,"Filename Filters");
	text->setFont(&arial14);
	text->setJustify(UI_JUSTIFY_CENTER);
	windowTextureFilter->addChild(text);

	for(int i=0;i<24;i++)
	{
		check=new checkboxUI(5+152.0f*(i/6),42+30*((i%6))+1,filterNames[i].c_str());
		check->setClickFunc(clickFileFilterTexture,i);
		windowTextureFilter->addChild(check);
	}

	check=new checkboxUI(240.0f,42+30*6+1,"Misc (Everything Else)");
	check->setClickFunc(clickFileFilterTexture,24);
	windowTextureFilter->addChild(check);

	//Tileset Filters
	text=new textUI(308.0f,254.0f,"Tileset Filters");
	text->setFont(&arial14);
	text->setJustify(UI_JUSTIFY_CENTER);
	windowTextureFilter->addChild(text);

	for(int i=0;i<48;i++)
	{
		check=new checkboxUI(5+152.0f*(i/12),42+30*((i%12)+3+4)+15,tilesetNames[i].c_str());
		check->setClickFunc(clickFilterTexture,i);
		windowTextureFilter->addChild(check);
	}
	//windowTextureFilter->hidden=true;

	return windowTextureFilter;
}

/*closeWindowUI	*windowMapChunk;
textUI			*chunkLocation;
textUI			*chunkAreaID;
textUI			*chunkFlags;
textureUI		*chunkHoles[16];
checkboxUI		*chunkFlagChecks[5];
textUI			*chunkEffectID;
textUI			*chunkNumEffects;
textUI			*chunkEffectModels[4];
textureUI		*chunkTexture[4];
textUI			*chunkTextureNames[4];
textUI			*chunkTextureFlags[4];
textUI			*chunkTextureEffectID[4];
*/

frame *CreateMapChunkWindow()
{
	window *chunkSettingsWindow,*chunkTextureWindow,*chunkEffectWindow;
	windowMapChunk=new closeWindowUI(video.xres/2.0f-316.0f,video.yres/2.0f-162.0f,634.0f,337.0f,"Map Chunk Settings");
	windowMapChunk->movable=true;
	
	chunkSettingsWindow=new window(11.0f,26.0f,300.0f,300.0f);
	windowMapChunk->addChild(chunkSettingsWindow);

	chunkLocation=new textUI(5.0f,4.0f,"Chunk x, y of Tile x, y at (x, y, z)");
	chunkLocation->setFont(&arial14);
	chunkLocation->setJustify(UI_JUSTIFY_LEFT);
	chunkSettingsWindow->addChild(chunkLocation);

	chunkAreaID=new textUI(5.0,chunkLocation->y+25.0f,"AreaID:");
	chunkAreaID->setFont(&arial14);
	chunkAreaID->setJustify(UI_JUSTIFY_LEFT);
	chunkSettingsWindow->addChild(chunkAreaID);

	chunkFlags=new textUI(5.0,chunkAreaID->y+25.0f,"Flags:");
	chunkFlags->setFont(&arial14);
	chunkFlags->setJustify(UI_JUSTIFY_LEFT);
	chunkSettingsWindow->addChild(chunkFlags);

	chunkFlagChecks[0]=new checkboxUI(6,chunkFlags->y+22.0f,"Shadow");
	chunkSettingsWindow->addChild(chunkFlagChecks[0]);

	chunkFlagChecks[1]=new checkboxUI(150,chunkFlags->y+22.0f,"Impassible");
	chunkSettingsWindow->addChild(chunkFlagChecks[1]);

	chunkFlagChecks[2]=new checkboxUI(chunkFlagChecks[0]->x,chunkFlagChecks[0]->y+30.0f,"River");
	chunkSettingsWindow->addChild(chunkFlagChecks[2]);

	chunkFlagChecks[3]=new checkboxUI(chunkFlagChecks[1]->x,chunkFlagChecks[1]->y+30.0f,"Ocean");
	chunkSettingsWindow->addChild(chunkFlagChecks[3]);

	chunkFlagChecks[4]=new checkboxUI(chunkFlagChecks[2]->x,chunkFlagChecks[2]->y+30.0f,"Magma");
	chunkSettingsWindow->addChild(chunkFlagChecks[4]);

	chunkEffectID=new textUI(5.0f,chunkFlagChecks[4]->y+35.0f,"EffectID:");
	chunkEffectID->setFont(&arial14);
	chunkEffectID->setJustify(UI_JUSTIFY_LEFT);
	chunkSettingsWindow->addChild(chunkEffectID);
	chunkNumEffects=new textUI(150.0f,chunkEffectID->y,"Num Effects:");
	chunkNumEffects->setFont(&arial14);
	chunkNumEffects->setJustify(UI_JUSTIFY_LEFT);
	chunkSettingsWindow->addChild(chunkNumEffects);

	int i;    

	chunkEffectWindow=new window(8.0f,chunkEffectID->y+23.0f,284.0f,300.0f-(chunkEffectID->y+23.0f+8.0f));
	chunkSettingsWindow->addChild(chunkEffectWindow);
	
	chunkEffectModels[0]=new textUI(8.0f,8.0f,"Effect Doodad");
	chunkEffectModels[0]->setFont(&arial14);
	chunkEffectModels[0]->setJustify(UI_JUSTIFY_LEFT);
	chunkEffectWindow->addChild(chunkEffectModels[0]);

    for(i=1;i<4;i++)
	{
		chunkEffectModels[i]=new textUI(8.0f,chunkEffectModels[i-1]->y+20.0f,"Effect Doodad");
		chunkEffectModels[i]->setFont(&arial14);
		chunkEffectModels[i]->setJustify(UI_JUSTIFY_LEFT);
		chunkEffectWindow->addChild(chunkEffectModels[i]);
	}

	chunkTextureWindow=new window(324.0f,26.0f,300.0f,300.0f);
	windowMapChunk->addChild(chunkTextureWindow);

	float yPos;

	yPos=11.0f;

	for(i=0;i<4;i++)
	{
		buttonUI	*RemoveTexture;

		chunkTexture[i]=new textureUI(10.0f,yPos,64.0f,64.0f,video.textures.add("tileset\\generic\\black.blp"));
		chunkTextureWindow->addChild(chunkTexture[i]);
		
		chunkTextureNames[i]=new textUI(83.0f,yPos+5.0f,"Texture Name");
		chunkTextureNames[i]->setFont(&arial14);
		chunkTextureNames[i]->setJustify(UI_JUSTIFY_LEFT);
		chunkTextureWindow->addChild(chunkTextureNames[i]);

		chunkTextureFlags[i]=new textUI(83.0f,yPos+30.0f,"Flags -");
		chunkTextureFlags[i]->setFont(&arial14);
		chunkTextureFlags[i]->setJustify(UI_JUSTIFY_LEFT);
		chunkTextureWindow->addChild(chunkTextureFlags[i]);

		chunkTextureEffectID[i]=new textUI(184.0f,yPos+30.0f,"EffectID -");
		chunkTextureEffectID[i]->setFont(&arial14);
		chunkTextureEffectID[i]->setJustify(UI_JUSTIFY_LEFT);
		chunkTextureWindow->addChild(chunkTextureEffectID[i]);


		yPos+=64.0f+8.0f;
	}
	scrollbarUI	*TempScroll=new scrollbarUI(10,10,150,10);
	chunkTextureWindow->addChild(TempScroll);

	return windowMapChunk;
}

void setChunkWindow(MapChunk *chunk)
{
	char Temp[255];

	sprintf(Temp,"Chunk %d, %d at (%.2f, %.2f, %.2f)",chunk->px,chunk->py,chunk->xbase,chunk->ybase,chunk->zbase);
	chunkLocation->setText(Temp);

	std::string areaName;
	try
	{
		AreaDB::Record rec = gAreaDB.getByAreaID(chunk->areaID);
		areaName = rec.getString(AreaDB::Name);
	}
	catch(...)
	{
		areaName = "";
	}
	sprintf(Temp,"AreaID: %s (%d)",areaName.c_str(),chunk->areaID);
	chunkAreaID->setText(Temp);
	
	sprintf(Temp,"Flags: %d",chunk->Flags);
	chunkFlags->setText(Temp);

	for(int i=0;i<5;i++)
		chunkFlagChecks[i]->setState(((chunk->Flags>>i)&1)==1);

	sprintf(Temp,"EffectID: %d",chunk->header.effectId);
	chunkEffectID->setText(Temp);

	sprintf(Temp,"Num Effects: %d",chunk->header.nEffectDoodad);
	chunkNumEffects->setText(Temp);
}