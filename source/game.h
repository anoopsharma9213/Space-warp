#if !defined(__GAME_H__)
#define __GAME_H__


#include "resource.h"

class gamePlay
{
private:

	struct save
	{
		float star,score;
		int m, sc, mw;
	}*_store;

	char str[50];
	int page, delay;
	float g_speed, l_speed, t_speed;

	int m_tem[4];

	//-----Ads---------------------

	s3eWindowsPhoneAd *ad_control, *disp_control;

	//-----Environment--------------
	CIwFVec2 pos_star[10];
	int size_star[10], phase_star[10], speed_star[10];

	CIwFVec2 pos_dot[50];

	CIwFVec2 p_pos[5], p_size[5], cp_pos, cp_size;
	int p_type[5], cp_col;

	//-----Main Page----------------

	int trans;

	//-----SpaceCraft---------------
	CIwFVec2 sc_pos, sc_size;
	CIwFVec2 exhaust_pos, exhaust_size[3];
	int exhaust_sel, sc_sel;

	CIwFVec2 bstar_pos[12], bstar_size;
	int bstar_x, bstar_y, bstar_i, bstar_show[12];
	
	CIwFVec2 com_pos[2], com_size;
	int com_x, com_i;

	CIwFVec2 ast_pos[8], ast_size;
	int ast_x, ast_i, ast_s[8], ast_show[8];

	int blast, b_x, b_y;
	int sequence;

	//------Interface---------------

	//int schannel;
	int music, energy;
	CIwFVec2 energy_size, energy_pos;
	CIwFVec2 panel_size, panel_pos, continue_pos, home_pos, sound_pos, button_size;

	//------Collectibles------------
	float star, score, lives, max_lives[5];
	float high_score, total_star;
	CIwFVec2 star_pos, star_size, score_pos, score_size, lives_pos, lives_size;

	//------Powerups and resources----
	CIwFVec2 reenergy_pos[2], reenergy_size;
	int reenergy_show[2], reenergy;
	CIwFVec2 power_pos[2], power_size, powerglow_pos, powerglow_size;
	int power, power_show[2], power_avail, power_on_time;
	CIwFVec2 megawarp_pos,megawarp_size;
	int megawarp, mega_active;

	//--------Powerups timer----------
	int power_time[4], power_down;

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

	void ini_pause_page();
	void draw_pause_page();
	void update_pause_page();

	bool compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);
	bool com_compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);
	bool star_compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);
	bool power_compare(CIwFVec2,CIwFVec2,CIwFVec2,CIwFVec2);

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