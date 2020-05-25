
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


CPrt_Demo_BIT::CPrt_Demo_BIT()
{

}

CPrt_Demo_BIT::~CPrt_Demo_BIT()
{

}


void CPrt_Demo_BIT::GetInitInfo(void)						//获得初始化数据
{
	//主要是位图参数的设置
	PDD.MAX_PARTICLES=5000;
	PDD.star_x=0;
	PDD.star_y=0;
	PDD.star_z=0;

	PDD.lifefate=3.0f;						// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=2.0f;						// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）
	PDD.speedMax=150.0;						// 速度的最大值
	PDD.speedMin=50.0;						// 速度的最小值

	PDD.GetXg=0.0;							//粒子受到的重力场（矢量）
	PDD.GetYg=2.0;
	PDD.GetZg=0.0;

	PDD.AngleYStar=0;						// Y轴的张角大小
	PDD.AngleYEnd=180;
	
	PDD.AngleXZStar=0;						// XZ平面的投影张角大小
	PDD.AngleXZEnd=360;	

	PDD.TextModel=0;						//贴图样式
	PDD.TextWidth=3.0;						//贴图大小

	PDD.pram[0]=0.1;						//缩放位图比例,数值越大图像越大

	/////////////////////////////////////////////////
	//小卫星的属性设置
	PDD.SonPDD.MAX_PARTICLES=100;
	PDD.SonPDD.lifefate=2.0;
	PDD.SonPDD.slowdown=0.5;

	PDD.SonPDD.GetXg=0;
	PDD.SonPDD.GetYg=0;
	PDD.SonPDD.GetZg=0;

	PDD.SonPDD.speedMax=10;
	PDD.SonPDD.speedMin=1;

	PDD.SonPDD.TextModel=0;
	PDD.SonPDD.TextWidth=0.8;

	PDD.pram[1]=8.0;			//卫星环绕半径
	/////////////////////////////////////////////////

	Init();
}

void CPrt_Demo_BIT::Init(void)
{
	//装载材质
	LoadGLTextures();

	FrameCount=2728;		//从0帧开始。每运行一次draw，帧数增加一
	FrameENDCount=5030;		//结束帧
	CartoonEnd=false;		//没有自动演示结束
}


////////////////////////////////////////////////////////////////////////
//总控显示
void CPrt_Demo_BIT::draw(CCamEye &camera,bool &Isplay,bool &DemoAutoFinish)
{
	/////////////////////////////////////////////////
	//各个动画演示的起始和结束帧数，在起始的第一帧对其进行初始化。
	unsigned int Snow[2]={1400,1850};	//雪花演示起始、结束帧。
	unsigned int Rain[2]={1800,2600};	//雨演示起始、结束帧。
	unsigned int F16[2]={2730,4501};	//F16战机演示起始、结束帧。
	unsigned int BITlogo[2]={10,3500};	//校徽演示起始、结束帧。

	//标题字的起始和结束帧
	unsigned int Text[5][2]={{2000,2099},		//0－北京理工大学
							{2100,2229},		//1-2004毕业设计
							{2230,2329},		//2-kingwise
							{2330,2429},		//3-王喆
							{2500,2599}
							};

	float px;//临时计算量具体怎么用以程序为准
	float py;//临时计算量
	float pz;//临时计算量	
	int i;//临时循环变量
	if(FrameCount==2728)
	{
		camera.SetTargetPos(0,0,0);		//设置目标中心
		camera.SetEyePos(0,0,10);		//设置相机位置
		glClearColor(0,0,0,0.5);		//清除屏幕
	}

	for(i=0;i<=2;i++)
		Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机位置
	/////////////////////////////////////////////////
	//战斗机出现
	if(FrameCount==F16[0])
	{
		Init_Plane();//初始化战斗机和其火焰
		camera.SetEyePos(-1000,10,0);//设置摄像机
		for(i=0;i<=2;i++)
			Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
	}

	if((FrameCount>F16[0])&&(FrameCount<=F16[1]))//战斗机的动画，持续到飞机于导弹相撞为止
	{
		//pz=（结束－起始）×/(Text[0][1]-Text[0][0])*(FrameCount-Text[0][0])+起始
		//px用来控制飞机向前飞
		//px=(-500-(-1000))/(float)(F16[1]-F16[0])*(float)(FrameCount-F16[0])-1000;
		//py用来控制飞机向下降
		draw_world();//画出天与地
		if(FrameCount<=(100+F16[0]))//使用100帧用于飞机向下降
		{
			px=(-990-(-1000))/100.0*(float)(FrameCount-F16[0])-1000;
			py=(8.5-(11))/100.0*(float)(FrameCount-F16[0])+11;
			glPushMatrix();
			glTranslatef(px,py,0);			//移动粒子源中心位置
				Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
				Demo_Fire.draw(Tar2Cam,Isplay);
			glPopMatrix();
		}

		if( (FrameCount>(100+F16[0])) && (FrameCount<(200+F16[0])) )//使用100帧用于战斗机垂直向前飞
		{
			px=(-850-(-990))/100.0*(float)(FrameCount-F16[0]-100)-990;
			py=8.5;
			glPushMatrix();
			glTranslatef(px,py,0);			//移动粒子源中心位置
				Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
				Demo_Fire.draw(Tar2Cam,Isplay);
			glPopMatrix();
		}


		//使用300帧完成摄像机盯着飞机横跨屏幕，同时飞机做翻滚动作。
		if( (FrameCount>=(200+F16[0])) && (FrameCount<(500+F16[0])) )//使用300帧用于战斗机垂直向前飞
		{

			//重新设置飞机在（-1000，0，0）的位置飞到800
			px=(-800-(-1000))/300.0*(float)(FrameCount-F16[0]-200)-1000;
			py=0;
			camera.SetTargetPos(px,py,0);//设置摄像机的焦点
			camera.SetEyePos(-900,10,10);//设置摄像机
			for(i=0;i<=2;i++)
				Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
			D_r_data.Angle_x+=2;

			glPushMatrix();
			glTranslatef(px,py,0);			//移动粒子源中心位置
				Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
				Demo_Fire.draw(Tar2Cam,Isplay);
			glPopMatrix();
		}

		//导弹出现初始化导弹
		if(FrameCount==(500+F16[0]))
		{
			for(i=0;i<=5;i++)
			{
				Demo_Missile[i].Init();//初始化导弹。
				Demo_Missile[i].pos[0]=RandMaxMin(1000,990);//x
				Demo_Missile[i].pos[1]=RandMaxMin(2.5,-2.5);//y
				Demo_Missile[i].pos[2]=RandMaxMin(2.5,-2.5);//z
				Demo_Missile[i].speed[0]=RandMaxMin(3,3);//速度X
			}
			camera.SetTargetPos(990,0,0);//设置摄像机的焦点
			camera.SetEyePos(1000,50,50);//设置摄像机
			for(i=0;i<=2;i++)
				Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
		}
		//导弹动画,500帧
		if( (FrameCount>(500+F16[0])) && (FrameCount<(1000+F16[0])) )
		{
			for(i=0;i<=5;i++)
			{
				Demo_Missile[i].draw(Tar2Cam,Isplay,Demo_Missile[i].pos[0],Demo_Missile[i].pos[1],Demo_Missile[i].pos[2]);
				Demo_Missile[i].pos[0]-=Demo_Missile[i].speed[0];
			}
			camera.SetTargetPos(Demo_Missile[0].pos[0],0,0);//设置摄像机的焦点
			pz=360.0/500.0*(float)(FrameCount-F16[0]-500);//使摄像机围着导弹转
			camera.SetEyePosA(10,90, pz);
			for(i=0;i<=2;i++)
				Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
		}

		if(FrameCount==(1000+F16[0]))//初始化飞机和导弹相遇时的镜头
		{
			for(i=0;i<=5;i++)
			{
				Demo_Missile[i].pos[0]=RandMaxMin(250,245);//重新分配导弹X坐标
				Demo_Missile[i].speed[0]=RandMaxMin(0.83,0.82);	//速度X
			}
			D_r_data.Angle_x=0;//使飞机角度恢复
			camera.SetTargetPos(0,0,0);//设置摄像机的焦点
			for(i=0;i<=2;i++)
				Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
		}
		//使用500帧描绘飞机于导弹相撞
		if( (FrameCount>(1000+F16[0])) && (FrameCount<(1500+F16[0])) )//导弹动画
		{
			//导弹动画，300帧描写飞机于导弹直撞
			if( (FrameCount>(1000+F16[0])) && (FrameCount<(1300+F16[0])) )
			{
				//重新设置飞机
				px=(-10-(-200))/300.0*(float)(FrameCount-F16[0]-1000)-200;

				//设置摄像机位置距离原点的位置
				py=(20-(500))/300.0*(float)(FrameCount-F16[0]-1000)+500;
				camera.SetEyePosA(py,80,90);//设置摄像机
				for(i=0;i<=2;i++)
					Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量

				//画飞机
				glPushMatrix();
				glTranslatef(px,0,0);			//移动粒子源中心位置
					Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
					Demo_Fire.draw(Tar2Cam,Isplay);
				glPopMatrix();

				//画导弹
				for(i=0;i<=5;i++)
				{
					Demo_Missile[i].draw(Tar2Cam,Isplay,Demo_Missile[i].pos[0],Demo_Missile[i].pos[1],Demo_Missile[i].pos[2]);
					Demo_Missile[i].pos[0]-=Demo_Missile[i].speed[0];
				}
			}
			//使用150帧表现黑客转
			if( (FrameCount>=(1300+F16[0])) 
				&&(FrameCount<(1450+F16[0]))
				)
			{
				//pz=（结束－起始）×/(Text[0][1]-Text[0][0])*(FrameCount-Text[0][0])+起始
				pz=(810-(90))/150*(float)(FrameCount-1300-F16[0])+90;
				camera.SetEyePosA(20,80, pz);
				for(i=0;i<=2;i++)
					Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量

				//画飞机
				glPushMatrix();
				glTranslatef(-10,0,0);			//移动粒子源中心位置
					Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
					Demo_Fire.draw(Tar2Cam,Isplay);
				glPopMatrix();

				//画导弹
				for(i=0;i<=5;i++)
				{
					Demo_Missile[i].draw(Tar2Cam,Isplay,Demo_Missile[i].pos[0],Demo_Missile[i].pos[1],Demo_Missile[i].pos[2]);
				}

			}//黑客转

			//导弹动画，50帧描写飞机于导弹相撞
			if( (FrameCount>=(1450+F16[0])) && (FrameCount<(1500+F16[0])) )
			{
				//重新设置飞机
				px=(-5-(-10))/50.0*(float)(FrameCount-F16[0]-1450)-10;
				//画飞机
				glPushMatrix();
				glTranslatef(px,0,0);			//移动粒子源中心位置
					Demo_3DSobj.Draw(D_IsLine,D_scale,D_hide,D_t_data,D_r_data);
					Demo_Fire.draw(Tar2Cam,Isplay);
				glPopMatrix();
				//画导弹
				for(i=0;i<=5;i++)
				{
					Demo_Missile[i].draw(Tar2Cam,Isplay,Demo_Missile[i].pos[0],Demo_Missile[i].pos[1],Demo_Missile[i].pos[2]);
					Demo_Missile[i].pos[0]-=0.1;
				}
			}

		}//使用500帧描绘飞机于导弹相撞

		//展现原子弹
		if( FrameCount==(1500+F16[0])) //初始原子弹
		{
			Demo_AtomicBomb.GetInitInfo();
			
			camera.SetEyePos(0,-9, 20);
			camera.SetTargetPos(0,-10,0);//设置摄像机的焦点
				for(i=0;i<=2;i++)
					Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
					
		}
		if(  (FrameCount>(1500+F16[0])) && (FrameCount<(1800+F16[0]))) //演示原子弹
		{
			//pz=（结束－起始）×/(Text[0][1]-Text[0][0])*(FrameCount-Text[0][0])+起始
			pz=(50.0-(20.0))/300.0*(float)(FrameCount-1500-F16[0])+20.0;
			px=(5.0-(-10.0))/300.0*(float)(FrameCount-1500-F16[0])-10;
			camera.SetTargetPos(0,px,0);//设置摄像机的焦点
			camera.SetEyePos(0,-9, pz);
				for(i=0;i<=2;i++)
					Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
			Demo_AtomicBomb.draw(Tar2Cam,Isplay);
		}

	}//飞机动画和原子弹演示


	//=============================================
	//校徽动画
	if(FrameCount==(1790+F16[0]))//初始化礼花和BIT字幕
	{
		Init_BITlogo();	//初始化校徽
		Init_FireWork();//初始化礼花
		camera.SetEyePos(0,0, 10);
		camera.SetTargetPos(0,0,-20);//设置摄像机的焦点
		for(i=0;i<=2;i++)
					Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量
		glClearColor(0,0,0,0.5);		//清除屏幕
	}
	if(  (FrameCount>(1790+F16[0])) && (FrameCount<(2300+F16[0])) )
	{
		//pz=（结束－起始）×/(Text[0][1]-Text[0][0])*(FrameCount-Text[0][0])+起始
		pz=(810-(90))/500*(float)(FrameCount-1300-F16[0])+90;
		camera.SetEyePosA(30,80, pz);
		for(i=0;i<=2;i++)
		Tar2Cam[i]=camera.eyePos[i]-camera.target[i];	//获得摄像机对准向量

		glPushMatrix();
		glTranslatef(0,0,-20);			//移动粒子源中心位置
			draw_BITlogo(Tar2Cam,Isplay);
		glPopMatrix();

		Demo_FireWork[0].draw(Tar2Cam,Isplay);
		Demo_FireWork[1].draw(Tar2Cam,Isplay);

	}

	//////////////////////////////////////////////////////////////////////////////
	FrameCount++;
	if(FrameCount>=FrameENDCount)
	{
		CartoonEnd=true;//演示结束
		DemoAutoFinish=true;
		camera.InitCamera();//初始摄像机
		glClearColor(0,0,0,0.5);		//清除屏幕
	}

}


//初始化校徽＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CPrt_Demo_BIT::Init_BITlogo(void)
{
	if(!Read_BMP_Graph("BIT.BMP"))
			AfxMessageBox("BIT.BMP Read ERROR!");

	//小卫星的初始化 4个
	for (loop=0;loop<4;loop++)
	{
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

		//指定出生地
		ex_Rot[loop][0]=RandMaxMin(135,45)/180.0f*PAI;
		ex_Rot[loop][1]=RandMaxMin(360,0)/180.0f*PAI;
		ret[loop]=1;
		explodePar[loop].PDD.star_x=PDD.star_x+PDD.pram[1]*sin(ex_Rot[loop][0])*cos(ex_Rot[loop][1]);
		explodePar[loop].PDD.star_y=PDD.star_y+PDD.pram[1]*cos(ex_Rot[loop][0]);
		explodePar[loop].PDD.star_z=PDD.star_z+PDD.pram[1]*sin(ex_Rot[loop][0])*sin(ex_Rot[loop][1]);

		explodePar[loop].texture[0]=&texture[0];
		//更新颜色
		for(int i=0;i<=4;i++)
		{
			explodePar[loop].PDD.PrtColor.Red[i]=1.0;
			explodePar[loop].PDD.PrtColor.Green[i]=0.1;
			explodePar[loop].PDD.PrtColor.Blue[i]=0.1;
		}
		explodePar[loop].Init();
	}
}


//校徽动画
void CPrt_Demo_BIT::draw_BITlogo(float *Tar2Cam,bool &Isplay)
{

	draw_PrtBMP(Tar2Cam,Isplay);//显示位图

	///////////////////////////////////////////////////

	for (loop=0;loop<4;loop++)
	{
		//显示粒子
		explodePar[loop].draw(Tar2Cam,Isplay,explodePar[loop].life);

		if(Isplay)
			{
			//控制y轴的反向
			if(ex_Rot[loop][0]>(135/180.0f*PAI) )
			{
				ex_Rot[loop][0]=135/180.0f*PAI;ret[loop]*=-1;
			}
			if( ex_Rot[loop][0]<(45/180.0f*PAI))
			{
				ex_Rot[loop][0]=45/180.0f*PAI;ret[loop]*=-1;
			}
			ex_Rot[loop][0]+=0.01*ret[loop];
			ex_Rot[loop][1]+=0.05;

			explodePar[loop].PDD.star_x=PDD.star_x+PDD.pram[1]*sin(ex_Rot[loop][0])*cos(ex_Rot[loop][1]);
			explodePar[loop].PDD.star_y=PDD.star_y+PDD.pram[1]*cos(ex_Rot[loop][0]);
			explodePar[loop].PDD.star_z=PDD.star_z+PDD.pram[1]*sin(ex_Rot[loop][0])*sin(ex_Rot[loop][1]);

			}

	}

}

void CPrt_Demo_BIT::ReSatPrt(int loop)
{
		particle[loop].life=1.0f;
		particle[loop].fade=float(((rand()%100)/1000.0f+0.003f)*PDD.lifefate);

		GenRandGXGY();//产生一个有颜色的坐标GX，GY
		particle[loop].x=PDD.star_x+(GX-width/2)*PDD.pram[0];
		particle[loop].y=PDD.star_y+(GY-height/2)*PDD.pram[0];
		particle[loop].z=PDD.star_z;

		//读取位图颜色
		int R,G,B;
		R=(Graphic_1[GX][GY]>>16)&255;
		G=(Graphic_1[GX][GY]>>8)&255;
		B=Graphic_1[GX][GY]&255;
		particle[loop].r=R/255.0;
		particle[loop].g=G/255.0;
		particle[loop].b=B/255.0;

		//粒子的速度
		speed=RandMaxMin(PDD.speedMax,PDD.speedMin);//速度的大小
		   ay=RandMaxMin(PDD.AngleYEnd,PDD.AngleYStar)/180.0*PAI;//速度的方向
		  axz=RandMaxMin(PDD.AngleXZEnd,PDD.AngleXZStar)/180.0*PAI;
		particle[loop].xi=speed*sin(ay)*cos(axz);
		particle[loop].yi=speed*cos(ay);
		particle[loop].zi=speed*sin(ay)*sin(axz);

		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;

}

///////////////////////////////////////////////////
//初始化飞机和其火焰
void CPrt_Demo_BIT::Init_Plane(void)					
{
	if(!Demo_3DSobj.LoadFile("F16.3DS"))
		AfxMessageBox("F16.3DS Read ERROR!");//读取战斗机
		D_IsLine=FALSE;	//初始化飞机参数
		D_scale=0.1;
		D_hide=FALSE;
		D_t_data.x=5.7;
		D_t_data.y=0.0;
		D_t_data.z=0.0;
		D_r_data.Angle_x=0.0;
		D_r_data.Angle_z=90.0;
		D_r_data.Angle_y=0.0;


	Demo_Fire.PDD.MAX_PARTICLES=1200;
	Demo_Fire.PDD.star_x=0;
	Demo_Fire.PDD.star_y=0.0;
	Demo_Fire.PDD.star_z=0;
	Demo_Fire.PDD.lifefate=8.5f;							// 削减粒子的生命，数值约大，粒子的生命就越短
	Demo_Fire.PDD.slowdown=2.0f;							// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）
	Demo_Fire.PDD.speedMax=400.0;							// 速度的最大值
	Demo_Fire.PDD.speedMin=380.0;							// 速度的最小值

	Demo_Fire.PDD.AngleYStar=90;							// Y轴的张角大小
	Demo_Fire.PDD.AngleYEnd=90;
	
	Demo_Fire.PDD.AngleXZStar=179;							// XZ平面的投影张角大小
	Demo_Fire.PDD.AngleXZEnd=181;	
	
	Demo_Fire.PDD.GetXg=-46.0;								//粒子受到的重力场（矢量）
	Demo_Fire.PDD.GetYg=0.0;
	Demo_Fire.PDD.GetZg=0.0;

	////////////////////////////////////////////
	Demo_Fire.PDD.TextModel=0;						//贴图样式
	Demo_Fire.PDD.TextWidth=2.4551;						//贴图大小
	//火焰颜色设置
	Demo_Fire.PDD.PrtColor.Red[0]=1.0f;		//浅黄（几乎是白色的黄）
	Demo_Fire.PDD.PrtColor.Green[0]=0.996f;
	Demo_Fire.PDD.PrtColor.Blue[0]=0.953f;

	Demo_Fire.PDD.PrtColor.Red[1]=0.153f;	//黄色（偏橙色）
	Demo_Fire.PDD.PrtColor.Green[1]=0.157f;
	Demo_Fire.PDD.PrtColor.Blue[1]=1.0f;

	Demo_Fire.PDD.PrtColor.Red[2]=0.165f;	//暗黄色（深橙色）
	Demo_Fire.PDD.PrtColor.Green[2]=0.127f;
	Demo_Fire.PDD.PrtColor.Blue[2]=1.0f;

	Demo_Fire.PDD.PrtColor.Red[3]=0.843f;	//灰白色
	Demo_Fire.PDD.PrtColor.Green[3]=0.816f;
	Demo_Fire.PDD.PrtColor.Blue[3]=0.784f;

	Demo_Fire.PDD.PrtColor.Red[4]=0.001;	//黑色
	Demo_Fire.PDD.PrtColor.Green[4]=0.001f;
	Demo_Fire.PDD.PrtColor.Blue[4]=0.001f;

	Demo_Fire.Init();//初始化粒子系统（包括位置、速度、……）
	
}

void CPrt_Demo_BIT::Init_FireWork(void)				//初始化礼花
{
	/////////////////////////////////////////
	//礼花一
	Demo_FireWork[0].PDD.MAX_PARTICLES=50;			//100个小的粒子源
	Demo_FireWork[0].PDD.star_x=-10;				//起爆位置
	Demo_FireWork[0].PDD.star_y=7;
	Demo_FireWork[0].PDD.star_z=-20;
	Demo_FireWork[0].PDD.lifefate=0.5f;				// 削减粒子的生命，数值约大，粒子的生命就越短
	Demo_FireWork[0].PDD.slowdown=1.0f;				// 减慢粒子的速度，数值约大越慢，越小越快

	Demo_FireWork[0].PDD.speedMax=100.0;			// 爆炸速度的值
	Demo_FireWork[0].PDD.speedMin=100.0;
	Demo_FireWork[0].PDD.AngleYStar=0;				// Y轴的张角大小
	Demo_FireWork[0].PDD.AngleYEnd=180;
	
	Demo_FireWork[0].PDD.AngleXZStar=0;				// XZ平面的投影张角大小
	Demo_FireWork[0].PDD.AngleXZEnd=360;	

	Demo_FireWork[0].PDD.GetXg=0.0f;				//粒子受到的重力场（矢量）
	Demo_FireWork[0].PDD.GetYg=-15.0f;
	Demo_FireWork[0].PDD.GetZg=0.0f;

	Demo_FireWork[0].PDD.TextWidth=1.0;
	Demo_FireWork[0].PDD.TextModel=0;

	/////////////////////////////////////// //颜色
	Demo_FireWork[0].PDD.PrtColor.Red[0]=1.0f;
	Demo_FireWork[0].PDD.PrtColor.Green[0]=1.0f;
	Demo_FireWork[0].PDD.PrtColor.Blue[0]=1.0f;

	Demo_FireWork[0].PDD.PrtColor.Red[1]=1.0f;
	Demo_FireWork[0].PDD.PrtColor.Green[1]=0.706f;
	Demo_FireWork[0].PDD.PrtColor.Blue[1]=0.706f;

	Demo_FireWork[0].PDD.PrtColor.Red[2]=1.0f;
	Demo_FireWork[0].PDD.PrtColor.Green[2]=0.392f;
	Demo_FireWork[0].PDD.PrtColor.Blue[2]=0.392f;

	Demo_FireWork[0].PDD.PrtColor.Red[3]=0.843f;	
	Demo_FireWork[0].PDD.PrtColor.Green[3]=0.392f;
	Demo_FireWork[0].PDD.PrtColor.Blue[3]=0.784f;

	Demo_FireWork[0].PDD.PrtColor.Red[4]=0.196;
	Demo_FireWork[0].PDD.PrtColor.Green[4]=0.95f;
	Demo_FireWork[0].PDD.PrtColor.Blue[4]=0.390f;

	/////////////////////////////////////////////////////////////
	//以下是子粒子的属性
	Demo_FireWork[0].PDD.SonPDD.MAX_PARTICLES=80;
	Demo_FireWork[0].PDD.SonPDD.lifefate=2.0;				// 削减粒子的生命，数值约大，粒子的生命就越短
	Demo_FireWork[0].PDD.SonPDD.slowdown=0.5;				// 减慢粒子的速度，数值约大越慢，越小越快

	Demo_FireWork[0].PDD.SonPDD.GetXg=0;
	Demo_FireWork[0].PDD.SonPDD.GetYg=0;
	Demo_FireWork[0].PDD.SonPDD.GetZg=0;

	Demo_FireWork[0].PDD.SonPDD.speedMax=1;
	Demo_FireWork[0].PDD.SonPDD.speedMin=1;

	Demo_FireWork[0].Init();

	///////////////////////////////////////////////////////////
	//礼花二
	//////////////////////////////////////////////////////////
	Demo_FireWork[1].PDD.MAX_PARTICLES=50;			//100个小的粒子源
	Demo_FireWork[1].PDD.star_x=10;					//起爆位置
	Demo_FireWork[1].PDD.star_y=5;
	Demo_FireWork[1].PDD.star_z=-20;
	Demo_FireWork[1].PDD.lifefate=0.5f;				// 削减粒子的生命，数值约大，粒子的生命就越短
	Demo_FireWork[1].PDD.slowdown=1.0f;				// 减慢粒子的速度，数值约大越慢，越小越快

	Demo_FireWork[1].PDD.speedMax=100.0;			// 爆炸速度的值
	Demo_FireWork[1].PDD.speedMin=100.0;

	Demo_FireWork[1].PDD.AngleYStar=0;				// Y轴的张角大小
	Demo_FireWork[1].PDD.AngleYEnd=180;
	
	Demo_FireWork[1].PDD.AngleXZStar=0;				// XZ平面的投影张角大小
	Demo_FireWork[1].PDD.AngleXZEnd=360;	

	Demo_FireWork[1].PDD.GetXg=0.0f;				//粒子受到的重力场（矢量）
	Demo_FireWork[1].PDD.GetYg=-15.0f;
	Demo_FireWork[1].PDD.GetZg=0.0f;

	Demo_FireWork[1].PDD.TextWidth=1.0;
	Demo_FireWork[1].PDD.TextModel=0;

	/////////////////////////////////////// //颜色
	Demo_FireWork[1].PDD.PrtColor.Red[0]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Green[0]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Blue[0]=1.0f;

	Demo_FireWork[1].PDD.PrtColor.Red[1]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Green[1]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Blue[1]=0.501961f;

	Demo_FireWork[1].PDD.PrtColor.Red[2]=0.501961f;
	Demo_FireWork[1].PDD.PrtColor.Green[2]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Blue[2]=0.501961f;

	Demo_FireWork[1].PDD.PrtColor.Red[3]=0.501961f;	
	Demo_FireWork[1].PDD.PrtColor.Green[3]=0.501961f;
	Demo_FireWork[1].PDD.PrtColor.Blue[3]=1.0f;

	Demo_FireWork[1].PDD.PrtColor.Red[4]=1.0f;
	Demo_FireWork[1].PDD.PrtColor.Green[4]=0.501961f;
	Demo_FireWork[1].PDD.PrtColor.Blue[4]=0.501961f;

	////////////////
	//以下是子粒子的属性
	Demo_FireWork[1].PDD.SonPDD.MAX_PARTICLES=80;
	Demo_FireWork[1].PDD.SonPDD.lifefate=2.0;				// 削减粒子的生命，数值约大，粒子的生命就越短
	Demo_FireWork[1].PDD.SonPDD.slowdown=0.5;				// 减慢粒子的速度，数值约大越慢，越小越快

	Demo_FireWork[1].PDD.SonPDD.GetXg=0;
	Demo_FireWork[1].PDD.SonPDD.GetYg=0;
	Demo_FireWork[1].PDD.SonPDD.GetZg=0;

	Demo_FireWork[1].PDD.SonPDD.speedMax=1;
	Demo_FireWork[1].PDD.SonPDD.speedMin=1;

	Demo_FireWork[1].Init();
}


/////////////////////////////////////////////////////////////////////////////

int CPrt_Demo_BIT::LoadGLTextures(void)					//给粒子贴图
{
	if(!CParticle::LoadGLTextures("圆球.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("演示圆球.bmp error!");return FALSE;}

	if(!CParticle::LoadGLTextures("W_Ground.bmp",WorldTexture[0]))	//给0号材质装入贴图
	{AfxMessageBox("W_Ground.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("W_Sky1.bmp",WorldTexture[1]))	//给1号材质装入贴图
	{AfxMessageBox("W_Sky1.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("W_Sky2.bmp",WorldTexture[2]))	//给2号材质装入贴图
	{AfxMessageBox("W_Sky2.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("W_Sky3.bmp",WorldTexture[3]))	//给3号材质装入贴图
	{AfxMessageBox("W_Sky3.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("W_Sky4.bmp",WorldTexture[4]))	//给4号材质装入贴图
	{AfxMessageBox("W_Sky4.bmp error!");return FALSE;}
	if(!CParticle::LoadGLTextures("W_Sky5.bmp",WorldTexture[5]))	//给5号材质装入贴图
	{AfxMessageBox("W_Sky5.bmp error!");return FALSE;}

	return TRUE;
}



void CPrt_Demo_BIT::GenRandGXGY(void)			//产生一对点阵可显示坐标GX，GY
{
	while(1)
	{
		GX=rand()%(width-1);	//宽度上随便选一个点
		GY=rand()%(height-1);	//高度上随便选一个点
		if(Graphic_1[GX][GY]!=0)break;
	}
}

bool CPrt_Demo_BIT::Read_BMP_Graph(char *BMPFileName)		//读取BMP文件获得位图信息 目前只支持128*128 24bit 位图
{

	FILE *fp;	//BMP文件指针
	int fint;	//从中读取的每一个字节（char型）
	fp=fopen(BMPFileName,"rb+");//打开“Graph1.bmp”
	if(!fp)
	{
		AfxMessageBox("Missing BMP!");
		return false;
	}

	fseek(fp,0L,0);//找到BMP图像的开头，读取相关信息
	if(fgetc(fp)!='B'){AfxMessageBox("This Graph1.BMP is not a real BMP File!");return false;}
	if(fgetc(fp)!='M'){AfxMessageBox("This Graph1.BMP is not a real BMP File!");return false;}

	fseek(fp,0x12,0);//读取图像宽度
	fint=fgetc(fp);
	if(fint>256){AfxMessageBox("Width can't more than 256!");return false;}
	else width=fint;

	fseek(fp,0x16,0);//读取图像高度
	fint=fgetc(fp);
	if(fint>256){AfxMessageBox("Height can't more than 256!");return false;}
	else height=fint;
	
	fseek(fp,28L,0);//找到颜色位数地址,查看是否为真彩色
	fint=fgetc(fp);
	if(fint!=24){AfxMessageBox("This Graph1.BMP is not a 24 bit BMP File!");return false;}

	int i, j;//行列
	int r, g, b, c;			//红绿兰和颜色综合值

	fseek(fp,54L,0);		//图像信息从这里开始
	for (j=0; j<height; j++)	//高度
	{
		for (i=0; i<width; i++)	//宽度
		{
					b=fgetc(fp);
					g=fgetc(fp);
					r=fgetc(fp);
					c=(r<<16)+(g<<8)+b;
					Graphic_1[i][j]=c;		//我们规定纯黑色为透明色
		}
		if (j>0)
			for (i=0; i<(width%4); i++) fgetc(fp);

	}

	fclose(fp);//关闭文件“Graph1.bmp”

	//初始位图属性
	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
			particle[loop].active=false;
	return true;
}



//显示当前位图
void CPrt_Demo_BIT::draw_PrtBMP(float *Tar2Cam,bool &Isplay)
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform

	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[PDD.TextModel]);	//选择当前贴图样式
	glEnable(GL_TEXTURE_2D);

	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)				// Loop Through All The Particles
	{
		//在这个例子中，所有的粒子都是活着的，但是可以让其灭亡
		if (particle[loop].active)						// 查看粒子是否存活着
		{
			//粒子的位置
			CParticle::ExchangeShowXYZ(Tar2Cam,particle[loop].x,
				particle[loop].y,
				particle[loop].z,
				0.1*PDD.TextWidth);	//改变贴图坐标

			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life*0.8);

			//每个粒子的形状都是一个正方形，但这个正方形是由两个三角形构成的
			//这样可以加快 显卡 和 CPU 的运算速度，注意一下，对三角形的贴图定点顺序
			glBegin(GL_TRIANGLE_STRIP);				// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();								// Done Building Triangle Strip

			if(Isplay)//如果允许播放则进行粒子的更新
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
			particle[loop].life-=particle[loop].fade;		// Reduce Particles Life By 'Fade'
			}
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)					// If Particle Is Burned Out
			{
				//ReSatPrt(loop);
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
	/////////////////////////////////////////////////////
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);									// 启用混合模式
	glDepthMask(GL_TRUE);
	glPopMatrix();
}



void CPrt_Demo_BIT::draw_world(void)//画天与地
{

	float a=3000.0f;
	float het=3000.0f;//高度
	float bot=-400.0f;			//底部位置
	//天地六个贴图的坐标
	float WorldTP[6][4][3]={
		{{a,bot,a},{a,bot,-a},{-a,bot,a},{-a,bot,-a}},//地0123
		{{a,bot+het,a},{a,bot+het,-a},{a,bot,a},{a,bot,-a}},//天1
		{{-a,bot+het,a},{a,bot+het,a},{-a,bot,a},{a,bot,a}},//天2
		{{-a,bot+het,-a},{-a,bot+het,a},{-a,bot,-a},{-a,bot,a}},//天3
		{{a,bot+het,-a},{-a,bot+het,-a},{a,bot,-a},{-a,bot,-a}},//天4
		{{a,bot+het,a},{a,bot+het,-a},{-a,bot+het,a},{-a,bot+het,-a}}//天顶
	};
	//*******************************************************************************
	//循环显示（更新）每个粒子
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	for(int i=0;i<=5;i++)
	{
	glBindTexture(GL_TEXTURE_2D, WorldTexture[i]);	//选择当前贴图样式
	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2d(1,1); glVertex3f(WorldTP[i][0][0],	WorldTP[i][0][1],	WorldTP[i][0][2]); // Top Right//0
		glTexCoord2d(0,1); glVertex3f(WorldTP[i][1][0],	WorldTP[i][1][1],	WorldTP[i][1][2]); // Top Left	//1
		glTexCoord2d(1,0); glVertex3f(WorldTP[i][2][0],	WorldTP[i][2][1],	WorldTP[i][2][2]); // Bottom Right//2
		glTexCoord2d(0,0); glVertex3f(WorldTP[i][3][0],	WorldTP[i][3][1],	WorldTP[i][3][2]); // Bottom Left//3
	glEnd();								// Done Building Triangle Strip
	}
	/////////////////////////////////////////////////////
	glPopMatrix();
}












