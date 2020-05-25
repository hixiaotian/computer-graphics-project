//PrtFirework.cpp
//烟花、礼花类
#include "stdafx.h"
#include "PrtSys.h"
#include "Particle.h"

//全局函数说明库
#include "glo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CPrt_FireWork::CPrt_FireWork()//构造函数
{	
}
CPrt_FireWork::~CPrt_FireWork()
{
}

//////////////////////////////////////////
//初始化烟花总类
//////////////////////////////////////////
void CPrt_FireWork::GetInitInfo()
{
	CPrt_Explode::GetInitInfo();
}

void CPrt_FireWork::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	CPrt_Explode::draw(Tar2Cam,Isplay);	
}
