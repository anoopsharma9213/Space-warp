#include "game.h"

void gamePlay::refresh()
{
	phase_star==0?size_star+=10:size_star-=15;
	if(size_star >= getresource->get_bg_star()->GetHeight())
	{
		phase_star=1;
	}
	else if(size_star <= 2)
	{
		phase_star=0;
	}
}

void gamePlay::draw()
{
	Iw2DDrawImage(getresource->get_bg_star(),CIwFVec2(getresource->get_bg_star()->GetHeight()-size_star/2,getresource->get_bg_star()->GetHeight()-size_star/2),CIwFVec2(size_star,size_star));
}

gamePlay::gamePlay()
{
	page = 1;
	play = 0;
	phase_star = 0;
	size_star = 0;
}

gamePlay::~gamePlay()
{
	
}

gamePlay *newgame = 0;