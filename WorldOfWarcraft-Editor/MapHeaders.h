#ifndef MAPHEADERS
#define MAPHEADERS

struct MHDR
{
 /*000h*/  uint32 pad;
 /*004h*/  uint32 MCIN_Offset;	//Positions of MCNK's
 /*008h*/  uint32 MTEX_Offset;	//List of all the textures used
 /*00Ch*/  uint32 MMDX_Offset;	//List of all the md2's used
 /*010h*/  uint32 MMID_Offset;	//Offsets into MMDX list for what each ID is
 /*014h*/  uint32 MWMO_Offset;	//list of all the WMO's used
 /*018h*/  uint32 MWID_Offset;	//Offsets into MWMO list for what each ID is
 /*01Ch*/  uint32 MDDF_Offset;	//Doodad Information
 /*020h*/  uint32 MODF_Offset;	//WMO Positioning Information
 /*024h*/  uint32 pad1;	
 /*028h*/  uint32 pad2;		
 /*02Ch*/  uint32 pad3;	
 /*030h*/  uint32 pad4;		
 /*034h*/  uint32 pad5;		
 /*038h*/  uint32 pad6;		
 /*03Ch*/  uint32 pad7;	
 /*040h*/
};

struct MCIN
{
	uint32	offset;
	uint32	size;
	uint32	flags;
	uint32	asyncID;
};

struct MDDF
{
	uint32	nameID;
	uint32	uniqueID;	
	float	pos[3];
	float	rot[3];
	//uint16	flags;
	uint32	scale;
};

struct MODF
{
	uint32	nameID;
	uint32	uniqueID;	
	float	pos[3];
	float	rot[3];
	float	extents[2][3];
	//uint16	flags;
	uint16	unknown;
	uint16	doodadSet;
	uint32	nameSet;
};

struct MCLY
{
	uint32	textureID;
	uint32	flags;
	uint32	ofsAlpha;
	uint32	effectID;
};

#endif