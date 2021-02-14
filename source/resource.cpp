#include "resource.h"

Resources::Resources()
{
	IwSoundInit();
	IwResManagerInit();
	
	#ifdef IW_BUILD_RESOURCES
		IwGetResManager()->AddHandler(new CIwResHandlerWAV);
	#endif

	IwGetResManager()->LoadGroup("Resource.group");

	Effects = IwGetResManager()->GetGroupNamed("Interface");

	bg_star = Iw2DCreateImageResource("bg_star");
	planet_1 = Iw2DCreateImageResource("planet1");
	planet_2 = Iw2DCreateImageResource("planet2");
	planet_3 = Iw2DCreateImageResource("planet3");
	planet_5 = Iw2DCreateImageResource("planet5");
	
	spacecraft[0] = Iw2DCreateImageResource("spacecraft_1");
	spacecraft[1] = Iw2DCreateImageResource("spacecraft_2");
	spacecraft[2] = Iw2DCreateImageResource("spacecraft_3");
	spacecraft[3] = Iw2DCreateImageResource("spacecraft_4");
	spacecraft[4] = Iw2DCreateImageResource("spacecraft_5");
	spacecraft[5] = Iw2DCreateImageResource("spacecraft_6");

	exhaust = Iw2DCreateImageResource("exhaust");
	energy = Iw2DCreateImageResource("energy");
	bonus_star = Iw2DCreateImageResource("bonus_star");
	power = Iw2DCreateImageResource("power");
	powerglow = Iw2DCreateImageResource("powerglow");

	astroid[0] = Iw2DCreateImageResource("astroid_1");
	astroid[1] = Iw2DCreateImageResource("astroid_2");
	astroid[2] = Iw2DCreateImageResource("astroid_3");

	comet = Iw2DCreateImageResource("comet");
	explosion = Iw2DCreateImageResource("explosion");

	panel = Iw2DCreateImageResource("panel");
	settings = Iw2DCreateImageResource("settings");
	home = Iw2DCreateImageResource("home");
	resume = Iw2DCreateImageResource("continue");
	sound_key = Iw2DCreateImageResource("sound_key");

	rate_us = Iw2DCreateImageResource("rate_us");
	about_us = Iw2DCreateImageResource("about_us");

	spacecraft_locked = Iw2DCreateImageResource("spacecraft_locked");
	spacecraft_set = Iw2DCreateImageResource("spacecraft_set");

	menu = Iw2DCreateImageResource("menu");
	ok = Iw2DCreateImageResource("ok");
	ranks = Iw2DCreateImageResource("ranks");
	power_level = Iw2DCreateImageResource("power_level");

	if(Iw2DGetSurfaceHeight()>=720)
	{
		font = Iw2DCreateFontResource("manteka26_white");
	}
	else
	{
		font = Iw2DCreateFontResource("manteka12_white");
	}	
}

Resources::~Resources()
{
	delete bg_star;
	delete planet_1;
	delete planet_2;
	delete planet_3;
	delete planet_5;
	
	for (int i = 0; i < 6; i++)
	{
		delete spacecraft[i];
	}

	delete exhaust;
	delete energy;
	delete bonus_star;

	delete power;
	delete powerglow;

	for (int i = 0; i < 3; i++)
	{
		delete astroid[i];
	}
	delete comet;
	delete explosion;

	delete settings;
	delete panel;
	delete sound_key;
	delete home;
	delete resume;
	delete rate_us;
	delete about_us;

	delete spacecraft_locked;
	delete spacecraft_set;
	delete menu;
	delete ok;
	delete ranks;
	delete power_level;

	delete font;

	IwGetResManager()->DestroyGroup("Resource");
	IwResManagerTerminate();
	IwSoundTerminate();
}

Resources *getresource = 0;