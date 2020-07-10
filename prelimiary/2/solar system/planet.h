
#pragma once

// 星球类
#include "windows.h"
#include <gl/GL.h>
#include "vector"
#include "math.h"
using namespace std;

// 位置结构
struct point
{
    float x;  
    float y;
    float z;

	//构造函数
    point(float _x = 0, float _y = 0, float _z = 0)
        : x(_x), y(_y), z(_z)
    {
    }

    // 单位化,即变为该方向的单位向量)
    void normalize()
    {
        float len = sqrtf(x * x + y * y + z * z);   //计算距离

		//归一化，即用分量除长度
        x /= len;
        y /= len;
        z /= len;
    }
};

// 星体
class planet
{
public :
    planet(point _pos, point _axes_rev, float _size, float _rev_speed, float _rota_speed, bool _sun, const char texname[]);
    virtual ~planet();
    
    // 公转，自转角度更新
    void update(float timepass);
    // 绘制
    void draw();
    // 加入附属星体，比如8/9大行星都是太阳的附属星体，月亮是地球的附属星体
    void add_secondary(planet* p);
private :
    // 附属星体列表
    vector<planet*> m_secondary;
    // 位置
    point m_pos;
    // 大小
    float m_size;
    
	// 是否是太阳
    bool m_sun;
    
	// 公转速度
    float m_rev_speed;
	// 自转速度
    float m_rota_speed;
    // 公转角度
    float m_revolution;
    // 自转角度
    float m_rotation;
    // 自转轴
    point m_axes;
    // 公转轴
    point m_axes_rev;
    
	// 和太阳的距离
    float m_distance2sun;
    
	// 纹理
    GLuint m_tex;

};