#if !defined(__GAME_H__)
#define __GAME_H__


#include "Iw2D.h"
#include "resource.h"

class gamePlay
{
private:
	int page, play, size_star, phase_star;

public:

	void refresh();
	void draw();

	gamePlay();
	~gamePlay();

};

extern gamePlay *newgame;

#endif