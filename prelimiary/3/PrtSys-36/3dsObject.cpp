// 3dsObject.cpp: implementation of the C3dsObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrtSys.h"
#include "3dsObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3dsObject::C3dsObject()
{

}

C3dsObject::~C3dsObject()
{

}

// 根据文件名装载3ds物体
// 成功返回true ,失败返回false
BOOL C3dsObject::LoadFile(LPCTSTR lpszPathName)		
{
	m_triList.Init();
	char* file = new char[strlen(lpszPathName)];
	strcpy(file, lpszPathName);	

	C3dsReader Loader;
	BOOL result;
	if( m_triList.getNumObjects() > 0 ) m_triList.removeAllObjects();
	
	result = Loader.Reader(file, &m_triList);
	if( result) 
	{
		m_triList.doAfterMath();
	}
	return result;
}

//显示函数，根据参数显示物体。
//它的参数依次为 线条显示？		为真：按线显示，为假：按面显示
//				 比例因子：		也就是显示的模型是原来真实模型的scale倍。
//				 隐藏显示？		为真：物体看不见，为假：看得见
//				 平移的数据，	就是3ds场景中心的位置
//				 旋转数据，		就是物体绕XYZ轴各自转动的角度。
void C3dsObject::Draw(BOOL &IsLine,GLfloat &scale,BOOL &hide,TraData &t_data,RotData &r_data)
{
	if(!hide)//如果没有隐藏就显示
	{
	glPushMatrix();
		glTranslatef( t_data.x, t_data.y, t_data.z );//移动物体
		glRotatef( r_data.Angle_x-90.0, 1.0F, 0.0F, 0.0F );	//旋转物体
		glRotatef( r_data.Angle_y, 0.0F, 1.0F, 0.0F );
		glRotatef( r_data.Angle_z, 0.0F, 0.0F, 1.0F );
		m_triList.drawGL(IsLine,scale);//3DS模型的显示
	glPopMatrix();
	}
}