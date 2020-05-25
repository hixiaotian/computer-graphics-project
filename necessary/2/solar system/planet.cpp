#include "planet.h"
#include "glut.h"
#include <string.h>
#include <stdio.h>
#include "io.h"
#include <gl\GLAux.h>
#include "math.h"
#pragma warning(disable : 4996)


planet::planet(point _pos, point _axes_rev, float _size, float _rev_speed, float _rota_speed, bool _sun, const char texname[])
    : m_pos(_pos), m_axes_rev(_axes_rev), m_size(_size), m_sun(_sun), m_tex(0),
      m_rev_speed(_rev_speed), m_rota_speed(_rota_speed),
     m_revolution(0),m_rotation(0), m_distance2sun(_pos.x),
      m_axes((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX)
{
    m_axes.normalize();

    if (access(texname, 0) == -1)
        return;
        
    // 加载贴图

    AUX_RGBImageRec* irec = auxDIBImageLoad(texname);

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, irec->sizeX, irec->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, irec->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (irec)
    {
        if (irec->data)
            free(irec->data);
        free(irec);
    }
}

planet::~planet()
{
    for (size_t i = 0; i < m_secondary.size(); ++i)
        delete[] m_secondary[i];
}

// 绘制球体函数
void draw_sphere()
{
    static GLuint uList = 0;
	int x=0;
	int i=0;
    if (uList == 0)
    {
        
        // 构建球体网格
        point** vs = new point*[37];
        for (int i = 0; i <= 36; ++i)
            vs[i] = new point[37];

        for (x = 0; x <= 36; ++x)
        {
            float ax = 3.141592654f * x * 10 / 180.f;
            point s = point(0, -sinf(ax), cosf(ax) );

            for (int y = 0; y <= 36; ++y)
            {
                point& v = vs[x][y];
                float ay = 3.141592654f * y * 10 / 180.f;
                v.x = s.x * cosf(ay) + s.z * sinf(ay);
                v.y = s.y;
                v.z = s.x * -sinf(ay) + s.z * cosf(ay);
            }
        }

        // 创建显示列表
        uList = glGenLists(1);
        glNewList(uList, GL_COMPILE);
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        for (x = 0; x < 36; ++x)
        {
            for (int y = 0; y < 36; ++y)
            {
                point& a = vs[x][y];
                point& b = vs[x][y + 1];
                point& c = vs[x + 1][y + 1];
                point& d = vs[x + 1][y];

                point na = a;
                point nb = b;
                point nc = c;
                point nd = d;

                na.normalize();
                nb.normalize();
                nc.normalize();
                nd.normalize();

                const float scal = 0.8f;

                glTexCoord2f((a.x * scal + 1) / 2, (a.y * scal + 1) / 2);
                glNormal3f(na.x, na.y, na.z);
                glVertex3f(a.x, a.y, a.z);

                glTexCoord2f((b.x * scal + 1) / 2, (b.y * scal + 1) / 2);
                glNormal3f(nb.x, nb.y, nb.z);
                glVertex3f(b.x, b.y, b.z);

                glTexCoord2f((c.x * scal + 1) / 2, (c.y * scal + 1) / 2);
                glNormal3f(nc.x, nc.y, nc.z);
                glVertex3f(c.x, c.y, c.z);

                glTexCoord2f((d.x * scal + 1) / 2, (d.y * scal + 1) / 2);
                glNormal3f(nd.x, nd.y, nd.z);
                glVertex3f(d.x, d.y, d.z);
            }
        }
        glEnd();
        glEndList();

        for (i = 0; i <= 36; ++i)
            delete[] vs[i];
        delete vs;

    }

    glCallList(uList);

}

void draw_circle()
{
    static GLuint uList = 0;

    if (uList == 0)
    {
        // 创建显示列表

        uList = glGenLists(1);
        glNewList(uList, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
        glColor3f(0.5, 0.5, 0.5);
        for (int x = 0; x <= 36; ++x)
        {
            float ay = 3.141592654f * x * 10 / 180.f;
            point v;
            v.x = sinf(ay);
            v.y = 0;
            v.z = cosf(ay);

            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
        glEndList();
    }

    glCallList(uList);
}

//更新自传角度和公转角度
void planet::update(float timepass)
{
   m_revolution += timepass * m_rev_speed;
   m_rotation += timepass * m_rota_speed;

    for (size_t i = 0; i < m_secondary.size(); ++i)
        m_secondary[i]->update(timepass);
}

void planet::draw()
{
    if (m_sun)
    {
        // 绘制各个附属太阳的行星的轨道
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        for (size_t i = 0; i < m_secondary.size(); ++i)
        {
            glPushMatrix();
            glScalef(m_secondary[i]->m_distance2sun, m_secondary[i]->m_distance2sun, m_secondary[i]->m_distance2sun);
            draw_circle();
            glPopMatrix();
        }

        glEnable(GL_TEXTURE_2D);

    }
    else
        glEnable(GL_LIGHTING);

    // 绘制星体
    glPushMatrix();
    glRotatef(m_revolution, m_axes_rev.x, m_axes_rev.y, m_axes_rev.z);
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glPushMatrix();
    glRotatef(m_rotation, m_axes.x, m_axes.y, m_axes.z);
    glScalef(m_size, m_size, m_size);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    draw_sphere();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    // 绘制附属星体
    for (size_t i = 0; i < m_secondary.size(); ++i)
        m_secondary[i]->draw();

    glPopMatrix();

    glDisable(GL_LIGHTING);

}

void planet::add_secondary(planet* p)
{
    m_secondary.push_back(p);
}
