#include "OpenGL.h"
#include "Video.h"
#include "UI.h"

void frame::render()
{
	if(hidden)
		return;

	glPushMatrix();
	glTranslatef(x, y, 0);
	for(unsigned int i = 0; i < children.size(); i++)
		if(!children[i]->hidden)
			children[i]->render();
	glPopMatrix();
}

void frame::addChild(frame* c)
{
	children.push_back(c);
	c->parent = this;
}

frame* frame::processLeftClick(float mx, float my)
{
	for(int i = children.size() - 1; i >= 0; i--)
	{
		if((!children[i]->hidden) && (children[i]->x < mx) && (children[i]->x + children[i]->width > mx) && (children[i]->y < my) && (children[i]->y + children[i]->height > my))
			return children[i]->processLeftClick(mx - children[i]->x, my - children[i]->y);
	}
	return 0;
}

bool frame::processLeftDrag(float mx, float my, float xDrag, float yDrag)
{
	if(movable)
	{
		x += xDrag;
		y += yDrag;

		return true;
	}

	return false;
}

bool frame::processRightClick(float mx, float my)
{
	for(unsigned int i = 0; i < children.size(); i++)
	{
		if((children[i]->x < mx) && (children[i]->x + children[i]->width > mx) && (children[i]->y < my) && (children[i]->y + children[i]->height > my))
			if(children[i]->processRightClick(mx - children[i]->x, my - children[i]->y))
				return true;
	}

	return false;
}

void frame::getOffset(float &xOff, float &yOff)
{
	float tx = 0.0f, ty = 0.0f;
	if(parent != 0)
		parent->getOffset(tx, ty);

	xOff = tx + x;
	yOff = ty + y;
}

bool frame::processKey(char key, bool shift, bool alt, bool ctrl)
{
	return false;
}

//Window Functions
window::window(float xPos, float yPos, float w, float h)
{
	movable = false;
	hidden = false;
	clickable = false;
	mouse = true;
	x = xPos;
	y = yPos;
	width = w;
	height = h;
	texture = video.textures.add("interface\\tooltips\\ui-tooltip-border.blp");
}

frame* window::processLeftClick(float mx, float my)
{
	frame* temp;

	for(int i = children.size() - 1; i >= 0; i--)
	{
		if((children[i]->x < mx) && (children[i]->x + children[i]->width > mx) && (children[i]->y < my) && (children[i]->y + children[i]->height > my))
		{
			temp = children[i]->processLeftClick(mx - children[i]->x, my - children[i]->y);
			if(temp != 0)
				return temp;
		}
	}
	return this;
}

//8 parts to the border image from left to right
//left side
//right side
//top side
//bottom side
//Bottom Left corner
//Bottom Right Corner
//Top Left Corner
//Top Right Corner

void window::render()
{
	glPushMatrix();
	glTranslatef(x, y, 0);

	glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0, 0);
	glVertex2f(width, 0);
	glVertex2f(0, height);
	glVertex2f(width, height);
	glEnd();

	for(unsigned int i = 0; i < children.size(); i++)
		if(!children[i]->hidden)
			children[i]->render();

	glColor3f(1.0f, 1.0f, 1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	//Draw Bottom left Corner First
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(-3, height + 3);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(13, height + 3);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(-3, height - 13);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(13, height - 13);
	glEnd();

	//Draw Bottom Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(width - 13, height + 3);	
	glTexCoord2f(8.0f/8.0f, 1.0f);
	glVertex2f(width + 3, height + 3);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height - 13);
	glTexCoord2f(8.0f/8.0f, 0.0f);
	glVertex2f(width + 3, height - 13);
	glEnd();

	//Draw Top Left Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(4.0f/8.0f, 1.0f);
	glVertex2f(-3, 13);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(13, 13);
	glTexCoord2f(4.0f/8.0f, 0.0f);
	glVertex2f(-3, -3);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(13, -3);
	glEnd();

	//Draw Top Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(width - 13, 13);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(width + 3, 13);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(width - 13, -3);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(width + 3, -3);
	glEnd();

	//Draw Left Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(0.0f/8.0f, 1.0f);
	glVertex2f(-3, height - 13);	
	glTexCoord2f(1.0f/8.0f, 1.0f);
	glVertex2f(13, height - 13);
	glTexCoord2f(0.0f/8.0f, 0.0f);
	glVertex2f(-3, 13);
	glTexCoord2f(1.0f/8.0f, 0.0f);
	glVertex2f(13, 13);
	glEnd();

	//Draw Right Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(1.0f/8.0f, 1.0f);
	glVertex2f(width - 13, height - 13);	
	glTexCoord2f(2.0f/8.0f, 1.0f);
	glVertex2f(width + 3, height - 13);
	glTexCoord2f(1.0f/8.0f, 0.0f);
	glVertex2f(width - 13, 13);
	glTexCoord2f(2.0f/8.0f, 0.0f);
	glVertex2f(width + 3, 13);
	glEnd();

	//Draw Top Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(4.0f/8.0f, 1.0f);
	glVertex2f(13, height + 3);	
	glTexCoord2f(4.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height + 3);	
	glTexCoord2f(3.0f/8.0f, 1.0f);
	glVertex2f(13, height - 13);
	glTexCoord2f(3.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height - 13);
	glEnd();

	//Draw Bottom Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(3.0f/8.0f, 1.0f);
	glVertex2f(13, 13);	
	glTexCoord2f(3.0f/8.0f, 0.0f);
	glVertex2f(width - 13, 13);
	glTexCoord2f(2.0f/8.0f, 1.0f);
	glVertex2f(13, -3);
	glTexCoord2f(2.0f/8.0f, 0.0f);
	glVertex2f(width - 13, -3);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//Slider Functions
slider::slider(float xPos, float yPos, float w, float s, float o)
{
	movable = false;
	hidden = false;
	clickable = true;
	mouse = true;
	x = xPos;
	y = yPos;
	width = w;
	height = 10.0f;
	value = 0.5f;
	func = 0;
	scale = s;
	offset = o;
	text[0] = 0;
	texture = video.textures.add("Interface\\Buttons\\UI-SliderBar-Border.blp");
	sliderTexture = video.textures.add("Interface\\Buttons\\UI-SliderBar-Button-Horizontal.blp");
}

void slider::setFunc(void(*f)(float))
{
	func = f;
}

void slider::setValue(float f)
{
	if(f > 1.0f)
		f = 1.0f;
	else if(f < 0.0f)
		f = 0.0f;

	value = f;
	func(value * scale + offset);
}

void slider::setText(const char* t)
{
	strcpy(text, t);
}

frame* slider::processLeftClick(float mx, float my)
{
	/*if((mx>(width*value-16))&&(mx<(width*value+16)))
        return this;
	return 0;*/

	value = mx/width;
	if(value < 0.0f)
		value = 0.0f;
	else if(value > 1.0f)
		value = 1.0f;
	if(func)
		func(value * scale + offset);

	return this;
}

bool slider::processLeftDrag(float mx, float my, float xChange, float yChange)
{
	float tx, ty;

	parent->getOffset(tx, ty);
	mx -= tx;
	my -= ty;
	value = mx/width;
	if(value < 0.0f)
		value = 0.0f;
	else if(value > 1.0f)
		value = 1.0f;
	if(func)
		func(value * scale + offset);

	return true;
}

//8 parts to the border image from left to right
//left side
//right side
//top side
//bottom side
//Bottom Left corner
//Bottom Right Corner
//Top Left Corner
//Top Right Corner

void slider::render()
{
	if(hidden)
		return;

	glPushMatrix();
	glTranslatef(x, y, 0);

	glColor3f(1.0f, 1.0f, 1.0f);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	char Temp[255];

	if(text[0] != 0)
	{
		int twidth;
		//OGLFT::BBox	Box;
		sprintf(Temp, text, value * scale + offset);
	
		twidth = freetype::width(arial12, Temp);
		
		
		freetype::shprint(arial12, width/2 - twidth/2, -16, Temp);	
	}
	
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texture);
	
	//Draw Bottom left Corner First
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(-1, height + 4);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(7, height + 4);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(-1, height - 4);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(7, height - 4);
	glEnd();

	//Draw Bottom Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(width - 7, height + 4);	
	glTexCoord2f(8.0f/8.0f, 1.0f);
	glVertex2f(width + 1, height + 4);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(width - 7, height - 4);
	glTexCoord2f(8.0f/8.0f, 0.0f);
	glVertex2f(width + 1, height - 4);
	glEnd();

	//Draw Top Left Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(4.0f/8.0f, 1.0f);
	glVertex2f(-1, 4);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(7, 4);
	glTexCoord2f(4.0f/8.0f, 0.0f);
	glVertex2f(-1, -4);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(7, -4);
	glEnd();

	//Draw Top Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(width - 7, 4);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(width + 1, 4);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(width - 7, -4);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(width + 1, -4);
	glEnd();

	if(height > 8)
	{
		//Draw Left Side
		glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2f(0.0f/8.0f, 1.0f);
		glVertex2f(-1, height - 4);	
		glTexCoord2f(1.0f/8.0f, 1.0f);
		glVertex2f(7, height - 4);
		glTexCoord2f(0.0f/8.0f, 0.0f);
		glVertex2f(-1, 4);
		glTexCoord2f(1.0f/8.0f, 0.0f);
		glVertex2f(7, 4);
		glEnd();

		//Draw Right Side
		glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2f(1.0f/8.0f, 1.0f);
		glVertex2f(width - 7, height - 4);	
		glTexCoord2f(2.0f/8.0f, 1.0f);
		glVertex2f(width + 1, height - 4);
		glTexCoord2f(1.0f/8.0f, 0.0f);
		glVertex2f(width - 7, 4);
		glTexCoord2f(2.0f/8.0f, 0.0f);
		glVertex2f(width + 1, 4);
		glEnd();
	}

	if(width > 14)
	{
		//Draw Top Side
		glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2f(4.0f/8.0f, 1.0f);
		glVertex2f(7, height + 4);	
		glTexCoord2f(4.0f/8.0f, 0.0f);
		glVertex2f(width - 7, height + 4);	
		glTexCoord2f(3.0f/8.0f, 1.0f);
		glVertex2f(7, height - 4);
		glTexCoord2f(3.0f/8.0f, 0.0f);
		glVertex2f(width - 7, height - 4);
		glEnd();

		//Draw Bottom Side
		glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2f(3.0f/8.0f, 1.0f);
		glVertex2f(7, 4);	
		glTexCoord2f(3.0f/8.0f, 0.0f);
		glVertex2f(width - 7, 4);
		glTexCoord2f(2.0f/8.0f, 1.0f);
		glVertex2f(7, -4);
		glTexCoord2f(2.0f/8.0f, 0.0f);
		glVertex2f(width - 7, -4);
		glEnd();
	}

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, sliderTexture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(width * value - 16.0f, height/2 - 16.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(width * value + 16.0f, height/2 - 16.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(width * value - 16.0f, height/2 + 16.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(width * value + 16.0f, height/2 + 16.0f);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//Gradient Functions
void gradient::render()
{
	if(hidden)
		return;
	 
	if(horiz)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor4fv(&MinColor.x);
		glVertex2f(x, y);		
		glVertex2f(x, y + height);
		glColor4fv(&MaxColor.x);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glEnd();
		if(clickable)
		{
			glColor4fv(&ClickColor.x);
			glBegin(GL_LINE);
			glVertex2f(x + width * value, y);
			glVertex2f(x + width * value, y + height);
			glEnd();
		}
	}
	else
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor4fv(&MaxColor.x);		
		glVertex2f(x + width, y);		
		glVertex2f(x, y);
		glColor4fv(&MinColor.x);
		glVertex2f(x + width,y + height);
		glVertex2f(x, y + height);
		glEnd();

		if(clickable)
		{
			glColor4fv(&ClickColor.x);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(x, y + height * value - 1);
			glVertex2f(x + width, y + height * value - 1);
			glVertex2f(x, y + height * value + 1);
			glVertex2f(x + width, y + height * value + 1);
			glEnd();
		}
	}
}

void gradient::setMaxColor(float r, float g, float b, float a)
{
	MaxColor = Vec4D(r, g, b, a);
}

void gradient::setMinColor(float r, float g, float b, float a)
{
	MinColor = Vec4D(r, g, b, a);
}

void gradient::setClickColor(float r, float g, float b, float a)
{
	ClickColor = Vec4D(r, g, b, a);
}

void gradient::setClickFunc(void(*f)(float))
{
	value = 0.0f;
	clickFunc = f;
	clickable = true;
}

frame* gradient::processLeftClick(float mx, float my)
{
	if(clickable)
	{
		lastLeftClick = true;
		if(horiz)
			value = mx/width;
		else
			value = my/height;
		if(value < 0.0f)
			value = 0.0f;
		else if(value > 1.0f)
			value = 1.0f;
		clickFunc(value);

		return this;
	}
	else
		return 0;
}

bool gradient::processLeftDrag(float mx, float my, float xDrag, float yDrag)
{
	float tx, ty;
	parent->getOffset(tx, ty);
	mx -= tx;
	my -= ty;
	if(clickable)
	{
		if(horiz)
			value = mx/width;
		else
			value = my/height;
		if(value < 0.0f)
			value = 0.0f;
		else if(value > 1.0f)
			value = 1.0f;
		clickFunc(value);

		return true;
	}

	if(movable)
	{
		x += xDrag;
		y += yDrag;

		return true;
	}

	return false;
}

void gradient::setValue(float f)
{
	value = f;
	if(value < 0.0f)
		value = 0.0f;
	else if(value > 1.0f)
		value = 1.0f;

	clickFunc(value);
}

//Text ui

textUI::textUI(float xPos, float yPos)
{
	x = xPos;
	y = yPos;
	text[0] = 0;
	background = false;
	justify = UI_JUSTIFY_LEFT;
}

textUI::textUI(float xPos, float yPos, const char* t)
{
	x = xPos;
	y = yPos;
	setText(t);
	background = false;
	justify = UI_JUSTIFY_LEFT;
}

void textUI::setText(const char* t)
{
	strcpy(text, t);
}

void textUI::setJustify(int j)
{
	justify = j;
}

void textUI::setFont(freetype::font_data* f)
{
	font = f;
}

void textUI::render()
{
	int twidth;
	twidth=freetype::width(*font,text);

	if(background)
	{
		glColor4fv(bgColor);
		glBegin(GL_TRIANGLE_STRIP);
		if(justify == UI_JUSTIFY_LEFT)
		{
				glVertex2f(x - 2.0f, y - 1.0f);
				glVertex2f(x + 2.0f + twidth, y - 1.0f);
				glVertex2f(x - 2.0f, y + 2.0f + font->h + 1.0f);
				glVertex2f(x + 2.0f + twidth, y + 2.0f + font->h + 1.0f);				
		}
		else if(justify == UI_JUSTIFY_CENTER)
		{
			glVertex2f(x - 2.0f - twidth/2.0f, y - 1.0f);
			glVertex2f(x + 2.0f + twidth/2.0f, y - 1.0f);
			glVertex2f(x - 2.0f - twidth/2.0f, y + 2.0f + font->h + 1.0f);
			glVertex2f(x + 2.0f + twidth/2.0f, y + 2.0f + font->h + 1.0f);	
		}
		else if(justify == UI_JUSTIFY_RIGHT)
		{
			glVertex2f(x - 2.0f - twidth, y - 1.0f);
			glVertex2f(x + 2.0f, y - 1.0f);
			glVertex2f(x - 2.0f - twidth, y + 2.0f + font->h + 1.0f);
			glVertex2f(x + 2.0f, y + 2.0f + font->h + 1.0f);	
		}
		glEnd();
	}

	if(justify == UI_JUSTIFY_LEFT)
		freetype::shprint(*font, x, y, text);	
	else if(justify == UI_JUSTIFY_CENTER)
		freetype::shprint(*font, x - twidth/2.0f, y, text);
	else if(justify == UI_JUSTIFY_RIGHT)
		freetype::shprint(*font, x - twidth, y, text);
}

void textUI::setBackground(float r, float g, float b, float a)
{
	background = true;
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
	bgColor[3] = a;
}

textureUI::textureUI(float xPos, float yPos, float w, float h, GLuint tex)
{
	x = xPos;
	y = yPos;
	width = w;
	height = h;
	texture = tex;
	highlight = false;
	clickFunc = 0;
	id = 0;
}

void textureUI::setTexture(GLuint tex)
{
	texture = tex;
}

void textureUI::render()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x, y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x + width, y);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y + height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x + width, y + height);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	if(highlight)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x - 1, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x - 1, y + height);
		glEnd();
	}
}

frame* textureUI::processLeftClick(float mx, float my)
{
	if(clickFunc)
	{
		clickFunc(this, id);
		return this;
	}

	return 0;
}

void textureUI::setClickFunc(void(*f)(frame*, int), int num)
{
	clickFunc = f;
	id = num;
}

//Button UI
buttonUI::buttonUI(float xPos, float yPos, float w, float h, GLuint tex, GLuint texd)
{
	x = xPos;
	y = yPos;
	width = w;
	height = h;
	texture = tex;
	textureDown = texd;
	clickFunc = 0;
	id = 0;
	clicked = false;
	text = new textUI(w/2.0f, 2.0f);
	text->setJustify(UI_JUSTIFY_CENTER);
	text->setFont(&arial12);
}

void buttonUI::setText(const char* t)
{
	text->setText(t);
}

void buttonUI::setFont(freetype::font_data* font)
{
	text->setFont(font);
}
void buttonUI::render()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	if(!clicked)
		glBindTexture(GL_TEXTURE_2D, texture);
	else
		glBindTexture(GL_TEXTURE_2D, textureDown);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x, y);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x + width, y);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y + height);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x + width, y + height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	text->render();
	glPopMatrix();
}

frame* buttonUI::processLeftClick(float mx, float my)
{
	clicked = true;
	if(clickFunc)
		clickFunc(this, id);

	return this;
}

void buttonUI::processUnclick()
{
	clicked = false;
}

void buttonUI::setClickFunc(void(*f)(frame*, int), int num)
{
	clickFunc = f;
	id = num;
}

//Minimizable Window
void minimizeWindow(frame* button, int id)
{
	((minWindowUI*)(button->parent))->minimized = true;
	((minWindowUI*)(button->parent))->minWindow->x = button->parent->x;
	((minWindowUI*)(button->parent))->minWindow->y = button->parent->y;
	button->parent->hidden = true;
	((minWindowUI*)(button->parent))->minWindow->hidden = false;
	if(!((minWindowUI*)(button->parent))->addedToParent)
	{
		button->parent->parent->addChild(((minWindowUI*)(button->parent))->minWindow);
		((minWindowUI*)(button->parent))->addedToParent = true;
	}
}

void maximizeWindow(frame* button, int id)
{
	((minWindowUI*)(button->parent->twindow))->minimized = false;
	(button->parent->twindow)->x = button->parent->x;
	(button->parent->twindow)->y = button->parent->y;
	button->parent->twindow->hidden = false;
	button->parent->hidden = true;
}

minWindowUI::minWindowUI(float xPos, float yPos, float w, float h, const char* title) : window(xPos, yPos, w, h)
{
	texture = video.textures.add("interface\\tooltips\\ui-tooltip-border.blp");

	minimized = false;

	textUI* T = new textUI(w/2.0f, 2.0f, title);
	T->setFont(&arial16);
	T->setJustify(UI_JUSTIFY_CENTER);
	
	addChild(T);
	buttonUI* B;

	B = new buttonUI(w - 16.0f - 3.0f, 3.0f, 16.0f, 16.0f, video.textures.add("Interface\\Buttons\\UI-MinusButton-Up.blp"), video.textures.add("Interface\\Buttons\\UI-MinusButton-Down.blp"));
	B->setClickFunc(minimizeWindow, 0);

	this->addChild(B);

	minWindow = new window(xPos, yPos, freetype::width(arial16, title) + 36.0f, 20);
	minWindow->movable = true;

	T = new textUI(freetype::width(arial16, title)/2.0f + 16.0f, 2.0f, title);
	T->setFont(&arial16);
	T->setJustify(UI_JUSTIFY_CENTER);
	minWindow->addChild(T);
	
	B = new buttonUI(freetype::width(arial16, title) + 36.0f - 16.0f - 3.0f, 3.0f, 16.0f, 16.0f, video.textures.add("Interface\\Buttons\\UI-PlusButton-Up.blp"), video.textures.add("Interface\\Buttons\\UI-PlusButton-Down.blp"));
	B->setClickFunc(maximizeWindow, 0);
	minWindow->addChild(B);

	minWindow->twindow = this;

	addedToParent = false;
}

void minWindowUI::render()
{
	glPushMatrix();
	glTranslatef(x, y, 0);

	glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0, 0);
	glVertex2f(width, 0);
	glVertex2f(0, height);
	glVertex2f(width, height);
	glEnd();

	for(unsigned int i = 0; i < children.size(); i++)
		if(!children[i]->hidden)
			children[i]->render();

	glColor3f(1.0f, 1.0f, 1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	//Draw Bottom left Corner First
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(-3, height + 3);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(13, height + 3);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(-3, height - 13);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(13, height - 13);
	glEnd();

	//Draw Bottom Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(7.0f/8.0f, 1.0f);
	glVertex2f(width - 13, height + 3);	
	glTexCoord2f(8.0f/8.0f, 1.0f);
	glVertex2f(width + 3, height + 3);
	glTexCoord2f(7.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height - 13);
	glTexCoord2f(8.0f/8.0f, 0.0f);
	glVertex2f(width + 3, height - 13);
	glEnd();

	//Draw Top Left Corner

	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(4.0f/8.0f, 1.0f);
	glVertex2f(-3, 13);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(13, 13);
	glTexCoord2f(4.0f/8.0f, 0.0f);
	glVertex2f(-3, -3);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(13, -3);
	glEnd();

	//Draw Top Right Corner
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(5.0f/8.0f, 1.0f);
	glVertex2f(width - 13, 13);	
	glTexCoord2f(6.0f/8.0f, 1.0f);
	glVertex2f(width + 3, 13);
	glTexCoord2f(5.0f/8.0f, 0.0f);
	glVertex2f(width - 13, -3);
	glTexCoord2f(6.0f/8.0f, 0.0f);
	glVertex2f(width + 3, -3);
	glEnd();

	//Draw Left Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(0.0f/8.0f, 1.0f);
	glVertex2f(-3, height - 13);	
	glTexCoord2f(1.0f/8.0f, 1.0f);
	glVertex2f(13, height - 13);
	glTexCoord2f(0.0f/8.0f, 0.0f);
	glVertex2f(-3, 13);
	glTexCoord2f(1.0f/8.0f, 0.0f);
	glVertex2f(13, 13);
	glEnd();

	//Draw Right Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(1.0f/8.0f, 1.0f);
	glVertex2f(width - 13, height - 13);	
	glTexCoord2f(2.0f/8.0f, 1.0f);
	glVertex2f(width + 3, height - 13);
	glTexCoord2f(1.0f/8.0f, 0.0f);
	glVertex2f(width - 13, 13);
	glTexCoord2f(2.0f/8.0f, 0.0f);
	glVertex2f(width + 3, 13);
	glEnd();

	//Draw Top Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(4.0f/8.0f, 1.0f);
	glVertex2f(13, height + 3);	
	glTexCoord2f(4.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height + 3);	
	glTexCoord2f(3.0f/8.0f, 1.0f);
	glVertex2f(13, height - 13);
	glTexCoord2f(3.0f/8.0f, 0.0f);
	glVertex2f(width - 13, height - 13);
	glEnd();

	//Draw Bottom Side
	glBegin(GL_TRIANGLE_STRIP);	
	glTexCoord2f(3.0f/8.0f, 1.0f);
	glVertex2f(13, 13);	
	glTexCoord2f(3.0f/8.0f, 0.0f);
	glVertex2f(width - 13, 13);
	glTexCoord2f(2.0f/8.0f, 1.0f);
	glVertex2f(13, -3);
	glTexCoord2f(2.0f/8.0f, 0.0f);
	glVertex2f(width - 13, -3);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//Closable Window
void closeWindow(frame* button, int id)
{
	button->parent->hidden = true;
}

closeWindowUI::closeWindowUI(float xPos, float yPos, float w, float h, const char* title) : window(xPos, yPos, w, h)
{
	texture = video.textures.add("interface\\tooltips\\ui-tooltip-border.blp");

	textUI* T = new textUI(w/2.0f, 2.0f, title);
	T->setFont(&arial16);
	T->setJustify(UI_JUSTIFY_CENTER);
	
	addChild(T);
	buttonUI* B;

	B = new buttonUI(w - 32.0f + 1.0f, -3.0f, 32.0f, 32.0f, video.textures.add("Interface\\Buttons\\UI-Panel-MinimizeButton-Up.blp"), video.textures.add("Interface\\Buttons\\UI-Panel-MinimizeButton-Down.blp"));
	B->setClickFunc(closeWindow, 0);

	this->addChild(B);
}

//Checkbox
checkboxUI::checkboxUI(float xPos, float yPos, const char* t)
{
	x = xPos;
	y = yPos;
	width = 30;
	height = 30;
	checked = false;

	box = new textureUI(0, 0, 32, 32, video.textures.add("Interface\\Buttons\\UI-CheckBox-Up.blp"));
	addChild(box);
	check = new textureUI(0, 0, 32, 32, video.textures.add("Interface\\Buttons\\UI-CheckBox-Check.blp"));
	check->hidden = true;
	addChild(check);
	text = new textUI(32, 8, t);
	text->setJustify(UI_JUSTIFY_LEFT);
	text->setFont(&arialn13);
	addChild(text);
	
	clickFunc = 0;	
}

void checkboxUI::setText(const char* t)
{
	text->setText(t);
}

void checkboxUI::setState(bool c)
{
	checked = c;

	if(checked)
		check->hidden = false;
	else
		check->hidden = true;
}

bool checkboxUI::getState()
{
	return checked;
}

frame* checkboxUI::processLeftClick(float mx, float my)
{
	checked = !checked;

	if(checked)
		check->hidden = false;
	else
		check->hidden = true;

	if(clickFunc != 0)
		clickFunc(checked, id);

	return this;
}

void checkboxUI::setClickFunc(void(*f)(bool, int), int i)
{
	id = i;
	clickFunc = f;
}

//Scroll Bar

/*class scrollbarUI:public frame
{
private:
	buttonUI	*ScrollUp;
	buttonUI	*ScrollDown;
	textureUI	*ScrollKnob;
	int			num;
	int			value;
	void (*changeFunc)(int);
public:
	scrollbarUI(float xpos, float ypos, float height, int num);
	int		getValue();
	void	setValue(int i);
	void	setNum(int i);
	void	setChangeFunc(void (*f)(int));
	frame	*processLeftClick(float mx,float my);
	void render();
};*/

void scrollbarProcessClick(frame* f, int id)
{
	((scrollbarUI*)(f->parent))->clickReturn(id);
}

scrollbarUI::scrollbarUI(float xpos, float ypos, float h, int n)
{
	x = xpos;
	y = ypos;
	width = 16;
	height = h;
	ScrollUp = new buttonUI(-6.0f, -8.0f, 32.0f, 32.0f, video.textures.add("Interface\\Buttons\\UI-ScrollBar-ScrollUpButton-Up.blp"), video.textures.add("Interface\\Buttons\\UI-ScrollBar-ScrollUpButton-Down.blp"));
	ScrollUp->setClickFunc(scrollbarProcessClick, 0);
	addChild(ScrollUp);

	ScrollDown = new buttonUI(-6.0f, height - 24.0f, 32.0f, 32.0f, video.textures.add("Interface\\Buttons\\UI-ScrollBar-ScrollDownButton-Up.blp"), video.textures.add("Interface\\Buttons\\UI-ScrollBar-ScrollDownButton-Down.blp"));
	ScrollDown->setClickFunc(scrollbarProcessClick, 1);
	addChild(ScrollDown);
	ScrollKnob = new textureUI(-6.0f, 10.0f, 32.0f, 32.0f, video.textures.add("Interface\\Buttons\\UI-ScrollBar-Knob.blp"));
	ScrollKnob->setClickFunc(scrollbarProcessClick, 2);
	addChild(ScrollKnob);
	value = 0;
	num = n;
	changeFunc = 0;
}

void scrollbarUI::clickReturn(int id)
{
	if(id == 0)//Scroll Up
	{
		if(value != 0)
			value--;
	}
	else if(id == 1)//Scroll Down
	{
		if(value != num - 1)
			value++;
	}
	else//ScrollKnob
	{
	}
	//Update ScrollKnob Position
	if(num > 0)
		ScrollKnob->y = 10.0f + (height - 40.0f) * value/num;
	if(changeFunc)
		changeFunc(value);
}

void scrollbarUI::setChangeFunc(void(*f)(int))
{
	changeFunc = f;
}