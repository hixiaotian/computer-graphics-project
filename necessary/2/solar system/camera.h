#pragma once

//相机类的声明
class camera  
{
public:
	camera();
	virtual ~camera();
    
    // 更新相机位置
    void update();
    
	// 响应键盘消息
    void keyfunc(int key);
    
	// 响应鼠标消息
    void mousefunc(int button, int state, int x, int y);
    void motion(int x, int y);

private:
    // 是否左键按下
    bool m_leftkey;
    // 是否右键按下
    bool m_rigtkey;
    
	// 上一次x坐标
    int m_px;
    // 上一次y坐标
    int m_py;
    
	// x轴移动
    int m_dx;
    // y轴移动
    int m_dy;
    // z轴移
    int m_dz;
    
	// x轴旋转
    int m_rx;
    // y轴旋转
    int m_ry;
};

