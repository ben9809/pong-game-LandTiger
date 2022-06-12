#include "Paddle.h"

paddle init_paddle(int x, int y, int w, int h) {
	paddle p;
	p.x = x; //120;
	p.y = y; //284;
	p.h = h;
	p.w = w;
	return p;
}

paddle move_paddle( paddle pad, int new_x) {
	int i, j;
	paddle new_pad = pad;
	if(new_x < pad.x + pad.w && new_x > pad.x) {
		for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = pad.x - pad.w; j < new_x - pad.w + 1; j++) {
				LCD_SetPoint(j, i, Black);
			}
			for(j = pad.x + pad.w; j < new_x + pad.w + 1; j++) {
				LCD_SetPoint(j, i, Green);
			}
		} 
	}
	else if( new_x > pad.x - pad.w && new_x < pad.x) {
		for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = pad.x + pad.w; j > new_x + pad.w - 1; j--) {
				LCD_SetPoint(j, i, Black);
			}
			for(j = pad.x - pad.w; j > new_x - pad.w - 1 ; j--) {
				LCD_SetPoint(j, i, Green);
			}
		} 
	}
	else if (new_x == pad.x) {
		return pad;
	}
	else {
		for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = pad.x - pad.w; j < pad.x + pad.w + 1; j++) {
				LCD_SetPoint(j, i, Black);
			}
		}
		for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = new_x - pad.w; j < new_x + pad.w + 1; j++) {
				LCD_SetPoint(j, i, Green);
			}
		}
	}
	new_pad.x = new_x;
	return new_pad;
}

void GUI_paddle_hide(paddle pad) {
	int i, j;
	for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = pad.x - pad.w; j < pad.x + pad.w + 1; j++) {
				LCD_SetPoint(j, i, Black);
			}
	}
}

void GUI_paddle(paddle pad) {
	int i, j;
	for( i = pad.y - pad.h; i < pad.y + pad.h + 1; i++) {
			for(j = pad.x - pad.w; j < pad.x + pad.w + 1; j++) {
				LCD_SetPoint(j, i, Green);
			}
	}
}

