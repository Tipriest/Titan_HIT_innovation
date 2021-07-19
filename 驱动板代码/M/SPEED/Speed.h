#ifndef __SPEED_H
#define __SPEED_H

#include "stm32f10x.h"
#include "PID.h"

#define car_length 200
#define car_width 200
#define ratio_x 1
#define ratio_y 1

void control(u8 velocity_x, u8 velocity_y, u8 velocity_angle);

#endif

