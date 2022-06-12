#include "Ball.h"
#include <math.h>
#include "timer/timer.h"
#define PI 3.14159265

ball init_ball(int x, int y, int w, int h, int dx, int dy) {
	ball b;
	b.x = x;
	b.y = y;
	b.w = w;
	b.h = h;
	b.dx = dx;
	b.dy = dy;
	b.collision_area_up = 0;
	b.collision_area_down = 0;
	return b;
}

void GUI_ball( ball bl) {
	
	int i, j;
	for(i = bl.y - bl.h; i< bl.y + bl.h + 1; i++) {
		for( j = bl.x - bl.w; j < bl.x +bl.w + 1; j++) {
			LCD_SetPoint(j, i, Green);
		}
	}
}

ball move_ball(ball bl, int play){

	ball new_ball = bl;
	int i,j;
	
	if(bl.y - bl.h > MAX_Y - 1 || bl.y + bl.h < 0) {
		if(!play) {
			disable_timer(0);
			return bl;
		}
		GUI_Text(225, MIDDLE_Y, (uint8_t *) " ", Black, Black);		
		GUI_Text(7, MIDDLE_Y, (uint8_t *) " ", Black, Black);
		return init_ball(231, 160, 2, 2, -1 , 1);
	}
	//delete previous ball
	for(i = bl.y - bl.h; i< bl.y + bl.h + 1; i++) {
		for( j = bl.x - bl.w; j < bl.x + bl.w + 1; j++) {
			LCD_SetPoint(j, i, Black);
		}
	}
	
	if( bl.y + bl.dy - bl.h < 38 && !bl.collision_area_up ) {
		new_ball.collision_area_up = 1;
		new_ball.collision_area_down = 0;
		new_ball.y = 41; 
	}
	else if( bl.y + bl.dy + bl.h > 277 && !bl.collision_area_down) {
		new_ball.collision_area_down = 1;
		new_ball.collision_area_up = 0;
		new_ball.y = 276;
	}
	else {
		new_ball.y += bl.dy;
	}
	
	if(bl.x + bl.dx - bl.w < 5) {
		new_ball.x = 7;
	}
	else if(bl.x + bl.dx + bl.w > 234) {
		new_ball.x = 232;
	}
	else {
		new_ball.x += bl.dx;
	}
	
	for(i = new_ball.y - new_ball.h; i < new_ball.y + new_ball.h + 1 && i < 360 && i >= 0; i++) {
		for( j = new_ball.x - new_ball.w; j < new_ball.x + new_ball.w + 1 && j < 240 && j >= 0; j++) {
			LCD_SetPoint(j, i, Green);
		}
	}
	return new_ball;
}

ball change_ball_direction_wall(ball bl, enum border bord) {
	ball new_ball = bl;

	switch(bord) {
		case(left_right):			
			new_ball.dx = -bl.dx;
			break;
		case(top):
			new_ball.dy = -bl.dy;
		break;
		default:
			break;
	}
	return new_ball;
}

int collision(paddle pad, ball bl) {
	int coll = 1;
	if( (bl.x + bl.w < pad.x - pad.w) || (bl.x - bl.w > pad.x + pad.w) ) {
		coll = 0;
	}
	return coll;
}

ball change_ball_direction_paddle_hit(paddle pad, ball bl){
	
	float angle;
	float tan_angle;
	float angle_rad;
	int new_dx = 0;
	int new_dy = 0;
	int x_left_paddle = pad.x - pad.w; 
	ball new_ball;
	
	
	if(bl.x > pad.x + pad.w  || bl.x < pad.x - pad.w) {
		new_ball = change_ball_direction_wall(bl, top);
	}
		
	else {
		new_ball = bl;
		//50:90=bl.x:alfa
		angle = 90 * (bl.x - x_left_paddle) / 50;
		angle_rad = (angle * PI)/180;
		// non è l'angolo di riflessione, per trovare l'angolo di riflessione : 90 - angle
		tan_angle = tan(angle_rad);
		
		if( angle >= 45) {
			new_dx = 1;
			while( (float)new_dy < (float)(new_dx * tan_angle)) {
				new_dy++;
			}
		}
		else {
			new_dy = 1;
			while( (float)new_dy > (float)(new_dx * tan_angle)) {
				new_dx++;
			}
		}	
		if(new_dx == 0 || new_dy == 0) {
			new_ball = change_ball_direction_wall(bl, top);
			return new_ball;
		}
		
		if(bl.dy > 0) {
			if(bl.dx <= 0) {
			new_ball.dx = -new_dx;
			new_ball.dy = -new_dy;
			}
			else if( bl.dx >= 0) {
				new_ball.dx = new_dx;  
				new_ball.dy = -new_dy;
			}
		}
		else {
			if(bl.dx <=0) {
				new_ball.dx = -new_dx;
				new_ball.dy = new_dy;
			}
			else if( bl.dx >= 0) {
				new_ball.dx = new_dx;  
				new_ball.dy = new_dy;
			}
		}
		
	}
	return new_ball;
}

void GUI_ball_hide(ball bl) {
	
	int i,j;
	for(i = bl.y - bl.h; i< bl.y + bl.h + 1; i++) {
		for( j = bl.x - bl.w; j < bl.x +bl.w + 1; j++) {
			LCD_SetPoint(j, i, Black);
		}
	}
}

