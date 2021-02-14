#include "resource.h"

Resources::Resources()
{
	bg_star = Iw2DCreateImage("design/bg_star.png");
	spacecraft = Iw2DCreateImage("design/spacecraft.png");
	exhaust = Iw2DCreateImage("design/exhaust.png");
	astroid = Iw2DCreateImage("design/astroid.png");
}

Resources::~Resources()
{
	delete bg_star;
	delete spacecraft;
	delete exhaust;
	delete astroid;
}

Resources *getresource = 0;