
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


//初始化导弹，主要是对尾烟进行初始化
void CPrt_Missile::Init(void)
{
	//对尾烟数据进行初始化
	RearFire.PDD.MAX_PARTICLES=1000;
	RearFire.PDD.star_x=0;
	RearFire.PDD.star_y=0;
	RearFire.PDD.star_z=0;
	RearFire.PDD.lifefate=1.0f;							// 削减粒子的生命，数值约大，粒子的生命就越短
	RearFire.PDD.slowdown=2.0f;							// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）
	RearFire.PDD.speedMax=10.0;							// 速度的最大值
	RearFire.PDD.speedMin=0.0;							// 速度的最小值

	RearFire.PDD.AngleYStar=90;							// Y轴的张角大小
	RearFire.PDD.AngleYEnd=90;
	
	RearFire.PDD.AngleXZStar=0;							// XZ平面的投影张角大小
	RearFire.PDD.AngleXZEnd=360;	
	
	RearFire.PDD.GetXg=20.0;							//粒子受到的重力场（矢量）
	RearFire.PDD.GetYg=0.0;
	RearFire.PDD.GetZg=0.0;

	////////////////////////////////////////////
	RearFire.PDD.TextModel=1;						//贴图样式
	RearFire.PDD.TextWidth=1.0;						//贴图大小
	RearFire.InitColor();							//初始化颜色
	RearFire.Init();//初始化粒子系统（包括位置、速度、……）
}



//显示函数，带有坐标
void CPrt_Missile::draw(float *Tar2Cam,bool &Isplay,float x,float y,float z)
{
	glPushMatrix();
		glTranslatef(x,y,z);//偏移导弹
		glRotatef(-90,0,1,0);
		drawModel();		//画出导弹
	glPopMatrix();
	RearFire.PDD.star_x=x+0.42;//更新尾烟位置
	RearFire.PDD.star_y=y;
	RearFire.PDD.star_z=z;
	RearFire.draw(Tar2Cam,Isplay);//画出尾烟。
}

//画导弹模型
void CPrt_Missile::drawModel(void)
{
	float scale=0.06;
	float P[3]={0*scale,0*scale,13*scale};	//导弹顶点

	float A[3]={ 1*scale, 1.732*scale, 7*scale};	//前六边形
	float B[3]={ 2*scale, 0*scale    , 7*scale};
	float C[3]={ 1*scale,-1.732*scale, 7*scale};
	float D[3]={-1*scale,-1.732*scale, 7*scale};
	float E[3]={-2*scale, 0*scale    , 7*scale};
	float F[3]={-1*scale, 1.732*scale, 7*scale};

	float Ar[3]={ 1*scale, 1.732*scale, -7*scale};	//后六边形
	float Br[3]={ 2*scale, 0*scale    , -7*scale};
	float Cr[3]={ 1*scale,-1.732*scale, -7*scale};
	float Dr[3]={-1*scale,-1.732*scale, -7*scale};
	float Er[3]={-2*scale, 0*scale    , -7*scale};
	float Fr[3]={-1*scale, 1.732*scale, -7*scale};


	//四个尾翼，每个尾翼4个点
	float WY[4][4][3]={
		{{2*scale  ,0*scale  ,3*scale},{3.5*scale  ,0*scale    ,-5.5*scale},{4*scale  ,0*scale    ,-9*scale},{2*scale   ,0*scale  ,-8*scale}},
		{{0*scale  ,1.732*scale  ,3*scale},{0*scale    ,3.5*scale  ,-5.5*scale},{0*scale    ,4*scale  ,-9*scale},{0*scale   ,1.732*scale  ,-8*scale}},
		{{-2*scale ,0*scale  ,3*scale},{-3.5*scale ,0*scale    ,-5.5*scale},{-4*scale ,0*scale    ,-9*scale},{-2*scale  ,0*scale  ,-8*scale}},
		{{0*scale  ,-1.732*scale ,3*scale},{0*scale    ,-3.5*scale ,-5.5*scale},{0*scale    ,-4*scale ,-9*scale},{0*scale   ,-1.732*scale ,-8*scale}}
	};	//1


		glColor3f(1,0,0);
		glBegin(GL_TRIANGLES);	//画顶盖=========================
			glVertex3fv(P);
			glVertex3fv(A);
			glVertex3fv(F);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3fv(P);
			glVertex3fv(F);
			glVertex3fv(E);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3fv(P);
			glVertex3fv(E);
			glVertex3fv(D);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3fv(P);
			glVertex3fv(D);
			glVertex3fv(C);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3fv(P);
			glVertex3fv(C);
			glVertex3fv(B);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3fv(P);
			glVertex3fv(B);
			glVertex3fv(A);
		glEnd();

		//画弹身===================================
		glColor3f(0.5,0.5,0.5);
		glBegin(GL_QUADS);
			glVertex3fv(A);
			glVertex3fv(Ar);
			glVertex3fv(Fr);
			glVertex3fv(F);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3fv(F);
			glVertex3fv(Fr);
			glVertex3fv(Er);
			glVertex3fv(E);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3fv(E);
			glVertex3fv(Er);
			glVertex3fv(Dr);
			glVertex3fv(D);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3fv(D);
			glVertex3fv(Dr);
			glVertex3fv(Cr);
			glVertex3fv(C);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3fv(C);
			glVertex3fv(Cr);
			glVertex3fv(Br);
			glVertex3fv(B);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3fv(B);
			glVertex3fv(Br);
			glVertex3fv(Ar);
			glVertex3fv(A);
		glEnd();

		//画底盖
		glBegin(GL_POLYGON);
			glVertex3fv(Ar);
			glVertex3fv(Br);
			glVertex3fv(Cr);
			glVertex3fv(Dr);
			glVertex3fv(Er);
			glVertex3fv(Fr);
		glEnd();

		//画尾翼========================================
		glColor3f(0.1,0.5,1.0);
		for(int i=0;i<=3;i++)
		{
		glBegin(GL_QUADS);
			glVertex3f(WY[i][0][0],WY[i][0][1],WY[i][0][2]);
			glVertex3f(WY[i][1][0],WY[i][1][1],WY[i][1][2]);
			glVertex3f(WY[i][2][0],WY[i][2][1],WY[i][2][2]);
			glVertex3f(WY[i][3][0],WY[i][3][1],WY[i][3][2]);
		glEnd();
		}
}