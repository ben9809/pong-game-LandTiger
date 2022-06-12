#include <stdint.h>
#include "GLCD/GLCD.h"
#include "Paddle.h"


typedef struct {
	int x, y;
	int w, h;
	int dx, dy;
	int collision_area_up;
	int collision_area_down;
}ball;

enum border {
	left_right,
	top,
};

ball init_ball(int x, int y, int w, int h, int dx, int dy);
void GUI_ball( ball bl);
ball move_ball(ball bl, int play);
ball change_ball_direction_wall(ball bl, enum border bord);
int collision(paddle pad, ball bl);
ball change_ball_direction_paddle_hit(paddle pad, ball bl);
void GUI_ball_hide(ball bl);
