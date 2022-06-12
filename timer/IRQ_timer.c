/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "stdio.h"
#include "../Ball.h"
#include "../Paddle.h"
#include "../ADC/adc.h"
#include <stdlib.h>

#define PADDLE_FREQ_NOTE 1062
#define	WALL_FREQ_NOTE   1125
#define PLAYER_1				 1
#define PLAYER_2				 2
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] =
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

extern ball bl;
extern paddle pad_down;
extern paddle pad_up;
extern int play;
extern int score_1;
extern int score_2;

unsigned border_right_x = 234;
unsigned int border_left_x = 5;
unsigned int border_top_y = 5;

extern char string_score_2[2];
extern char string_score_1[2];

void emit_sound(uint32_t TimerInterval) {
	init_timer(1, TimerInterval);
			enable_timer(1);
}

void update_score(int player){
	if(player == 1) {
		score_1++;
		sprintf(string_score_1, "%d", score_1);
	}
	else {
		score_2++;
		sprintf(string_score_2, "%d", score_2);
	}
	
	if(score_1 == 5) {
		GUI_Text(7, MIDDLE_Y, (uint8_t *) "You Win", White, Black);		
		GUI_Text(169, MIDDLE_Y, (uint8_t *) "You Lose", White, Black);	
		play = 0;
	}
	else if(score_2 == 5) {
		GUI_Text(7, MIDDLE_Y, (uint8_t *) "You Lose", White, Black);		
		GUI_Text(169, MIDDLE_Y, (uint8_t *) "You Win", White, Black);
		play = 0;
	}
	else {
		GUI_Text(7, MIDDLE_Y, (uint8_t *) string_score_1, White, Black);	
		GUI_Text(225, MIDDLE_Y, (uint8_t *) string_score_2, White, Black);	
	}	
}

void TIMER0_IRQHandler (void)
{		
	static int position_pad_2 = 30;
	static int direction_pad_2 = 1;
	
	if(bl.y + bl.h  == pad_down.y - pad_down.h - 1) {
		//change direction
		if(collision(pad_down, bl)){
			emit_sound(PADDLE_FREQ_NOTE);			
			bl = change_ball_direction_paddle_hit(pad_down, bl);
		}
		else {
			//update score player 2
			update_score(2);
		}
	}
	else if(bl.y - bl.h == pad_up.y + pad_up.h + 1) {
		if(collision(pad_up, bl)){
			emit_sound(PADDLE_FREQ_NOTE);			
			bl = change_ball_direction_paddle_hit(pad_up, bl);
		}
		else {
			//update score player 1
			update_score(1);
		}
	}
	else if(bl.x - bl.w == border_left_x) {
		emit_sound(WALL_FREQ_NOTE);
		bl = change_ball_direction_wall(bl, left_right);
	}
	else if(bl.x + bl.w == border_right_x) {
		emit_sound(WALL_FREQ_NOTE);
		bl = change_ball_direction_wall(bl, left_right);
	}

	bl = move_ball(bl, play);	

	if(position_pad_2 == 209) {
		direction_pad_2 = -1;
	}
	else if(position_pad_2 == 30) {
		direction_pad_2 = 1;
	}
	position_pad_2 += direction_pad_2;
	pad_up = move_paddle(pad_up, position_pad_2);
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = SinTable[ticks]<<6;
	ticks++;
	if(ticks==45) {
		ticks = 0;
		LPC_DAC->DACR = 0;
		disable_timer(1);
		reset_timer(1);
		GUI_Text(225, MIDDLE_Y, (uint8_t *) string_score_2, Black, Black);		
		GUI_Text(7, MIDDLE_Y, (uint8_t *) string_score_1, Black, Black);
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
