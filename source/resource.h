#if !defined(__RESOURCE_H__)
#define __RESOURCE_H__

#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"
#include "s3eWindowsPhoneAd.h"

class Resources
{
private:

	// Environment----------------

	CIw2DImage *bg_star;
	CIw2DImage *planet_1;
	CIw2DImage *planet_2;
	CIw2DImage *planet_3;
	CIw2DImage *planet_5;
	
	// Gameplay--------------------

	CIw2DImage* spacecraft[5];
	CIw2DImage *exhaust;
	
	CIw2DImage *astroid;
	CIw2DImage *comet;
	CIw2DImage *explosion;

	CIw2DImage *bonus_star;
	CIw2DFont *font;

	

public:

	CIw2DImage *get_bg_star()			{return bg_star;}
	CIw2DImage *get_planet_1()			{return planet_1;}
	CIw2DImage *get_planet_2()			{return planet_2;}
	CIw2DImage *get_planet_3()			{return planet_3;}
	CIw2DImage *get_planet_5()			{return planet_5;}
	
	CIw2DImage *get_spacecraft(int a)	{return spacecraft[a];}
	CIw2DImage *get_exhaust()			{return exhaust;}

	CIw2DImage *get_astroid()			{return astroid;}
	CIw2DImage *get_comet()				{return comet;}
	CIw2DImage *get_explosion()			{return explosion;}

	CIw2DImage *get_bonus_star()		{return bonus_star;}
	
	CIw2DFont *get_font()				{return font;}

	Resources();
	~Resources();
};

extern Resources *getresource;

#endif