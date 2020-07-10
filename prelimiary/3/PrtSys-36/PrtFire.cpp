
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

CPrt_Fire::CPrt_Fire()
{

}

CPrt_Fire::~CPrt_Fire()
{

}

void CPrt_Fire::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);										// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[PDD.TextModel]);	//选择当前贴图样式
	glEnable(GL_TEXTURE_2D);
	glTranslatef(PDD.star_x,PDD.star_y,PDD.star_z);			//移动粒子源中心位置
	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)							// 查看粒子是否存活着
		{
			//粒子的位置
			float x=particle[loop].x;
			float y=particle[loop].y;
			float z=particle[loop].z;

			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,0.1*PDD.TextWidth);	//改变贴图坐标
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

			if(Isplay)//如果允许播放则进行粒子的更新
			{
			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			particle[loop].x+=particle[loop].xi/(PDD.slowdown*1000)*(particle[loop].life-0.1);
			particle[loop].y+=particle[loop].yi/(PDD.slowdown*1000)*(particle[loop].life-0.1);
			particle[loop].z+=particle[loop].zi/(PDD.slowdown*1000)*(particle[loop].life-0.1);

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
				//给粒子赋予重生颜色
				particle[loop].active=false;
			}
		}
		else 
		{
			particle[loop].active=true;
			ReSatPrt(loop);
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);									// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}


void CPrt_Fire::changeForce(int loop)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;
}


void CPrt_Fire::ReSatPrt(int loop)
{
		particle[loop].life=1.0f;// 给所有的粒子最长的生命，1.0F最长，0.0f最短
		particle[loop].fade=float(((rand()%100)/1000.0f+0.003f)*PDD.lifefate);		// 随机的生命减少量，即退色速度

		particle[loop].x=0;
		particle[loop].y=0;
		particle[loop].z=0;
		//粒子的速度
		speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度的大小
		   ay=RandMaxMin(PDD.AngleYEnd,PDD.AngleYStar)/180.0*PAI;//速度的方向
		  axz=RandMaxMin(PDD.AngleXZEnd,PDD.AngleXZStar)/180.0*PAI;
		particle[loop].xi=speed*sin(ay)*cos(axz);
		particle[loop].yi=speed*cos(ay);
		particle[loop].zi=speed*sin(ay)*sin(axz);
		changeForce(loop);
}


int CPrt_Fire::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//装载材质
	if (!LoadGLTextures())
	{
		return FALSE;
	}	
	for (loop=0;loop<9999;loop++)
	{
		particle[loop].active=false;					// 使所有的粒子都被激活
	}
	return TRUE;										// 初始化 OK
}


int CPrt_Fire::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("圆球.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("圆球.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("火焰1.bmp",texture[1]))	//给1号材质装入贴图
	{AfxMessageBox("火焰1.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("火焰2.bmp",texture[2]))	//给2号材质装入贴图
	{AfxMessageBox("火焰2.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("火焰3.bmp",texture[3]))	//给3号材质装入贴图
	{AfxMessageBox("火焰3.bmp error!");return FALSE;}
	return true;
}


void CPrt_Fire::GetInitInfo(void)//获得初始化数据
{
	PDD.MAX_PARTICLES=1000;
	PDD.star_x=0;
	PDD.star_y=0.0;
	PDD.star_z=0;
	PDD.lifefate=1.0f;							// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=2.0f;							// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）
	PDD.speedMax=50.0;							// 速度的最大值
	PDD.speedMin=0.0;							// 速度的最小值

	PDD.AngleYStar=0;							// Y轴的张角大小
	PDD.AngleYEnd=90;
	
	PDD.AngleXZStar=0;							// XZ平面的投影张角大小
	PDD.AngleXZEnd=360;	
	
	PDD.GetXg=0.0;								//粒子受到的重力场（矢量）
	PDD.GetYg=0.1;
	PDD.GetZg=0.0;

	////////////////////////////////////////////
	PDD.TextModel=0;						//贴图样式
	PDD.TextWidth=1.0;						//贴图大小
	InitColor();							//初始化颜色
	Particle_type=1;						//1号代表火焰
	Init();//初始化粒子系统（包括位置、速度、……）
}

//初始化粒子的颜色
void CPrt_Fire::InitColor(void)
{
	PDD.PrtColor.Red[0]=1.0f;		//浅黄（几乎是白色的黄）
	PDD.PrtColor.Green[0]=0.996f;
	PDD.PrtColor.Blue[0]=0.953f;

	PDD.PrtColor.Red[1]=0.953f;		//黄色（偏橙色）
	PDD.PrtColor.Green[1]=0.757f;
	PDD.PrtColor.Blue[1]=0.125f;

	PDD.PrtColor.Red[2]=0.965f;			//暗黄色（深橙色）
	PDD.PrtColor.Green[2]=0.627f;
	PDD.PrtColor.Blue[2]=0.09f;

	PDD.PrtColor.Red[3]=0.843f;			//灰白色
	PDD.PrtColor.Green[3]=0.816f;
	PDD.PrtColor.Blue[3]=0.784f;

	PDD.PrtColor.Red[4]=0.001;			//黑色
	PDD.PrtColor.Green[4]=0.001f;
	PDD.PrtColor.Blue[4]=0.001f;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_Fire::changeColor(int loop)
{
		int lifecolorset;					//生命颜色选取值

		float alpha;
		alpha=particle[loop].life*0.5;
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.8f){lifecolorset=0;alpha=0.01;}
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=1;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.4f)lifecolorset=2;
		if(particle[loop].life<0.4f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=PDD.PrtColor.Red[lifecolorset];
		particle[loop].g=PDD.PrtColor.Green[lifecolorset];
		particle[loop].b=PDD.PrtColor.Blue[lifecolorset];

		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,alpha);
}

