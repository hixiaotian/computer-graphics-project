#include <stdio.h>
#include <gl\GLAux.h>
#include "glut.h"
#include "math.h"
#include "camera.h"
#include "planet.h"
#include "background.h"

#pragma comment(lib, "glaux.lib")

// 窗口大小
#define VIEWWEIGHT 600
#define VIEWHIGHT  500 

// 相机
camera cameraobj;
// 宇宙背景
background bk;

// 太阳
planet* solar = 0;

void Display(void)
{
    static DWORD ptime = GetTickCount();
    DWORD ntime = GetTickCount();

    glClear(GL_DEPTH_BUFFER_BIT);
    // 更新相机
    cameraobj.update();
    // 绘制背景
    bk.draw();

    // 绘制太阳及其附属行星
    if (solar)
    {
        solar->update((float)(ntime - ptime) / 1000.f);
        glEnable(GL_TEXTURE_2D);
        solar->draw();
        glDisable(GL_TEXTURE_2D);
    }

    ptime = ntime;

    glutSwapBuffers();

}

void Run()
{	
    Display();
}

void KeyboardFunc(unsigned char key,int x, int y)
{
	cameraobj.keyfunc(key);
}

void Init()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    // 初始化光照
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float amb[4]={0.2,0.2,0.2,1};    //
    float dif[4]={1.0,1.0,1.0,1};    //方向向量
    float pos[4]={0,0,0,1};			 //原点


    glLightfv(GL_LIGHT0,GL_AMBIENT,amb);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);
    glLightfv(GL_LIGHT0,GL_SPECULAR,dif);
    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dif);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

    // 初始化背景
    bk.Init();

    // 初始化太阳系
    solar = new planet(point(0, 0, 0), point(0, 1, 0), 300, 0, 10, true, "tex\\sun.bmp");
    solar->add_secondary(new planet(point(400, 0, 0), point(0, 1, 0), 40, 10, 20, false, "tex\\p1.bmp"));
    solar->add_secondary(new planet(point(580, 0, 0), point(0, 1, 0), 60, 20, 30, false, "tex\\p2.bmp"));
    planet* ear = new planet(point(750, 0, 0), point(0, 1, 0), 55, 30, 40, false, "tex\\ear.bmp");
    ear->add_secondary(new planet(point(100, 0, 0), point(1, 1, 0), 10, 200, 40, false, "tex\\moon.bmp"));
    solar->add_secondary(ear);
    solar->add_secondary(new planet(point(890, 0, 0), point(0, 1, 0), 50, 35, 25, false, "tex\\p4.bmp"));
    solar->add_secondary(new planet(point(1200, 0, 0), point(0, 1, 0), 120, 15, 50, false, "tex\\p5.bmp"));
    solar->add_secondary(new planet(point(1500, 0, 0), point(0, 1, 0), 100, 50, 30, false, "tex\\p6.bmp"));
    solar->add_secondary(new planet(point(1800, 0, 0), point(0, 1, 0), 90, 20, 35, false, "tex\\p7.bmp"));
    solar->add_secondary(new planet(point(2050, 0, 0), point(0, 1, 0), 90, 40, 20, false, "tex\\p8.bmp"));

#if 0
    printf("太阳系模拟程序\n\n");
    printf("键盘 A   : 左移\n");
    printf("键盘 D   : 右移\n");
    printf("键盘 W   : 前移\n");
    printf("键盘 S   : 后移\n");
    printf("鼠标左键 : 平移\n");
    printf("鼠标右键 : 旋转\n");
#endif
}

void UnInit()
{
    if (solar)
        delete solar;
}

void Reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,GLfloat(w)/h,1,50000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
}

void MouseFunc(int button, int state, int x, int y)
{
    cameraobj.mousefunc(button, state, x, y);
}

void MotionFunc(int x, int y)
{
    cameraobj.motion(x, y);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(VIEWWEIGHT, VIEWHIGHT);
    glutCreateWindow("太阳系");
    glutDisplayFunc(&Display);
	glutIdleFunc(&Run);
	glutReshapeFunc(&Reshape);
	glutKeyboardFunc(&KeyboardFunc);
    glutMouseFunc(&MouseFunc);           //注册鼠标响应函数
    glutMotionFunc(&MotionFunc);
	
    Init();
    glutMainLoop();    //主消息循环
    UnInit();

    return 0;
}