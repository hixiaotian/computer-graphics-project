
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

CPrt_BMP::CPrt_BMP()
{

}

CPrt_BMP::~CPrt_BMP()
{

}
//读取BMP文件获得位图信息 目前只支持256*256 24bit 位图
bool CPrt_BMP::Read_BMP_Graph(char *BMPFileName)		
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
	for (j=0; j<height;j++)	//高度
	{
		for (i=0; i<width; i++)	//宽度
		{
					b=fgetc(fp);
					g=fgetc(fp);
					r=fgetc(fp);
					c=(r<<16)+(g<<8)+b;
					Graphic_1[i][j]=c;		//我们规定纯黑色为透明色
		}
		if (j>=0)
			for (i=0; i<(width%4); i++) fgetc(fp);

	}

	fclose(fp);//关闭文件“Graph1.bmp”

	//初始位图属性
	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
			particle[loop].active=false;

	return true;
}

//产生一对点阵可显示坐标GX，GY
void CPrt_BMP::GenRandGXGY(void)			
{
	while(1)
	{
		GX=rand()%(width-1);	//宽度上随便选一个点
		GY=rand()%(height-1);	//高度上随便选一个点
		if(Graphic_1[GX][GY]!=0)break;
	}
}



void CPrt_BMP::draw(float *Tar2Cam,bool Isplay)//显示粒子
{
	//*******************************************************************************
	//循环显示（更新）每个粒子
	if(PDD.dead)//有了文件更新
	{
		PDD.dead=false;
		if(!Read_BMP_Graph(BMP_FileName))
		{
			MessageBox(NULL,"位图文件读取失败!装载默认位图!","致命错误",MB_OK|MB_ICONSTOP);

			//AfxMessageBox("位图文件读取失败!装载默认位图!");
			if(!Read_BMP_Graph("kingwise.bmp"))
				AfxMessageBox("请确认 Kingwise.bmp 是否存在! 程序出现意外!");
		}
		
	}
	glPushMatrix();
	glEnable(GL_BLEND);									// 启用混合模式
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, texture[PDD.TextModel]);	//选择当前贴图样式
	glEnable(GL_TEXTURE_2D);
	glTranslatef(PDD.star_x,PDD.star_y,PDD.star_z);			//移动粒子源中心位置

	for (loop=0;loop<PDD.MAX_PARTICLES;loop++)
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
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(1,1); glVertex3f(ParShowXYZ[0][0],	ParShowXYZ[0][1],	ParShowXYZ[0][2]); // Top Right//0
				glTexCoord2d(0,1); glVertex3f(ParShowXYZ[1][0],	ParShowXYZ[1][1],	ParShowXYZ[1][2]); // Top Left	//1
				glTexCoord2d(1,0); glVertex3f(ParShowXYZ[2][0],	ParShowXYZ[2][1],	ParShowXYZ[2][2]); // Bottom Right//2
				glTexCoord2d(0,0); glVertex3f(ParShowXYZ[3][0],	ParShowXYZ[3][1],	ParShowXYZ[3][2]); // Bottom Left//3
			glEnd();

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
			particle[loop].life-=particle[loop].fade;
			}
			//当粒子的生命结束，要让其重生，否则粒子数便减少了，达不到循环的效果。
			if (particle[loop].life<0.0f)					
			{
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

void CPrt_BMP::changeForce(int loop)//更改粒子的加速度（速度由加速度而改变）
{
		//粒子的初始受力方向(重力加速度)
		particle[loop].xg=PDD.GetXg;
		particle[loop].yg=PDD.GetYg;
		particle[loop].zg=PDD.GetZg;
}


void CPrt_BMP::ReSatPrt(int loop)
{
		particle[loop].life=1.0f;
		particle[loop].fade=float(((rand()%100)/1000.0f+0.003f)*PDD.lifefate);

		GenRandGXGY();//产生一个有颜色的坐标GX，GY
		particle[loop].x=PDD.star_x+(GX-width/2)*PDD.pram[0];
		particle[loop].y=PDD.star_y+(GY-height/2)*PDD.pram[0];
		particle[loop].z=PDD.star_z+RandMaxMin(PDD.pram[1]/2.0,-PDD.pram[1]/2.0);

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
		changeForce(loop);
}


int CPrt_BMP::Init(void)//初始化粒子系统（包括位置、速度、……）
{
	//装载材质
	if (!LoadGLTextures())
	{
		return FALSE;
	}	
	for (loop=0;loop<9999;loop++)
	{
		particle[loop].active=false;					// 使所有的粒子都不被激活
	}
	return TRUE;										// 初始化 OK
}


int CPrt_BMP::LoadGLTextures(void)//给粒子贴图
{
	if(!CParticle::LoadGLTextures("圆球.bmp",texture[0]))	//给0号材质装入贴图
	{AfxMessageBox("圆球.bmp error!");return FALSE;}
	return true;
}


void CPrt_BMP::GetInitInfo(void)//获得初始化数据
{

	//主要是位图参数的设置
	PDD.MAX_PARTICLES=7799;
	PDD.star_x=0;
	PDD.star_y=0;
	PDD.star_z=0;

	PDD.lifefate=9.4f;						// 削减粒子的生命，数值约大，粒子的生命就越短
	PDD.slowdown=2.8f;						// 减慢粒子的速度，数值约大越慢，越小越快（大于零的数）
	PDD.speedMax=150.0;						// 速度的最大值
	PDD.speedMin=50.0;						// 速度的最小值

	PDD.GetXg=0.0;							//粒子受到的重力场（矢量）
	PDD.GetYg=2.0;
	PDD.GetZg=0.0;

	PDD.AngleYStar=0;						//Y轴的张角大小
	PDD.AngleYEnd=180;
	
	PDD.AngleXZStar=0;						//XZ平面的投影张角大小
	PDD.AngleXZEnd=360;	

	PDD.TextModel=0;						//贴图样式
	PDD.TextWidth=3.0;						//贴图大小

	PDD.pram[0]=0.1;						//缩放位图比例,数值越大图像越大
	PDD.pram[1]=0.1;						//图像的厚度
	PDD.dead=false;							//用于检测是否有文件读入更新
	strcpy(BMP_FileName,"kingwise.bmp");
	if(!Read_BMP_Graph(BMP_FileName))
			AfxMessageBox("位图文件读取失败!");
	Init();//初始化粒子系统（包括位置、速度、……）
}

