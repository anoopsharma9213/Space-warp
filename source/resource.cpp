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
	exhaust = Iw2DCreateImageResource("exhaust");
	energy = Iw2DCreateImageResource("energy");
	bonus_star = Iw2DCreateImageResource("bonus_star");

	astroid[0] = Iw2DCreateImageResource("astroid_1");
	astroid[1] = Iw2DCreateImageResource("astroid_2");
	astroid[2] = Iw2DCreateImageResource("astroid_3");

	comet = Iw2DCreateImageResource("comet");
	explosion = Iw2DCreateImageResource("explosion");

	font = Iw2DCreateFontResource("viner26_white");
}

Resources::~Resources()
{
	delete bg_star;
	delete planet_1;
	delete planet_2;
	delete planet_3;
	delete planet_5;
	
	delete spacecraft[0];
	delete spacecraft[1];
	delete exhaust;
	delete energy;
	delete bonus_star;

	for (int i = 0; i < 3; i++)
	{
		delete astroid[i];
	}
	delete comet;
	delete explosion;

	delete font;

	IwGetResManager()->DestroyGroup("Resource");
	IwResManagerTerminate();
	IwSoundTerminate();
}

Resources *getresource = 0;