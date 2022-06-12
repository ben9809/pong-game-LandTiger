#ifndef __PADDLE_H
#define __PADDLE_H

#include <stdint.h>
#include "GLCD/GLCD.h"

typedef struct {
	int x, y;
	int w, h;
} paddle;

paddle init_paddle(int x, int y, int w, int h);
paddle move_paddle(paddle pad, int new_x);
void GUI_paddle_hide(paddle pad);
void GUI_paddle(paddle pad);

#endif
