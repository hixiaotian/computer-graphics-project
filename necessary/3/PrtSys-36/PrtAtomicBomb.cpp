//原子弹效果类
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

CPrt_AtomicBomb::CPrt_AtomicBomb()	//构造函数
{
}

CPrt_AtomicBomb::~CPrt_AtomicBomb()
{
}


void CPrt_AtomicBomb::GetInitInfo(void)			//获得初始化数据
{
	PDD.MAX_PARTICLES=1000;						//粒子的最大数（不可超过10000）

	PDD.star_x=0.0f;							//粒子源的位置
	PDD.star_y=-10.0f;
	PDD.star_z=0.0f;

	PDD.lifefate=1.0f;							// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=1.0f;							// 减慢粒子的速度，数值约大越慢，越小越快
	PDD.speedMax=50.0f;							// 速度值
	LoadGLTextures();
	Init();
}

int CPrt_AtomicBomb::LoadGLTextures(void)					//给粒子贴图
{
	kw_GroudShockWave.LoadGLTextures();//地面冲击波
	kw_BaseWave.LoadGLTextures();		//底波
	kw_BombCore.LoadGLTextures();		//爆核
	kw_BombCircle.LoadGLTextures();		//爆环
	kw_BombColumn.LoadGLTextures();		//爆柱
	return true;
}


void CPrt_AtomicBomb::Init(void)					//初始化函数
{
	screenLife=1.0f;	//最大为1，最小为0
	screenLifeFate=0.003;

	//从属类的实例生成
	//地面冲击波的初始化********************************
	kw_GroudShockWave.PDD.MAX_PARTICLES=PDD.MAX_PARTICLES/2;		//粒子数目
	kw_GroudShockWave.PDD.star_x=0;//PDD.star_x;					//中心位置
	kw_GroudShockWave.PDD.star_y=0;//PDD.star_y;
	kw_GroudShockWave.PDD.star_z=0;//PDD.star_z;
	kw_GroudShockWave.PDD.lifefate=PDD.lifefate;					//生命削减量
	kw_GroudShockWave.PDD.slowdown=PDD.slowdown;					//速度减少量
	kw_GroudShockWave.speed=PDD.speedMax/5000;
	kw_GroudShockWave.PDD.dead=false;//还活着
	kw_GroudShockWave.PDD.diecount=0;//没有粒子死亡。
	kw_GroudShockWave.Init();
	
	//底波的初始化**************************************
	kw_BaseWave.PDD.MAX_PARTICLES=PDD.MAX_PARTICLES/2;				//粒子数目
	kw_BaseWave.PDD.star_x=0;//PDD.star_x;								//中心位置
	kw_BaseWave.PDD.star_y=0;//PDD.star_y;
	kw_BaseWave.PDD.star_z=0;//PDD.star_z;
	kw_BaseWave.PDD.lifefate=PDD.lifefate;							//生命削减量
	kw_BaseWave.PDD.slowdown=PDD.slowdown;							//速度减少量
	kw_BaseWave.speed=PDD.speedMax/12;
	kw_BaseWave.PDD.dead=false;//还活着
	kw_BaseWave.PDD.diecount=0;//没有粒子死亡。
	kw_BaseWave.Init();

	//爆核的初始化***************************************
	kw_BombCore.PDD.MAX_PARTICLES=PDD.MAX_PARTICLES/2;				//粒子数目（不可超过5000！！）
	kw_BombCore.PDD.star_x=0;//PDD.star_x;								//中心位置
	kw_BombCore.PDD.star_y=0;//PDD.star_y;
	kw_BombCore.PDD.star_z=0;//PDD.star_z;
	kw_BombCore.PDD.lifefate=PDD.lifefate;							//生命削减量
	kw_BombCore.PDD.slowdown=PDD.slowdown;							//速度减少量
	kw_BombCore.speed=PDD.speedMax/10;
	kw_BombCore.PDD.dead=false;//还活着
	kw_BombCore.PDD.diecount=0;//没有粒子死亡。

	kw_BombCore.Init();

	//爆环的初始化********************************
	kw_BombCircle.PDD.MAX_PARTICLES=PDD.MAX_PARTICLES/10;			//粒子数目
	kw_BombCircle.PDD.star_x=0;//PDD.star_x;							//中心位置
	kw_BombCircle.PDD.star_y=0;//star_y;
	kw_BombCircle.PDD.star_z=0;//PDD.star_z;
	kw_BombCircle.PDD.lifefate=PDD.lifefate;						//生命削减量
	kw_BombCircle.PDD.slowdown=PDD.slowdown;						//速度减少量
	kw_BombCircle.speed=PDD.speedMax/10;
	kw_BombCircle.PDD.dead=false;//还活着
	kw_BombCircle.PDD.diecount=0;//没有粒子死亡。

	kw_BombCircle.Init();

	//爆柱的初始化***********************************

	kw_BombColumn.PDD.MAX_PARTICLES=PDD.MAX_PARTICLES/5;			//粒子数目
	kw_BombColumn.PDD.star_x=0;//PDD.star_x;							//中心位置
	kw_BombColumn.PDD.star_y=0;//star_y;
	kw_BombColumn.PDD.star_z=0;//PDD.star_z;
	kw_BombColumn.PDD.lifefate=PDD.lifefate;						//生命削减量
	kw_BombColumn.PDD.slowdown=PDD.slowdown;						//速度减少量
	kw_BombColumn.speed=PDD.speedMax/10;
	kw_BombColumn.PDD.dead=false;//还活着
	kw_BombColumn.PDD.diecount=0;//没有粒子死亡。

	kw_BombColumn.Init();
}

void CPrt_AtomicBomb::draw(float *Tar2Cam,bool Isplay)					//显示函数
{
	if(Isplay)	//如果没有暂停
	{
		if(screenLife<0.0)
		{
			screenLife=1.0;
			Init();
		}
		else screenLife-=screenLifeFate;
	}
	glClearColor(screenLife,screenLife,screenLife,0.5);

	glPushMatrix();
	glTranslatef(PDD.star_x,PDD.star_y,PDD.star_z);
		kw_GroudShockWave.draw(Tar2Cam,Isplay);						//地面冲击波
		kw_BaseWave.draw(Tar2Cam,Isplay);							//底波
		kw_BombCore.draw(Tar2Cam,Isplay);							//爆核
		kw_BombCircle.draw(Tar2Cam,Isplay);							//爆环
		kw_BombColumn.draw(Tar2Cam,Isplay);							//爆柱
	glPopMatrix();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//从属类的函数实现
///////////////////////////////////////////////////////////////////////////////////////////////////
//地面冲击波的实现
int CPrt_AB_GroudShockWave::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//初始化所有粒子
		speed*=450;
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)			// Initials All The Textures
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				particle[loop].life=1.0f;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
				particle[loop].fade=0.003f*PDD.lifefate;		// 统一的生命减少量

				//出生位置，爆心
				particle[loop].x=PDD.star_x;					// Center On X Axis
				particle[loop].y=PDD.star_y;					// Center On Y Axis
				particle[loop].z=PDD.star_z;					// Center On Z Axis
				//粒子的速度

				GWspeed[loop]=speed;
				PDD.speedMax=speed;
				PDD.speedMin=speed;
				//给粒子赋予颜色
				changeColor(loop,0);			
		}
	return TRUE;										// 初始化 OK
}


void CPrt_AB_GroudShockWave::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	 
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

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

			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,(1.1-particle[loop].life)*2);	//改变贴图坐标
			//给粒子赋予动态颜色
			changeColor(loop,1);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);	
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();

			if(Isplay)
			{
				//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。

				  axz=RandMaxMin(3600,0)/1800.0*PAI;			//爆炸的角度是全方位的
				  ay=RandMaxMin(GWspeed[loop],GWspeed[loop]*0.8);
					particle[loop].x=ay*cos(axz);
					particle[loop].y=PDD.star_y;
					particle[loop].z=ay*sin(axz);

				//粒子速度的改变，当生命再小于0.8后速度要顿时减下来
				if((particle[loop].life<0.95)&&(particle[loop].life>0.95-particle[loop].fade))
				{
				GWspeed[loop]+=PDD.speedMax*2;
				}
				else GWspeed[loop]+=PDD.speedMax/100;
				//粒子的生命减小
				particle[loop].life-=particle[loop].fade;
			}
			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				//赋予生命和衰减期
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_TEXTURE_2D);

	//////////////////////////////////////////////////////////////////
	//冲击波贴图
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);

	if(Isplay)
		PDD.speedMin*=1.25;

	glColor4f(1.0,1.0,1.0,speed/PDD.speedMin);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(PDD.speedMin,		PDD.star_y,	PDD.speedMin); 
		glTexCoord2d(0,1); glVertex3f(-PDD.speedMin,	PDD.star_y,	PDD.speedMin); 
		glTexCoord2d(1,0); glVertex3f(PDD.speedMin,		PDD.star_y,	-PDD.speedMin); 
		glTexCoord2d(0,0); glVertex3f(-PDD.speedMin,	PDD.star_y,	-PDD.speedMin);
	glEnd();

	
	glDisable(GL_TEXTURE_2D);
	//////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}

int CPrt_AB_GroudShockWave::LoadGLTextures(void)//给粒子贴图
{

	if(!CParticle::LoadGLTextures("地面冲击波烟雾.BMP",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("地面冲击波烟雾.BMP error!");return FALSE;}
	if(!CParticle::LoadGLTextures("地面冲击波.bmp",texture[1]))	//给1号材质装入贴图
	{AfxMessageBox("地面冲击波.bmp error!");return FALSE;}
	return true;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_AB_GroudShockWave::changeColor(int loop,int model)
{

	static GLfloat colors[5][3]=	//用于爆炸效果的5种颜色（Glfloat选值）
	{
		{1.0f,0.996f,0.824f},		//浅黄（几乎是白色的黄）
		{0.953f,0.757f,0.125f},		//黄色（偏橙色）
		{0.965f,0.627f,0.09f},		//暗黄色（深橙色）
		{0.843f,0.816f,0.784f},		//灰白色
		{0.0001f,0.0001f,0.0001f}	//黑色
	};
	int lifecolorset;				//生命颜色选取值

	switch(model)
	{
	case 0:	//给粒子赋予初始颜色
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];		// Select Red Rainbow Color
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];		// Select Red Rainbow Color
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];		// Select Red Rainbow Color
		break;
	case 1:
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.8f)lifecolorset=0;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=1;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.4f)lifecolorset=2;
		if(particle[loop].life<0.4f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=colors[lifecolorset][0];
		particle[loop].g=colors[lifecolorset][1];
		particle[loop].b=colors[lifecolorset][2];
		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,(0.5-fabs(particle[loop].life-0.5))*0.05);
		break;
	default:
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];	
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];	
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
//底波的实现
//********************************************************************************
////////////////////////////////////////////////////////////////////////////////////
int CPrt_AB_BaseWave::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//初始化所有粒子
	//粒子的速度
	PDD.speedMax=speed;
	PDD.speedMin=speed;
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				particle[loop].life=1.0f;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
				particle[loop].fade=0.003f*PDD.lifefate;	// 统一的生命减少量

				//出生位置，爆心
				particle[loop].x=PDD.star_x;
				particle[loop].y=PDD.star_y+RandMaxMin(0.1,0);
				particle[loop].z=PDD.star_z;

				speed=RandMaxMin(PDD.speedMax,PDD.speedMax/5.0);	//爆炸的速度
				  prtAngle[loop]=RandMaxMin(360,0)/180.0*PAI;			//爆炸的角度是全方位的
												//记录下转过角度好贴图
				particle[loop].xi=speed*cos(prtAngle[loop]);
				particle[loop].yi=0;
				particle[loop].zi=speed*sin(prtAngle[loop]);

				//给粒子赋予颜色
				changeColor(loop,0);			
		}
	return TRUE;// 初始化 OK
}


void CPrt_AB_BaseWave::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//底波的冲击波
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);
	for (loop=0;loop<PDD.MAX_PARTICLES/2;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			float x=particle[loop].x;
			float y=particle[loop].y;
			float z=particle[loop].z;
			//改变贴图坐标
			float a=(1.1-particle[loop].life)*2;	//贴图宽度
			//给粒子赋予动态颜色
			changeColor(loop,1);

			ExchangeShowXYZ(x,y,z,a);	//改变贴图坐标
			
			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();
		

		if(Isplay)
		{
			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			particle[loop].x+=particle[loop].xi/(PDD.slowdown*10);
			particle[loop].y+=particle[loop].yi/(PDD.slowdown*10);
			particle[loop].z+=particle[loop].zi/(PDD.slowdown*10);

			//粒子速度的改变，当生命再小于0.8后速度要顿时减下来
			if((particle[loop].life<0.95)&&(particle[loop].life>0.95-particle[loop].fade))
			{

			particle[loop].xi*=0.1;
			particle[loop].yi*=0.1;
			particle[loop].zi*=0.1;

			}

			//粒子的生命减小
			particle[loop].life-=particle[loop].fade;
		}
			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{

				//赋予生命和衰减期
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

	//////////////////////////////////////////////////////////////////
	//底波的烟雾
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	 
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);

	for (loop=PDD.MAX_PARTICLES/2;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			float x=particle[loop].x;				 
			float y=particle[loop].y;
			float z=particle[loop].z;
			
			//给粒子赋予动态颜色
			changeColor(loop,2);
			//改变贴图坐标
			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,(1.1-particle[loop].life));

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
				particle[loop].x+=particle[loop].xi/(PDD.slowdown*10);
				particle[loop].y+=particle[loop].yi/(PDD.slowdown*10);
				particle[loop].z+=particle[loop].zi/(PDD.slowdown*10);
				//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
				//粒子速度的改变，当生命再小于0.95后速度要顿时减下来

				if((particle[loop].life<0.9)&&(particle[loop].life>0.9-particle[loop].fade))
				{
				particle[loop].xi*=-0.05;
				particle[loop].yi=1/(particle[loop].x*particle[loop].x+particle[loop].z*particle[loop].z);			// Take Pull On Y Axis Into Account
				particle[loop].zi*=-0.05;
				}

				//粒子的生命减小
				particle[loop].life-=particle[loop].fade;
			}
			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				//赋予生命和衰减期
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	//////////////////////////////////////////////////////////
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}

int CPrt_AB_BaseWave::LoadGLTextures(void)//给粒子贴图
{

	if(!CParticle::LoadGLTextures("烟雾21.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("烟雾21.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("火焰4.bmp",texture[1]))	//给1号材质装入贴图
	{AfxMessageBox("火焰4.bmp error!");return FALSE;}
	return true;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_AB_BaseWave::changeColor(int loop,int model)
{

	static GLfloat colors[5][3]=	//用于爆炸效果的5种颜色（Glfloat选值）
	{
		{1.0f,0.996f,0.824f},		//浅黄（几乎是白色的黄）
		{0.953f,0.757f,0.125f},		//黄色（偏橙色）
		{0.965f,0.627f,0.09f},		//暗黄色（深橙色）
		{0.843f,0.816f,0.784f},		//灰白色
		{0.001f,0.001f,0.001f}			//黑色
	};
	int lifecolorset;					//生命颜色选取值

	switch(model)
	{
	case 0:	//给粒子赋予初始颜色
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];
		break;
	case 1:
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.8f)lifecolorset=0;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=1;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.4f)lifecolorset=2;
		if(particle[loop].life<0.4f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=colors[lifecolorset][0];
		particle[loop].g=colors[lifecolorset][1];
		particle[loop].b=colors[lifecolorset][2];
		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life*0.2);
		break;
	case 2:	//给粒子赋予初始颜色
		float g;
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.5f)
		{
			g=(1-particle[loop].life)*0.7;

		}
		if(particle[loop].life<0.5f)
		{
			g=particle[loop].life*0.7;
		}
			glColor4f(0.965f,0.627f,0.09f,g);
		break;
	default:
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];
		break;
	}
}

void CPrt_AB_BaseWave::ExchangeShowXYZ(float x,float y,float z,float a)
{
	
	float z0=z-PDD.star_z;
	float x0=x-PDD.star_x;
	if(x0==0)//斜率的特殊情况，特殊考虑
	{
		if(z<PDD.star_z)
		{
			ParShowXYZ[0][0]=0;//x+a;
			ParShowXYZ[0][1]=0;//y;
			ParShowXYZ[0][2]=0;//z+2*a;

			ParShowXYZ[1][0]=0;//x-a;
			ParShowXYZ[1][1]=0;//y;
			ParShowXYZ[1][2]=0;//z+2*a;

			ParShowXYZ[2][0]=0;//x+a;
			ParShowXYZ[2][1]=0;//y;
			ParShowXYZ[2][2]=0;//z-2*a;

			ParShowXYZ[3][0]=0;//x-a;
			ParShowXYZ[3][1]=0;//y;
			ParShowXYZ[3][2]=0;//z-2*a;
		}
		else
		{
			ParShowXYZ[3][0]=x+a;
			ParShowXYZ[3][1]=y;
			ParShowXYZ[3][2]=z+2*a;

			ParShowXYZ[2][0]=x-a;
			ParShowXYZ[2][1]=y;
			ParShowXYZ[2][2]=z+2*a;

			ParShowXYZ[1][0]=x+a;
			ParShowXYZ[1][1]=y;
			ParShowXYZ[1][2]=z-2*a;

			ParShowXYZ[0][0]=x-a;
			ParShowXYZ[0][1]=y;
			ParShowXYZ[0][2]=z-2*a;
		}
	}
	else if(z0==0)
		{
		if(x<=PDD.star_x)
			{
				ParShowXYZ[0][0]=x+2*a;
				ParShowXYZ[0][1]=y;
				ParShowXYZ[0][2]=z-a;

				ParShowXYZ[1][0]=x+2*a;
				ParShowXYZ[1][1]=y;
				ParShowXYZ[1][2]=z+a;

				ParShowXYZ[2][0]=x-2*a;
				ParShowXYZ[2][1]=y;
				ParShowXYZ[2][2]=z+a;

				ParShowXYZ[3][0]=x-2*a;
				ParShowXYZ[3][1]=y;
				ParShowXYZ[3][2]=z-a;
			}
			else
			{
				ParShowXYZ[2][0]=x+2*a;
				ParShowXYZ[2][1]=y;
				ParShowXYZ[2][2]=z-a;

				ParShowXYZ[3][0]=x+2*a;
				ParShowXYZ[3][1]=y;
				ParShowXYZ[3][2]=z+a;

				ParShowXYZ[0][0]=x-2*a;
				ParShowXYZ[0][1]=y;
				ParShowXYZ[0][2]=z+a;

				ParShowXYZ[1][0]=x-2*a;
				ParShowXYZ[1][1]=y;
				ParShowXYZ[1][2]=z-a;
			}
		}
	else //排除斜率的特殊情况后的计算,计算过程见笔记本
	{
		float k=z0/x0;
		float t=k/(k*k+1);
		float m1=z-k*x-a*sqrt(1+k*k);
		float m2=z-k*x+a*sqrt(1+k*k);
		float m3=z+(1/k)*x-2*a/k*sqrt(1+k*k);
		float m4=z+(1/k)*x+2*a/k*sqrt(1+k*k);

		int a,b,c,d;
		if(x0>0)
		{a=0;b=1;c=2;d=3;
		}
		else
		{
			a=3;b=2;c=1;d=0;
		}
				ParShowXYZ[a][0]=t*(m3-m1);
				ParShowXYZ[a][1]=y;
				ParShowXYZ[a][2]=k*t*(m3-m1)+m1;

				ParShowXYZ[b][0]=t*(m3-m2);
				ParShowXYZ[b][1]=y;
				ParShowXYZ[b][2]=k*t*(m3-m2)+m2;

				ParShowXYZ[c][0]=t*(m4-m1);
				ParShowXYZ[c][1]=y;
				ParShowXYZ[c][2]=k*t*(m4-m1)+m1;

				ParShowXYZ[d][0]=t*(m4-m2);
				ParShowXYZ[d][1]=y;
				ParShowXYZ[d][2]=k*t*(m4-m2)+m2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//爆核的实现
//=======================================================================================
/////////////////////////////////////////////////////////////////////////////////////////
int CPrt_AB_BombCore::Init(void)//初始化粒子系统（包括位置、速度、……）
{

	core_y=PDD.star_y;	//爆心的位置 Y轴
	corei=speed/80;		//爆心的速度 Y轴
	coreg=0;			//爆心的加速度 Y轴
	PDD.speedMax=speed;
	PDD.speedMin=corei;
	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				particle[loop].life=1.0;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
				particle[loop].fade=0.003f*PDD.lifefate;		// 统一的生命减少量

				//出生位置，爆心
				particle[loop].x=PDD.star_x;
				particle[loop].y=core_y;
				particle[loop].z=PDD.star_z;

				//粒子的速度
				speed=RandMaxMin(PDD.speedMax,PDD.speedMax/1.5);
				  prtAxz[loop]=RandMaxMin(360,0)/180.0*PAI;			//爆炸的角度是全方位的
				   prtAy[loop]=RandMaxMin(90,0)/180.0*PAI;
				particle[loop].xi=speed*cos(prtAxz[loop])*sin(prtAy[loop])*2;	
				particle[loop].yi=speed*cos(prtAy[loop]);						
				particle[loop].zi=speed*sin(prtAxz[loop])*sin(prtAy[loop])*2;			
				a[loop]=-1;
				//给粒子赋予颜色
				changeColor(loop,1);			
		}
	return TRUE;										// 初始化 OK
}


void CPrt_AB_BombCore::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	 
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glEnable(GL_TEXTURE_2D);
	if(Isplay)
	{
		core_y+=corei;	//控制核心的速度
		if(corei>0)corei+=coreg;
		else corei=0;
	}

	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			if(Isplay)
			{
				particle[loop].x+=particle[loop].xi/(PDD.slowdown*10);
				particle[loop].y+=particle[loop].yi/(PDD.slowdown*10)+corei;
				particle[loop].z+=particle[loop].zi/(PDD.slowdown*10);
				changeForce(loop);//改变粒子的位置、速度和加速度
			}
			float x=particle[loop].x;				 
			float y=particle[loop].y;
			float z=particle[loop].z;

			float R=sqrt((PDD.star_x-x)*(PDD.star_x-x)+(PDD.star_z-z)*(PDD.star_z-z));
			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,(R*0.5+0.02)*0.3);	//改变贴图坐标
			//给粒子赋予动态颜色
			changeColor(loop,1);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();

		
			//粒子的生命减小
			if(Isplay)
				particle[loop].life-=particle[loop].fade;

			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}

int CPrt_AB_BombCore::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("地面冲击波烟雾.BMP",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("地面冲击波烟雾.BMP error!");return FALSE;}
	return TRUE;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_AB_BombCore::changeColor(int loop,int model)
{

	static GLfloat colors[5][3]=	//用于爆炸效果的5种颜色（Glfloat选值）
	{
		{1.0f,0.996f,0.824f},		//浅黄（几乎是白色的黄）
		{0.953f,0.757f,0.125f},		//黄色（偏橙色）
		{0.965f,0.627f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.429f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.359f,0.09f},		//暗黄色（深橙色）

	};
	int lifecolorset;					//生命颜色选取值

	switch(model)
	{
	case 0:	//给粒子赋予初始颜色
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];		// Select Red Rainbow Color
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];		// Select Red Rainbow Color
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];		// Select Red Rainbow Color
		break;
	case 1:
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.9f)lifecolorset=0;
		if(particle[loop].life<0.9f&&particle[loop].life>=0.8f)lifecolorset=1;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=2;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;
		particle[loop].r=colors[lifecolorset][0];
		particle[loop].g=colors[lifecolorset][1];
		particle[loop].b=colors[lifecolorset][2];
		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,(particle[loop].life+0.2)*0.5);
		break;
	default:
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];	
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];	
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	}
}

void CPrt_AB_BombCore::changeForce(int loop)
{
	float R=sqrt((PDD.star_x-particle[loop].x)*(PDD.star_x-particle[loop].x)
		+(core_y-particle[loop].y)*(core_y-particle[loop].y)
		+(PDD.star_z-particle[loop].z)*(PDD.star_z-particle[loop].z));
	//////////////////////////////////////
	float	rotatespeed;	//控制翻滚速度
	rotatespeed=0.005;
	if(particle[loop].life<0.2)
		rotatespeed=0.002;

	if(particle[loop].life>0.98)
	{
		particle[loop].xg=0;
		particle[loop].yg=0;
		particle[loop].zg=0;

		particle[loop].xi+=particle[loop].xg;
		particle[loop].yi+=particle[loop].yg;
		particle[loop].zi+=particle[loop].zg;

	}
	else	if((particle[loop].life<=0.98)&&(particle[loop].life>=(0.99-particle[loop].fade)))
	{
		//将粒子速度降下来
			particle[loop].xi=0;
			particle[loop].yi=0;
			particle[loop].zi=0;

	}
	else 	if((particle[loop].life<(0.99-particle[loop].fade))&&(particle[loop].life>0.02))
	{
		if(particle[loop].y>=core_y)	//上半球翻滚速度
		{
			prtAy[loop]+=rotatespeed;
			particle[loop].x=PDD.star_x+R*cos(prtAxz[loop])*sin(prtAy[loop]);
			particle[loop].y=core_y+R*cos(prtAy[loop]);	
			particle[loop].z=PDD.star_z+R*sin(prtAxz[loop])*sin(prtAy[loop]);
		}
		else						//下半球翻滚速度。
		{
			if(a[loop]<0)a[loop]=R;	//只在第一次对球坐标系的半径进行赋值
			prtAy[loop]+=rotatespeed;
			R=sqrt(-a[loop]*a[loop]*cos(2*prtAy[loop]));
			particle[loop].x=PDD.star_x+R*cos(prtAxz[loop])*sin(prtAy[loop]);
			particle[loop].y=core_y+R*cos(prtAy[loop]);
			particle[loop].z=PDD.star_z+R*sin(prtAxz[loop])*sin(prtAy[loop]);
		}

			particle[loop].xi=0;
			particle[loop].yi=0;
			particle[loop].zi=0;
	}
	else
	{
		particle[loop].xi=0;
		particle[loop].yi=0;
		particle[loop].zi=0;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//00000000000000000000000000000000000000000000000000000000000000000000000000000000000000
//爆环的实现
//00000000000000000000000000000000000000000000000000000000000000000000000000000000000000
////////////////////////////////////////////////////////////////////////////////////////
int CPrt_AB_BombCircle::Init(void)//初始化粒子系统（包括位置、速度、……）
{

	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				particle[loop].life=1.0f;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
				particle[loop].fade=0.003f*PDD.lifefate;		// 统一的生命减少量

				//出生位置，爆心
				particle[loop].x=PDD.star_x;
				particle[loop].y=PDD.star_y;
				particle[loop].z=PDD.star_z;
				//粒子的速度

				  axz=RandMaxMin(360,0)/180.0*PAI;			//爆炸的角度是全方位的
				  ay=RandMaxMin(speed*2,speed);
				particle[loop].xi=ay*cos(axz);			
				particle[loop].zi=ay*sin(axz);
				particle[loop].yi=speed*0.005;

				PDD.speedMax=speed;
				PDD.speedMin=speed;
				//给粒子赋予颜色
				changeColor(loop,0);			
		}
	return TRUE;										// 初始化 OK
}


void CPrt_AB_BombCircle::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	 
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

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

			float R=sqrt((PDD.star_x-x)*(PDD.star_x-x)+(PDD.star_z-z)*(PDD.star_z-z));
			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,(R*0.5+2)*0.3);	//改变贴图坐标

			//给粒子赋予动态颜色
			changeColor(loop,1);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); 
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); 
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); 
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); 
			glEnd();

			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			if(Isplay)
			{

				particle[loop].x+=particle[loop].xi/(PDD.slowdown*100);	
				particle[loop].z+=particle[loop].zi/(PDD.slowdown*100);
				particle[loop].y+=particle[loop].yi;

				//粒子速度的改变，当生命再小于0.8后速度要顿时减下来
				if((particle[loop].life<0.80)&&(particle[loop].life>0.80-particle[loop].fade))
				{
				particle[loop].xi*=-0.15;
				particle[loop].zi*=-0.15;			
				}
				//粒子的生命减小
				particle[loop].life-=particle[loop].fade;
			}
			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				//赋予生命和衰减期
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_TEXTURE_2D);

	//////////////////////////////////////////////////////////////////
	//冲击波贴图
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);
	if(Isplay)
		PDD.speedMin*=1.05;//控制贴图扩大的加速度
	glColor4f(1.0,1.0,1.0,speed/PDD.speedMin+0.2);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(PDD.speedMin,		PDD.star_y+PDD.speedMin*0.08,	PDD.speedMin); // Top Right//0
		glTexCoord2d(0,1); glVertex3f(-PDD.speedMin,	PDD.star_y+PDD.speedMin*0.08,	PDD.speedMin); // Top Left	//1
		glTexCoord2d(1,0); glVertex3f(PDD.speedMin,		PDD.star_y+PDD.speedMin*0.08,	-PDD.speedMin); // Bottom Right//2
		glTexCoord2d(0,0); glVertex3f(-PDD.speedMin,	PDD.star_y+PDD.speedMin*0.08,	-PDD.speedMin); // Bottom Left//3
	glEnd();

	
	glDisable(GL_TEXTURE_2D);
	//////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}

int CPrt_AB_BombCircle::LoadGLTextures(void)//给粒子贴图
{
	//烟雾贴图
	if(!CParticle::LoadGLTextures("地面冲击波烟雾.BMP",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("地面冲击波烟雾.BMP error!");return FALSE;}
	//冲击波贴图
	if(!CParticle::LoadGLTextures("爆环冲击波.bmp",texture[1]))	//给0号材质装入贴图
	{AfxMessageBox("爆环冲击波.bmp error!");return FALSE;}
	return TRUE;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_AB_BombCircle::changeColor(int loop,int model)
{

	static GLfloat colors[5][3]=				//用于爆炸效果的5种颜色（Glfloat选值）
	{
		{1.0f,0.996f,0.824f},		//浅黄（几乎是白色的黄）
		{0.953f,0.757f,0.125f},		//黄色（偏橙色）
		{0.965f,0.627f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.429f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.359f,0.09f}		//暗黄色（深橙色）
	};
	int lifecolorset;					//生命颜色选取值

	switch(model)
	{
	case 0:	//给粒子赋予初始颜色
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	case 1:
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.9f)lifecolorset=0;
		if(particle[loop].life<0.9f&&particle[loop].life>=0.8f)lifecolorset=1;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=2;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=colors[lifecolorset][0];
		particle[loop].g=colors[lifecolorset][1];
		particle[loop].b=colors[lifecolorset][2];
		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,(0.5-fabs(particle[loop].life-0.5))*1.0);
		break;
	default:
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//00000000000000000000000000000000000000000000000000000000000000000000000000000000000000
//爆柱的实现
//00000000000000000000000000000000000000000000000000000000000000000000000000000000000000
////////////////////////////////////////////////////////////////////////////////////////
int CPrt_AB_BombColumn::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	PDD.AngleYStar=speed/2;//柱的粗度
	//*******************************************************************************
	//初始化所有粒子
		for (loop=0;loop<1999;loop++)
		{
				particle[loop].active=true;					// 使所有的粒子都被激活
				//赋予生命和衰减期
				particle[loop].life=1.0f;					// 给所有的粒子最长的生命，1.0F最长，0.0f最短
				particle[loop].fade=0.003f*PDD.lifefate;	// 统一的生命减少量
				//出生位置，爆心
				 axz=RandMaxMin(360,0)/180.0*PAI;			//爆炸的角度是全方位的
				  ay=RandMaxMin(speed*2,speed/2);
				particle[loop].x=PDD.star_x+ay*cos(axz);
				particle[loop].y=PDD.star_y;					
				particle[loop].z=PDD.star_z+ay*sin(axz);	
				//粒子的速度

				particle[loop].xi=(PDD.star_x-particle[loop].x)*0.02;			
				particle[loop].zi=(PDD.star_z-particle[loop].z)*0.02;
				particle[loop].yi=0;

				//给粒子赋予颜色
				changeColor(loop,1);			
		}
	return TRUE;										// 初始化 OK
}


void CPrt_AB_BombColumn::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	 
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

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

			float R=sqrt((PDD.star_x-x)*(PDD.star_x-x)+(PDD.star_z-z)*(PDD.star_z-z));
			CParticle::ExchangeShowXYZ(Tar2Cam,x,y,z,0.8);	//改变贴图坐标

			//给粒子赋予动态颜色
			changeColor(loop,1);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();

			//粒子下一次在屏幕出现的位置，即现在的位置加上速度的增量的位置。
			if(Isplay)
			{

				particle[loop].x+=particle[loop].xi/(PDD.slowdown*100);
				particle[loop].z+=particle[loop].zi/(PDD.slowdown*100);
				particle[loop].y+=particle[loop].yi/(PDD.slowdown*100);

				R=sqrt((PDD.star_x-particle[loop].x)*(PDD.star_x-particle[loop].x)
					+(PDD.star_z-particle[loop].z)*(PDD.star_z-particle[loop].z));
				if(R<=PDD.AngleYStar)//如果半径小于粗度，则收缩缓慢
				{
					particle[loop].xi=(PDD.star_x-particle[loop].x)*0.3;			
					particle[loop].zi=(PDD.star_z-particle[loop].z)*0.3;
					particle[loop].yi=speed;
				}
				else
				{
				particle[loop].xi=(PDD.star_x-particle[loop].x)*0.4;			
				particle[loop].zi=(PDD.star_z-particle[loop].z)*0.4;
				particle[loop].yi=1/R*1.8;
				}
				particle[loop].life-=particle[loop].fade;
			}
			//没有必要让它重生
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)
			{
				//赋予生命和衰减期
				particle[loop].active=false;
				PDD.diecount++;//死亡粒子数增加
				if(PDD.diecount>=PDD.MAX_PARTICLES)//如果粒子全死了则这个粒子源也就死了
					PDD.dead=true;
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);							// 启用深度测试(WWWWWWWWWWWWWWW)
	glDisable(GL_BLEND);								// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();

}

int CPrt_AB_BombColumn::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("地面冲击波烟雾.BMP",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("地面冲击波烟雾.BMP error!");return FALSE;}
	return TRUE;
}


//更改粒子的颜色loop是粒子的编号，model为染色模式
void CPrt_AB_BombColumn::changeColor(int loop,int model)
{

	static GLfloat colors[5][3]=	//用于爆炸效果的5种颜色（Glfloat选值）
	{
		{1.0f,0.996f,0.824f},		//浅黄（几乎是白色的黄）
		{0.953f,0.757f,0.125f},		//黄色（偏橙色）
		{0.965f,0.627f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.429f,0.09f},		//暗黄色（深橙色）
		{0.965f,0.359f,0.09f}		//暗黄色（深橙色）
	};
	int lifecolorset;					//生命颜色选取值

	switch(model)
	{
	case 0:	//给粒子赋予初始颜色
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	case 1:
		
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.9f)lifecolorset=0;
		if(particle[loop].life<0.9f&&particle[loop].life>=0.8f)lifecolorset=1;
		if(particle[loop].life<0.8f&&particle[loop].life>=0.6f)lifecolorset=2;
		if(particle[loop].life<0.6f&&particle[loop].life>=0.2f)lifecolorset=3;
		if(particle[loop].life<0.2f)lifecolorset=4;

		particle[loop].r=colors[lifecolorset][0];
		particle[loop].g=colors[lifecolorset][1];
		particle[loop].b=colors[lifecolorset][2];
		float g;
		if(particle[loop].life<=1.0f&&particle[loop].life>=0.5f)
		{
			g=(1-particle[loop].life)*1.0;

		}
		if(particle[loop].life<0.5f)
		{
			g=particle[loop].life*1.0;
		}
		//给每个粒子赋予色彩，用它的生命值作为Alpha,可以控制其的明暗
		glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,g);
		break;
	default:
		particle[loop].r=colors[loop*(5/PDD.MAX_PARTICLES)][0];	
		particle[loop].g=colors[loop*(5/PDD.MAX_PARTICLES)][1];	
		particle[loop].b=colors[loop*(5/PDD.MAX_PARTICLES)][2];	
		break;
	}
}
