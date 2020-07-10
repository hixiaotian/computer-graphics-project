// 3dsObject.h: interface for the C3dsObject class.
// 3D物体类，也就是3D场景
// 由它来统一控制3DS物体的读取和显示
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSOBJECT_H__E9508E48_2E44_440D_B416_524C5D2F1B19__INCLUDED_)
#define AFX_3DSOBJECT_H__E9508E48_2E44_440D_B416_524C5D2F1B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "glStructures.h"	// 数据结构定义
#include "3dsReader.h"		// C3dsReader说明文件
#include "TriList.h"		// CTriList说明文件
#include "glo.h"			// 全局结构和函数的定义


////////////////////////////////////////////////////////////////
//3ds物体控制总类，一个它其实就是一个3ds物体
class C3dsObject  
{
public:
	C3dsObject();
	virtual ~C3dsObject(); 

	BOOL LoadFile(LPCTSTR lpszPathName);		// 根据文件名装载3ds物体
												// 成功返回true ,失败返回false

//显示函数，根据参数显示物体。
//它的参数依次为 线条显示？		为真：按线显示，为假：按面显示
//				 比例因子：		也就是显示的模型是原来真实模型的scale倍。
//				 隐藏显示？		为真：物体看不见，为假：看得见
//				 平移的数据，	就是3ds场景中心的位置
//				 旋转数据，		就是物体绕XYZ轴各自转动的角度。
	void Draw(BOOL &IsLine,GLfloat &scale,BOOL &hide,TraData &t_data,RotData &r_data);
	CTriList			m_triList;	//3DS物体三角形列表对象
};

#endif // !defined(AFX_3DSOBJECT_H__E9508E48_2E44_440D_B416_524C5D2F1B19__INCLUDED_)
