/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "update.h"
#include "sprite.h"
#include "audio.h"
#include "DrawScore.h"
#include "fatfs.h"
#include "DrawLives.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Screen_Start (unsigned char*)0x20020000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
//file variables
FRESULT fres;
FATFS fs;
UINT numread;
UINT numwritten;
FIL fil;

uint16_t filecontents[3];
uint8_t ans;
uint8_t output[17] = "\nHigh Score: 000\n";




//gameflag variables
int screenrefreshcounter = 0;
int pos = 0;
int place = 0;
int gamestate = 1;
int displayscore =1;
int power_up = 0;
int randomnumber;
int powerupactivepaddle = 0;
int powerupactivelives = 0;
int live = 3;



//brick variables
int BRICK_START_Y = 30;
int topy, bottomy, leftx, rightx;
int BRICK_START_X = 6;
int BRICK_WIDTH = 20;
int BRICK_HEIGHT = 8;
int  brick_hit = 0;
int tempby;
int tempty;
int templx;
int temprx;
int greyflag =0;



//paddle variables
int paddleflag;
int PADDLE_START_X = 144;
int PADDLE_START_Y = 187;
int PADDLE_WIDTH = 32;
int PADDLE_HEIGHT = 8;
int PADDLE_SPEED = 8;
int WIDEPADDLE_WIDTH = 48;
int WIDEPADDLE_HEIGHT = 8;


//ball variables
int BALL_START_X = 157;
int BALL_START_Y = 	186;
int Ball_Height = 4;
int Ball_Width = 4;
int angle = 95;
int speed = 10;


//interrupt variables
extern  uint8_t screenFlag;
extern  uint8_t button_0;
extern  uint8_t button_1;
extern  uint8_t button_2;
extern  uint8_t button_3;
extern  uint8_t pressed;



//mouse variables
uint8_t i2cdata[10];
int16_t ax=0;

//adc variables
uint16_t tempx = 2042;
int decreasing = 0;
int increasing = 0;

//score variables
int greenpoint=0;
int yellowpoint = 0;
int orangepoint = 0;
int redpoint = 0;
int greypoint = 0;
int points = 0;
int fail = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */


//function declarations
volatile void ClearSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT );
volatile void DrawSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT);
int abs(int x);

//ball struct
struct ball_state{
	   uint16_t x;
	   uint16_t y;
	   uint8_t width;
	   uint8_t height;
	   int16_t mov_direction;
	   uint16_t speed;
	   int speeddirectiony;
	   int speeddirectionx;
};

//paddle struct
 struct Paddle_state{
	    int x;
		int y;
		int PADDLE_WIDTH;
		int PADDLE_HEIGHT;
		int lives;
		int right;
		int left;
};

//powerups struct
struct power_ups
{
	int increasepaddle;
	int lifeup;


};

// brick struct
typedef struct {

	 uint16_t rightx;
     uint16_t leftx;
     uint16_t bottomy;
         uint16_t topy;
         uint16_t hitbox;


}Bricks;



///// FUNCTIONS




void Collisiondetection(struct ball_state* ball_state,struct Paddle_state* paddle_state, Bricks* bricks)
{


	int tempX = ball_state->x + deltaX(ball_state->speed, ball_state->mov_direction);
	int	tempY = ball_state->y + deltaY(ball_state->speed, ball_state->mov_direction);


	int balltop = tempY;
	int ballbottom =  tempY + ball_state->height;
	int ballleft = tempX;
	int ballright = tempX + ball_state->width;

		if(ballleft<0)//left side of screen
		{

			ball_state->mov_direction = 180 - ball_state->mov_direction; //bounce right
			ball_state->x = -tempX;
			ball_state->speeddirectionx = speed;
		}
		else if(ballright > 320)//right side of screen
		{

			ball_state->mov_direction = 180 - ball_state->mov_direction; //bounce left
			ball_state->x = tempX - 2*((tempX+ball_state->width)-320);
			ball_state->speeddirectionx = -1*speed;
		}
		else
		{
			ball_state->x = tempX;
		}

			if(balltop<0)//top of screen
			{

				ball_state->mov_direction = 360 - ball_state->mov_direction; //bounce down
				ball_state->y = -tempY;
				ball_state->speeddirectiony = speed;
			}

			//bounce of of paddle

			else if(((ballbottom) >= paddle_state->y) && (ballleft <= paddle_state->x+(paddle_state->PADDLE_WIDTH)) && (ballright >= paddle_state->x))
				{
					if((paddle_state->right) && (ball_state->mov_direction < 90)&& (ball_state->mov_direction >= 0)) // ball moving right and paddle moving right
					{
						ball_state->speeddirectiony = -1*speed;
									   ball_state->mov_direction = 375 - ball_state->mov_direction; //update
									   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
									   paddle_state->right = 0;
					}
					else if((paddle_state->left)&& (ball_state->mov_direction <= 180)&& (ball_state->mov_direction > 90)) // ball moving left and paddle moving left
					{
						ball_state->speeddirectiony = -1*speed;
									   ball_state->mov_direction = 345 - ball_state->mov_direction; //update
									   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
									   paddle_state->left = 0;

					}
					else if((paddle_state->right) && (ball_state->mov_direction <= 180)&& (ball_state->mov_direction > 90)) // ball moving left and paddle moving right
								  {
									ball_state->speeddirectiony = -1*speed;
												   ball_state->mov_direction = 375 - ball_state->mov_direction; //update
												   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
												   paddle_state->right = 0;
								  }
								  else if((paddle_state->left)&& (ball_state->mov_direction < 90)&& (ball_state->mov_direction >= 0)) // ball moving right and paddle moving left
								{
									ball_state->speeddirectiony = -1*speed;
												   ball_state->mov_direction = 345 - ball_state->mov_direction; //update
												   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
												   paddle_state->left = 0;

								}
								  else if((paddle_state->left)&& (ball_state->mov_direction == 90)) // ball moving straight  and paddle moving left
																{
																	ball_state->speeddirectiony = -1*speed;
																				   ball_state->mov_direction = 345 - ball_state->mov_direction; //update
																				   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
																				   paddle_state->left = 0;

																}
								  else if((paddle_state->right)&& (ball_state->mov_direction == 90)) // ball moving straight  and paddle moving right
																								{
																									ball_state->speeddirectiony = -1*speed;
																												   ball_state->mov_direction = 375 - ball_state->mov_direction; //update
																												   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
																												   paddle_state->left = 0;

																								}



				else
				{
				   ball_state->speeddirectiony = -1*speed;
				   ball_state->mov_direction = 360 - ball_state->mov_direction; //update
				   ball_state->y = tempY - 2*((tempY+ball_state->height)-paddle_state->y);
				   paddle_state->left = 0;
				   paddle_state->right = 0;
				}

				}


			else if(ballbottom >= 200)
			{

				fail = 1;


				 paddle_state->lives--;
									    live--;
						                button_0 = 0;
										pressed = 0;
						                ball_state->speed = 0;
						                ball_state->y = BALL_START_Y;
						                ball_state->mov_direction = angle;


			}
			else
  		{
  			ball_state->y = tempY;
  		}


			// check collision for all bricks
			for(pos = 0; pos<65; pos++)
				{



					if ((balltop <= bricks[pos].bottomy) && ((ballbottom) >= bricks[pos].topy) && ((ballright)>= bricks[pos].leftx) &&(ballleft<= bricks[pos].rightx) ) //check for collision
					{
                        place = pos;


                        power_up =1;



                   	 if(ball_state ->mov_direction > 360)
                   	 {
                   		 ball_state ->mov_direction -= 360;
                   	 }
                   	 else if(ball_state ->mov_direction < -360)
                   	 {
                   		 ball_state ->mov_direction += 360;
                   	 }




						//set score flags


						if(pos >= 52 && pos <= 64) //green brick is hit
						{
							 brick_hit = 1;
												greenpoint = 1;

											}

						else if(pos >= 39 && pos <= 51) //yellow brick is hit
																	{
							 brick_hit = 1;
																		yellowpoint = 1;

																	}
											else if(pos >= 26 && pos <= 38) //orange brick is hit
																	{
												 brick_hit = 1;
																		orangepoint = 1;

																	}
											else if(pos >= 13 && pos <= 25) //red brick is hit
																	{
												 brick_hit = 1;
												                  	redpoint = 1;

																	}
											else if(pos >=0 && pos <=12)
											{

												if(bricks[pos].hitbox != 1)
												{
												greyflag = 1;
												}
						                                    ClearSprite(Screen_Start, sprite, bricks[pos].leftx, bricks[pos].topy, BRICK_WIDTH, BRICK_HEIGHT);

															DrawSprite(Screen_Start, crackedGreyBrick, bricks[pos].leftx, bricks[pos].topy, BRICK_WIDTH, BRICK_HEIGHT);

															if(bricks[pos].hitbox ==1)
															{

																 brick_hit = 1;


															greypoint = 1;


															}

														}


						ball_state->mov_direction = 360 - ball_state->mov_direction; //bounce down
						ball_state->y = (tempY - 2*((tempY)-bricks[pos].bottomy));


					tempby = bricks[pos].bottomy;
					tempty = bricks[pos].topy;
					templx = bricks[pos].leftx;
					temprx = bricks[pos].rightx;


					}

				}




if(pressed == 0)
{
		ball_state->x = paddle_state->x + (PADDLE_WIDTH/2);

}

}

void PowerUpCheck(struct power_ups* power)
{

	if(power_up)
	{

  randomnumber = ((rand() %200)+1); // create random number for powerup from



    if(randomnumber > 60 && randomnumber < 90 ) //lives up power up
    {
    	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*) poweruplives, AUDIOLENpoweruplives);
   	 power->lifeup = 1;

    }

    else if(randomnumber > 150 && randomnumber < 180 && !powerupactivepaddle) // paddlesize up power up
    {
    	powerupactivepaddle = 1;
    	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*) poweruppaddle, AUDIOLENpoweruppaddle);
   	 power->increasepaddle = 1;

    }

    else
    {
    	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*) audiohit, AUDIOLENhit);
    }
    power_up= 0;


}
}




void PowerUp(struct Paddle_state* paddle,struct power_ups* power)
{

	if(power->increasepaddle)
	{

		paddle->PADDLE_WIDTH = WIDEPADDLE_WIDTH;
		paddleflag=1;
		power->increasepaddle = 0;
	}
	else if(power->lifeup)
	{
		live++;
		paddle->lives++;
		power->lifeup = 0;


	}

}


void UpdatePaddle(struct Paddle_state* Paddle_state)
{


	if(button_1)
	{

		uint32_t JoyStick = GPIOD -> IDR;
	JoyStick = JoyStick & 0x600;

	  if(JoyStick == 0x200)
			  {
				  if(Paddle_state ->x == 320 - Paddle_state->PADDLE_WIDTH)
				  {
					  Paddle_state ->x = 320 -  Paddle_state->PADDLE_WIDTH;
				  }
				  else
				  {
					  Paddle_state->right = 1;
					  Paddle_state ->x += PADDLE_SPEED;

					  screenFlag = 0;
				  }


			  }else if(JoyStick == 0x400)

			  {
				  if(Paddle_state ->x == 0)
				  {
					  Paddle_state ->x = 0;
				  }
				  else
				  {
					  Paddle_state->left=1;
					  Paddle_state ->x -= PADDLE_SPEED;

					  screenFlag = 0;
				  }


			  }
			  else
			  {

				  Paddle_state->left = 0;
				  Paddle_state->right = 0;

			  }

	}

	if(button_2)
	{
		i2cdata[0] = 0xA8;

		HAL_I2C_Master_Transmit(&hi2c1, 0x32, i2cdata,1, 10);
		 HAL_I2C_Master_Receive(&hi2c1, 0x32, i2cdata, 6, 10);

		 uint16_t tmp = i2cdata[0] + (i2cdata[1] << 8);
		 ax = *((int16_t*)&tmp);

		 if(ax > 0)
					  {


						  if(Paddle_state ->x == 320 -  Paddle_state->PADDLE_WIDTH)
						  {
							  Paddle_state ->x = 320 -  Paddle_state->PADDLE_WIDTH;
						  }
						  else
						  {
							  Paddle_state->right = 1;
							  Paddle_state ->x += PADDLE_SPEED;

							  screenFlag = 0;
						  }


					  }else if(ax < 0 )

					  {


						  if(Paddle_state ->x == 0)
						  {
							  Paddle_state ->x = 0;
						  }
						  else
						  {
							  Paddle_state->left = 1;
							  Paddle_state ->x -= PADDLE_SPEED;

							  screenFlag = 0;
						  }

					  }
					  else
					  {
						  Paddle_state->left = 0;
						  Paddle_state->right = 0;
					  }

	}
	if(button_3)
	{

		 uint16_t x = HAL_ADC_GetValue(&hadc1);
		 if ((x < tempx) || (x == 0))
		 {
			 decreasing = 1;

		 }
		 else if ((x > tempx)||(x >= 4084))
		 {

			 increasing = 1;

		 }
		 tempx = x;

		 if (decreasing)
		 {


			  if(Paddle_state ->x <= 0)
			  {
				  Paddle_state ->x = 0;
			  }
			  else
			  {
				  Paddle_state->left = 1;
				  Paddle_state ->x -= PADDLE_SPEED;

				  screenFlag = 0;
			  }

decreasing = 0;

		 }


		 else if(increasing)
		 {

			  if(Paddle_state ->x == 320 -  Paddle_state->PADDLE_WIDTH)
							  {
				  Paddle_state ->x = 320 -  Paddle_state->PADDLE_WIDTH;
							  }
							  else
							  {
								  Paddle_state->right = 1;
								  Paddle_state ->x += PADDLE_SPEED;

								  screenFlag = 0;
							  }

increasing = 0;

		 }
		 else
					  {

						  Paddle_state->left = 0;
						  Paddle_state->right = 0;

					  }
	}
}





void updateScore()
{

	if(greenpoint)
	{

		points++; // +1 for green brick
		greenpoint = 0;
	}
	else if(yellowpoint)
		{
         points += 3;// +3 for yellow brick
         yellowpoint = 0;
		}
	else if(orangepoint)
		{
        points += 5;// +5 for orange brick
        orangepoint = 0;
		}
	else if(redpoint)
		{
        points += 7;// +7 for red brick
        redpoint = 0;
		}
	else if(greypoint)
		{
        points += 10;// +10 for grey brick
        greypoint = 0;
		}

}

void GameOver(struct Paddle_state* paddle_state)
{

				int middlex = 130;
				int middley = 100;




				    DrawSprite(Screen_Start,char_G,middlex, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_A,middlex+9, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_M,middlex+18, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_E,middlex+27, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_O,middlex+45, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_V,middlex+54, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_E,middlex+63, middley,SCORE_WIDTH,SCORE_HEIGHT);
					DrawSprite(Screen_Start,char_R,middlex+72, middley,SCORE_WIDTH,SCORE_HEIGHT);





}


void Fail(struct Paddle_state* paddle_state)
{

	if(fail)
	{



		ClearSprite(Screen_Start, widePaddle, paddle_state->x, paddle_state->y, paddle_state->PADDLE_WIDTH, paddle_state->PADDLE_HEIGHT);

		paddleflag = 0;
		powerupactivepaddle = 0;
		paddle_state->PADDLE_WIDTH = PADDLE_WIDTH;

		if(paddle_state->lives == 0)
					{
				gamestate = 0;
					}

		HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*) audiofail2, AUDIOLENfail2);
		fail = 0;

	}
}

void IncScrCnt()
{
if(brick_hit)
{
	screenrefreshcounter++;
}


}

void BrickAnimation(Bricks* bricks)
{

	if(brick_hit)
	{
	if(place >= 52 && place <= 64) //green brick is hit
						{
		                                 bricks[place].bottomy = -69;
										bricks[place].topy = -69;
										bricks[place].leftx =-69;
										bricks[place].rightx = -69;
		                                                            if (screenrefreshcounter==0){
		                                                                   ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

																			DrawSprite(Screen_Start,  Brickanimation1G, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
		                                                                 }
																			if (screenrefreshcounter==2) {
																				ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

																				DrawSprite(Screen_Start,  Brickanimation2G, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
																			}

																			else if (screenrefreshcounter==4) {
																				ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

																				DrawSprite(Screen_Start,  Brickanimation3G,templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
																			}

																			else if (screenrefreshcounter==6) {
																				ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
																				screenrefreshcounter = 0;
																				brick_hit =0;
																			}



						}

	else if(place >= 39 && place <= 51) //yellow brick is hit
												{
		                                bricks[place].bottomy = -69;
										bricks[place].topy = -69;
										bricks[place].leftx =-69;
										bricks[place].rightx = -69;
        if (screenrefreshcounter==0){
                             ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

								DrawSprite(Screen_Start,  Brickanimation1Y, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
}
								if (screenrefreshcounter==2) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation2Y, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==4) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation3Y,templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==6) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
									screenrefreshcounter = 0;
									brick_hit =0;
								}
												}
	else if(place >= 26 && place <= 38) //orange brick is hit
												{
		                                bricks[place].bottomy = -69;
										bricks[place].topy = -69;
										bricks[place].leftx =-69;
										bricks[place].rightx = -69;
        if (screenrefreshcounter==0){
                             ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

								DrawSprite(Screen_Start,  Brickanimation1O, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
}
								if (screenrefreshcounter==2) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation2O, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==4) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation3O,templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==6) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
									screenrefreshcounter = 0;
									brick_hit =0;
								}
												}
	else if(place >= 13 && place <= 25) //red brick is hit
												{
		                               bricks[place].bottomy = -69;
										bricks[place].topy = -69;
										bricks[place].leftx =-69;
										bricks[place].rightx = -69;
        if (screenrefreshcounter==0){
                             ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

								DrawSprite(Screen_Start,  Brickanimation1R, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
}
								if (screenrefreshcounter==2) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation2R, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==4) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation3R,templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==6) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
									screenrefreshcounter = 0;
									brick_hit =0;

								}
												}


	else if(place >= 0 && place <= 12) // grey brick is hit
	{
		if((bricks[place].hitbox==1) && (brick_hit == 1) )
		{
			                                bricks[place].bottomy = -69;
											bricks[place].topy = -69;
											bricks[place].leftx =-69;
											bricks[place].rightx = -69;
        if (screenrefreshcounter==0){
                             ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

								DrawSprite(Screen_Start,  Brickanimation1GR, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
}
								if (screenrefreshcounter==2) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation2GR, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==4) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);

									DrawSprite(Screen_Start,  Brickanimation3GR,templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
								}

								else if (screenrefreshcounter==6) {
									ClearSprite(Screen_Start, sprite, templx, tempty, BRICK_WIDTH, BRICK_HEIGHT);
									screenrefreshcounter = 0;
									brick_hit =0;


								}

}


	}

}
	if(greyflag)
	{
	bricks[place].hitbox++;
	greyflag = 0;
	}
}




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


	uint8_t Paddle[256] = {0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x10, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x10, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
	                       0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x00, 0x00,
	                       0x00, 0x0B, 0x0C, 0x0F, 0x0F, 0x0F, 0x0F, 0x10, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x10, 0x0F, 0x0F, 0x0F, 0x0F, 0x06, 0x0B, 0x00,
	                       0x0B, 0x0F, 0x0F, 0x0C, 0x0C, 0x0C, 0x0C, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x0F, 0x0B,
	                       0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x0B, 0x0B,
	                       0x00, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x10, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x0B, 0x00,
	                       0x00, 0x00, 0x0C, 0x06, 0x06, 0x06, 0x06, 0x10, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x10, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00,
	                       0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,};

	uint8_t Ball[16] = {0x00, 0x0F, 0x0F, 0x00,
	 0x0F, 0x0B, 0x0B, 0x0F,
	 0x0F, 0x0B, 0x0B, 0x0F,
	 0x00, 0x0F, 0x0F, 0x00};



  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */


  DrawSprite(Screen_Start, titlescreen, 0, 0, 320, 200);
  HAL_Delay(50);
  ClearSprite(Screen_Start, titlescreen, 0, 0, 320, 200);

  // start adc continious conversion mode
  HAL_ADC_Start(&hadc1);

  //draw bricks
  for(int i = 0; i < 5; i++)
  {
	  BRICK_START_X = 6;
  	for(int j = 0; j <13; j++ )
  		{
  			for(int k = 0; k< sizeof(sprite); k++)
  			{
  				sprite[k] = color[i];
  			}
  			ClearSprite(Screen_Start, sprite, BRICK_START_X, BRICK_START_Y, BRICK_WIDTH, BRICK_HEIGHT);
  			DrawSprite(Screen_Start, sprite, BRICK_START_X, BRICK_START_Y, BRICK_WIDTH, BRICK_HEIGHT);
  			BRICK_START_X += 24;
  		}
  	BRICK_START_Y += 12;
  }

  BRICK_START_Y = 30;
  BRICK_START_X = 6;


// initialize bricks variables
  Bricks bricks[65];

topy = BRICK_START_Y;
bottomy = BRICK_START_Y + BRICK_HEIGHT;
rightx = BRICK_START_X + BRICK_WIDTH;
leftx = BRICK_START_X;
int position = 0;

for( int i = 0; i< 5; i++)
{
 rightx = BRICK_START_X + BRICK_WIDTH;
 leftx = BRICK_START_X;

	for(int j = 0;j<13;j++)
	{
	bricks[position].topy = topy;
	bricks[position].bottomy = bottomy;
	bricks[position].rightx = rightx;
	bricks[position].leftx = leftx;

	rightx += 24;
	leftx += 24;
	position++;

	}

	topy += 12;
	bottomy += 12;
}





//initiliaze balll and paddle variables
  struct ball_state current_ball_state = {BALL_START_X, BALL_START_Y, Ball_Width, Ball_Height, angle,0,0,0};
  struct Paddle_state current_paddle_state = {PADDLE_START_X, PADDLE_START_Y, PADDLE_WIDTH, PADDLE_HEIGHT,3,0,0};
  struct power_ups power_up = { 0,0};


  // draw paddle in starting position
  DrawSprite(Screen_Start, Paddle, current_paddle_state.x, current_paddle_state.y, current_paddle_state.PADDLE_WIDTH, current_paddle_state.PADDLE_HEIGHT);

  // set data registers for mouse input detection for x velocity
  i2cdata[0] = 0x20;
  i2cdata[1] = 0x47;
  HAL_I2C_Master_Transmit(&hi2c1, 0x32, i2cdata, 2, 10);

  i2cdata[0] = 0x23;
    i2cdata[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, 0x32, i2cdata, 2, 10);


	DrawSprite(Screen_Start,char_S,SCORE_START_X, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_C,SCORE_START_X+9, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_O,SCORE_START_X+18, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_R,SCORE_START_X+27, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_E,SCORE_START_X+36, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_Colon,SCORE_START_X+45, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,SCORE_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,SCORE_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,SCORE_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);

	DrawSprite(Screen_Start,char_L,LIVES_START_X, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_I,LIVES_START_X+9, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_V,LIVES_START_X+18, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_E,LIVES_START_X+27, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_S,LIVES_START_X+36, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_Colon,LIVES_START_X+45, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,LIVES_START_X+54, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,LIVES_START_X+63, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);
	DrawSprite(Screen_Start,char_0,LIVES_START_X+72, SCORE_START_y,SCORE_WIDTH,SCORE_HEIGHT);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {

	  if (screenFlag)
	  {


 //clear paddle
		  if(gamestate)
		  {
			if (!paddleflag) {
				ClearSprite(Screen_Start, Paddle, current_paddle_state.x, current_paddle_state.y, current_paddle_state.PADDLE_WIDTH, current_paddle_state.PADDLE_HEIGHT);
				//update paddle according to input
				UpdatePaddle(&current_paddle_state);
				// draw new paddle
				DrawSprite(Screen_Start, Paddle, current_paddle_state.x, current_paddle_state.y, current_paddle_state.PADDLE_WIDTH, current_paddle_state.PADDLE_HEIGHT);
			}

			if (paddleflag) {
							ClearSprite(Screen_Start, widePaddle, current_paddle_state.x, current_paddle_state.y, current_paddle_state.PADDLE_WIDTH, current_paddle_state.PADDLE_HEIGHT);
							//update paddle according to input
							UpdatePaddle(&current_paddle_state);
							// draw new paddle
							DrawSprite(Screen_Start, widePaddle, current_paddle_state.x, current_paddle_state.y, current_paddle_state.PADDLE_WIDTH, current_paddle_state.PADDLE_HEIGHT);
						}


			      //clear ball

		       ClearSprite(Screen_Start,Ball,current_ball_state.x, current_ball_state.y, current_ball_state.width, current_ball_state.height);

		       //detect collisions between ball and objects

		// launch ball
		       if(button_0)
		       {

                   HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t*) audiolaunch, AUDIOLENlaunch);
		    	   current_ball_state.speed = 10;
		    	   current_ball_state.speeddirectiony = -1*speed;
		    	   current_ball_state.speeddirectionx = -1*speed;

		    	   button_0 = 0;
		       }

			      Collisiondetection(&current_ball_state, &current_paddle_state, bricks);

			      PowerUpCheck(&power_up);
			      PowerUp(&current_paddle_state, &power_up);

			      BrickAnimation(bricks);

			      Fail(&current_paddle_state);

			  updateScore();


			  DrawLives(lives,live);
			  DrawScore(score,points);


// draw new ball after collision

			      DrawSprite(Screen_Start, Ball,current_ball_state.x, current_ball_state.y, current_ball_state.width, current_ball_state.height);

			      IncScrCnt();

		 	   screenFlag = 0;



		  }

		  if(gamestate == 0)
				 			  {
			  //HighScore();

			  GameOver(&current_paddle_state);


				 			  }
	  }






    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Macro to configure the PLL multiplication factor 
  */
  __HAL_RCC_PLL_PLLM_CONFIG(16);
  /** Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 16;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_8K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */
void DrawSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT)
{
	unsigned char* scr = ptr_screen;		 // Pointer pointing to screen addresses
		  unsigned char* spr = ptr_sprite;		 // Pointer pointing to the sprite

		  scr = scr + 320*y + x;

		  for(int i = 0; i < SPRITE_HEIGHT; i++)		             // Amount of rows
		 	 {
		 		 for(int j = 0; j < SPRITE_WIDTH; j++)	             // Amount of columns
		 	 	 	 {
		 	           if(*spr != 0)
		 	           {
		 	              *scr = *spr;
		 	           }
		 	           scr++;					     // Incrementing screen pointer position
		 	           spr++;					     // Incrementing sprite pointer position
		 	         }
		 	 scr = scr + (320 -SPRITE_WIDTH);
		 	 }
}

void ClearSprite(unsigned char* ptr_screen, unsigned char* ptr_sprite, int x, int y, int SPRITE_WIDTH, int SPRITE_HEIGHT )
{
	  unsigned char* scr = ptr_screen;		 // Pointer pointing to screen addresses
	  unsigned char* spr = ptr_sprite;		 // Pointer pointing to the sprite

	 scr = scr + 320*y + x;        // Getting the offset of where the position is

	 for(int i = 0; i < SPRITE_HEIGHT; i++)		             // Amount of rows
	 {
		 for(int j = 0; j < SPRITE_WIDTH; j++)	             // Amount of columns
	 	 	 {
	           if(*spr != 0)
	           {
	              *scr = 0;					 // setting to 0 to clear the paddle
	           }
	           scr++;					     // Incrementing screen pointer position
	           spr++;					     // Incrementing sprite pointer position
	         }
	 scr = scr + (320 - SPRITE_WIDTH);
	 }
}

int abs(int x)
{

	if(x<0)
	{
		return -1*x;

	}

	else{
		return x;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
