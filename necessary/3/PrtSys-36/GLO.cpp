// GLO.cpp: implementation of the CCamEye class.
//全局函数说明库
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrtSys.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//随机返回Max与Min之间的一个浮点值
extern  double	RandMaxMin(double Max,double Min)
{
	if(Max<Min)Max=Min+0.0001f;
	return (rand()%100/100.0)*(Max-Min)+Min;
}

