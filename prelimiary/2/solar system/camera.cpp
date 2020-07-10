#include "camera.h"
#include "glut.h"
#include "stdio.h"

camera::camera()
    : m_leftkey(false), m_rigtkey(false),
      m_dx(0), m_dy(0), m_dz(0),
      m_rx(25), m_ry(2)
{

}

camera::~camera()
{
}


//响应键盘输入
void camera::keyfunc(int key)
{
    switch (key)
    {
    case 'A' :
    case 'a' :
        {
            m_dx -= 50;
        }
        break;
    case 'D' :
    case 'd' :
        {
            m_dx += 50;
        }
        break;
    case 'W' :
    case 'w' :
        {
            m_dz += 50;
        }
        break;
    case 'S' :
    case 's' :
        {
            m_dz -= 50;
        }
        break;
    default :
        break;
    }
}

//响应鼠标输入
void camera::mousefunc(int button, int state, int x, int y)
{
    switch (button)
    {
    case 0 :
        m_leftkey = !state;
        break;
    case 1 :
        break;
    case 2 :
        m_rigtkey = !state;
        break;
    default :
        break;
    }

    m_px = x;
    m_py = y;
}

void camera::motion(int x, int y)
{
    if (m_leftkey)
    {
        m_dx += x - m_px;
        m_dy += m_py - y;
    }

    if (m_rigtkey)
    {
        m_ry += (m_px - x) / 5;
        m_rx += (y - m_py) / 5; 
    }

    m_px = x;
    m_py = y;
}

void camera::update()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,4000, 0,0,0, 0,1,0);
    glTranslatef(m_dx, m_dy, m_dz);
    glRotatef(m_rx, 1, 0 ,0);
    glRotatef(m_ry, 0, 1, 0);

}
