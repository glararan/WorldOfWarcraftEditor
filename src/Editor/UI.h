#include <vector>
#include <string>
#include "Vec3D.h"
#include "Quaternion.h"

class frame
{
protected:
	std::vector<frame *> children;
	
public:
	float x,y;
	float width, height;

	frame	*parent;
	frame *twindow;

	bool mouse;
	bool movable;
	bool hidden;
	bool clickable;
	bool parentLastLeftClick;
	bool lastLeftClick;

	frame(){movable=false;hidden=false;clickable=false;parent=0;};

	void addChild(frame *);
	virtual void render();
	virtual frame *processLeftClick(float mx,float my);
	virtual bool processLeftDrag(float mx,float my, float xChange, float yChange);
	virtual	void processUnclick(){};
	virtual bool processRightClick(float mx,float my);
	virtual bool processKey(char key, bool shift, bool alt, bool ctrl);
	void getOffset(float &xOff,float &yOff);
};

class window:public frame
{
private:
	GLuint	texture;
public:
	
	window(float x, float y, float width, float height);
	frame *processLeftClick(float mx,float my);
	void render();	
};

class slider:public frame
{
private:
	GLuint	texture;
	GLuint	sliderTexture;
	float	value;
	float	scale;
	float	offset;
	void (*func)(float);
	char	text[255];
public:
	void	setFunc(void (*f)(float));
	void	setValue(float f);
	void	setText(const char *);
	slider(float x, float y, float width, float s,float o);
	frame *processLeftClick(float mx,float my);
	bool processLeftDrag(float mx,float my, float xChange, float yChange);
	void render();	
};

class gradient:public frame
{
private:	
	Vec4D	MinColor;
	Vec4D	MaxColor;
	Vec4D	ClickColor;
	float	value;
	void (*clickFunc)(float);
public:
	bool	horiz;

	void	setValue(float f);
	void	setClickFunc(void (*f)(float));
	void	setMinColor(float r, float g, float b, float a);
	void	setMaxColor(float r, float g, float b, float a);
	void	setClickColor(float r, float g, float b, float a);
	frame	*processLeftClick(float mx,float my);
	bool	processLeftDrag(float mx,float my, float xChange, float yChange);
	void	render();
};

#define UI_JUSTIFY_LEFT		0
#define UI_JUSTIFY_CENTER	1
#define UI_JUSTIFY_RIGHT	2

class textUI:public frame
{
private:
	freetype::font_data	*font;
	char	text[255];
	int		justify;
	bool	background;
	float	bgColor[4];
public:
	textUI(float x,float y);
	textUI(float x,float y,const char *t);
	void	setText(const char *t);
	void	setJustify(int j);
	void	setFont(freetype::font_data	*font);
	void	setBackground(float r,float g,float b, float a);
	void	render();
};

class textureUI:public frame
{
private:
	GLuint	texture;
	bool	highlight;
	void (*clickFunc)(frame *,int);
	int		id;
public:
	textureUI(float x,float y,float width,float height,GLuint tex);
	void	setTexture(GLuint tex);	
	unsigned int	getTexture(){return texture;};
	void	render();

	frame *processLeftClick(float mx,float my);
	void	setClickFunc(void (*f)(frame *,int), int num);
	void	setHighlight(bool h){highlight=h;};
};

class buttonUI:public frame
{
private:
	GLuint	texture;
	GLuint	textureDown;
	void (*clickFunc)(frame *,int);
	int		id;

	
	bool	clicked;
	textUI	*text;
public:
	buttonUI(float x,float y,float width,float height,GLuint tex,GLuint texd);
	void	render();

	void	setText(const char *t);
	void	setFont(freetype::font_data	*font);

	frame *processLeftClick(float mx,float my);
	void	setClickFunc(void (*f)(frame *,int), int num);
	void processUnclick();
};

class minWindowUI:public window
{
private:
	GLuint	texture;
public:
	bool addedToParent;

	window	*minWindow;
	bool	minimized;
	
	minWindowUI(float x, float y, float width, float height,const char *title);
	void render();	
};

class closeWindowUI:public window
{
private:
	GLuint	texture;
public:
	
	closeWindowUI(float x, float y, float width, float height,const char *title);
};

class checkboxUI:public frame
{
private:
	textureUI	*box;
	textureUI	*check;
	textUI		*text;
	bool		checked;
	int			id;
	void (*clickFunc)(bool,int);
public:
	checkboxUI(float x,float y,const char *t);
	void	setText(const char*);
	void	setState(bool c);
	bool	getState();
	void	setClickFunc(void (*f)(bool,int),int i);
	frame	*processLeftClick(float mx,float my);
};

class textboxUI:public frame
{
private:
	char		text[256];
	int			length;
	textureUI	*background;
	textUI		*theText;
public:
	textboxUI(float xpos, float ypos, float w)
	{
		x=xpos;
		y=ypos;
		width=w;
		height=32;
		background=new textureUI(0,0,w,32,video.textures.add("Interface\\Common\\Common-Input-Border.blp"));
		//theText=new textUI(
	};
	frame *processLeftClick(float mx,float my){return this;};
	bool processKey(char key, bool shift, bool alt, bool ctrl){text[length]=key;text[length+1]=0;length++;return true;};
	void render();
};

class scrollbarUI:public frame
{
private:
	buttonUI	*ScrollUp;
	buttonUI	*ScrollDown;
	textureUI	*ScrollKnob;
	int			num;
	int			value;
	void		(*changeFunc)(int);	
public:
	void	clickReturn(int);
	scrollbarUI(float xpos, float ypos, float height, int num);
	int		getValue(){return value;};
	void	setValue(int i)
	{
		value=i;
		if(value>=num)
			value=num-1;
		if(value<0)
			value=0;
		if(num>0)
			ScrollKnob->y=14.0f+(height-48.0f)*value/num;
	};
	void	setNum(int i)
	{
		num=i;
		if(value>=num)
			value=num-1;
	};
	void	setChangeFunc(void (*f)(int));
};
/*
class listboxUI:public window
{
private:
	int		numLines;
	textUI	*textlines;
public:
//	vector<string> text;
};*/