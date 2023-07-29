/*
 * DrawLives.h
 *
 *  Created on: Nov 2, 2021
 *      Author: dean Jeggels
 */

#ifndef INC_DRAWLIVES_H_
#define INC_DRAWLIVES_H_

extern void ClearSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT );
extern void DrawSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT);

int lives[3];
int LIVES_START_X = 6;



void DrawLives(int lives[],int live)
{

	lives[0] = (live/100)%10;
		lives[1] = (live/10)%10;
	    lives[2] = (live%10);

	    ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	    		ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	    		ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	for(int i = 0; i<3;i++)
	{
		if (lives[i] == 0)
				{


			if(i == 0)
			{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_0,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

			}
			else if(i == 1)
			{
				ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_0,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

			}
			else if(i == 2)
			{
				ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_0,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			}
				}
			else if (lives[i] == 1)
		{


	if(i == 0)
	{
		 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_1,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

	}
	else if(i == 1)
	{
		ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_1,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

	}
	else if(i == 2)
	{
		ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			DrawSprite(Screen_Start,char_1,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	}

		}
		else if(lives[i] == 2)
		{



			if(i == 0)
			{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_2,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

			}
			else if(i == 1)
			{
				ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_2,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

			}
			else if(i == 2)
			{
				ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_2,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
			}
		}

		else if(lives[i] == 3)
			{
			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_3,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_3,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_3,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 4)
			{

			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_4,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_4,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_4,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 5)
			{

			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_5,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_5,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_5,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 6)
			{

			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_6,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_6,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_6,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 7)
			{

			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_7,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_7,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_7,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 8)
			{

			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_8,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_8,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_8,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}
		else if(lives[i] == 9)
			{
			if(i == 0)
					{
				 ClearSprite(Screen_Start,char_def,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_9,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 1)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_9,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

					}
					else if(i == 2)
					{
						ClearSprite(Screen_Start,char_def,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
							DrawSprite(Screen_Start,char_9,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
					}
			}


				}
	}

#endif /* INC_DRAWLIVES_H_ */
