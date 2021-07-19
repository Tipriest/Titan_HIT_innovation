#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

struct pid_unit
{
	s32 U_kk;    	    //��һ�ε������
	s32 ekk;		 	//��һ�ε�����ƫ��
	s32 ekkk;			//ǰһ�ε�����ƫ��
	s32 Ur;				//�޷����ֵ,���ʼ��
	s32 Kp;				//����
	s32 Ki;				//����
	s32 Kd;				//΢��
	
	u8  En;             //����
	s16 Adjust;         //������
	s16 speedSet;       //�ٶ�����
	s16 speedNow;       //��ǰ�ٶ�
};
/****************************��Ӻ���***************************/

extern struct pid_unit pid_Motor1;
extern struct pid_unit pid_Motor2;
extern struct pid_unit pid_Motor3;
extern struct pid_unit pid_Motor4;

void  PID_Init(void);
void  reset_Uk(struct pid_unit *p);
s32   PID_common(int set,int jiance,struct pid_unit *p);
void Pid_Ctrl(int *Motor1,int  *Motor2, int *Motor3,int  *Motor4);

#endif //__PID_H__
