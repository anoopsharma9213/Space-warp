#include "game.h"

void gamePlay::Update()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		if(page == 0)
		{
			_store->score = high_score;
			_store->star = total_star;
			_store->m = music;
			_store->mw = megawarp;
			_store->sc = sc_sel;
			_store->cstar = collected_star;
			for (int i = 0; i < 6; i++)
			{
				_store->sc_l[i] = sc_locked[i];
			}
			for (int i = 0; i < 4; i++)
			{
				_store->pt[i] = power_time[i];
			}

			s3eSecureStoragePut(_store,sizeof(struct save));
			s3eAudioStop();
			s3eDeviceRequestQuit();
		}
	}


	if(s3eDeviceGetInt(S3E_DEVICE_OS)==S3E_OS_ID_WP8)
	{
		switch (m_tem[0])
		{
			case 0:
				m_tem[1] = s3eAudioGetInt(S3E_AUDIO_POSITION);
				break;
			case 10:
				m_tem[2] = s3eAudioGetInt(S3E_AUDIO_POSITION);
				break;
			case 15:
				m_tem[3] = s3eAudioGetInt(S3E_AUDIO_POSITION);
				break;
		}
		m_tem[0]++;
		if(m_tem[0]==16 && m_tem[1]==m_tem[2] && m_tem[2]==m_tem[3])
		{
			s3eAudioPause();
			s3eAudioResume();
		}
		if (m_tem[0]==16)
		{
			m_tem[0]=0;
		}
		if(s3eAudioGetInt(S3E_AUDIO_DURATION)-s3eAudioGetInt(S3E_AUDIO_POSITION) <= 1000)
		{
			s3eAudioSetInt(S3E_AUDIO_POSITION,0);
		}
	}
	if(delay>0)
	{
		delay--;
	}
	update_Environment();

	switch (page)
	{
		case 0: update_main_page();
			break;
		case 1: update_play_page();
			break;
		case -1: update_pause_page();
			break;
		case -2: update_personalize_page();
			break;
		case -3: update_about_page();
			break;
	}
}

void gamePlay::Render()
{
	Iw2DSurfaceClear(0xff000000);
	Iw2DSetColour(0xffffffff);

	IwRandSeed((int)s3eTimerGetMs());

	draw_Environment();

	switch (page)
	{
		case 0: draw_main_page();
			break;
		case 1: draw_play_page();
			break;
		case -1: draw_play_page(); draw_pause_page();
			break;
		case -2: draw_main_page(); draw_personalize_page();
			break;
		case -3:  draw_about_page();
			break;
	}
	Iw2DSurfaceShow();
}

gamePlay::gamePlay()
{
	page = 0;
	subpage = 0;
	//resume = 0;

	/*if(s3eWindowsPhoneAdAvailable())
	{
		ad_control = s3eWindowsPhoneAdCreate("test_client","Image480_80");
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_HEIGHT,80);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_WIDTH,480);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_HALIGN,S3E_WINDOWSPHONE_ADCONTROL_HALIGN_CENTER);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_VALIGN,S3E_WINDOWSPHONE_ADCONTROL_VALIGN_BOTTOM);
		if(s3eSocketGetInt(S3E_SOCKET_NETWORK_AVAILABLE))
		{
			s3eWindowsPhoneAdShow(ad_control);
		}
	}*/

	for (int i = 0; i < 4; i++)
	{
		m_tem[i] = 0;
	}
	_store = new struct save;

	if(s3eSecureStorageGet(_store,sizeof(struct save)) == S3E_RESULT_SUCCESS)
	{
		high_score = _store->score;
		total_star = _store->star;
		music = _store->m;
		sc_sel = _store->sc;
		megawarp = _store->mw;
		collected_star = _store->cstar;
		for (int i = 0; i < 6; i++)
		{
			sc_locked[i] = _store->sc_l[i];
		}
		for (int i = 0; i < 4; i++)
		{
			power_time[i] = _store->pt[i];
		}
	}
	else
	{
		high_score = 100000;
		total_star = 100000;
		collected_star = 100000;
		music = 2;
		sc_sel = 0;
		megawarp = 1;
		sc_locked[0] = 0;
		for (int i = 1; i < 6; i++)
		{
			sc_locked[i] = 1;
		}
		power_time[0] = 10;
		power_time[2] = power_time[1] = 5;
		power_time[3] = 50;
		page = -3;
	}

	max_lives[0] = 2;
	max_lives[1] = 3;
	max_lives[2] = 4;
	max_lives[3] = 5;
	max_lives[4] = 2;
	max_lives[5] = 5;
	
	s3eAudioPlay("sound/bg.mp3",0);

	if(music != 2)
	{
		s3eAudioSetInt(S3E_AUDIO_VOLUME,0);
	}
	else
	{
		s3eAudioSetInt(S3E_AUDIO_VOLUME,S3E_AUDIO_MAX_VOLUME);
	}
	Explosion_sound = (CIwSoundSpec*)getresource->Effects->GetResNamed("explosion", IW_SOUND_RESTYPE_SPEC);
	Star_sound = (CIwSoundSpec*)getresource->Effects->GetResNamed("star", IW_SOUND_RESTYPE_SPEC);
	Fuel_sound = (CIwSoundSpec*)getresource->Effects->GetResNamed("fuel", IW_SOUND_RESTYPE_SPEC);
	Power_sound = (CIwSoundSpec*)getresource->Effects->GetResNamed("power", IW_SOUND_RESTYPE_SPEC);

	if(Iw2DGetSurfaceHeight()>=1080)
	{
		max_speed[0] = 8;
		max_speed[1] = 8;
		max_speed[2] = 10;
		max_speed[3] = 8;
		max_speed[4] = 12;
		max_speed[5] = 12;
	}
	else if(Iw2DGetSurfaceHeight()>=720)
	{
		max_speed[0] = 5;
		max_speed[1] = 5;
		max_speed[2] = 7;
		max_speed[3] = 5;
		max_speed[4] = 9;
		max_speed[5] = 9;
	}
	else if (Iw2DGetSurfaceHeight()>=400)
	{
		max_speed[0] = 2;
		max_speed[1] = 2;
		max_speed[2] = 4;
		max_speed[3] = 2;
		max_speed[4] = 6;
		max_speed[5] = 6;
	}
	else
	{
		max_speed[0] = 1;
		max_speed[1] = 1;
		max_speed[2] = 3;
		max_speed[3] = 1;
		max_speed[4] = 5;
		max_speed[5] = 5;
	}
	g_speed = max_speed[sc_sel];
	l_speed = g_speed;
	delay = 0;
	slide = 0;
	slide_add = 0;

	Iw2DSetFont(getresource->get_font());
	font_size = (float)(getresource->get_font()->GetHeight());

	IwRandSeed((int)s3eTimerGetMs());
	ini_Environment();
	ini_main_page();
	ini_play_page();
}

gamePlay::~gamePlay()
{
	delete _store;
}

//--------------------------------ENVIRONMENT-----------------------------------------------
//---------------------------------------------------------------------------------------------------------

void gamePlay::ini_Environment()
{
	for (int i = 0; i < 50; i++)
	{
		pos_dot[i].x = (float)IwRandRange(Iw2DGetSurfaceWidth());
		pos_dot[i].y = (float)IwRandRange(Iw2DGetSurfaceHeight());
	}

	for (int i = 0; i < 10; i++)
	{
		size_star[i] = IwRandMinMax((int)(Iw2DGetSurfaceWidth()*0.001f),(int)(Iw2DGetSurfaceWidth()*0.04f));
		phase_star[i] = IwRandMinMax(0,1);
		pos_star[i].x = (float)IwRandRange((int)(2*Iw2DGetSurfaceWidth()-Iw2DGetSurfaceWidth()*0.04f));
		pos_star[i].y = (float)IwRandRange((int)(Iw2DGetSurfaceHeight()-Iw2DGetSurfaceWidth()*0.04f));
		speed_star[i] = IwRandMinMax(1,4);
	}

	for (int i = 0; i < 5; i++)
	{
		p_size[i].y = p_size[i].x = (float)(IwRandMinMax((int)(Iw2DGetSurfaceHeight()*0.05f),(int)(Iw2DGetSurfaceHeight()*0.09f)));

		p_pos[i].x = Iw2DGetSurfaceWidth()*(0.20f*i);
		p_pos[i].y = (float)(IwRandRange(Iw2DGetSurfaceHeight()-(int)p_size[i].x));

		switch (IwRandRange(5))
		{
			case 0:
				p_type[i] = 0;
				break;
			case 1:
				p_type[i] = 1;
				break;
			case 2:
				p_type[i] = 2;
				break;
			case 3:
				p_type[i] = 1;
				break;
			case 4:
				p_type[i] = 0;
				break;
		}
	}

	cp_size.x = (float)(IwRandMinMax((int)(Iw2DGetSurfaceWidth()*0.20f),(int)(Iw2DGetSurfaceWidth()*0.25f)));
	cp_size.y = cp_size.x*0.56f;

	cp_pos.x = (float)(IwRandRange(2*Iw2DGetSurfaceWidth()));
	cp_pos.y = (float)(IwRandRange(Iw2DGetSurfaceHeight()-(int)cp_size.x));

	if(IwRandRange(100)%7 == 0)
	{
		cp_col = 2;
	}
	else if(IwRandRange(100)%3 == 0)
	{
		cp_col = 1;
	}
	else
	{
		cp_col = 0;
	}

	panel_size.x = Iw2DGetSurfaceWidth()*0.70f;
	panel_size.y = Iw2DGetSurfaceHeight()*0.70f;
	panel_pos.x = Iw2DGetSurfaceWidth()*0.50f-panel_size.x/2;
	panel_pos.y = Iw2DGetSurfaceHeight()*0.50f-panel_size.y/2;

	main_panel_size.x = Iw2DGetSurfaceWidth()*0.99f;
	main_panel_size.y = Iw2DGetSurfaceHeight()*0.99f;
	main_panel_pos.x = Iw2DGetSurfaceWidth()*0.50f-main_panel_size.x/2;
	main_panel_pos.y = Iw2DGetSurfaceHeight()*0.50f-main_panel_size.y/2;

	button_size.x = button_size.y = Iw2DGetSurfaceHeight()*0.17f;
	soundm_pos.y = home_pos.y = continue_pos.y = Iw2DGetSurfaceHeight()*0.50f - button_size.y/2;

	continue_pos.x = Iw2DGetSurfaceWidth()*0.50f - button_size.x/2;
	home_pos.x = (Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceWidth()*0.15f)/2 - button_size.x/2;
	soundm_pos.x = (Iw2DGetSurfaceWidth()*0.85f+Iw2DGetSurfaceWidth()*0.50f)/2 - button_size.x/2;

	spacecraft_set_size.y = spacecraft_set_size.x = Iw2DGetSurfaceWidth()*0.17f;
	spacecraft_set_pos[3].x = spacecraft_set_pos[0].x = Iw2DGetSurfaceWidth()*.10f;
	spacecraft_set_pos[4].x = spacecraft_set_pos[1].x = Iw2DGetSurfaceWidth()*.50f-spacecraft_set_size.x/2;
	spacecraft_set_pos[5].x = spacecraft_set_pos[2].x = Iw2DGetSurfaceWidth()*.90f-spacecraft_set_size.x;

	spacecraft_set_pos[2].y = spacecraft_set_pos[1].y = spacecraft_set_pos[0].y = Iw2DGetSurfaceHeight()*0.10f;
	spacecraft_set_pos[5].y = spacecraft_set_pos[4].y = spacecraft_set_pos[3].y = Iw2DGetSurfaceHeight()*0.80f-spacecraft_set_size.y;
	spacecraft_tick_size.x = spacecraft_tick_size.y = Iw2DGetSurfaceWidth()*0.04f;

	about_info_pos.x = 0;
	about_info_pos.y = Iw2DGetSurfaceHeight()*0.05f;
}

void gamePlay::update_Environment()
{
	for (int i = 0; i < 10; i++)
	{
		phase_star[i]==0?size_star[i]+=speed_star[i]:size_star[i]-=speed_star[i];
		if(size_star[i] >= Iw2DGetSurfaceWidth()*0.04f)
		{
			phase_star[i]=1;
		}
		else if(size_star[i] <= 2)
		{
			phase_star[i]=0;
		}
				
		pos_star[i].x-=l_speed;
		if(pos_star[i].x < -Iw2DGetSurfaceWidth()*0.04f)
		{
			pos_star[i].x = (float)IwRandMinMax(Iw2DGetSurfaceWidth(),2*Iw2DGetSurfaceWidth());
			pos_star[i].y = (float)IwRandRange((int)(Iw2DGetSurfaceHeight()-Iw2DGetSurfaceWidth()*0.04f));
			speed_star[i] = IwRandMinMax(1,4);
			phase_star[i] = IwRandMinMax(0,1);
			size_star[i] = IwRandMinMax((int)(Iw2DGetSurfaceWidth()*0.001f),(int)(Iw2DGetSurfaceWidth()*0.04f));
		}
	}	

	for (int i = 0; i < 50; i++)
	{
		pos_dot[i].x -= l_speed*0.85f;
		if(pos_dot[i].x < 0)
		{
			pos_dot[i].x = (float)Iw2DGetSurfaceWidth();
			pos_dot[i].y = (float)IwRandRange(Iw2DGetSurfaceHeight());
		}
	}

	for (int i = 0; i < 5; i++)
	{
		p_pos[i].x -= l_speed;
		if(p_pos[i].x <= -p_size[i].x)
		{
			p_size[i].y = p_size[i].x = (float)(IwRandMinMax((int)(Iw2DGetSurfaceHeight()*0.05f),(int)(Iw2DGetSurfaceHeight()*0.09f)));

			p_pos[i].x = (float)IwRandMinMax(Iw2DGetSurfaceWidth(),Iw2DGetSurfaceWidth()+(int)(Iw2DGetSurfaceWidth()*0.05f));
			p_pos[i].y = (float)(IwRandRange(Iw2DGetSurfaceHeight()-(int)p_size[i].x));

			switch (IwRandRange(5))
			{
				case 0:
					p_type[i] = 0;
					break;
				case 1:
					p_type[i] = 1;
					break;
				case 2:
					p_type[i] = 2;
					break;
				case 3:
					p_type[i] = 1;
					break;
				case 4:
					p_type[i] = 0;
					break;
			}
		}
	}

	cp_pos.x -= l_speed;
	if(cp_pos.x <= -cp_size.x)
	{
		cp_size.x = (float)(IwRandMinMax((int)(Iw2DGetSurfaceWidth()*0.20f),(int)(Iw2DGetSurfaceWidth()*0.25f)));
		cp_size.y = cp_size.x*0.56f;

		cp_pos.x = (float)IwRandMinMax(Iw2DGetSurfaceWidth(),2*Iw2DGetSurfaceWidth());
		cp_pos.y = (float)(IwRandRange(Iw2DGetSurfaceHeight()-(int)cp_size.x));

		if(IwRandRange(100)%7 == 0)
		{
			cp_col = 2;
		}
		else if(IwRandRange(100)%3 == 0)
		{
			cp_col = 1;
		}
		else
		{
			cp_col = 0;
		}
	}
}

void gamePlay::draw_Environment()
{
	for (int i = 0; i < 10; i++)
	{
		if (pos_star[i].x >= -size_star[i] && pos_star[i].x <= Iw2DGetSurfaceWidth())
		{
			Iw2DDrawImage(getresource->get_bg_star(),CIwFVec2(pos_star[i].x-size_star[i]/2,pos_star[i].y-size_star[i]/2),CIwFVec2((float)size_star[i],(float)size_star[i]));
		}
	}

	for (int i = 0; i < 50; i++)
	{
		Iw2DDrawRect(pos_dot[i],CIwFVec2(1,1));
	}
	
	for (int i = 0; i < 5; i++)
	{
		if(p_pos[i].x >= -p_size[i].x && p_pos[i].x <= Iw2DGetSurfaceWidth())
		{
			switch (p_type[i])
			{
				case 0:
					Iw2DDrawImage(getresource->get_planet_1(),p_pos[i],p_size[i]);
					break;
				case 1:
					Iw2DDrawImage(getresource->get_planet_2(),p_pos[i],p_size[i]);
					break;
				case 2:
					Iw2DDrawImage(getresource->get_planet_3(),p_pos[i],p_size[i]);
					break;
			}
		}
	}

	if(cp_pos.x >= -cp_size.x && cp_pos.x <= Iw2DGetSurfaceWidth())
	{
		switch (cp_col)
		{
			case 1:
				Iw2DSetColour(0xff0000ff);
				break;
			case 2:
				Iw2DSetColour(0xff00ffff);
				break;
		}
		Iw2DDrawImage(getresource->get_planet_5(),cp_pos,cp_size);
	}
	Iw2DSetColour(0xffffffff);
}

//---------------------------------------------MAIN PAGE--------------------------------------------
//---------------------------------------------------------------------------------------------------------

void gamePlay::ini_main_page()
{
	l_speed = g_speed;

	switch (sc_sel)
	{
		case 0:	
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*0.74f;
			break;
		case 1:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*1.2f;
			break;
		case 2:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*1.2f;
			break;
		case 3:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y;
			break;
		case 4:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*1.2f;
			break;
		case 5:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y;
			break;
	}
	sc_pos.x = Iw2DGetSurfaceWidth()*0.5f - sc_size.x*0.5f;
	sc_pos.y = Iw2DGetSurfaceHeight()*0.5f - sc_size.y*0.5f;

	exhaust_size[0].y = Iw2DGetSurfaceHeight()*0.020f;
	exhaust_size[1].y = Iw2DGetSurfaceHeight()*0.030f;
	exhaust_size[2].y = Iw2DGetSurfaceHeight()*0.040f;
	for (int i = 0; i < 3; i++)
	{
		exhaust_size[i].x = exhaust_size[i].y*2.14f;
	}
	
	com_size.y = Iw2DGetSurfaceHeight()*0.30f;
	com_size.x = com_size.y*2.56f;
	com_x = 0;
	com_i = 0;

	ast_size.y = Iw2DGetSurfaceHeight()*0.15f;
	ast_size.x = ast_size.y*0.94f;
	ast_x = 0;
	ast_i = 0;

	bstar_size.x = bstar_size.y = Iw2DGetSurfaceHeight()*0.07f;
	bstar_x = 0;
	bstar_y = 0;

	exhaust_sel = 0;
	exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
	exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;

	trans = 0;

	star = 0;
	star_size.x = Iw2DGetSurfaceWidth()*0.30f;
	star_size.y = font_size;
	star_pos.x = Iw2DGetSurfaceWidth()*0.05f+2*(Iw2DGetSurfaceWidth()*0.005f);
	star_pos.y = Iw2DGetSurfaceHeight()*0.005f+Iw2DGetSurfaceHeight()*0.05f/2-star_size.y/2;
	
	score = 0;
	score_size = star_size;
	score_pos.x = Iw2DGetSurfaceWidth()*0.70f;
	score_pos.y = star_pos.y;

	lives = max_lives[sc_sel];
	lives_size = star_size;
	lives_pos.x = Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.005f;
	lives_pos.y = star_pos.y;

	energy = 500;
	energy_size.x = Iw2DGetSurfaceWidth()*0.0004f*energy;
	energy_size.y = lives_size.y/4;
	energy_pos.x = star_pos.x;
	energy_pos.y = Iw2DGetSurfaceHeight()*0.05f*1.5f+2*(Iw2DGetSurfaceHeight()*0.005f)-energy_size.y/2;

	reenergy_size.y = reenergy_size.x = Iw2DGetSurfaceHeight()*0.15f;
	reenergy = 0;

	power = 0;
	power_avail = 6;
	power_on_time = 0;
	power_size.y = power_size.x = Iw2DGetSurfaceHeight()*0.15f;
	powerglow_size.y = powerglow_size.x = Iw2DGetSurfaceHeight()*0.30f;
	powerglow_pos.x = Iw2DGetSurfaceWidth()*0.5f - powerglow_size.x*0.5f;
	powerglow_pos.y = Iw2DGetSurfaceHeight()*0.5f - powerglow_size.y*0.5f;

	mega_active = megawarp==0?0:1;
	megawarp_size = star_size;
	megawarp_pos.x = Iw2DGetSurfaceHeight()*0.10f+2*(Iw2DGetSurfaceWidth()*0.005f);
	megawarp_pos.y = Iw2DGetSurfaceHeight()*0.90f+Iw2DGetSurfaceHeight()*0.10f-megawarp_size.y;

	about_pos.x = 0;
	about_pos.y = Iw2DGetSurfaceHeight()-button_size.y;

	rate_pos.x = Iw2DGetSurfaceWidth()-button_size.x;
	rate_pos.y = about_pos.y;

	settings_pos.x = (Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceWidth()*0.10f)/2-button_size.x/2;
	settings_pos.y = Iw2DGetSurfaceHeight()*0.50f - button_size.y/2;

	sound_pos.x = (Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceWidth()*0.90f)/2-button_size.x/2;
	sound_pos.y = settings_pos.y;
	
	if(s3eWindowsPhoneAdAvailable())
	{
		ad_control = s3eWindowsPhoneAdCreate("test_client","Image480_80");
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_HEIGHT,80);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_WIDTH,480);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_HALIGN,S3E_WINDOWSPHONE_ADCONTROL_HALIGN_CENTER);
		s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_VALIGN,S3E_WINDOWSPHONE_ADCONTROL_VALIGN_BOTTOM);
		if(s3eSocketGetInt(S3E_SOCKET_NETWORK_AVAILABLE))
		{
			s3eWindowsPhoneAdShow(ad_control);
		}
	}
}

void gamePlay::update_main_page()
{
	/*
	if (delay == 0)
	{
		sc_sel++;
		if (sc_sel ==6)
		{
			sc_sel = 0;
		}
		delay = 200;
		switch (sc_sel)
	{
		case 0:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*0.74f;
			break;
		case 1:
			sc_size.y = Iw2DGetSurfaceHeight()*0.13f;
			sc_size.x = sc_size.y*1.53f;
			break;
		case 2:
			sc_size.y = Iw2DGetSurfaceHeight()*0.15f;
			sc_size.x = sc_size.y*1.33f;
			break;
		case 3:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*0.99f;
			break;
		case 4:
			sc_size.y = Iw2DGetSurfaceHeight()*0.09f;
			sc_size.x = sc_size.y*2.12f;
			break;
		case 5:
			sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
			sc_size.x = sc_size.y*1.02f;
			break;
	}
		sc_pos.x = Iw2DGetSurfaceWidth()*0.5f - sc_size.x*0.5f;
	sc_pos.y = Iw2DGetSurfaceHeight()*0.5f - sc_size.y*0.5f;
	}
	*/if(trans == 1)
	{
		sc_pos.x -= l_speed;
		powerglow_pos.x -= l_speed;
		
		if(sc_pos.x <= Iw2DGetSurfaceWidth()*0.04f)
		{
			trans = 0;
			page = 1;
			if(s3eWindowsPhoneAdAvailable())
			{
				s3eWindowsPhoneAdSetIntProperty(ad_control,S3E_WINDOWSPHONE_ADCONTROL_WIDTH,0);
			}
			ini_pause_page();
		}
	}
	else
	{
		if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		{
			if((s3ePointerGetX()>=sc_pos.x && s3ePointerGetX()<=sc_pos.x+sc_size.x) &&
				(s3ePointerGetY()>=sc_pos.y && s3ePointerGetY()<=sc_pos.y+sc_size.y))
			{
				if(delay == 0)
				{
					trans = 1;
				}
			}
			else if (s3ePointerGetX()>=sound_pos.x && s3ePointerGetX()<=sound_pos.x+button_size.x &&
				s3ePointerGetY()>=sound_pos.y && s3ePointerGetY()<=sound_pos.y+button_size.y)
			{
				if(music == 0 && delay == 0)
				{
					music = 1;
					CIwSoundInst* SoundInstance = Star_sound->Play();
					delay = 30;
				}
				else if(music == 1 && delay == 0)
				{
					music = 2;
					s3eAudioSetInt(S3E_AUDIO_VOLUME,S3E_AUDIO_MAX_VOLUME);
					delay = 30;
				}
				else if(music == 2 && delay == 0)
				{
					music = 0;
					s3eAudioSetInt(S3E_AUDIO_VOLUME,0);
					delay = 30;
				}
			}
			else if (s3ePointerGetX()>=rate_pos.x && s3ePointerGetX()<=rate_pos.x+button_size.x &&
				s3ePointerGetY()>=rate_pos.y && s3ePointerGetY()<=rate_pos.y+button_size.y)
			{
				if(delay == 0)
				{
					switch (s3eDeviceGetInt(S3E_DEVICE_OS))
					{
						case S3E_OS_ID_ANDROID:
							s3eOSExecExecute("http://www.windowsphone.com/s?appid=ab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
						case S3E_OS_ID_WP8:
							s3eOSExecExecute("zune:navigate?appid=appab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
						default:
							s3eOSExecExecute("http://www.windowsphone.com/s?appid=ab3f9c1b-68bc-43fe-ac24-231ef3d92122",false);
							break;
					}
					delay = 30;
				}
			}
			else if (s3ePointerGetX()>=settings_pos.x && s3ePointerGetX()<=settings_pos.x+button_size.x &&
				s3ePointerGetY()>=settings_pos.y && s3ePointerGetY()<=settings_pos.y+button_size.y)
			{
				if(delay == 0)
				{
					page = -2;
					delay = 30;
				}
			}
			else if (s3ePointerGetX()>=about_pos.x && s3ePointerGetX()<=about_pos.x+button_size.x &&
				s3ePointerGetY()>=about_pos.y && s3ePointerGetY()<=about_pos.y+button_size.y)
			{
				if(delay == 0)
				{
					page = -3;
					delay = 30;
				}
			}
		}
	}

	//-----------exhaust animation-------------

	exhaust_sel++;
	if(exhaust_sel==3)
	{
		exhaust_sel = 0;
	}
	exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
	exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;

	//-----------star animation----------------
	bstar_x+=50;
	if(bstar_x == 1000)
	{
		if(bstar_y == 0)
		{
			bstar_y = 50;
		}
		else
		{
			bstar_y = 0;
		}
		bstar_x = 0;
	}
}

void gamePlay::draw_main_page()
{
	if (page == 0)
	{
		Iw2DDrawImage(getresource->get_spacecraft(sc_sel),sc_pos,sc_size);
		Iw2DDrawImage(getresource->get_exhaust(),exhaust_pos,exhaust_size[exhaust_sel]);
	}
	
	if(trans == 0)
	{
		if(page==0)
		{
			Iw2DDrawString("TAP",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.40f-font_size),CIwFVec2((float)Iw2DGetSurfaceWidth(),font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawString("to START",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.60f),CIwFVec2((float)Iw2DGetSurfaceWidth(),font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
			Iw2DDrawImage(getresource->get_settings(),settings_pos,button_size);
			if(music == 0)
			{
				Iw2DDrawImageRegion(getresource->get_sound_key(),sound_pos,button_size,CIwFVec2(200,0),CIwFVec2(100,98));
			}
			else if(music == 1)
			{
				Iw2DDrawImageRegion(getresource->get_sound_key(),sound_pos,button_size,CIwFVec2(0,0),CIwFVec2(100,98));
			}
			else
			{
				Iw2DDrawImageRegion(getresource->get_sound_key(),sound_pos,button_size,CIwFVec2(100,0),CIwFVec2(100,98));
			}
		}
		Iw2DDrawImage(getresource->get_about_us(),about_pos,button_size);
		Iw2DDrawImage(getresource->get_rate_us(),rate_pos,button_size);
			
	}

	Iw2DDrawImageRegion(getresource->get_bonus_star(),CIwFVec2(Iw2DGetSurfaceWidth()*0.005f,Iw2DGetSurfaceHeight()*0.005f),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f,Iw2DGetSurfaceHeight()*0.05f),CIwFVec2((float)bstar_x,(float)bstar_y),CIwFVec2(50,50));

	sprintf(str,"%.0f",total_star);
	Iw2DDrawString(str,star_pos,star_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
	
	sprintf(str,"Highscore : %.0f",high_score);
	Iw2DDrawString(str,score_pos,score_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

	Iw2DDrawImage(getresource->get_spacecraft(0),CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceHeight()*0.05f*0.99f,Iw2DGetSurfaceHeight()*0.005f),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f*0.99f,Iw2DGetSurfaceHeight()*0.05f));
	sprintf(str,"X %.0f",lives);
	Iw2DDrawString(str,lives_pos,lives_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

	Iw2DDrawImage(getresource->get_energy(),CIwFVec2(Iw2DGetSurfaceWidth()*0.005f,Iw2DGetSurfaceHeight()*0.05f+2*(Iw2DGetSurfaceHeight()*0.005f)),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f,Iw2DGetSurfaceHeight()*0.05f));
	Iw2DSetColour(0xff00ff00);
	Iw2DFillRect(energy_pos,energy_size);
	Iw2DSetColour(0xffffffff);
	Iw2DDrawRect(energy_pos,energy_size);
}

//---------------------------------------------PAUSE PAGE------------------------------------------
//---------------------------------------------------------------------------------------------------------

void gamePlay::ini_pause_page()
{
	
}

void gamePlay::draw_pause_page()
{
	Iw2DDrawImage(getresource->get_panel(),panel_pos,panel_size);
	Iw2DDrawImage(getresource->get_home(),home_pos,button_size);
	Iw2DDrawImage(getresource->get_resume(),continue_pos,button_size);
	if(music == 0)
	{
		Iw2DDrawImageRegion(getresource->get_sound_key(),soundm_pos,button_size,CIwFVec2(200,0),CIwFVec2(100,98));
	}
	else if(music == 1)
	{
		Iw2DDrawImageRegion(getresource->get_sound_key(),soundm_pos,button_size,CIwFVec2(0,0),CIwFVec2(100,98));
	}
	else
	{
		Iw2DDrawImageRegion(getresource->get_sound_key(),soundm_pos,button_size,CIwFVec2(100,0),CIwFVec2(100,98));
	}
}

void gamePlay::update_pause_page()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		page = 1;
		s3eKeyboardClearState();
	}
	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
	{
		if(s3ePointerGetX() >= home_pos.x && s3ePointerGetX() <= home_pos.x+button_size.x &&
			s3ePointerGetY() >= home_pos.y && s3ePointerGetY() <= home_pos.y+button_size.y)
		{
			blast = 0;
			page = 0;
			delay = 30;
			ini_main_page();
			ini_play_page();
		}
		else if(s3ePointerGetX() >= continue_pos.x && s3ePointerGetX() <= continue_pos.x+button_size.x &&
			s3ePointerGetY() >= continue_pos.y && s3ePointerGetY() <= continue_pos.y+button_size.y)
		{
			page = 1;
		}
		else if(s3ePointerGetX() >= soundm_pos.x && s3ePointerGetX() <= soundm_pos.x+button_size.x &&
			s3ePointerGetY() >= soundm_pos.y && s3ePointerGetY() <= soundm_pos.y+button_size.y)
		{
			if(music == 0 && delay == 0)
			{
				music = 1;
				CIwSoundInst* SoundInstance = Star_sound->Play();
				delay = 30;
			}
			else if(music == 1 && delay == 0)
			{
				music = 2;
				s3eAudioSetInt(S3E_AUDIO_VOLUME,S3E_AUDIO_MAX_VOLUME);
				delay = 30;
			}
			else if(music == 2 && delay == 0)
			{
				music = 0;
				s3eAudioSetInt(S3E_AUDIO_VOLUME,0);
				delay = 30;
			}
		}
	}
}

//---------------------------------------------ABOUT PAGE-------------------------------------------
//------------------------------------------------------------------------------------------------------

void gamePlay::ini_about_page()
{
	
}

void gamePlay::update_about_page()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		page = 0;
		s3eKeyboardClearState();
	}
	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
	{
		if(s3ePointerGetX()>=Iw2DGetSurfaceHeight()*0.10f && s3ePointerGetX()<=Iw2DGetSurfaceHeight()*0.10f+button_size.x &&
			s3ePointerGetY()>=Iw2DGetSurfaceHeight()*0.10f && s3ePointerGetY()<=Iw2DGetSurfaceHeight()*0.10f+button_size.y)
		{
			page = 0;
			delay = 30;
		}
		if(isslide == false)
		{
			isslide = true;
			slide_add = (float)s3ePointerGetY();
		}
		else
		{
			if(slide_add < s3ePointerGetY())
			{
				slide_add = (float)s3ePointerGetY();
				if(slide>0)
				{
					slide -= (int)max_speed[0]*8;
					if(slide<0)
					{
						slide = 0;
					}
				}
			}
			else if(slide_add > s3ePointerGetY())
			{
				slide_add = (float)s3ePointerGetY();
				if(slide < 840)
				{
					slide += (int)max_speed[0]*8;
					if(slide>840)
					{
						slide = 840;
					}
				}
			}
		}
	}
	else
	{
		isslide = false;
	}
}

void gamePlay::draw_about_page()
{
	Iw2DDrawImage(getresource->get_panel(),main_panel_pos,main_panel_size);
	Iw2DDrawImageRegion(getresource->get_about(),about_info_pos,CIwFVec2((float)Iw2DGetSurfaceWidth(),Iw2DGetSurfaceHeight()*0.85f),CIwFVec2(0,(float)slide+50),CIwFVec2(1280,660));
	Iw2DDrawImage(getresource->get_resume(),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),button_size);
}

//-------------------------------------------PERSONALIZE PAGE-------------------------------------------
//------------------------------------------------------------------------------------------------------

void gamePlay::ini_personalize_page()
{

}

void gamePlay::update_personalize_page()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		page = 0;
		subpage = 0;
		switch (sc_sel)
		{
			case 0:	
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y*0.74f;
				break;
			case 1:
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y*1.2f;
				break;
			case 2:
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y*1.2f;
				break;
			case 3:
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y;
				break;
			case 4:
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y*1.2f;
				break;
			case 5:
				sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
				sc_size.x = sc_size.y;
				break;
		}
		sc_pos.x = Iw2DGetSurfaceWidth()*0.5f - sc_size.x*0.5f;
		sc_pos.y = Iw2DGetSurfaceHeight()*0.5f - sc_size.y*0.5f;
		lives = max_lives[sc_sel];
		l_speed = g_speed = max_speed[sc_sel];

		_store->score = high_score;
		_store->star = total_star;
		_store->m = music;
		_store->mw = megawarp;
		_store->sc = sc_sel;
		_store->cstar = collected_star;
		for (int i = 0; i < 6; i++)
		{
			_store->sc_l[i] = sc_locked[i];
		}
		for (int i = 0; i < 4; i++)
		{
			_store->pt[i] = power_time[i];
		}

		s3eSecureStoragePut(_store,sizeof(struct save));

		s3eKeyboardClearState();
	}
	switch (subpage)
	{
		case 0:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
			{
				if(s3ePointerGetX() >= home_pos.x && s3ePointerGetX() <= home_pos.x+button_size.x &&
					s3ePointerGetY() >= home_pos.y && s3ePointerGetY() <= home_pos.y+button_size.y)
				{
					if(delay == 0)
					{
						subpage = 1;
						delay = 30;
					}
				}
				else if(s3ePointerGetX() >= continue_pos.x && s3ePointerGetX() <= continue_pos.x+button_size.x &&
					s3ePointerGetY() >= continue_pos.y && s3ePointerGetY() <= continue_pos.y+button_size.y)
				{
					if (delay == 0)
					{
						subpage = 2;
						delay = 30;
					}
				}
				else if(s3ePointerGetX() >= soundm_pos.x && s3ePointerGetX() <= soundm_pos.x+button_size.x &&
					s3ePointerGetY() >= soundm_pos.y && s3ePointerGetY() <= soundm_pos.y+button_size.y)
				{
					if (delay == 0)
					{
						subpage = 3;
						delay = 30;
					}
				}
			}
			break;
		case 1:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
			{
				for (int i = 0; i < 6; i++)
				{
					if(s3ePointerGetX() >= spacecraft_set_pos[i].x && s3ePointerGetX() <= spacecraft_set_pos[i].x+spacecraft_set_size.x &&
						s3ePointerGetY() >= spacecraft_set_pos[i].y && s3ePointerGetY() <= spacecraft_set_pos[i].y+spacecraft_set_size.y)
					{
						if(sc_locked[i] == 1)
						{
							switch (i)
							{
								case 1:
									if(total_star>=5000)
									{
										total_star-=5000;
										sc_locked[i] = 0;
										sc_sel = i;
										CIwSoundInst* SoundInstance = Power_sound->Play();
									}
									break;
								case 2:
									if(total_star>=10000)
									{
										total_star-=10000;
										sc_locked[i] = 0;
										sc_sel = i;
										CIwSoundInst* SoundInstance = Power_sound->Play();
									}
									break;
								case 3:
									if(total_star>=20000)
									{
										total_star-=20000;
										sc_locked[i] = 0;
										sc_sel = i;
										CIwSoundInst* SoundInstance = Power_sound->Play();
									}
									break;
								case 4:
									if(total_star>=30000)
									{
										total_star-=30000;
										sc_locked[i] = 0;
										sc_sel = i;
										CIwSoundInst* SoundInstance = Power_sound->Play();
									}
									break;
								case 5:
									if(total_star>=50000)
									{
										total_star-=50000;
										sc_locked[i] = 0;
										sc_sel = i;
										CIwSoundInst* SoundInstance = Power_sound->Play();
									}
									break;
							}
						}
						else if(sc_locked[i] == 0 && sc_sel != i)
						{
							sc_sel = i;
							CIwSoundInst* SoundInstance = Star_sound->Play();
						}
					}
				}
			}
			break;
		case 2:
			if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
			{
				if(s3ePointerGetX() >= 2*Iw2DGetSurfaceWidth()*0.11f && s3ePointerGetX() <= 2*Iw2DGetSurfaceWidth()*0.11f+Iw2DGetStringWidth("UPGRADE 100000") &&
						s3ePointerGetY() >= Iw2DGetSurfaceHeight()*0.19f+Iw2DGetSurfaceHeight()*0.12f+font_size && s3ePointerGetY() <= Iw2DGetSurfaceHeight()*0.19f+Iw2DGetSurfaceHeight()*0.12f+2*font_size)
				{
					if(delay == 0)
					{
						switch (power_time[0])
						{
							case 10:
								if(total_star >=1000)
								{
									total_star-=1000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[0] = 15;
								}
								break;
							case 15:
								if(total_star >=2500)
								{
									total_star-=2500;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[0] = 20;
								}
								break;
							case 20:
								if(total_star >=5000)
								{
									total_star-=5000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[0] = 25;
								}
								break;
							case 25:
								if(total_star >=10000)
								{
									total_star-=10000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[0] = 30;
								}
								break;
						}
					}
				}
				else if(s3ePointerGetX() >= Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f && s3ePointerGetX() <= Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f+Iw2DGetStringWidth("UPGRADE 100000") &&
						s3ePointerGetY() >= Iw2DGetSurfaceHeight()*0.19f+Iw2DGetSurfaceHeight()*0.12f+font_size && s3ePointerGetY() <= Iw2DGetSurfaceHeight()*0.19f+Iw2DGetSurfaceHeight()*0.12f+2*font_size)
				{
					if(delay == 0)
					{
						switch (power_time[1])
						{
							case 5:
								if(total_star >=1000)
								{
									total_star-=1000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[1] = 10;
								}
								break;
							case 10:
								if(total_star >=2500)
								{
									total_star-=2500;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[1] = 15;
								}
								break;
							case 15:
								if(total_star >=5000)
								{
									total_star-=5000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[1] = 20;
								}
								break;
							case 20:
								if(total_star >=10000)
								{
									total_star-=10000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[1] = 25;
								}
								break;
						}
					}
				}
				else if(s3ePointerGetX() >= 2*Iw2DGetSurfaceWidth()*0.11f && s3ePointerGetX() <= 2*Iw2DGetSurfaceWidth()*0.11f+Iw2DGetStringWidth("UPGRADE 100000") &&
						s3ePointerGetY() >= Iw2DGetSurfaceHeight()*0.54f+Iw2DGetSurfaceHeight()*0.12f+font_size && s3ePointerGetY() <= Iw2DGetSurfaceHeight()*0.54f+Iw2DGetSurfaceHeight()*0.12f+2*font_size)
				{
					if(delay == 0)
					{
						switch (power_time[2])
						{
							case 5:
								if(total_star >=1000)
								{
									total_star-=1000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[2] = 10;
								}
								break;
							case 10:
								if(total_star >=2500)
								{
									total_star-=2500;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[2] = 15;
								}
								break;
							case 15:
								if(total_star >=5000)
								{
									total_star-=5000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[2] = 20;
								}
								break;
							case 20:
								if(total_star >=10000)
								{
									total_star-=10000;
									delay = 30;
									CIwSoundInst* SoundInstance = Star_sound->Play();
									power_time[2] = 25;
								}
								break;
						}
					}
				}
				else if(s3ePointerGetX() >= Iw2DGetSurfaceWidth()*0.58f && s3ePointerGetX() <= Iw2DGetSurfaceWidth()*0.58f+Iw2DGetStringWidth("UPGRADE 100000") &&
						s3ePointerGetY() >= Iw2DGetSurfaceHeight()*0.54f+Iw2DGetSurfaceHeight()*0.12f+font_size && s3ePointerGetY() <= Iw2DGetSurfaceHeight()*0.54f+Iw2DGetSurfaceHeight()*0.12f+2*font_size)
				{
					if(delay == 0 && total_star>=1000)
					{
						megawarp++;
						total_star -= 1000;
						CIwSoundInst* SoundInstance = Star_sound->Play();
						delay = 30;
					}
				}
			}
			break;
	}
}

void gamePlay::draw_personalize_page()
{
	if(subpage == 0)
	{
		//Iw2DDrawImage(getresource->get_panel(),panel_pos,panel_size);
		Iw2DDrawImageRegion(getresource->get_menu(),home_pos,button_size,CIwFVec2(0,0),CIwFVec2(100,100));
		Iw2DDrawImageRegion(getresource->get_menu(),continue_pos,button_size,CIwFVec2(100,0),CIwFVec2(100,100));
		Iw2DDrawImageRegion(getresource->get_menu(),soundm_pos,button_size,CIwFVec2(200,0),CIwFVec2(100,100));
	}
	else if (subpage == 1)
	{
		Iw2DDrawImage(getresource->get_panel(),main_panel_pos,main_panel_size);
		for (int i = 0; i < 6; i++)
		{
			Iw2DDrawImageRegion(getresource->get_spacecraft_set(),spacecraft_set_pos[i],spacecraft_set_size,CIwFVec2((float)(i<3?i*200:(i-3)*200),(float)(i<3?0:200)),CIwFVec2(200,200));
			if(sc_locked[i]==1)
			{
				Iw2DDrawImageRegion(getresource->get_spacecraft_locked(),spacecraft_set_pos[i],spacecraft_set_size,CIwFVec2((float)(i-1)*200,0),CIwFVec2(200,200));
			}
		}
		Iw2DDrawImageRegion(getresource->get_ok(),spacecraft_set_pos[sc_sel],spacecraft_tick_size,CIwFVec2(0,0),CIwFVec2(25,25));
	}
	else if(subpage == 2)
	{
		Iw2DDrawImage(getresource->get_panel(),main_panel_pos,main_panel_size);
		CIwFVec2 size = CIwFVec2(Iw2DGetSurfaceHeight()*0.12f,Iw2DGetSurfaceHeight()*0.12f);

		Iw2DDrawString("WARP",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.15f+size.y/2-font_size/2),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.15f),size,CIwFVec2(0,0),CIwFVec2(50,50));
		Iw2DDrawString("Cover Longer Distance",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.17f+size.y),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		switch (power_time[0])
		{
			case 10:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,0),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 1000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 1000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 15:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,18),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 2500",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 2500"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 20:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,36),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 5000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 5000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 25:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,54),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 10000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 10000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 30:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,72),CIwFVec2(60,18));
				Iw2DDrawString("MAX LEVEL",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				break;
		}

		Iw2DDrawString("ENCHANTER",CIwFVec2(Iw2DGetSurfaceWidth()*0.45f,Iw2DGetSurfaceHeight()*0.15f+size.y/2-font_size/2),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.15f),size,CIwFVec2(50,0),CIwFVec2(50,50));
		Iw2DDrawString("Magically Attract Stars",CIwFVec2(Iw2DGetSurfaceWidth()*0.45f,Iw2DGetSurfaceHeight()*0.17f+size.y),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		switch (power_time[1])
		{
			case 5:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,0),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 1000",CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.13f+Iw2DGetStringWidth("UPGRADE 1000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 10:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,18),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 2500",CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.13f+Iw2DGetStringWidth("UPGRADE 2500"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 15:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,36),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 5000",CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.13f+Iw2DGetStringWidth("UPGRADE 5000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 20:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,54),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 10000",CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.13f+Iw2DGetStringWidth("UPGRADE 10000"),Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 25:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.20f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,72),CIwFVec2(60,18));
				Iw2DDrawString("MAX LEVEL",CIwFVec2(Iw2DGetSurfaceWidth()*0.55f+Iw2DGetSurfaceWidth()*0.12f,Iw2DGetSurfaceHeight()*0.19f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				break;
		}

		Iw2DDrawString("IMMORTAL",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.50f+size.y/2-font_size/2),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.50f),size,CIwFVec2(100,0),CIwFVec2(50,50));
		Iw2DDrawString("Pass Through Objects",CIwFVec2(0,Iw2DGetSurfaceHeight()*0.52f+size.y),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		switch (power_time[2])
		{
			case 5:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.55f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,0),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 1000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 1000"),Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 10:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.55f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,18),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 2500",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 2500"),Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 15:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.55f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,36),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 5000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 5000"),Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 20:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.55f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,54),CIwFVec2(60,18));
				Iw2DDrawString("UPGRADE 10000",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(2*Iw2DGetSurfaceWidth()*0.115f+Iw2DGetStringWidth("UPGRADE 10000"),Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
				break;
			case 25:
				Iw2DDrawImageRegion(getresource->get_power_level(),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceHeight()*0.55f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.10f,Iw2DGetSurfaceWidth()*0.10f*0.3f),CIwFVec2(0,72),CIwFVec2(60,18));
				Iw2DDrawString("MAX LEVEL",CIwFVec2(2*Iw2DGetSurfaceWidth()*0.11f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
				break;
		}

		sprintf(str,"MEGA WARP X %d",megawarp);
		Iw2DDrawString(str,CIwFVec2(Iw2DGetSurfaceWidth()*0.48f,Iw2DGetSurfaceHeight()*0.50f+size.y/2-font_size/2),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.55f,Iw2DGetSurfaceHeight()*0.50f),size,CIwFVec2(150,0),CIwFVec2(50,50));
		Iw2DDrawString("AVAILABLE ONCE in each play",CIwFVec2(Iw2DGetSurfaceWidth()*0.45f,Iw2DGetSurfaceHeight()*0.52f+size.y),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawString("PURCHASE 1000",CIwFVec2(Iw2DGetSurfaceWidth()*0.58f,Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f,font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
		Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(Iw2DGetSurfaceWidth()*0.58f+Iw2DGetSurfaceWidth()*0.01f+Iw2DGetStringWidth("PURCHASE 1000"),Iw2DGetSurfaceHeight()*0.54f+size.y+font_size),CIwFVec2(font_size,font_size),CIwFVec2(50,50),CIwFVec2(50,50));
	}
	else if (subpage == 3)
	{
		Iw2DDrawImage(getresource->get_panel(),main_panel_pos,main_panel_size);

		if(collected_star>=1000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.18f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.18f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}		
		Iw2DDrawImageRegion(getresource->get_star_rank(),CIwFVec2(Iw2DGetSurfaceWidth()*0.03f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.15f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(0,0),CIwFVec2(100,100));
		Iw2DDrawString("Attentive [Star 1000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.04f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.20f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Attentive [Star 1000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(collected_star>=10000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.30f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.30f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_star_rank(),CIwFVec2(Iw2DGetSurfaceWidth()*0.03f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.27f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(100,0),CIwFVec2(100,100));
		Iw2DDrawString("Anxious [Star 10000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.04f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.32f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Anxious [Star 10000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(collected_star>=20000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.42f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.42f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_star_rank(),CIwFVec2(Iw2DGetSurfaceWidth()*0.03f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.39f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(200,0),CIwFVec2(100,100));
		Iw2DDrawString("Collector [Star 20000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.04f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.44f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Collector [Star 20000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(collected_star>=50000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.54f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.54f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_star_rank(),CIwFVec2(Iw2DGetSurfaceWidth()*0.03f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.51f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(300,0),CIwFVec2(100,100));
		Iw2DDrawString("Implausible [Star 50000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.04f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.56f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Implausible [Star 50000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(collected_star>=100000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.66f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.02f,Iw2DGetSurfaceHeight()*0.66f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_star_rank(),CIwFVec2(Iw2DGetSurfaceWidth()*0.03f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.63f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(400,0),CIwFVec2(100,100));
		Iw2DDrawString("Magnate [Star 100000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.04f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.68f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Magnate [Star 100000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(high_score>=50000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.18f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.18f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_ranks(),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.15f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(0,0),CIwFVec2(100,100));
		Iw2DDrawString("Novice [Highscore 50000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.51f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.20f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Novice [Highscore 50000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(high_score>=100000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.30f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.30f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_ranks(),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.27f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(100,0),CIwFVec2(100,100));
		Iw2DDrawString("Apprentice [Highscore 100000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.51f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.32f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Apprentice [Highscore 100000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if (high_score>=200000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.42f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.42f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_ranks(),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.39f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(200,0),CIwFVec2(100,100));
		Iw2DDrawString("Paramount [Highscore 200000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.51f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.44f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Paramount [Highscore 200000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if(high_score>=300000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.54f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.54f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_ranks(),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.51f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(300,0),CIwFVec2(100,100));
		Iw2DDrawString("Mythical [Highscore 300000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.51f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.56f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Mythical [Highscore 300000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

		if (high_score>=500000)
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.66f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(0,0),CIwFVec2(25,25));
		}
		else
		{
			Iw2DDrawImageRegion(getresource->get_ok(),CIwFVec2(Iw2DGetSurfaceWidth()*0.49f,Iw2DGetSurfaceHeight()*0.66f),CIwFVec2(Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.04f),CIwFVec2(25,0),CIwFVec2(25,25));
		}
		Iw2DDrawImageRegion(getresource->get_ranks(),CIwFVec2(Iw2DGetSurfaceWidth()*0.50f+Iw2DGetSurfaceHeight()*0.04f,Iw2DGetSurfaceHeight()*0.63f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(400,0),CIwFVec2(100,100));
		Iw2DDrawString("Legendary [Highscore 500000]",CIwFVec2(Iw2DGetSurfaceWidth()*0.51f+Iw2DGetSurfaceHeight()*0.14f,Iw2DGetSurfaceHeight()*0.68f-font_size/2),CIwFVec2((float)Iw2DGetStringWidth("Legendary [Highscore 500000]"),font_size),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
	}
}

//----------------------------------------------PLAY PAGE-------------------------------------------
//---------------------------------------------------------------------------------------------------------

void gamePlay::ini_play_page()
{
	for (int i = 0; i < 12; i++)
	{
		bstar_pos[i] = CIwFVec2(-1000,-1000);
		bstar_show[i] = 1;
	}
	for (int i = 0; i < 8; i++)
	{
		ast_pos[i] = CIwFVec2(-1000,-1000);
		ast_s[i] = IwRandRange(3);
		ast_show[i] = 1;
	}
	for (int i = 0; i < 2; i++)
	{
		com_pos[i] = CIwFVec2(-1000,-1000);
		reenergy_pos[i] = CIwFVec2(-1000,-1000);;
		reenergy_show[i] = 1;
		power_show[i] = 0;
	}

	sequence = 0;

	switch (IwRandRange(12))
	{
		case 0: sequence_1(sequence);
			break;
		case 1: sequence_2(sequence);
			break;
		case 2: sequence_3(sequence);
			break;
		case 3: sequence_4(sequence);
			break;
		case 4: sequence_5(sequence);
			break;
		case 5: sequence_6(sequence);
			break;
		case 6: sequence_7(sequence);
			break;
		case 7: sequence_8(sequence);
			break;
		case 8: sequence_9(sequence);
			break;
		case 9: sequence_10(sequence);
			break;
		case 10: sequence_11(sequence);
			break;
		case 11: sequence_12(sequence);
			break;
	}

	blast = 0;
	energy = 500;
}

void gamePlay::update_play_page()
{
	if (blast == 0 && energy>0)
	{
		//-----------Back Key Press control---------------
		if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
		{
			page = -1;
			s3eKeyboardClearState();
		}

		//---------------speed control-------------------------

		if(l_speed < g_speed+1)
		{
			l_speed += 0.1f;
		}
		else if(l_speed < g_speed+2)
		{
			l_speed += 0.05f;
		}
		else if(l_speed < g_speed+3)
		{
			l_speed += 0.005f;
		}

		//-----------------spacecraft up and down animation---------------------

		if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		{
			if(s3ePointerGetX()>=0 && 
				s3ePointerGetX()<= Iw2DGetSurfaceHeight()*0.20f &&
				s3ePointerGetY()>=Iw2DGetSurfaceHeight()*0.85f &&
				s3ePointerGetY()<=Iw2DGetSurfaceHeight()*0.85f+Iw2DGetSurfaceHeight()*0.15f &&
				mega_active == 1 && !(power == 1 && power_on_time!=0) && !(power == 2 && power_on_time!=0) && !(power == 3 && power_on_time!=0))
			{
				power = 4;
				power_on_time = power_time[3];
				power_avail = power_time[3]+6;
				power_down = 0;
				t_speed = l_speed;
				l_speed = g_speed+15;
				megawarp--;
				mega_active = 0;
			}
			else if(s3ePointerGetY() < Iw2DGetSurfaceHeight()*0.5f)
			{
				if(sc_pos.y > Iw2DGetSurfaceHeight()*0.25f-sc_size.y/2)
				{
					sc_pos.y-=l_speed*3.0f;
					powerglow_pos.y -= l_speed*3.0f;
				}
			}
			else
			{
				if(sc_pos.y < Iw2DGetSurfaceHeight()*0.75f-sc_size.y/2)
				{
					sc_pos.y+=l_speed*3.0f;
					powerglow_pos.y += l_speed*3.0f;
				}
			}
		}
		
		//---------score calculation------------------

		score += l_speed;

		//----------exhaust animation----------------

		exhaust_sel++;
		if(exhaust_sel==3)
		{
			exhaust_sel = 0;
		}
		exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
		exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;

		for (int i = 0; i < 8; i++)
		{
			//----------------astroid collison---------
			if (compare(sc_pos,ast_pos[i],sc_size,ast_size) && ast_show[i]==1 && !(power == 1 && power_on_time!=0) && !(power == 3 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
			{
				ast_show[i] = 0;
				blast=1;
				b_x = 0;
				b_y = 0;
				if(music!=0)
				{
					CIwSoundInst* SoundInstance = Explosion_sound->Play();
				}
			}
		}
		for (int i = 0; i < 2; i++)
		{
			//-----------------comet collison----------

			if (com_compare(sc_pos,com_pos[i],sc_size,com_size) && !(power == 1 && power_on_time!=0) && !(power == 3 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
			{
				blast=1;
				b_x = 0;
				b_y = 0;
				if(music!=0)
				{
					CIwSoundInst* SoundInstance = Explosion_sound->Play();
				}
			}

			//-----------energy absorption-----------------

			if (power_compare(sc_pos,reenergy_pos[i],sc_size,reenergy_size) && reenergy_show[i] == 1 && !(power == 1 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
			{
				reenergy_show[i] = 0;
				reenergy = 1;//energy = 500;
				if(music != 0)
				{
					CIwSoundInst* SoundInstance = Fuel_sound->Play();
				}
			}

			if (power_compare(sc_pos,power_pos[i],sc_size,power_size) && power_show[i] == 1)
			{
				if (music != 0)
				{
					CIwSoundInst* SoundInstance = Power_sound->Play();
				}
				if(power == 1|| power == 2 || power == 3)
				{
					power_avail = power_time[power-1]+6;
					power_show[i] = 0;
					power_on_time = power_time[power-1];
					power_down = 0;
	
					if(power == 1)
					{
						t_speed = l_speed;
						l_speed = g_speed+15;
					}
				}
				else
				{
					switch (power)
					{
						case 4:
							megawarp++;
							break;
						case 5:
							if(lives < max_lives[sc_sel])
							{
								lives++;
							}
							break;
						case 6:
							star+=100;
							break;
						case 7:
							star+=500;
							break;
						case 8:
							star+=1000;
							break;
					}
					power_show[i] = 0;
				}
			}
		}

		for (int i = 0; i < 12; i++)
		{
			//------------------star absortion and calculation-------------------

			if(star_compare(sc_pos,bstar_pos[i],sc_size,bstar_size) && bstar_show[i] == 1)
			{
				bstar_show[i]=0;
				star++;
				if(music!=0)
				{
					CIwSoundInst* SoundInstance = Star_sound->Play();
				}
			}
		}

		//----------------sequence generator -------------------------

		if (bstar_pos[sequence*6].x <= 0)
		{
			sequence = sequence==0?1:0;
			switch (IwRandRange(12))
			{
				case 0: sequence_1(sequence);
					break;
				case 1: sequence_2(sequence);
					break;
				case 2: sequence_3(sequence);
					break;
				case 3: sequence_4(sequence);
					break;
				case 4: sequence_5(sequence);
					break;
				case 5: sequence_6(sequence);
					break;
				case 6: sequence_7(sequence);
					break;
				case 7: sequence_8(sequence);
					break;
				case 8: sequence_9(sequence);
					break;
				case 9: sequence_10(sequence);
					break;
				case 10: sequence_11(sequence);
					break;
				case 11: sequence_12(sequence);
					break;
			}

			for (int i = sequence*4; i < sequence*4+4; i++)
			{
				ast_s[i] = IwRandRange(3);
				ast_show[i] = 1;
			}
			for (int i = sequence*6; i < sequence*6+6; i++)
			{
				bstar_show[i] = 1;
			}
			reenergy_show[sequence] = 1;

			if(power_on_time != 0)
			{
				power_on_time--;
				if(power_on_time == 0 && (power == 1||power == 4))
				{
					l_speed = t_speed;
					ini_play_page();
				}
			}

			if(power_avail == 0)
			{
				int x = IwRandRange(100)+1;
				power = x%50==0?8:(x%25==0?7:(x%12==0?5:(x%7==0?4:(x%5==0?6:(x%4==0?3:IwRandRange(3)+1)))));
				power_show[sequence] = 1;
				power_avail = 6;
			}
			else
			{
				power_avail--;
				power_show[sequence] = 0;
			}
		}
		
		//----------------------energy decrease control---------------

		if(!(power == 1 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
		{
			if(reenergy == 1)
			{
				energy += (int)(l_speed*2.0f);
				if(energy >= 500)
				{
					reenergy = 0;
				}
			}
			else
			{
				energy -= (int)(l_speed*0.3f);
			}
		}

		//--------------comet animation---------------------

		if(com_i == 2)
		{
			com_x += 128;
			if(com_x == 1280)
			{
				com_x = 0;
			}
			com_i = 0;
		}
		else
		{
			com_i++;
		}
	}
	else if (blast == 1)
	{
		//---------blast animation--------------------------

		b_x+=100;
		if(b_x==1300)
		{
			if(b_y == 0)
			{
				b_y = 75;
				b_x = 0;
			}
			else
			{
				lives--;
				if (lives != 0)
				{
					blast = 0;
					ini_play_page();
				}
				else
				{
					power_on_time = 0;
					power = 0;
					blast = -1;
				}
			}
		}
	}
	else if(lives == 0 || energy <= 0)
	{
		//-----------------Game Over State controls--------------------

		if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN) || (s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED))
		{
			if (high_score < score)
			{
				high_score = score;
			}
			total_star += star;
			collected_star += star;
			blast = 0;
			delay = 30;
			page = 0;

			_store->score = high_score;
			_store->star = total_star;
			_store->m = music;
			_store->mw = megawarp;
			_store->sc = sc_sel;
			_store->cstar = collected_star;
			for (int i = 0; i < 6; i++)
			{
				_store->sc_l[i] = sc_locked[i];
			}
			for (int i = 0; i < 4; i++)
			{
				_store->pt[i] = power_time[i];
			}

			s3eSecureStoragePut(_store,sizeof(struct save));

			ini_main_page();
			ini_play_page();
		}
	}

	//-------------comet position------------------------

	for (int i = 0; i < 2; i++)
	{
		com_pos[i].x -= l_speed*3.5f;
		reenergy_pos[i].x -= l_speed*2.0f;
		if(power_show[i] == 1)
		{
			power_pos[i].x -= l_speed*2.0f;
		}
	}

	//---------------astroid position-------------------
	
	for (int i = 0; i < 8; i++)
	{
		ast_pos[i].x -= l_speed*2.0f;
	}

	//----------------star position---------------------
	
	for (int i = 0; i < 12; i++)
	{
		if((power == 2 && power_on_time != 0) && (bstar_pos[i].x-sc_pos.x < Iw2DGetSurfaceWidth()*0.50f && bstar_pos[i].x-sc_pos.x > -Iw2DGetSurfaceWidth()*0.50f))
		{
			if((int)((bstar_pos[i].y+bstar_size.y/2)-(sc_pos.y+sc_size.y/2)) < -10)
			{
				bstar_pos[i].y += l_speed*2.0f;
			}
			else if((int)((bstar_pos[i].y+bstar_size.y/2)-(sc_pos.y+sc_size.y/2)) > 10)
			{
				bstar_pos[i].y -= l_speed*2.0f;
			}
		}
		bstar_pos[i].x -= l_speed*2.0f;
	}

	//-------------astroid animation---------------------------
	if(ast_i == 1)
	{
		ast_x += 75;
		if(ast_x == 1500)
		{
			ast_x = 0;
		}
		ast_i = 0;
	}
	else
	{
		ast_i++;
	}

	//----------------star animation----------------------------
	
	bstar_x+=50;
	if(bstar_x == 1000)
	{
		if(bstar_y == 0)
		{
			bstar_y = 50;
		}
		else
		{
			bstar_y = 0;
		}
		bstar_x = 0;
	}
}

void gamePlay::draw_play_page()
{
	if(blast == 0 && energy > 0)
	{
		Iw2DDrawImage(getresource->get_spacecraft(sc_sel),sc_pos,sc_size);
		Iw2DDrawImage(getresource->get_exhaust(),exhaust_pos,exhaust_size[exhaust_sel]);

		if(!(power == 1 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
		{
			for (int i = 0; i < 2; i++)
			{
				if (com_pos[i].x >= -com_size.x && com_pos[i].x <= Iw2DGetSurfaceWidth())
				{
					Iw2DDrawImageRegion(getresource->get_comet(),com_pos[i],com_size,CIwFVec2((float)com_x,0),CIwFVec2(128,50));
				}
			}
		}
	}
	else if(blast == 1)
	{
		Iw2DDrawImageRegion(getresource->get_explosion(),sc_pos,CIwFVec2(sc_size.y*1.33f,sc_size.y),CIwFVec2((float)b_x,(float)b_y),CIwFVec2(100,75));
	}

	if(!(power == 1 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
	{
		for (int i = 0; i < 8; i++)
		{
			if(ast_pos[i].x >= -ast_size.x && ast_pos[i].x <= Iw2DGetSurfaceWidth())
			{
				if(ast_show[i] == 1)
				{
					switch (ast_s[i])
					{
						case 0:
							Iw2DDrawImageRegion(getresource->get_astroid(0),ast_pos[i],ast_size,CIwFVec2((float)ast_x,0),CIwFVec2(75,80));
							break;
						case 1:
							Iw2DDrawImageRegion(getresource->get_astroid(1),ast_pos[i],ast_size,CIwFVec2((float)ast_x,0),CIwFVec2(75,75));
							break;
						case 2:
							Iw2DDrawImageRegion(getresource->get_astroid(2),ast_pos[i],ast_size,CIwFVec2((float)ast_x,0),CIwFVec2(75,68));
							break;
					}
				}	
			}	
		}
	}
	for (int i = 0; i < 12; i++)
	{
		if(bstar_pos[i].x >= - bstar_size.x && bstar_pos[i].x <= Iw2DGetSurfaceWidth())
		{
			if (bstar_show[i] == 1)
			{
				Iw2DDrawImageRegion(getresource->get_bonus_star(),bstar_pos[i],bstar_size,CIwFVec2((float)bstar_x,(float)bstar_y),CIwFVec2(50,50));
			}
		}
	}

	Iw2DDrawImageRegion(getresource->get_bonus_star(),CIwFVec2(Iw2DGetSurfaceWidth()*0.005f,Iw2DGetSurfaceHeight()*0.005f),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f,Iw2DGetSurfaceHeight()*0.05f),CIwFVec2((float)bstar_x,(float)bstar_y),CIwFVec2(50,50));

	sprintf(str,"%.0f",star);
	Iw2DDrawString(str,star_pos,star_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);
	
	sprintf(str,"Score : %.0f",score);
	Iw2DDrawString(str,score_pos,score_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

	Iw2DDrawImage(getresource->get_spacecraft(0),CIwFVec2(Iw2DGetSurfaceWidth()*0.5f-Iw2DGetSurfaceHeight()*0.05f*0.99f,Iw2DGetSurfaceHeight()*0.005f),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f*0.99f,Iw2DGetSurfaceHeight()*0.05f));
	sprintf(str,"X %.0f",lives);
	Iw2DDrawString(str,lives_pos,lives_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

	if(mega_active == 1)
	{
		Iw2DSetColour(0xffffffff);
	}
	else
	{
		Iw2DSetColour(0x77ffffff);
	}
	Iw2DDrawImageRegion(getresource->get_power(),CIwFVec2(0,Iw2DGetSurfaceHeight()*0.90f),CIwFVec2(Iw2DGetSurfaceHeight()*0.10f,Iw2DGetSurfaceHeight()*0.10f),CIwFVec2(150,0),CIwFVec2(50,50));
	Iw2DSetColour(0xffffffff);
	sprintf(str,"X %d",megawarp);
	Iw2DDrawString(str,megawarp_pos,megawarp_size,IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_CENTRE);

	Iw2DDrawImage(getresource->get_energy(),CIwFVec2(Iw2DGetSurfaceWidth()*0.005f,Iw2DGetSurfaceHeight()*0.05f+2*(Iw2DGetSurfaceHeight()*0.005f)),CIwFVec2(Iw2DGetSurfaceHeight()*0.05f,Iw2DGetSurfaceHeight()*0.05f));
	if (energy < 150)
	{
		Iw2DSetColour(0xff0000ff);
	}
	else if(energy < 300)
	{
		Iw2DSetColour(0xff00ffff);
	}
	else
	{
		Iw2DSetColour(0xff00ff00);
	}

	Iw2DFillRect(energy_pos,CIwFVec2(Iw2DGetSurfaceWidth()*0.0004f*energy,energy_size.y));
	Iw2DSetColour(0xffffffff);
	Iw2DDrawRect(energy_pos,energy_size);

	if(!(power == 1 && power_on_time!=0) && !(power == 4 && power_on_time!=0))
	{
		for (int i = 0; i < 2; i++)
		{
			if (reenergy_pos[i].x >= -reenergy_size.x && reenergy_pos[i].x <= Iw2DGetSurfaceWidth())
			{
				if (reenergy_show[i] == 1)
				{
					Iw2DDrawImage(getresource->get_energy(),reenergy_pos[i],reenergy_size);
				}
			}
	
			if (power_pos[i].x >= -power_size.x && power_pos[i].x <= Iw2DGetSurfaceWidth())
			{
				if(power_show[i] == 1)
				{
					switch (power)
					{
						case 1:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(0,0),CIwFVec2(50,50));
							break;
						case 2:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(50,0),CIwFVec2(50,50));
							break;
						case 3:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(100,0),CIwFVec2(50,50));
							break;
						case 4:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(150,0),CIwFVec2(50,50));
							break;
						case 5:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(0,50),CIwFVec2(50,50));
							break;
						case 6:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(50,50),CIwFVec2(50,50));
							break;
						case 7:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(100,50),CIwFVec2(50,50));
							break;
						case 8:
							Iw2DDrawImageRegion(getresource->get_power(),power_pos[i],power_size,CIwFVec2(150,50),CIwFVec2(50,50));
							break;
					}
				}
			}
		}
	}
	if(power_on_time != 0)
	{
		if (power_on_time <= 1)
		{
			if(power_down == 0)
			{
				power_down++;
				Iw2DSetColour(0xffffffff);
			}
			else
			{
				power_down = 0;
				Iw2DSetColour(0x00ffffff);
			}
		}
		switch (power)
		{
			case 1:
				Iw2DDrawImageRegion(getresource->get_powerglow(),powerglow_pos,powerglow_size,CIwFVec2(0,0),CIwFVec2(300,300));
				break;
			case 2:
				Iw2DDrawImageRegion(getresource->get_powerglow(),powerglow_pos,powerglow_size,CIwFVec2(300,0),CIwFVec2(300,300));
				break;
			case 3:
				Iw2DDrawImageRegion(getresource->get_powerglow(),powerglow_pos,powerglow_size,CIwFVec2(0,300),CIwFVec2(300,300));
				break;
			case 4:
				Iw2DDrawImageRegion(getresource->get_powerglow(),powerglow_pos,powerglow_size,CIwFVec2(300,300),CIwFVec2(300,300));
				break;
		}
	}

	if(lives == 0 || energy <= 0)
	{
		if(score>high_score)
		{
			sprintf(str,"GAME OVER\n\nCongratulations New Highscore : %.0f",score);
		}
		else
		{
			sprintf(str,"GAME OVER\n\nScore : %.0f",score);
		}
		
		Iw2DDrawString(str,CIwFVec2(0,Iw2DGetSurfaceHeight()*0.5f-font_size/2),CIwFVec2((float)Iw2DGetSurfaceWidth(),font_size),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
	}
}

bool gamePlay::compare(CIwFVec2 var_a, CIwFVec2 var_b, CIwFVec2 var_a_size, CIwFVec2 var_b_size)
{
	float a = var_a.x+var_a_size.x*0.20f,
		b = var_a.x+var_a_size.x-var_a_size.x*0.20f,
		c = var_a.y+var_a_size.y*0.20f,
		d = var_a.y+var_a_size.y-var_a_size.y*0.20f;
	float p = var_b.x+var_b_size.x*0.20f,
		q = var_b.x+var_b_size.x-var_b_size.x*0.20f,
		r = var_b.y+var_b_size.y*0.20f,
		s = var_b.y+var_b_size.y-var_b_size.y*0.20f;

	if((a<=p && b>=p)||(a<=q && b>=q)||(a>=p && b<=q))
	{
		if((c<=r && d>=r)||(c<=s && d>=s)||(c>=r && d<=s))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool gamePlay::com_compare(CIwFVec2 var_a, CIwFVec2 var_b, CIwFVec2 var_a_size, CIwFVec2 var_b_size)
{
	float a = var_a.x+var_a_size.x*0.20f,
		b = var_a.x+var_a_size.x-var_a_size.x*0.20f,
		c = var_a.y+var_a_size.y*0.20f,
		d = var_a.y+var_a_size.y-var_a_size.y*0.20f;
	float p = var_b.x+var_b_size.x*0.20f,
		q = var_b.x+var_b_size.x-var_b_size.x*0.70f,
		r = var_b.y+var_b_size.y*0.20f,
		s = var_b.y+var_b_size.y-var_b_size.y*0.20f;

	if((a<=p && b>=p)||(a<=q && b>=q)||(a>=p && b<=q))
	{
		if((c<=r && d>=r)||(c<=s && d>=s)||(c>=r && d<=s))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool gamePlay::star_compare(CIwFVec2 var_a, CIwFVec2 var_b, CIwFVec2 var_a_size, CIwFVec2 var_b_size)
{
	float a = var_a.x+var_a_size.x*0.20f,
		b = var_a.x+var_a_size.x-var_a_size.x*0.20f,
		c = var_a.y+var_a_size.y*0.20f,
		d = var_a.y+var_a_size.y-var_a_size.y*0.20f;
	float p = var_b.x+var_b_size.x,
		q = var_b.x+var_b_size.x-var_b_size.x,
		r = var_b.y+var_b_size.y,
		s = var_b.y+var_b_size.y-var_b_size.y;

	if((a<=p && b>=p)||(a<=q && b>=q)||(a>=p && b<=q))
	{
		if((c<=r && d>=r)||(c<=s && d>=s)||(c>=r && d<=s))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool gamePlay::power_compare(CIwFVec2 var_a, CIwFVec2 var_b, CIwFVec2 var_a_size, CIwFVec2 var_b_size)
{
	float a = var_a.x+var_a_size.x*0.20f,
		b = var_a.x+var_a_size.x-var_a_size.x*0.20f,
		c = var_a.y+var_a_size.y*0.20f,
		d = var_a.y+var_a_size.y-var_a_size.y*0.20f;
	float p = var_b.x+var_b_size.x*0.20f,
		q = var_b.x+var_b_size.x-var_b_size.x*0.20f,
		r = var_b.y+var_b_size.y*0.20f,
		s = var_b.y+var_b_size.y-var_b_size.y*0.20f;

	if((a<=p && b>=p)||(a<=q && b>=q)||(a>=p && b<=q))
	{
		if((c<=r && d>=r)||(c<=s && d>=s)||(c>=r && d<=s))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void gamePlay::sequence_1(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;

	bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;

	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_2(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_3(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-power_size.y/2;
}

void gamePlay::sequence_4(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-power_size.y/2;
}

void gamePlay::sequence_5(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_6(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_7(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+4].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+3].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-power_size.y/2;
}

void gamePlay::sequence_8(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-power_size.y/2;
}

void gamePlay::sequence_9(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_10(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::sequence_11(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	reenergy_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	
	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-power_size.y/2;
}

void gamePlay::sequence_12(int x)
{
	bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	power_pos[x].x = ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = reenergy_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
	reenergy_pos[x].y = Iw2DGetSurfaceHeight()*0.50f-reenergy_size.y/2;

	power_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-power_size.y/2;
}

void gamePlay::set_page()
{
	page = -1;
}

gamePlay *newgame = 0;