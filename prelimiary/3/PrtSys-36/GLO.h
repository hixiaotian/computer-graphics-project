#if !defined(AFX_GLO_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_)
#define AFX_GLO_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_
//全局数据结构
//返回Max和Min之间的一个随机数
double	RandMaxMin(double Max,double Min);

////////////////////////////////////////////////////////////////////////////////
struct TraData		//平移的数据，就是3ds场景中心的位置
{
	float x;
	float y;
	float z;
};

struct RotData		//旋转数据，就是物体绕XYZ轴各自转动的角度。
{
	float Angle_x;
	float Angle_y;
	float Angle_z;
};

typedef struct 		//3ds物体对话框数据
{
	BOOL		IsLine;
	GLfloat		scale;
	BOOL		hide;
	TraData		t_data;
	RotData		r_data;
}DlgData_3ds;


//*******************************************************************************
typedef struct			// 一个粒子的结构
{
	bool	active;					// 是否活着？Active (Yes/No)
	float	life;					// 粒子的生命期Particle Life
	float	fade;					// 粒子的退色速度Fade Speed，其实就是Life每次的减少量

	//粒子的色彩参数
	float	r;					// 	红	
	float	g;					// 	绿
	float	b;					// 	蓝

	//粒子在屏幕上的位置
	float	x;
	float	y;
	float	z;

	//粒子的运动方向
	float	xi;
	float	yi;
	float	zi;

	//粒子的受力方向
	float	xg;	
	float	yg;	
	float	zg;	
}particleModel;	

//888888888888888888888888888888888888888888888888888888888888888888888
//用于粒子颜色的结构，传递粒子生命五个阶段的颜色。
typedef struct
{
	float	Red[6];
	float	Green[6];
	float	Blue[6];
}Dlg_COLOR_EDIT;

//子粒子对话框数据结构,用来和外界对话框沟通
typedef struct 							
{
	unsigned int MAX_PARTICLES;			//粒子的最大数（不可超过10000）

	float star_x;						//粒子源的位置
	float star_y;
	float star_z;

	float	lifefate;					// 削减粒子的生命，数值约大，粒子的生命就越短

	float	slowdown;					// 减慢粒子的速度，数值约大越慢，越小越快
	float	speedMax;					// 速度最大值（大于零！）与方向无关
	float	speedMin;					// 速度最小值（大于零！）

	float	AngleYStar;					//速度向量与Y轴的夹角Star<End;
	float	AngleYEnd;					//速度向量与Y轴的夹角(0~180)

	float	AngleXZStar;				//速度向量在XZ平面与X的夹角（从Y的正向看顺时针）(0~360)
	float	AngleXZEnd;					//Star<End;
	
	float	GetXg;						//粒子系统受到的空间重力场（矢量）
	float	GetYg;
	float	GetZg;

	/////////////////////////////////////////////////
	float	TextWidth;					//贴图大小
	int		TextModel;					//贴图样式（最多5种 0~4）
	Dlg_COLOR_EDIT	PrtColor;			//粒子的颜色
}DlgData_SonPRT;


//粒子对话框数据结构,用来和外界对话框沟通
typedef struct 							
{
	unsigned int MAX_PARTICLES;			//粒子的最大数（不可超过10000）

	float star_x;						//粒子源的位置
	float star_y;
	float star_z;

	float	lifefate;					// 削减粒子的生命，数值约大，粒子的生命就越短

	float	slowdown;					// 减慢粒子的速度，数值约大越慢，越小越快
	float	speedMax;					// 速度最大值（大于零！）与方向无关
	float	speedMin;					// 速度最小值（大于零！）

	float	AngleYStar;					//速度向量与Y轴的夹角Star<End;
	float	AngleYEnd;					//速度向量与Y轴的夹角(0~180)

	float	AngleXZStar;				//速度向量在XZ平面与X的夹角（从Y的正向看顺时针）(0~360)
	float	AngleXZEnd;					//Star<End;
	
	float	GetXg;						//粒子系统受到的空间重力场（矢量）
	float	GetYg;
	float	GetZg;

	/////////////////////////////////////////////////
	float	TextWidth;					//贴图大小
	int		TextModel;					//贴图样式（最多5种 0~4）
	Dlg_COLOR_EDIT	PrtColor;			//粒子的颜色
	DlgData_SonPRT	SonPDD;				//用来控制子粒子源的属性。

	/////////////////////////////////////////////////
	//高级控制
	bool dead;							//死亡标志，用来控制以后便于生成和杀死粒子源
										//在kingwise这个程序中，它用来管理统一的控制原子弹各类行动统一
	unsigned int	diecount;			//已死亡的粒子总数
	//////////////////////////////////////////////////
	//预留接口,提供给某些粒子模型的特殊需要
	float pram[2];						//在这个程序中，我们使用pram[0]作为雪花三叶玫瑰线的转动速度


}DlgData_PRT;

//用于粒子控制类的结构，传递各种开关信息
typedef struct
{
	bool	p_fire;			//火焰显示开关，true显示。
	bool	p_waterfall;	//瀑布显示开关
	bool	p_firework;		//礼花显示开关
	bool	p_atomicbomb;	//原子弹显示开关
	bool	p_snow;			//雪花显示开关
	bool	p_rain;			//雨显示开关
	bool	p_bmp;			//位图显示开关
	bool	IsPlay;			//播放暂停开关控制,TRUE 播放动画，FALSE暂停动画。
	bool	p_demo;			//演示开关
}Dlg_PRT_Control;

typedef struct
{
	int Width;
	int Height;
	bool IsFullScreen;	//是否全屏了
}ScreenDataStruct;

#endif // !defined(AFX_GLO_H__A617780B_5DDA_4730_951D_ECB307C8D1AF__INCLUDED_)


