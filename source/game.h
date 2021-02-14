#if !defined(__GAME_H__)
#define __GAME_H__


#include "resource.h"

class gamePlay
{
private:
	int page, resume, g_speed;

	//-----Environment--------------
	CIwFVec2 pos_star[10];
	int size_star[10], phase_star[10], speed_star[10];

	CIwFVec2 pos_dot[50];

	CIwFVec2 p_pos[5], p_size[5], cp_pos, cp_size;
	int p_type[5], cp_col;


	//-----SpaceCraft---------------
	CIwFVec2 sc_pos, sc_size;
	int enable_move;

	CIwFVec2 bstar_pos[12], bstar_size;
	int bstar_x, bstar_y, bstar_i;
	
	CIwFVec2 com_pos[2], com_size;
	int com_x, com_i;

	CIwFVec2 ast_pos[8], ast_size;
	int ast_x, ast_i;

	int blast, b_x, b_y;
	int sequence;

public:

	void Update();
	void Render();

	void ini_Environment();
	void draw_Environment();
	void update_Environment();

	void ini_main_page();
	void draw_main_page();
	void update_main_page();

	void ini_play_page();
	void draw_play_page();
	void update_play_page();

	bool compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);
	bool com_compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);
	void sequence_1(int);
	void sequence_2(int);
	void sequence_3(int);
	void sequence_4(int);
	void sequence_5(int);
	void sequence_6(int);
	void sequence_7(int);
	void sequence_8(int);
	void sequence_9(int);
	void sequence_10(int);
	void sequence_11(int);
	void sequence_12(int);

	gamePlay();
	~gamePlay();

};

extern gamePlay *newgame;

#endif