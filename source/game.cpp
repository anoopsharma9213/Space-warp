#include "game.h"

void gamePlay::Update()
{
	if(s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_PRESSED)
	{
		s3eDeviceRequestQuit();
	}

	update_Environment();

	if(page == 0)
	{
		update_main_page();
	}
	else if(page == 1)
	{
		update_play_page();
	}
}

void gamePlay::Render()
{
	Iw2DSurfaceClear(0xff000000);
	Iw2DSetColour(0xffffffff);

	IwRandSeed((int)s3eTimerGetMs());

	draw_Environment();

	if(page == 0)
	{
		draw_main_page();
	}
	else if(page == 1)
	{
		draw_play_page();
	}
		
	Iw2DSurfaceShow();
}

gamePlay::gamePlay()
{
	page = 0;
	resume = 0;

	if(Iw2DGetSurfaceHeight()>=1080)
	{
		g_speed = 9;
	}
	else if(Iw2DGetSurfaceHeight()>=720)
	{
		g_speed = 6;
	}
	else if (Iw2DGetSurfaceHeight()>=400)
	{
		g_speed = 4;
	}
	else
	{
		g_speed = 3;
	}

	Iw2DSetFont(getresource->get_font());

	IwRandSeed((int)s3eTimerGetMs());
	ini_Environment();
	ini_main_page();
	ini_play_page();
}

gamePlay::~gamePlay()
{
	
}

//--------------------------------ENVIRONMENT-----------------------------------------------

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
				
		pos_star[i].x-=g_speed;
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
		pos_dot[i].x -= g_speed*0.85f;
		if(pos_dot[i].x < 0)
		{
			pos_dot[i].x = (float)Iw2DGetSurfaceWidth();
			pos_dot[i].y = (float)IwRandRange(Iw2DGetSurfaceHeight());
		}
	}

	for (int i = 0; i < 5; i++)
	{
		p_pos[i].x -= g_speed;
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

	cp_pos.x -= g_speed;
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
		Iw2DDrawImage(getresource->get_bg_star(),CIwFVec2(pos_star[i].x-size_star[i]/2,pos_star[i].y-size_star[i]/2),CIwFVec2((float)size_star[i],(float)size_star[i]));
	}

	for (int i = 0; i < 50; i++)
	{
		Iw2DDrawRect(pos_dot[i],CIwFVec2(1,1));
	}
	
	for (int i = 0; i < 5; i++)
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
	Iw2DSetColour(0xffffffff);
	
}

//---------------------------------------------MAIN PAGE--------------------------------------------

void gamePlay::ini_main_page()
{
	sc_size.y = Iw2DGetSurfaceHeight()*0.20f;
	sc_size.x = sc_size.y*0.99f;

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

	sc_pos.x = Iw2DGetSurfaceWidth()*0.5f - sc_size.x*0.5f;
	sc_pos.y = Iw2DGetSurfaceHeight()*0.5f - sc_size.y*0.5f;

	exhaust_sel = 0;
	exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
	exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;

	trans = 0;
}

void gamePlay::update_main_page()
{
	if(trans == 1)
	{
		sc_pos.x -= g_speed;
		
		if(sc_pos.x <= Iw2DGetSurfaceWidth()*0.04f)
		{
			trans = 0;
			page = 1;
		}
	}
	else
	{
		if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		{
			if((s3ePointerGetX()>=sc_pos.x && s3ePointerGetX()<=sc_pos.x+sc_size.x) && (s3ePointerGetY()>=sc_pos.y && s3ePointerGetY()<=sc_pos.y+sc_size.y))
			{
				trans = 1;
			}
		}
	}

	exhaust_sel++;
	if(exhaust_sel==3)
	{
		exhaust_sel = 0;
	}
	exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
	exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;
}

void gamePlay::draw_main_page()
{
	Iw2DDrawImage(getresource->get_spacecraft(1),sc_pos,sc_size);
	Iw2DDrawImage(getresource->get_exhaust(),exhaust_pos,exhaust_size[exhaust_sel]);
	
	if(trans == 0)
	{
		Iw2DDrawString("Tap to Start",CIwFVec2(Iw2DGetSurfaceWidth()*0.40f,Iw2DGetSurfaceHeight()*0.75f),CIwFVec2(Iw2DGetSurfaceWidth()*0.30f,30),IW_2D_FONT_ALIGN_CENTRE,IW_2D_FONT_ALIGN_CENTRE);
	}
}

//----------------------------------------------PLAY PAGE-------------------------------------------

void gamePlay::ini_play_page()
{
	for (int i = 0; i < 12; i++)
	{
		bstar_pos[i] = CIwFVec2(-1000,-1000);
	}
	for (int i = 0; i < 8; i++)
	{
		ast_pos[i] = CIwFVec2(-1000,-1000);
		ast_s[i] = IwRandRange(3);
	}
	for (int i = 0; i < 2; i++)
	{
		com_pos[i] = CIwFVec2(-1000,-1000);
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
}

void gamePlay::update_play_page()
{
	if (blast == 0)
	{
		if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
		{
			if(s3ePointerGetY() < Iw2DGetSurfaceHeight()*0.5f)
			{
				if(sc_pos.y > Iw2DGetSurfaceHeight()*0.25f-sc_size.y/2)
				{
					sc_pos.y-=g_speed*3.0f;
				}
			}
			else
			{
				if(sc_pos.y < Iw2DGetSurfaceHeight()*0.75f-sc_size.y/2)
				{
					sc_pos.y+=g_speed*3.0f;
				}
			}
		}
		
		exhaust_sel++;
		if(exhaust_sel==3)
		{
			exhaust_sel = 0;
		}
		exhaust_pos.x = sc_pos.x - exhaust_size[exhaust_sel].x;
		exhaust_pos.y = sc_pos.y + sc_size.y/2 -exhaust_size[exhaust_sel].y/2;
		
		for (int i = 0; i < 2; i++)
		{
			com_pos[i].x -= g_speed*3.5f;
		}
		
		for (int i = 0; i < 8; i++)
		{
			ast_pos[i].x -= g_speed*2.0f;
		}

		for (int i = 0; i < 12; i++)
		{
			bstar_pos[i].x -= g_speed*2.0f;
		}

		for (int i = 0; i < 8; i++)
		{
			if (compare(sc_pos,ast_pos[i],sc_size,ast_size))
			{
				blast=1;
				b_x = 0;
				b_y = 0;
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (com_compare(sc_pos,com_pos[i],sc_size,com_size))
			{
				blast=1;
				b_x = 0;
				b_y = 0;
			}
		}

		if (bstar_pos[sequence*6].x <= 0)
		{
			//sequence==0?sequence_2(1):sequence_1(0);
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
			}
		}
	}
}

void gamePlay::draw_play_page()
{
	if(blast ==0 )
	{
		Iw2DDrawImage(getresource->get_spacecraft(0),sc_pos,sc_size);
		Iw2DDrawImage(getresource->get_exhaust(),exhaust_pos,exhaust_size[exhaust_sel]);

		//Iw2DDrawImage(getresource->get_comet(),com_pos,com_size);
		for (int i = 0; i < 2; i++)
		{
			Iw2DDrawImageRegion(getresource->get_comet(),com_pos[i],com_size,CIwFVec2((float)com_x,0),CIwFVec2(128,50));
		}
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
	else if(blast == 1)
	{
		Iw2DDrawImageRegion(getresource->get_explosion(),sc_pos,CIwFVec2(sc_size.y*1.33f,sc_size.y),CIwFVec2((float)b_x,(float)b_y),CIwFVec2(100,75));
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
				blast = 0;
				ini_play_page();
			}
		}
	}

	for (int i = 0; i < 8; i++)
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

	for (int i = 0; i < 12; i++)
	{
		Iw2DDrawImageRegion(getresource->get_bonus_star(),bstar_pos[i],bstar_size,CIwFVec2((float)bstar_x,(float)bstar_y),CIwFVec2(50,50));
	}
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
		q = var_b.x+var_b_size.x-var_b_size.x*0.60f,
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
	bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;

	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+3].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;

	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_2(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
}

void gamePlay::sequence_3(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+3].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
}

void gamePlay::sequence_4(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_5(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_6(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+4].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_7(int x)
{
	bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_8(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x+1].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+4].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+3].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
}

void gamePlay::sequence_9(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+2].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_10(int x)
{
	bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	com_pos[x].x = ast_pos[4*x].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+2].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+1].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+5].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	ast_pos[4*x+2].y = Iw2DGetSurfaceHeight()*0.75f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

void gamePlay::sequence_11(int x)
{
	ast_pos[4*x].x = bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+3].x = bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+5].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.50f-bstar_size.y/2;
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.25f-ast_size.y/2;
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x+1].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;
	
	com_pos[x].y = Iw2DGetSurfaceHeight()*0.25f-com_size.y/2;
}

void gamePlay::sequence_12(int x)
{
	bstar_pos[6*x].x = (float)Iw2DGetSurfaceWidth();
	ast_pos[4*x].x = bstar_pos[6*x+1].x = bstar_pos[6*x].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+1].x = bstar_pos[6*x+2].x = bstar_pos[6*x+1].x + Iw2DGetSurfaceWidth()/6;
	ast_pos[4*x+2].x = bstar_pos[6*x+3].x = bstar_pos[6*x+2].x + Iw2DGetSurfaceWidth()/6;
	com_pos[x].x = ast_pos[4*x+3].x = bstar_pos[6*x+4].x = bstar_pos[6*x+3].x + Iw2DGetSurfaceWidth()/6;
	bstar_pos[6*x+5].x = bstar_pos[6*x+4].x + Iw2DGetSurfaceWidth()/6;
	
	bstar_pos[6*x+4].y = bstar_pos[6*x+3].y = bstar_pos[6*x+2].y = bstar_pos[6*x+1].y = Iw2DGetSurfaceHeight()*0.25f-bstar_size.y/2;
	bstar_pos[6*x+5].y = bstar_pos[6*x].y = Iw2DGetSurfaceHeight()*0.75f-bstar_size.y/2;
	
	ast_pos[4*x+3].y = ast_pos[4*x+2].y = ast_pos[4*x+1].y = ast_pos[4*x].y = Iw2DGetSurfaceHeight()*0.50f-ast_size.y/2;

	com_pos[x].y = Iw2DGetSurfaceHeight()*0.75f-com_size.y/2;
}

gamePlay *newgame = 0;