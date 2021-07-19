#include "Speed.h"

void control(u8 velocity_x, u8 velocity_y, u8 velocity_angle)
{
	u8 trans_x = velocity_x * ratio_x;
	u8 trans_y = velocity_y * ratio_y;
	u8 rotate = (car_length + car_width) * velocity_angle;
	
	pid_Motor1.speedSet=trans_x - trans_y - rotate;
	pid_Motor2.speedSet=trans_x - trans_y + rotate;
	pid_Motor3.speedSet=trans_x + trans_y + rotate;
	pid_Motor4.speedSet=trans_x + trans_y - rotate;
}
