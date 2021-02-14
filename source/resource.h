#if !defined(__RESOURCE_H__)
#define __RESOURCE_H__

#include "Iw2D.h"

class Resources
{
private:
	CIw2DImage *bg_star;
	CIw2DImage *spacecraft;
	CIw2DImage *astroid;
	CIw2DImage *exhaust;

public:

	CIw2DImage *get_bg_star()		{return bg_star;}
	CIw2DImage *get_spacecraft()	{return spacecraft;}
	CIw2DImage *get_astroid()		{return astroid;}
	CIw2DImage *get_exhaust()		{return exhaust;}

	Resources();
	~Resources();
};

extern Resources *getresource;

#endif