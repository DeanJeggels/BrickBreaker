/*
 * DrawScore.h
 *
 *  Created on: 01 Nov 2021
 *      Author: Dean Jeggels
 */

#ifndef INC_DRAWSCORE_H_
#define INC_DRAWSCORE_H_
#define Screen_Start (unsigned char*)0x20020000

extern void ClearSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT );
extern void DrawSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT);

int SCORE_START_X = 235;
int SCORE_START_y = 5;
int SCORE_WIDTH = 7;
int SCORE_HEIGHT = 7;
int score[3];

void DrawScore(int score[],int points)
{

	score[0] = (points/100)%10;
	score[1] = (points/10)%10;
    score[2] = (points%10);

    ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
    		ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
    		ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
for(int i = 0; i<3;i++)
{

	if (score[i] == 0)
		{


	if(i == 0)
	{
		 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_0,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

	}
	else if(i == 1)
	{
		ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_0,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

	}
	else if(i == 2)
	{
		ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_0,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	}
		}

	else if (score[i] == 1)
	{


if(i == 0)
{
	 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
		DrawSprite(Screen_Start,char_1,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

}
else if(i == 1)
{
	ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
		DrawSprite(Screen_Start,char_1,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

}
else if(i == 2)
{
	ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
		DrawSprite(Screen_Start,char_1,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
}

	}
	else if(score[i] == 2)
	{



		if(i == 0)
		{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				DrawSprite(Screen_Start,char_2,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

		}
		else if(i == 1)
		{
			ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				DrawSprite(Screen_Start,char_2,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

		}
		else if(i == 2)
		{
			ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				DrawSprite(Screen_Start,char_2,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
		}
	}

	else if(score[i] == 3)
		{
		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_3,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_3,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_3,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 4)
		{

		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_4,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_4,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_4,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 5)
		{

		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_5,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_5,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_5,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 6)
		{

		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_6,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_6,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_6,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 7)
		{

		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_7,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_7,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_7,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 8)
		{

		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_8,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_8,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_8,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}
	else if(score[i] == 9)
		{
		if(i == 0)
				{
			 ClearSprite(Screen_Start,char_def,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_9,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 1)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_9,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

				}
				else if(i == 2)
				{
					ClearSprite(Screen_Start,char_def,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
						DrawSprite(Screen_Start,char_9,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
				}
		}



}
}


#endif /* INC_DRAWSCORE_H_ */
