class brush
{
private:
	float	hardness;
	float	iradius;
	float	oradius;
	float	radius;
	GLuint	texID;
	char	tex[256*256];

	bool	update;
public:
	void	GenerateTexture();
	void	setHardness(float H){hardness=H;iradius=hardness*radius;oradius=radius-iradius;update=true;};
	void	setRadius(float R){radius=R;iradius=hardness*radius;oradius=radius-iradius;};
	float	getHardness(){return hardness;};
	float	getRadius(){return radius;};
	float getValue(float dist)
	{
		if(dist>radius)
			return 0.0f;
		if(dist<iradius)
			return 1.0f;	
		return (1.0f-(dist-iradius)/oradius);
	};
	GLuint	getTexture(){return texID;};
	bool	needUpdate(){return update;};
	void init();
};