#include "PID.h"

struct pid_unit pid_Motor1;
struct pid_unit pid_Motor2;
struct pid_unit pid_Motor3;
struct pid_unit pid_Motor4;

void PID_Init(void)
{
//乘以1024原因避免出现浮点数运算，全部是整数运算，这样PID控制器运算速度会更快
/***********************左前轮速度pid****************************/
	pid_Motor1.Kp = 1024 * 0.5;//0.4
 	pid_Motor1.Ki = 1024 * 0;	
	pid_Motor1.Kd = 1024 * 0.08; 
	pid_Motor1.Ur = 1024 * 4000;
	pid_Motor1.Adjust   = 0;
	pid_Motor1.En       = 1;
	pid_Motor1.speedSet = 0;
	pid_Motor1.speedNow = 0;
	reset_Uk(&pid_Motor1);		
/***********************左后轮速度pid****************************/
	pid_Motor2.Kp = 1024 * 0.35;//0.2
 	pid_Motor2.Ki = 1024 * 0;	//不使用积分
	pid_Motor2.Kd = 1024 * 0.06; 
	pid_Motor2.Ur = 1024 * 4000;
	pid_Motor2.Adjust   = 0;
	pid_Motor2.En       = 1;
	pid_Motor2.speedSet = 0;
	pid_Motor2.speedNow = 0;
	reset_Uk(&pid_Motor2);
//右后轮速度pid
	pid_Motor3.Kp = 1024 * 0.5;//0.4
 	pid_Motor3.Ki = 1024 * 0;	
	pid_Motor3.Kd = 1024 * 0.08; 
	pid_Motor3.Ur = 1024 * 4000;
	pid_Motor3.Adjust   = 0;
	pid_Motor3.En       = 1;
	pid_Motor3.speedSet = 0;
	pid_Motor3.speedNow = 0;
	reset_Uk(&pid_Motor3);		
/**********************右前轮速度pid****************************/
	pid_Motor4.Kp = 1024 * 0.35;//0.2
 	pid_Motor4.Ki = 1024 * 0;	//不使用积分
	pid_Motor4.Kd = 1024 * 0.06; 
	pid_Motor4.Ur = 1024 * 4000;
	pid_Motor4.Adjust   = 0;
	pid_Motor4.En       = 1;
	pid_Motor4.speedSet = 0;
	pid_Motor4.speedNow = 0;
	reset_Uk(&pid_Motor4);
}

s32 PID_common(int set,int jiance,struct pid_unit *p)
{
	int ek=0,U_k=0;

	ek=jiance - set;                                                               
	
	U_k=p->U_kk + p->Kp*(ek - p->ekk) + p->Ki*ek + p->Kd*(ek - 2*p->ekk + p->ekkk);
	
	p->U_kk=U_k;
    p->ekkk=p->ekk;
	p->ekk=ek;
	
	if(U_k>(p->Ur))		                                    
		U_k=p->Ur;
	if(U_k<-(p->Ur))
		U_k=-(p->Ur);
	
	return U_k>>10; 
}

void Pid_Which(struct pid_unit *p1, struct pid_unit *p2, struct pid_unit *p3, struct pid_unit *p4)
{
//左前轮pid
	if(p1->En == 1)
	{									
		p1->Adjust = -PID_common(p1->speedSet, p1->speedNow, p1);		
	}	
	else
	{
		p1->Adjust = 0;
		reset_Uk(p1);
		p1->En = 2; 
	}
//左后轮pid
	if(p2->En == 1)
	{
		p2->Adjust = -PID_common(p2->speedSet, p2->speedNow, p2);		
	}	
	else
	{
		p2->Adjust = 0;
		reset_Uk(p2);
		p2->En = 2; 
	}
	//右后轮pid
	if(p3->En == 1)
	{									
		p3->Adjust = -PID_common(p3->speedSet, p3->speedNow, p3);		
	}	
	else
	{
		p3->Adjust = 0;
		reset_Uk(p3);
		p3->En = 2; 
	}
	//右前轮pid
	if(p4->En == 1)
	{
		p4->Adjust = -PID_common(p4->speedSet, p4->speedNow, p4);		
	}	
	else
	{
		p4->Adjust = 0;
		reset_Uk(p4);
		p4->En = 2; 
	}
}

/*******************************************************************************
 * 函数名：Pid_Ctrl(int *leftMotor,int  *rightMotor)
 * 描述  ：Pid控制
 *******************************************************************************/

void Pid_Ctrl(int *Motor1,int  *Motor2, int *Motor3,int  *Motor4)
{
	Pid_Which(&pid_Motor1, &pid_Motor2, &pid_Motor3, &pid_Motor4);
	*Motor1  += pid_Motor1.Adjust;
	*Motor2  += pid_Motor2.Adjust;
	*Motor3  += pid_Motor3.Adjust;
	*Motor4  += pid_Motor4.Adjust;
}

void reset_Uk(struct pid_unit *p)
{
	p->U_kk=0;
	p->ekk=0;
	p->ekkk=0;
}
