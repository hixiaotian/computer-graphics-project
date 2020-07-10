// CamEye.h: interface for the CCamEye class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMEYE_H__51245644_3691_4C6F_AD2A_DD3CCE052A9A__INCLUDED_)
#define AFX_CAMEYE_H__51245644_3691_4C6F_AD2A_DD3CCE052A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCamEye  
{
public:
	CCamEye();
	virtual ~CCamEye();

	float		eyePos[3];			//摄像机（眼睛）的位置
	float		preeyePos_value[3];	//存储用于旋转的前角度
									//preeyePos_value[0]:用于存放X-Z平面上转过的角度
									//preeyePos_value[1]：用于存放z_y平面上转过的角度
									//preeyePos_value[2]
	float		eyeRadius;			//存放摄像机到target的距离
	float		target[3];			//目标的位置
	float		eyeUp[3];			//摄像机的向上方向
	float		sliderspeed;		//平移摄像机的速度，就以每一次移动的距离。

	//设置视点的一系列函数
	//
	void SetEyeUp(int axis, int value);		//设置摄像机向上的方向,目前没有使用

	//设置目标的一系列函数
	void SetTargetPos(int axis, int value);	//设置目标的位置（围着摄像机球形旋转）
	void SetTargetPos(float PX, float PY, float PZ);	//适用于由绝对坐标三点设置

	//设置摄像机的位置的一系列重载函数
	void SetEyePos(int axis, int value);				//适用于鼠标拖动控制的（围绕目标球形旋转）
	void SetEyePos(float PX, float PY, float PZ);		//适用于由绝对坐标三点设置

	//适用于球坐标系设置，以目标为球心
	void SetEyePosA(float Radius, float AngelY, float AngleXZ);

	void SetEyeRadius(int value);			//设置摄像机到目标的距离
	void InitCamera();						//初始化摄像机

	void SliderCamera(UINT nChar);			//根据按键平移摄像机（WASD前进、向左、后退、向右）

};

#endif // !defined(AFX_CAMEYE_H__51245644_3691_4C6F_AD2A_DD3CCE052A9A__INCLUDED_)
