// CamEye.cpp: implementation of the CCamEye class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrtSys.h"
#include "CamEye.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamEye::CCamEye()
{

}

CCamEye::~CCamEye()
{

}

void CCamEye::InitCamera()					//初始化摄像机
{
	//摄像机（眼睛）的位置
	eyePos[0]	= 5.0f;		//x
	eyePos[1]	= 5.0f;	//y
	eyePos[2]	= 10.0f;	//z
	preeyePos_value[0] = 0.0f;//存储用于旋转的前角度
	preeyePos_value[1] = 0.0f;//存储用于旋转的前角度
	preeyePos_value[2] = 0.0f;//存储用于旋转的前角度

	//目标的位置
	target[0]	= 0.0f;		//x
	target[1]	= 0.0f;		//y
	target[2]	= 0.0f;		//z

	//存放摄像机到target的距离
	eyeRadius	=sqrt((eyePos[0]-target[0])*(eyePos[0]-target[0])
				+(eyePos[1]-target[1])*(eyePos[1]-target[1])
				+(eyePos[2]-target[2])*(eyePos[2]-target[2]));

	//摄像机的向上方向
	eyeUp[0]	= 0.0f;		//x
	eyeUp[1]	= 1.0f;		//y
	eyeUp[2]	= 0.0f;		//z

	//移动速度
	sliderspeed	= 0.1;
}





//设置摄像机的位置
void CCamEye::SetEyePos(int axis, int value)
{
	switch(axis)
	{
		case 0:
			preeyePos_value[0]-=value;
			if(preeyePos_value[0]>360)preeyePos_value[0]-=360;
			if(preeyePos_value[0]<-360)preeyePos_value[0]+=360;
			eyePos[0]=target[0]+eyeRadius*sin(preeyePos_value[0]/180.0*PAI);//value/10.0f;
			eyePos[2]=target[2]+eyeRadius*cos(preeyePos_value[0]/180.0*PAI);//value/10.0f;
			break;
		case 1:
			
			preeyePos_value[1]+=value;
			if(preeyePos_value[1]>360)preeyePos_value[1]-=360;
			if(preeyePos_value[1]<-360)preeyePos_value[1]+=360;
			eyePos[1]=target[1]+eyeRadius*sin(preeyePos_value[1]/180.0*PAI);//value/10.0f;
			eyePos[0]=target[0]+eyeRadius*sin(preeyePos_value[0]/180.0*PAI)*cos(preeyePos_value[1]/180.0*PAI);//value/10.0f;
			eyePos[2]=target[2]+eyeRadius*cos(preeyePos_value[0]/180.0*PAI)*cos(preeyePos_value[1]/180.0*PAI);//value/10.0f;
			break;
		case 2:
			break;
		default:break;
	}
}

//适用于由绝对坐标三点设置
void CCamEye::SetEyePos(float PX, float PY, float PZ)
{
	eyePos[0]=PX;
	eyePos[1]=PY;
	eyePos[2]=PZ;

	//更新到目标的距离
	eyeRadius	=sqrt((eyePos[0]-target[0])*(eyePos[0]-target[0])
				+(eyePos[1]-target[1])*(eyePos[1]-target[1])
				+(eyePos[2]-target[2])*(eyePos[2]-target[2]));

}

//适用于球坐标系设置，以Target为球心。，单位是度数0～360，不是弧度
void CCamEye::SetEyePosA(float Radius, float AngelY, float AngleXZ)
{
		eyeRadius=Radius;//更新半径

		//更新位置
		eyePos[0]=target[0]+eyeRadius*cos(AngleXZ/180.0*PAI)*sin(AngelY/180.0*PAI);
		eyePos[1]=target[1]+eyeRadius*cos(AngelY/180.0*PAI);
		eyePos[2]=target[2]+eyeRadius*sin(AngleXZ/180.0*PAI)*sin(AngelY/180.0*PAI);

}



//////////////////////////////////////////////////////

//设置摄像机目标位置
void CCamEye::SetTargetPos(int axis, int value)
{
	target[axis]+=value/100.0f;
}

void CCamEye::SetTargetPos(float PX, float PY, float PZ)
{
	target[0]=PX;
	target[1]=PY;
	target[2]=PZ;

	//更新到摄像机的距离
	eyeRadius	=sqrt((eyePos[0]-target[0])*(eyePos[0]-target[0])
				+(eyePos[1]-target[1])*(eyePos[1]-target[1])
				+(eyePos[2]-target[2])*(eyePos[2]-target[2]));
}



//设置摄像机向上方向
void CCamEye::SetEyeUp(int axis, int value)
{
	//暂时保留
}

//设置摄像机到目标的距离
void CCamEye::SetEyeRadius(int value)
{
	//直线的参数方程(L,M,N为方向向量)
	/*
	X=x0+Lt;
	Y=y0+Mt;
	Z=z0+Nt;
	*/
	//摄像机的位置
	eyePos[0]+=((eyePos[0]-target[0])/eyeRadius) *(value/15.0f);
	eyePos[1]+=((eyePos[1]-target[1])/eyeRadius) *(value/15.0f);
	eyePos[2]+=((eyePos[2]-target[2])/eyeRadius) *(value/15.0f);

	//到目标的距离
	eyeRadius	=sqrt((eyePos[0]-target[0])*(eyePos[0]-target[0])
				+(eyePos[1]-target[1])*(eyePos[1]-target[1])
				+(eyePos[2]-target[2])*(eyePos[2]-target[2]));
}

void CCamEye::SliderCamera(UINT nChar)
{
	//直线的参数方程(L,M,N为方向向量)
	/*
	X=x0+Lt;
	Y=y0+Mt;
	Z=z0+Nt;
	*/
	//L、M、N是摄目向量
	float L=(eyePos[0]-target[0])/eyeRadius;
	float M=(eyePos[1]-target[1])/eyeRadius;
	float N=(eyePos[2]-target[2])/eyeRadius;

	//Li,Mi,Ni垂直于 摄目向量与摄像机向上向量组成的平面
	float Lt=M*eyeUp[2]-N*eyeUp[1];
	float Mt=N*eyeUp[0]-L*eyeUp[2];
	float Nt=L*eyeUp[1]-M*eyeUp[0];

	float D=sqrt(Lt*Lt+Mt*Mt+Nt*Nt);//求长度，准备单位化

	float Li=Lt/D;
	float Mi=Mt/D;
	float Ni=Nt/D;

	switch(nChar)
	{
	case 'W' ://向前沿着摄目向量，同时移动摄像机和目标，它们之间距离不变。
		eyePos[0]-=L *sliderspeed;
		eyePos[1]-=M *sliderspeed;
		eyePos[2]-=N *sliderspeed;

		target[0]-=L *sliderspeed;
		target[1]-=M *sliderspeed;
		target[2]-=N *sliderspeed;
		break;
	case 'A' ://向左沿着垂直摄目向量，同时移动摄像机和目标，它们之间距离不变。
		eyePos[0]+=Li *sliderspeed;
		eyePos[1]+=Mi *sliderspeed;
		eyePos[2]+=Ni *sliderspeed;

		target[0]+=Li *sliderspeed;
		target[1]+=Mi *sliderspeed;
		target[2]+=Ni *sliderspeed;
		break;

	case 'S' ://向后沿着摄目向量，同时移动摄像机和目标，它们之间距离不变。
		eyePos[0]+=L *sliderspeed;
		eyePos[1]+=M *sliderspeed;
		eyePos[2]+=N *sliderspeed;

		target[0]+=L *sliderspeed;
		target[1]+=M *sliderspeed;
		target[2]+=N *sliderspeed;
		break;

	case 'D' ://向右沿着垂直摄目向量，同时移动摄像机和目标，它们之间距离不变。
		eyePos[0]-=Li *sliderspeed;
		eyePos[1]-=Mi *sliderspeed;
		eyePos[2]-=Ni *sliderspeed;

		target[0]-=Li *sliderspeed;
		target[1]-=Mi *sliderspeed;
		target[2]-=Ni *sliderspeed;
		break;

	default:break;
	}
}