//瀑布类的实现函数，其定义在"Particle.h"里
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


CPrt_WaterFall::CPrt_WaterFall()
{

}

CPrt_WaterFall::~CPrt_WaterFall()
{

}

void CPrt_WaterFall::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[PDD.TextModel]);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(PDD.star_x,PDD.star_y,PDD.star_z);
	//方向角
	glRotatef(PDD.AngleXZEnd,0,1,0);
	float ADDwidth;

	//将不显示的粒子弄死
	for (loop=PDD.MAX_PARTICLES;loop<=9999;loop++)
	{
			particle[loop].active=false;
	}

	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			float x=particle[loop].x;
			float y=particle[loop].y;
			float z=particle[loop].z;

			ADDwidth=(1.1f-particle[loop].life)*PDD.TextWidth;

			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,ADDwidth);	//改变贴图坐标

			//给粒子赋予动态颜色
			changeColor(loop);
			
			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]);
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]);
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]);
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]);
			glEnd();
			
			if(Isplay)//如果没有暂停。
			{
				//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
				particle[loop].x+=particle[loop].xi/(PDD.slowdown*1000);
				particle[loop].y+=particle[loop].yi/(PDD.slowdown*1000);
				particle[loop].z+=particle[loop].zi/(PDD.slowdown*1000);

				//粒子速度的改变，Xg其实就是加速度!!!!
				particle[loop].xi+=particle[loop].xg;
				particle[loop].yi+=particle[loop].yg;
				particle[loop].zi+=particle[loop].zg;

				//粒子的生命减小
				particle[loop].life-=particle[loop].fade;
			}
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				particle[loop].life=1;
				ReSatPrt(loop);
			}
		}
		else
		{
				particle[loop].active=true;
				particle[loop].life=float((rand()%100)/100.0f);	
				ReSatPrt(loop);
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 禁用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();
}


void CPrt_WaterFall::changeForce(int loop/*particleModel *particle*/)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;
}

void CPrt_WaterFall::ReSatPrt(int loop)					//具体初始化速度等等
{
	//赋予生命和衰减期
	particle[loop].fade=0.004*PDD.lifefate;		// 随机的生命减少量，即退色速度

	//指定其地
	particle[loop].x=PDD.AngleXZStar*float((rand()%100)/50.0f-1.0f);
	particle[loop].y=0;					
	particle[loop].z=0;					
	//粒子的速度
	speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度大小
	//瀑布的喷射方向
	ay=PDD.AngleYEnd/180.0*PAI;	//扬角（0-180）
	particle[loop].xi=speed*0.01*particle[loop].x*float((rand()%100)/100.0f);
	particle[loop].yi=speed*cos(ay);
	particle[loop].zi=speed*sin(ay);
	changeForce(loop);//更改粒子的加速度（速度由加速度而改变）

}



int CPrt_WaterFall::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//装载材质
	if (!LoadGLTextures())
	{
		return FALSE;
	}
	
	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<=9999;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				particle[loop].life=float((rand()%100)/100.0f);	
				ReSatPrt(loop);
		}
	return TRUE;										// 初始化 OK
}


int CPrt_WaterFall::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("瀑布.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("瀑布.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("瀑布1.bmp",texture[1]))	//给1号材质装入贴图
	{AfxMessageBox("瀑布1.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("瀑布2.bmp",texture[2]))	//给2号材质装入贴图
	{AfxMessageBox("瀑布2.bmp error!");return FALSE;}
	return true;
}

void CPrt_WaterFall::GetInitInfo(void)//获得初始化数据
{
	PDD.MAX_PARTICLES=1500;
	PDD.star_x=0;
	PDD.star_y=0;
	PDD.star_z=0;
	PDD.lifefate=2.0f;				// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=1.0f;				// 减慢粒子的速度，数值约大越慢，越小越快

	PDD.AngleXZStar=5;				// 瀑布的宽度，必须为正值

	PDD.speedMax=150.0;					// 喷射速度的最大值
	PDD.speedMin=100.0;					// 喷射速度的最小值

	PDD.GetXg=0.0;						//粒子受到的重力场（矢量）
	PDD.GetYg=-3.8;
	PDD.GetZg=0.0;

	//瀑布的喷射方向
	PDD.AngleYEnd=80;	//扬角（0-180）
	PDD.AngleXZEnd=0;	//方向角（0-360）

	PDD.TextModel=0;						//贴图样式
	PDD.TextWidth=1.5;						//贴图大小

	Init();//初始化粒子系统（包括位置、速度、……）
	InitColor();
}

//初始化粒子的颜色
void CPrt_WaterFall::InitColor(void)
{
	PDD.PrtColor.Red[0]=1.0f;
	PDD.PrtColor.Green[0]=1.0f;
	PDD.PrtColor.Blue[0]=1.0f;

	PDD.PrtColor.Red[1]=1.0f;
	PDD.PrtColor.Green[1]=1.0f;
	PDD.PrtColor.Blue[1]=1.0f;

	PDD.PrtColor.Red[2]=1.0f;
	PDD.PrtColor.Green[2]=1.0f;
	PDD.PrtColor.Blue[2]=1.0f;

	PDD.PrtColor.Red[3]=1.0f;
	PDD.PrtColor.Green[3]=1.0f;
	PDD.PrtColor.Blue[3]=1.0f;

	PDD.PrtColor.Red[4]=1.0f;
	PDD.PrtColor.Green[4]=1.0f;
	PDD.PrtColor.Blue[4]=1.0f;
}

//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_WaterFall::changeColor(int loop)
{
		int lifecolorset;					//生命颜色选取值
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.8f)lifecolorset=0;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=1;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.4f)lifecolorset=2;
		if(particle[loop].life<0.4f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=PDD.PrtColor.Red[lifecolorset];
		particle[loop].g=PDD.PrtColor.Green[lifecolorset];
		particle[loop].b=PDD.PrtColor.Blue[lifecolorset];
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life*0.2);

}


