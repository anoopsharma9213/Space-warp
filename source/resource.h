#if !defined(__RESOURCE_H__)
#define __RESOURCE_H__

#include "Iw2D.h"
#include "s3e.h"
#include "IwResManager.h"
#include "s3eWindowsPhoneAd.h"
#include "s3eOSExec.h"
#include "IwSound.h"

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

	CIw2DImage* spacecraft[6];
	CIw2DImage* power;
	CIw2DImage* powerglow;
	CIw2DImage *exhaust;
	CIw2DImage *energy;
	
	CIw2DImage *astroid[3];
	CIw2DImage *comet;
	CIw2DImage *explosion;

	CIw2DImage *bonus_star;
	CIw2DFont *font;

	// Interface----------------------

	CIw2DImage *settings;
	CIw2DImage *sound_key;
	CIw2DImage *home;
	CIw2DImage *resume;
	CIw2DImage *panel;
	CIw2DImage *rate_us;
	CIw2DImage *about_us;
	CIw2DImage *spacecraft_set;
	CIw2DImage *spacecraft_locked;
	CIw2DImage *menu;
	CIw2DImage *ok;
	CIw2DImage *power_level;
	CIw2DImage *ranks;

public:

	CIwResGroup *Effects;

	CIw2DImage *get_bg_star()			{return bg_star;}
	CIw2DImage *get_planet_1()			{return planet_1;}
	CIw2DImage *get_planet_2()			{return planet_2;}
	CIw2DImage *get_planet_3()			{return planet_3;}
	CIw2DImage *get_planet_5()			{return planet_5;}
	
	CIw2DImage *get_spacecraft(int a)	{return spacecraft[a];}
	CIw2DImage *get_exhaust()			{return exhaust;}
	CIw2DImage *get_energy()			{return energy;}
	CIw2DImage *get_power()				{return power;}
	CIw2DImage *get_powerglow()			{return powerglow;}

	CIw2DImage *get_astroid(int a)		{return astroid[a];}
	CIw2DImage *get_comet()				{return comet;}
	CIw2DImage *get_explosion()			{return explosion;}

	CIw2DImage *get_bonus_star()		{return bonus_star;}

	CIw2DImage *get_sound_key()			{return sound_key;}
	CIw2DImage *get_home()				{return home;}
	CIw2DImage *get_resume()			{return resume;}
	CIw2DImage *get_settings()			{return settings;}
	CIw2DImage *get_panel()				{return panel;}
	CIw2DImage *get_rate_us()			{return rate_us;}
	CIw2DImage *get_about_us()			{return about_us;}

	CIw2DImage *get_spacecraft_set()	{return spacecraft_set;}
	CIw2DImage *get_spacecraft_locked()	{return spacecraft_locked;}
	CIw2DImage *get_menu()				{return menu;}
	CIw2DImage *get_ok()				{return ok;}
	CIw2DImage *get_ranks()				{return ranks;}
	CIw2DImage *get_power_level()		{return power_level;}

	CIw2DFont *get_font()				{return font;}

	Resources();
	~Resources();
};

extern Resources *getresource;

#endif