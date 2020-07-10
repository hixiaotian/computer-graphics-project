#include "stdafx.h"
#include "PrtSys.h"
#include "Particle.h"
#include <math.h>
#include "glo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CPrt_Explode::CPrt_Explode()
{
}
CPrt_Explode::~CPrt_Explode()
{
}


//初始化爆炸总类
void CPrt_Explode::GetInitInfo()
{
	PDD.MAX_PARTICLES=100;			//100个小的粒子源
	PDD.star_x=0;					//起爆位置
	PDD.star_y=0;
	PDD.star_z=0;
	PDD.lifefate=0.5f;				// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=1.0f;				// 减慢粒子的速度，数值约大越慢，越小越快

	PDD.speedMax=100.0;				// 爆炸速度的值
	PDD.speedMin=100.0;

	PDD.AngleYStar=0;				// Y轴的张角大小
	PDD.AngleYEnd=180;
	
	PDD.AngleXZStar=0;				// XZ平面的投影张角大小
	PDD.AngleXZEnd=360;	

	PDD.GetXg=0.0f;					//粒子受到的重力场（矢量）
	PDD.GetYg=-15.0f;
	PDD.GetZg=0.0f;

	PDD.TextWidth=1.0;
	PDD.TextModel=0;

	/////////////////////////////////////// //颜色
	PDD.PrtColor.Red[0]=1.0f;
	PDD.PrtColor.Green[0]=1.0f;
	PDD.PrtColor.Blue[0]=1.0f;

	PDD.PrtColor.Red[1]=1.0f;
	PDD.PrtColor.Green[1]=0.706f;
	PDD.PrtColor.Blue[1]=0.706f;

	PDD.PrtColor.Red[2]=1.0f;
	PDD.PrtColor.Green[2]=0.392f;
	PDD.PrtColor.Blue[2]=0.392f;

	PDD.PrtColor.Red[3]=0.843f;	
	PDD.PrtColor.Green[3]=0.392f;
	PDD.PrtColor.Blue[3]=0.784f;

	PDD.PrtColor.Red[4]=0.196;
	PDD.PrtColor.Green[4]=0.95f;
	PDD.PrtColor.Blue[4]=0.390f;

	/////////////////////////////////////////////////////////////
	//以下是子粒子的属性
	PDD.SonPDD.MAX_PARTICLES=100;
	PDD.SonPDD.lifefate=2.0;				// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.SonPDD.slowdown=0.5;				// 减慢粒子的速度，数值约大越慢，越小越快

	PDD.SonPDD.GetXg=0;
	PDD.SonPDD.GetYg=0;
	PDD.SonPDD.GetZg=0;

	PDD.SonPDD.speedMax=1;
	PDD.SonPDD.speedMin=1;

	Init();
}

void CPrt_Explode::ReSatPrt(int loop)
{
	//赋予生命和衰减期
	explodePar[loop].life=1.0f;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
	explodePar[loop].fade=0.03*PDD.lifefate;

	//指定其地
	explodePar[loop].PDD.star_x=PDD.star_x;
	explodePar[loop].PDD.star_y=PDD.star_y;
	explodePar[loop].PDD.star_z=PDD.star_z;

	float	a=RandMaxMin(PDD.AngleYEnd,PDD.AngleYStar)/180.0f*PAI;
	float	b=RandMaxMin(PDD.AngleXZEnd,PDD.AngleXZStar)/180.0f*PAI;

	speed=RandMaxMin(PDD.speedMax,PDD.speedMin);
	explodePar[loop].xi=speed*sin(a)*cos(b);
	explodePar[loop].yi=speed*cos(a);
	explodePar[loop].zi=speed*sin(a)*sin(b);

	/////////////////////////////////////
	//子粒子源的属性及时更改
	explodePar[loop].PDD.MAX_PARTICLES=PDD.SonPDD.MAX_PARTICLES;
	explodePar[loop].PDD.lifefate=PDD.SonPDD.lifefate;				// 削减粒子的生命，数值约大，粒子的生命就越短
	explodePar[loop].PDD.slowdown=PDD.SonPDD.slowdown;				// 减慢粒子的速度，数值约大越慢，越小越快
	explodePar[loop].PDD.GetXg=PDD.SonPDD.GetXg;
	explodePar[loop].PDD.GetYg=PDD.SonPDD.GetYg;
	explodePar[loop].PDD.GetZg=PDD.SonPDD.GetZg;
	explodePar[loop].PDD.speedMax=PDD.SonPDD.speedMax;
	explodePar[loop].PDD.speedMin=PDD.SonPDD.speedMin;
	explodePar[loop].PDD.TextModel=PDD.TextModel;
	explodePar[loop].PDD.TextWidth=PDD.TextWidth;

	//更新颜色
	for(int i=0;i<=4;i++)
	{
		explodePar[loop].PDD.PrtColor.Red[i]=PDD.PrtColor.Red[i];
		explodePar[loop].PDD.PrtColor.Green[i]=PDD.PrtColor.Green[i];
		explodePar[loop].PDD.PrtColor.Blue[i]=PDD.PrtColor.Blue[i];
	}
	changeForce(loop);
}

int CPrt_Explode::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//*******************************************************************************
	//初始化所有粒子
	//装载材质
	if (!LoadGLTextures())							
	{
		return FALSE;
	}
		for (loop=0;loop<200;loop++)
		{
				explodePar[loop].active=true;// 使所有的粒子都被激活	explodePar[loop].PDD.MAX_PARTICLES=100;//（max 1500）
				ReSatPrt(loop);
				for(int i=0;i<=3;i++)
					explodePar[loop].texture[i]=&texture[i];
				explodePar[loop].Init();
		}


	return TRUE;										// 初始化 OK
}

void CPrt_Explode::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (explodePar[loop].active)
		{
			//显示粒子
			explodePar[loop].draw(Tar2Cam,Isplay,explodePar[loop].life);

			if(Isplay)
			{
			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			explodePar[loop].PDD.star_x+=explodePar[loop].xi/(PDD.slowdown*1000);
			explodePar[loop].PDD.star_y+=explodePar[loop].yi/(PDD.slowdown*1000);
			explodePar[loop].PDD.star_z+=explodePar[loop].zi/(PDD.slowdown*1000);

			//粒子速度的改变，Xg其实就是加速度!!!!
			explodePar[loop].xi+=explodePar[loop].xg;
			explodePar[loop].yi+=explodePar[loop].yg;
			explodePar[loop].zi+=explodePar[loop].zg;

			//粒子的生命减小
			explodePar[loop].life-=explodePar[loop].fade;
			}

			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (explodePar[loop].life<0.0f)					// If Particle Is Burned Out
			{
				ReSatPrt(loop);	
			}
		}
	}
}

void CPrt_Explode::changeForce(int loop)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		explodePar[loop].xg=PDD.GetXg*0.1;
		explodePar[loop].yg=PDD.GetYg*0.1;
		explodePar[loop].zg=PDD.GetZg*0.1;
}



AUX_RGBImageRec * CPrt_Explode::LoadBMP(char *Filename)//打开贴图文件，并返回句柄
{
	CFileStatus status;
	if(!CFile::GetStatus(Filename, status ))//测试是否打开文件
		return NULL;
	else
	{
		return auxDIBImageLoad(Filename);
	}
}

int CPrt_Explode::LoadTextures(char *Filename,GLuint &texture)//给粒子贴图
{
	int Status=FALSE;
	AUX_RGBImageRec *TextureImage[1];
	memset(TextureImage,0,sizeof(void *)*1);
	if (TextureImage[0]=LoadBMP(Filename))
	{
		Status=TRUE;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}	
	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
		{
			free(TextureImage[0]->data);
		}
		free(TextureImage[0]);
	}
	return Status;
}

int CPrt_Explode::LoadGLTextures(void)//给粒子贴图
{
	if(!LoadTextures("圆球.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("圆球.bmp error!");return FALSE;}
	if(!LoadTextures("六角形.bmp",texture[1]))	//给0号材质装入贴图
	{AfxMessageBox("六角形.bmp error!");return FALSE;}
	if(!LoadTextures("星形.bmp",texture[2]))	//给0号材质装入贴图
	{AfxMessageBox("星形.bmp error!");return FALSE;}
	if(!LoadTextures("雪花形.bmp",texture[3]))	//给0号材质装入贴图
	{AfxMessageBox("雪花形.bmp error!");return FALSE;}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
CPrt_ExplodePRT::CPrt_ExplodePRT()
{
}

CPrt_ExplodePRT::~CPrt_ExplodePRT()
{

}

void	CPrt_ExplodePRT::GetInitInfo(void)
{

}

//粒子对准摄像机贴图坐标转换函数
void CPrt_ExplodePRT::ExchangeShowXYZ(float *Tar2Cam,float x,float y,float z,float a)//a=ADDwidth)
{
	//float ParShowXYZ[4][3];具体操作和计算过程见笔记本
	//Tar2Cam是由目标指向摄像机的向量
	//粒子所在平面方程：A(X-x0)+B(Y-y0)+C(Z-z0)=0;
	//具体方程：(CameraPos[0]-x)*(X-x)+(CameraPos[1]-y)*(Y-y)+(CameraPos[2]-z)*(Z-z)=0;
	float	T0=Tar2Cam[0];
	float	T1=Tar2Cam[1];
	float	T2=Tar2Cam[2];
	float	l,m,n,t;
	if((T0*T0+T1*T1)!=0)
	{
		float	K1=sqrt((2*T1*T1*a*a)/(T0*T0+T1*T1));
		float	K0=sqrt((2*T0*T0*a*a)/(T0*T0+T1*T1));
		if( ((T0>=0)&&(T1>=0)) || ((T0<=0)&&(T1<=0)) )
		{
			l=2*T2*K0;
			m=2*T2*K1;
			n=-2*T1*K1-2*T0*K0;
			t=sqrt((2*a*a)/(l*l+m*m+n*n));

			ParShowXYZ[3][0]=x+K1;	//3点
			ParShowXYZ[3][1]=y-K0;
			ParShowXYZ[3][2]=z;

			ParShowXYZ[0][0]=x-K1;	//0点
			ParShowXYZ[0][1]=y+K0;
			ParShowXYZ[0][2]=z;

			ParShowXYZ[1][0]=x+l*t;	//1点
			ParShowXYZ[1][1]=y+m*t;
			ParShowXYZ[1][2]=z+n*t;

			ParShowXYZ[2][0]=x-l*t;	//2点
			ParShowXYZ[2][1]=y-m*t;
			ParShowXYZ[2][2]=z-n*t;
		}
		if( ((T0<0)&&(T1>=0)) || ((T0>0)&&(T1<=0)) )
		{
			l=2*T2*K0;
			m=-2*T2*K1;
			n=2*T1*K1-2*T0*K0;
			t=sqrt((2*a*a)/(l*l+m*m+n*n));

			ParShowXYZ[3][0]=x-K1;	//3点
			ParShowXYZ[3][1]=y-K0;
			ParShowXYZ[3][2]=z;

			ParShowXYZ[0][0]=x+K1;	//0点
			ParShowXYZ[0][1]=y+K0;
			ParShowXYZ[0][2]=z;

			ParShowXYZ[1][0]=x+l*t;	//1点
			ParShowXYZ[1][1]=y+m*t;
			ParShowXYZ[1][2]=z+n*t;

			ParShowXYZ[2][0]=x-l*t;	//2点
			ParShowXYZ[2][1]=y-m*t;
			ParShowXYZ[2][2]=z-n*t;
		}
	}
	else
	{

		ParShowXYZ[0][0]=x+a;	//0点
		ParShowXYZ[0][1]=y+a;
		ParShowXYZ[0][2]=z;

		ParShowXYZ[3][0]=x-a;	//3点
		ParShowXYZ[3][1]=y-a;
		ParShowXYZ[3][2]=z;

		ParShowXYZ[1][0]=x-a;	//1点
		ParShowXYZ[1][1]=y+a;
		ParShowXYZ[1][2]=z;

		ParShowXYZ[2][0]=x+a;	//2点
		ParShowXYZ[2][1]=y-a;
		ParShowXYZ[2][2]=z;
	}
}



void CPrt_ExplodePRT::draw(float *Tar2Cam,bool Isplay,float selflife)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, *texture[PDD.TextModel]);


	glEnable(GL_TEXTURE_2D);
	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			float x=particle[loop].x;
			float y=particle[loop].y;
			float z=particle[loop].z;

			ExchangeShowXYZ(Tar2Cam,x,y,z,0.1*PDD.TextWidth);	//改变贴图坐标
			//给粒子赋予动态颜色
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life*0.5);

			//changeColor(loop,selflife);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]);
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]);
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]);
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]);
			glEnd();
		
			if(Isplay)
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
				ReSatPrt(loop);
				changeColor(loop,selflife);

			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);						// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);							// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}


void CPrt_ExplodePRT::changeForce(int loop)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;
}

void CPrt_ExplodePRT::ReSatPrt(int loop)
{
	//赋予生命和衰减期
	particle[loop].life=1.0f;// 给所有的粒子最长的生命，1.0F最长，0.0f最短
	particle[loop].fade=float(((rand()%100)/1000.0f+0.003f)*PDD.lifefate);//0.002*RandMaxMin(PDD.lifefate,PDD.lifefate/2.0);		// 随机的生命减少量，即退色速度

	//指定其地
	particle[loop].x=PDD.star_x;
	particle[loop].y=PDD.star_y;
	particle[loop].z=PDD.star_z;

	speed=RandMaxMin(PDD.speedMax,PDD.speedMin);
	particle[loop].xi=float((rand()%50)-26.0f)/10.0*speed;//*10.0f;			// Random Speed On X Axis
	particle[loop].yi=float((rand()%50)-25.0f)/10.0*speed;//*10.0f;			// Random Speed On Y Axis
	particle[loop].zi=float((rand()%50)-26.0f)/10.0*speed;//*10.0f;;//float((rand()%50)-25.0f);//*10.0f;			// Random Speed On Z Axis
	changeForce(loop);
}



int CPrt_ExplodePRT::Init(void)//初始化粒子系统（包括位置、速度、……）
{

	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<1500;loop++)			// Initials All The Textures
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				ReSatPrt(loop);
				//给粒子赋予颜色
				//changeColor(loop);
				changeColor(loop,1.0);
				changeForce(loop);//更改粒子的加速度（速度由加速度而改变）
		}
	//初始化粒子的颜色

	return TRUE;										// 初始化 OK
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_ExplodePRT::changeColor(int loop,float selflife)
{

	int lifecolorset;					//生命颜色选取值

	//给粒子赋予初始颜色
		if(selflife<=1.0f&&selflife>=0.8f)lifecolorset=0;
		if(selflife<0.8f&&selflife>=0.6f)lifecolorset=1;
		if(selflife<0.6f&&selflife>=0.4f)lifecolorset=2;
		if(selflife<0.4f&&selflife>=0.2f)lifecolorset=3;
		if(selflife<0.2f)lifecolorset=4;

		particle[loop].r=PDD.PrtColor.Red[lifecolorset];			// Select Red From Color Table
		particle[loop].g=PDD.PrtColor.Green[lifecolorset];			// Select Green From Color Table
		particle[loop].b=PDD.PrtColor.Blue[lifecolorset];			// Select Blue From Color Table

}