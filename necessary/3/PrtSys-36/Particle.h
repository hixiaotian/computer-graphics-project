// Particle.h: interface for the CParticle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLE_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_)
#define AFX_PARTICLE_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_


#include "glo.h"	//引用全局总结构类
#include "cameye.h"	//引用摄像机类
#include "3dsObject.h"			// 3ds物体说明类
#include <math.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CParticle  
{
public:
	CParticle();
	virtual ~CParticle();
	
	void draw(float *camPos);					//显示粒子,引用当前相机位置

	//loop为每个粒子的唯一编号0～MAX_PARTICLES,model为各种模式
	void changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）
	void changeColor(int loop,int model);		//更改粒子的颜色
	void changeInitParticle(int model);

	int Init(void);								//初始化粒子系统（包括位置、速度、……）
												//调用ReSatPrt(int loop)
	void ReSatPrt(int loop){};					//具体初始化速度等等

	AUX_RGBImageRec *LoadBMP(char *Filename);					//打开贴图文件，并返回句柄
	int		LoadGLTextures(char *Filename,GLuint &texture);		//给texture材质赋予Filename贴图
	void	GetInitInfo(void);									//获得初始化数据


	//粒子对准摄像机贴图坐标转换函数
	void ExchangeShowXYZ(float *CameraPos,float x,float y,float z,float ADDwidth);
	float			ParShowXYZ[4][3];	//粒子的显示贴图坐标，其平面始终垂直于摄像机（0，1，2，3四点）
	particleModel	particle[10000];
	GLuint			texture[8];			//粒子的材质，可支持8种
	int				Particle_type;		//粒子的类型
	char			TextFileName[250];	//材质贴图的文件名
	GLuint	loop;						// 每个粒子的唯一标号
	DlgData_PRT		PDD;				//接受外界对话框数据的结构。

	//////////////////////////
	//随机产生值
	//////////////////////////
	float	speed;					// 粒子的速度（在speedMax和speedMin之间）
	float	ay;						// 速度向量与Y轴的夹角(0~AngleY)
	float	axz;					// 速度向量在XZ平面与X的夹角(0~AngleXZ)

};


//烟火类
class CPrt_Fire:public CParticle
{
public:
	CPrt_Fire();
	virtual ~CPrt_Fire();
	void	draw(float *camPos,bool Isplay);		//显示粒子
	//loop为每个粒子的唯一编号0～MAX_PARTICLES,model为各种模式
	void	changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）
	void	changeColor(int loop);					//更改粒子的颜色
	int		Init(void);								//初始化粒子系统（包括位置、速度、……）
	void	ReSatPrt(int loop);
	int		LoadGLTextures(void);					//给粒子贴图
	void	GetInitInfo(void);						//获得初始化数据
	void	InitColor(void);						//初始化粒子的颜色
};


//瀑布类
class CPrt_WaterFall:public CParticle
{
public:
	CPrt_WaterFall();
	virtual ~CPrt_WaterFall();
	void	draw(float *camPos,bool Isplay);	//显示粒子
	//loop为每个粒子的唯一编号0～MAX_PARTICLES,model为各种模式
	void	changeForce(int loop);				//更改粒子的加速度（速度由加速度而改变）
	void	changeColor(int loop);				//更改粒子的颜色
	int		Init(void);							//初始化粒子系统（包括位置、速度、……）
	void	ReSatPrt(int loop);					//具体初始化速度等等
	int		LoadGLTextures(void);				//给粒子贴图
	void	GetInitInfo(void);					//获得初始化数据
	void	InitColor(void);					//初始化粒子的颜色
};




//////////////////////////////////////////////////////////////////////////////////////
//爆炸子类
//////////////////////////////////////////////////////////////////////////////////////
class CPrt_ExplodePRT
{
public:
	CPrt_ExplodePRT();	//构造函数
	virtual ~CPrt_ExplodePRT();

	//显示粒子,引用当前相机位置,注意和以前的的显示函数不同，
	//因为自身也是对于父粒子源来说也是一个粒子，所以他的显示要和自身的生命相关。
	void draw(float *camPos,bool Isplay,float selflife);

	//loop为每个粒子的唯一编号0～MAX_PARTICLES,model为各种模式
	void changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）

	//更改粒子的颜色,不但但和自己所发出的小粒子的生命有关，还和自身粒子源的生命相关
	void changeColor(int loop,float selflife);	

	int Init(void);								//初始化粒子系统（包括位置、速度、……）
	void ReSatPrt(int loop);
	void	GetInitInfo(void);

	//粒子对准摄像机贴图坐标转换函数
	void ExchangeShowXYZ(float *CameraPos,float x,float y,float z,float ADDwidth);
	float ParShowXYZ[4][3];				//粒子的显示贴图坐标，其平面始终垂直于摄像机（0，1，2，3四点）
	particleModel particle[1500];
	GLuint	loop;						// 每个粒子的唯一标号
	GLuint	*texture[5];				//粒子的材质


	DlgData_PRT		PDD;				//接受外界对话框数据的结构。
	//////////////////////////
	//随机产生值
	//////////////////////////
	float	speed;					// 粒子的速度（在speedMax和speedMin之间）

	//粒子源的属性
	bool	active;					// 是否活着？Active (Yes/No)
	float	life;					// 粒子的生命期Particle Life
	float	fade;					// 粒子的退色速度Fade Speed，其实就是Life每次的减少量

	//粒子源的运动方向
	float	xi;	
	float	yi;
	float	zi;

	//粒子源的受力方向
	float	xg;
	float	yg;
	float	zg;
};

///////////////////////////////////////////////
//爆炸类(就是爆炸祖先)
///////////////////////////////////////////////
class CPrt_Explode
{
public:
	CPrt_Explode();	//构造函数
	virtual ~CPrt_Explode();
	void GetInitInfo(void);						//获得初始化数据
	void draw(float *camPos,bool Isplay);		//显示粒子
	void changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）
	int Init(void);								//初始化粒子系统（包括位置、速度、……）
	void ReSatPrt(int loop);
	GLuint	loop;								// 每个粒子的唯一标号
	CPrt_ExplodePRT	explodePar[200];			//最多支持200个小的爆炸粒子源
	AUX_RGBImageRec * CPrt_Explode::LoadBMP(char *Filename);
	int LoadGLTextures(void);									//给粒子贴图
	int LoadTextures(char *Filename,GLuint &texture);			//给粒子贴图
	GLuint	texture[5];							//粒子的材质

	DlgData_PRT		PDD;						//接受外界对话框数据的结构。
	//随机生成量
	float speed;				//颜色模式。
	

};


//////////////////////////////////////////////////
//礼花类，继承爆炸类。
//////////////////////////////////////////////////
class CPrt_FireWork:public CPrt_Explode
{
public:
	CPrt_FireWork();	//构造函数
	virtual ~CPrt_FireWork();
	void GetInitInfo(void);						//获得初始化数据
	void draw(float *Tar2Cam,bool Isplay);		//显示函数
};


///////////////////////////////////////////////////////////////
//原子弹效果类（合类）
///////////////////////////////////////////////////////////////

//*********************
//地面冲击波-为原子弹的从属类
//*********************
class CPrt_AB_GroudShockWave:public CParticle
{
public:
	CPrt_AB_GroudShockWave(){};			//构造函数
	virtual ~CPrt_AB_GroudShockWave(){};

	int Init(void);							//初始化粒子系统（包括位置、速度、……）
	void draw(float *Tar2Cam,bool Isplay);				//显示函数
	int LoadGLTextures(void);				//给粒子贴图
	void changeColor(int loop,int model);	//赋予颜色
	float	GWspeed[5000];	//每一个粒子的速度。
};

//*********************
//底波－为原子弹的从属类
//*********************
class CPrt_AB_BaseWave:public CParticle
{
public:
	CPrt_AB_BaseWave(){};	//构造函数
	virtual ~CPrt_AB_BaseWave(){};

	int Init(void);							//初始化粒子系统（包括位置、速度、……）
	void draw(float *Tar2Cam,bool Isplay);				//显示函数
	int LoadGLTextures(void);				//给粒子贴图
	void changeColor(int loop,int model);	//赋予颜色
	void ExchangeShowXYZ(float x,float y,float z,float a);//转换贴图坐标
	float prtAngle[10000];					//存放每一个粒子贴图的角度
};

//*********************
//爆核－为原子弹的从属类
//*********************
class CPrt_AB_BombCore:public CParticle
{
public:
	CPrt_AB_BombCore(){};	//构造函数
	virtual ~CPrt_AB_BombCore(){};

	int Init(void);							//初始化粒子系统（包括位置、速度、……）
	void draw(float *Tar2Cam,bool Isplay);				//显示函数
	int LoadGLTextures(void);				//给粒子贴图
	void changeColor(int loop,int model);	//赋予颜色
	void changeForce(int loop);				//改变重力
	float core_y;	//核的Y坐标
	float corei;	//核的速度 Y轴
	float coreg;	//核的加速度 Y轴

	float prtAy[5000];		//粒子的角度 Y轴
	float prtAxz[5000];		//XZ平面

	float a[5000];			//用来存放收缩半径
};

//*********************
//爆环－为原子弹的从属类
//*********************
class CPrt_AB_BombCircle:public CParticle
{
public:
	CPrt_AB_BombCircle(){};	//构造函数
	virtual ~CPrt_AB_BombCircle(){};

	int Init(void);							//初始化粒子系统（包括位置、速度、……）
	void draw(float *Tar2Cam,bool Isplay);				//显示函数
	int LoadGLTextures(void);				//给粒子贴图
	void changeColor(int loop,int model);	//赋予颜色
};

//*********************
//爆柱－为原子弹的从属类
//*********************
class CPrt_AB_BombColumn:public CParticle
{
public:
	CPrt_AB_BombColumn(){};	//构造函数
	virtual ~CPrt_AB_BombColumn(){};

	int Init(void);							//初始化粒子系统（包括位置、速度、……）
	void draw(float *Tar2Cam,bool Isplay);				//显示函数
	int LoadGLTextures(void);				//给粒子贴图
	void changeColor(int loop,int model);	//赋予颜色
};


//##################################################
//原子弹的真实类－由它来实现最终的效果
//##################################################
class CPrt_AtomicBomb
{
public:
	CPrt_AtomicBomb();	//构造函数
	virtual ~CPrt_AtomicBomb();

	void GetInitInfo(void);						//获得初始化数据
	void draw(float *Tar2Cam,bool Isplay);		//显示函数
	void Init(void);							//初始化函数
	int LoadGLTextures(void);					//给粒子贴图

	//--------------------------------------------------------
	DlgData_PRT		PDD;			//接受外界对话框数据的结构。
	//-------------------------------------------------------

	//从属类的实例生成
	CPrt_AB_GroudShockWave	kw_GroudShockWave;	//地面冲击波
	CPrt_AB_BaseWave		kw_BaseWave;		//底波
	CPrt_AB_BombCore		kw_BombCore;		//爆核
	CPrt_AB_BombCircle		kw_BombCircle;		//爆环
	CPrt_AB_BombColumn		kw_BombColumn;		//爆柱

	//-----------------------------------------------------
	//屏幕的生命,控制屏幕的亮度
	float screenLife;	//最大为1，最小为0
	float screenLifeFate;//生命消减率
};

//*****************************************************************
//雪雨类——实现下雪或下雨的效果
//*****************************************************************
class CPrt_Snow:public CParticle
{
public:
	CPrt_Snow();	//构造函数
	virtual ~CPrt_Snow();

	int Init(void);								//初始化粒子系统（包括位置、速度、……）
	void draw(float *camPos,bool Isplay);		//显示粒子
	//loop为每个粒子的唯一编号0～MAX_PARTICLES,model为各种模式
	void changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）
	void changeColor(int loop);		//更改粒子的颜色
	void changeInitParticle(int model);
	void ReSatPrt(int loop);
	int LoadGLTextures(void);					//给粒子贴图
	void GetInitInfo(void);						//获得初始化数据
	//雨粒子粒子对准摄像机贴图坐标转换函数
	void RainExchangeShowXYZ(float *CameraPos,float x,float y,float z,float ADDwidth);

	float PrtSnowAngle[10000];					//用来存放每个雪花粒子的三叶玫瑰线的转过角度
	float PrtSnowRadius[10000];					//用来存放每个雪花粒子的三叶玫瑰线的半径a
	float oldHeight;							//用来存放雪场原来的高度，当高度变高时需要重置所有粒子
	float oldnum;							//三叶玫瑰线角度的增量,使用PDD.pram[0]来代替
};

/////////////////////////////////////////////////////////////////////////
//		下面是演示特效的各类，一般不具备可操控能力
///////////////////////////////////////////////////////////////////////////
//小导弹类
class CPrt_Missile  
{
public:
	CPrt_Missile(){};
	virtual ~CPrt_Missile(){};
	void Init(void);						//初始化
	
	//显示函数，带有坐标
	void draw(float *Tar2Cam,bool &Isplay,float x,float y,float z);	
	void drawModel(void);//画导弹模型

	CPrt_Fire			RearFire;		//尾烟
	float pos[3];//位置
	float speed[3];//速度，在这个演示中只需用到X轴的速度
};


///////////////////////////////////////////////
//		演示控制总类
///////////////////////////////////////////////
class CPrt_Demo_BIT:public CParticle
{
public:
	CPrt_Demo_BIT();	//构造函数
	virtual ~CPrt_Demo_BIT();

	void Init(void);							//初始化粒子系统
	void InitGraphic(void);						//初始化图案位图

	//============================================================
	//显示的控制
	void draw(CCamEye &camera,bool &Isplay,bool &DemoAutoFinish);		//根据帧数总控显示粒子，引用摄像机，方便切换镜头
	void draw_BITlogo(float *Tar2Cam,bool &Isplay);		//显示BITlogo

	//根据文件显示位图,参数是BMP文件名，开始帧，持续长度（帧数）
	void draw_PrtBMP(float *Tar2Cam,bool &Isplay);//显示当前程序的位图
	void draw_world(void);//画天与地

	//=======================================================
	//各种效果初始化
	void Init_BITlogo(void);				//初始化BITlogo。
	void Init_Plane(void);					//初始化飞机和其火焰
	void Init_FireWork(void);				//初始化礼花

	//loop为每个粒子的唯一编号0～MAX_PARTICLES,
	void ReSatPrt(int loop);
	int LoadGLTextures(void);				//给粒子贴图
	void GetInitInfo(void);					//获得初始化数据
	void Read_TXT_Graph(void);				//读取TXT文件获得位图信息
	bool Read_BMP_Graph(char *BMPFileName);	//读取BMP文件获得位图信息 目前只支持128*128 24bit 位图
											//成功返回true
	//bool Read_BMP_Graph(LPCTSTR lpszPathName);
	void GenRandGXGY(void);					//产生一对点阵可显示坐标GX，GY

	//================================================================
	//图像点阵数组我们最多支持256×256象素 24真色彩
	int Graphic_1[256][256];				//256*256点阵的位图
	int GX,GY;								//可显示的点阵坐标
	int width,height;						//图像的宽度和高度

	CPrt_ExplodePRT	explodePar[4];			//最多支持4个小卫星。
	float	ex_Rot[4][2];					//记录四个小卫星的转过角度,和方向
	int		ret[4];							//小卫星的运动方向

	//==============================================================
	//屏幕显示参数
	unsigned int FrameCount;				//屏幕帧数控制从0开始
	unsigned int FrameENDCount;				//结束帧
	float	Tar2Cam[3];						//由目标指向摄像机的方向向量位置
	bool	CartoonEnd;						//演示是否结束，自动演示完成才结束
	//===================================================
	//粒子对象
	CPrt_Fire			Demo_Fire;			//火焰类实现飞机的尾焰
	CPrt_Missile		Demo_Missile[6];	//导弹类，作为一个粒子
	CPrt_AtomicBomb		Demo_AtomicBomb;	//原子弹类
	CPrt_FireWork		Demo_FireWork[2];	//礼花类
	//===================================================
	//3DS物体，主要是飞机。
	C3dsObject	Demo_3DSobj;		//3DS物体对象,读取F16飞机
	BOOL		D_IsLine;			//3DS物体对象控制数据
	float		D_scale;
	BOOL		D_hide;
	TraData		D_t_data;
	RotData		D_r_data;
	GLuint		WorldTexture[6];	//世界贴图，主要是天和地，0为地。1234为四周的天，5为顶盖天
};
////////////////////////////////////////////



//Bmp粒子位图类
class CPrt_BMP:public CParticle
{
public:
	CPrt_BMP();
	virtual ~CPrt_BMP();
	void	draw(float *camPos,bool Isplay);		//显示位图粒子
	void	changeForce(int loop);					//更改粒子的加速度（速度由加速度而改变）
	void	ReSatPrt(int loop);
	int		LoadGLTextures(void);					//给粒子贴图
	int		Init(void);								//初始化粒子系统（包括位置、速度、……）
	void	GetInitInfo(void);						//获得初始化数据

	bool Read_BMP_Graph(char *BMPFileName);	//读取BMP文件获得位图信息 目前只支持128*128 24bit 位图
											//成功返回true
	void GenRandGXGY(void);					//产生一对点阵可显示坐标GX，GY

	//================================================================
	//图像点阵数组我们最多支持256×256象素 24真色彩
	int Graphic_1[256][256];				//256*256点阵的位图
	int GX,GY;								//可显示的点阵坐标
	int width,height;						//图像的宽度和高度
	char BMP_FileName[256];					//图像文件名

};

////////////////////////////////////////////

//使用要有先有后！！
//粒子控制类，也就是由他来统一控制所有的粒子系统，方便和界面接口
class CParticleControl
{
public:
	CParticleControl(){}
	virtual ~CParticleControl(){}
	void	GetInitInfo(void);					//获得初始化数据
	void	draw(CCamEye &camera,Dlg_PRT_Control	&ptrCtrl);		//显示粒子,使用当前摄像机
	float	Tar2Cam[3];							//由目标指向摄像机的方向向量位置
	CPrt_Fire			m_prtFire;			//火焰类
	CPrt_WaterFall		m_prtWaterFall;		//瀑布类
	CPrt_FireWork		m_prtFireWork;		//礼花类
	CPrt_AtomicBomb		m_prtAtomicBomb;	//原子弹类
	CPrt_Snow			m_prtSnow;			//雪类
	CPrt_BMP			m_prtBMP;			//位图粒子类
};




#endif // !defined(AFX_PARTICLE_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_)
