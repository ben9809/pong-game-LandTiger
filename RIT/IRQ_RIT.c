/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../Ball.h"
#include "../timer/timer.h"
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int down = 0;
unsigned int play = 0;
ball bl;
int score_1 = 0;
int score_2 = 0;
char string_score_2[2];
char string_score_1[2];
paddle pad_down;
paddle pad_up;

void RIT_IRQHandler (void)
{						
	if(down>1){ 
		if((NVIC_GetEnableIRQ(EINT1_IRQn) == 0) && ((LPC_GPIO2->FIOPIN & (1<<11)) == 0)){	/* KEY1 pressed */
			switch(down){				
				case 2:
				if(!play){
					play = 1;
					bl = init_ball(231, 160, 2, 2, 1 , 1);
					pad_down = init_paddle(30, 284, 25, 5);
					pad_up = init_paddle(120, 33, 25, 5);
					GUI_Text(60, 160, (uint8_t *) "Key 1 To Play", Black, Black);
					GUI_ball(bl);
					sprintf(string_score_1, "%d", score_1);
					sprintf(string_score_2, "%d", score_2);
					GUI_paddle(pad_down);
					GUI_paddle(pad_up);
					enable_timer(0);
				}
					break;
				default:
					break;
			}
			down++;
		}
		else if((NVIC_GetEnableIRQ(EINT0_IRQn) == 0) && ((LPC_GPIO2->FIOPIN & (1<<10)) == 0)) {  /* INT0 pressed */
			switch(down){				
				case 2:
				play = 0;
				reset_timer(0);
				GUI_Text(7, 160, (uint8_t *) "        ", Black, Black);		
				GUI_Text(169, 160, (uint8_t *) "        ", Black, Black);
				GUI_paddle_hide(pad_down);
				GUI_paddle_hide(pad_up);
				GUI_Text(60, 160, (uint8_t *) "Key 1 To Play", White, Black);
				score_1 = 0;
				score_2 = 0;
					break;
				default:
					break;
			}
			down++;
		}
		else if((NVIC_GetEnableIRQ(EINT2_IRQn) == 0) && ((LPC_GPIO2->FIOPIN & (1<<12)) == 0)){	/* KEY2 pressed */
			switch(down){				
				case 2:
				if(play){
					play = 0;
					disable_timer(0);
					GUI_ball_hide(bl);
					GUI_paddle_hide(pad_down);
					GUI_paddle_hide(pad_up);
					GUI_Text(60, 160, (uint8_t *) "Key 2 To Resume", White, Black);					
				}
				else {
					GUI_Text(60, 160, (uint8_t *) "Key 2 To Resume", Black, Black);	
					GUI_ball(bl);
					GUI_paddle(pad_down);
					GUI_paddle(pad_up);
					enable_timer(0);
					play = 1;
				}
					break;
				default:
					break;
			}
			down++;
		}
		else {	/* button released */
			down=0;
			if(NVIC_GetEnableIRQ(EINT0_IRQn) == 0) {
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4    |= (1 << 20);
			}
			else if(NVIC_GetEnableIRQ(EINT1_IRQn) == 0) {
				NVIC_EnableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4    |= (1 << 22); 
			}
			else if(NVIC_GetEnableIRQ(EINT2_IRQn) == 0) {
				NVIC_EnableIRQ(EINT2_IRQn);
				LPC_PINCON->PINSEL4    |= (1 << 24); 
			}		
		}
	}
	else{
			if(down==1)
				down++;
	}
	
	/* ADC management */
	if(play) {
		ADC_start_conversion();
	}
 	
				
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
