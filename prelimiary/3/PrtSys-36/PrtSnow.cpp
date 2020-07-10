//雪的效果类
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

//double	RandMaxMin(double Max,double Min);

CPrt_Snow::CPrt_Snow()
{

}

CPrt_Snow::~CPrt_Snow()
{

}

void CPrt_Snow::GetInitInfo(void)//获得初始化数据
{
	PDD.MAX_PARTICLES=500;
	oldnum=PDD.MAX_PARTICLES;
	PDD.star_x=0;						//雪场的中心位置
	PDD.star_y=5.0;
	PDD.star_z=0;
	PDD.lifefate=10.0f;					// 雪场的高度！
	oldHeight=PDD.lifefate;
	PDD.slowdown=25.8f;					// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）


	PDD.pram[0]=0.015;					// 三叶玫瑰线角度增量
	PDD.dead=false;						// 是否需要打散

	PDD.speedMax=10.0;					// 速度的最大值
	PDD.speedMin=5.0;					// 速度的最小值

	PDD.AngleYStar=2;					// 三叶玫瑰线最大半径
	PDD.AngleYEnd=0.5;					// 三叶玫瑰线最小半径
	
	PDD.AngleXZStar=10;					// 雪场的长（X）
	PDD.AngleXZEnd=10;					// 雪场的宽（Z）
	
	PDD.GetXg=0.0;						//粒子受到的重力场（矢量）
	PDD.GetYg=-0.2;
	PDD.GetZg=0.0;

	PDD.TextModel=0;		//贴图模式
	PDD.TextWidth=0.1;		//贴图大小
	//strcpy(TextFileName,"火焰.bmp");		//材质贴图名称

	Init();//初始化粒子系统（包括位置、速度、……）
}

void CPrt_Snow::ReSatPrt(int loop)
{
		//particle[loop].life=1;//float((rand()%100)/100.0f);		// 给所有的粒子最长的生命，1.0F最长，0.0f最短
		//particle[loop].fade=RandMaxMin(0.004f,0.001f);			// 随机的生命减少量，即退色速度

		//particle[loop].active=true;
		//指定其地
		particle[loop].x=PDD.star_x+RandMaxMin(PDD.AngleXZStar/2.0,-PDD.AngleXZStar/2.0);	// 雪场的长度（X）
		particle[loop].y=PDD.star_y;
		particle[loop].z=PDD.star_z+RandMaxMin(PDD.AngleXZEnd/2.0,-PDD.AngleXZEnd/2.0);	//雪场的宽度（Z）

		//粒子的速度,刚开始雪滴只有下落的速度
		//speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度的大小
		particle[loop].xi=0;
		//particle[loop].yi=-speed;								// Random Speed On Y Axis
		particle[loop].zi=0;
		PrtSnowAngle[loop]=RandMaxMin(360.0,0.0);				//转过角度
		PrtSnowRadius[loop]=RandMaxMin(PDD.AngleYStar,PDD.AngleYEnd);	//半径
		//changeColor(loop);
		changeForce(loop);
}


void CPrt_Snow::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
//	glDisable(GL_DEPTH_TEST);							// 禁用深度测试(WWWWWWWWWWWWWWW)
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Type Of Blending To Perform
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[PDD.TextModel]);

	//glTranslatef(PDD.star_x,PDD.star_y,PDD.star_z);//物体中心位置。
	glEnable(GL_TEXTURE_2D);
	if(oldnum!=PDD.MAX_PARTICLES)//粒子数量改变了，需要打散
	{
		PDD.dead=true;
		oldnum=PDD.MAX_PARTICLES;
	}

	if(PDD.dead)//如果需要打散
	{
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)				// Loop Through All The Particles
		{
			particle[loop].active=true;
			ReSatPrt(loop);
			speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度的大小
			particle[loop].yi=-speed;
			particle[loop].y=PDD.star_y-RandMaxMin(PDD.lifefate,0);//雪花均匀的分布在高度里
		}
		PDD.dead=false;
	}

	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)				// Loop Through All The Particles
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		//if (particle[loop].active)						// 查看粒子是否存活着
		//{
			//粒子的位置
			//记住！只有particle[loop].x、y、z才是真正的的粒子中心，
			//每一个粒子是围着他作所谓的三叶玫瑰线运动的
			float x,y,z;//粒子的显示位置：根据中心然后进行三叶玫瑰线运算。
			y=particle[loop].y;
			x=particle[loop].x+PrtSnowRadius[loop]*cos(PrtSnowAngle[loop]);
			z=particle[loop].z+PrtSnowRadius[loop]*sin(PrtSnowAngle[loop]);

			if(Isplay)
				PrtSnowAngle[loop]+=PDD.pram[0];//转过的角度增加

			//改变贴图坐标,只需计算摄像机在XZ平面上的投影向量就行了
			//Tar2Cam[1]=0.0;
			if(PDD.TextModel==4||PDD.TextModel==5)
					//RainExchangeShowXYZ(Tar2Cam,x,y,z,0.03*PDD.TextWidth);
					RainExchangeShowXYZ(Tar2Cam,x,y,z,0.5*PDD.TextWidth);
			else CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,PDD.TextWidth);	
			//给粒子赋予动态颜色
			changeColor(loop);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);				// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();								// Done Building Triangle Strip

			if(Isplay)
			{
			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			particle[loop].x+=particle[loop].xi/(PDD.slowdown*50);
			particle[loop].y+=particle[loop].yi/(PDD.slowdown*10);
			particle[loop].z+=particle[loop].zi/(PDD.slowdown*50);

			//粒子速度的改变，Xg其实就是加速度!!!!
			particle[loop].xi+=particle[loop].xg;
			particle[loop].zi+=particle[loop].zg;
			}
			//当粒子跑出雪场规定高度时便要重生
			if((PDD.star_y-particle[loop].y)>=PDD.lifefate)
			{
				ReSatPrt(loop);

			}

	}
	//========================
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}


void CPrt_Snow::changeForce(int loop)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;
}





int CPrt_Snow::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//装载材质
	if (!LoadGLTextures())
	{
		return FALSE;
	}	
	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				ReSatPrt(loop);
				speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度的大小
				particle[loop].yi=-speed;
				particle[loop].y=PDD.star_y-RandMaxMin(PDD.lifefate+0.001,-0.001);//雪花均匀的分布在高度里

		}
		//设置颜色和透明度
		PDD.PrtColor.Red[0]=1.0;
		PDD.PrtColor.Green[0]=1.0;
		PDD.PrtColor.Blue[0]=1.0;
		PDD.PrtColor.Red[1]=0.5;
	return TRUE;										// 初始化 OK
}


int CPrt_Snow::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("圆球.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("圆球.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("六角形.bmp",texture[1]))	//给1号材质装入贴图
	{AfxMessageBox("六角形.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("星形.bmp",texture[2]))	//给2号材质装入贴图
	{AfxMessageBox("星形.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("雪花形.bmp",texture[3]))	//给3号材质装入贴图
	{AfxMessageBox("雪花形.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("雨2.bmp",texture[4]))	//给0号材质装入贴图
	{AfxMessageBox("雨2.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("雨M.bmp",texture[5]))	//给1号材质装入贴图
	{AfxMessageBox("雨M.bmp error!");return FALSE;}

	return true;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_Snow::changeColor(int loop)
{
	glColor4f(PDD.PrtColor.Red[0],PDD.PrtColor.Green[0],PDD.PrtColor.Blue[0],PDD.PrtColor.Red[1]);
}

//雨粒子对准摄像机贴图坐标转换函数
void CPrt_Snow::RainExchangeShowXYZ(float *CameraPos,float x,float y,float z,float a)
{
	//float ParShowXYZ[4][3];具体操作和计算过程见笔记本
	//Tar2Cam是由目标指向摄像机的向量
	//粒子所在平面方程：A(X-x0)+B(Y-y0)+C(Z-z0)=0;
	//具体方程：(CameraPos[0]-x)*(X-x)+(CameraPos[1]-y)*(Y-y)+(CameraPos[2]-z)*(Z-z)=0;
	float	T0=CameraPos[0];
	float	T1=0;
	float	T2=CameraPos[2];
	float k;
	if(T0==0.0)
	{
		ParShowXYZ[0][0]=x+a;	//0点
		ParShowXYZ[0][1]=y+8*a;
		ParShowXYZ[0][2]=z;

		ParShowXYZ[3][0]=x-a;	//3点
		ParShowXYZ[3][1]=y-8*a;
		ParShowXYZ[3][2]=z;

		ParShowXYZ[1][0]=x-a;	//1点
		ParShowXYZ[1][1]=y+8*a;
		ParShowXYZ[1][2]=z;

		ParShowXYZ[2][0]=x+a;	//2点
		ParShowXYZ[2][1]=y-8*a;
		ParShowXYZ[2][2]=z;
	}
	else 
	{
		k=-T0/T2;
		ParShowXYZ[0][0]=x+a*cos(atan(k));	//0点
		ParShowXYZ[0][1]=y+8*a;
		ParShowXYZ[0][2]=z+a*sin(atan(k));

		ParShowXYZ[3][0]=x-a*cos(atan(k));	//3点
		ParShowXYZ[3][1]=y-8*a;
		ParShowXYZ[3][2]=z-a*sin(atan(k));

		ParShowXYZ[1][0]=x-a*cos(atan(k));	//1点
		ParShowXYZ[1][1]=y+8*a;
		ParShowXYZ[1][2]=z-a*sin(atan(k));

		ParShowXYZ[2][0]=x+a*cos(atan(k));	//2点
		ParShowXYZ[2][1]=y-8*a;
		ParShowXYZ[2][2]=z+a*sin(atan(k));
	}
}

