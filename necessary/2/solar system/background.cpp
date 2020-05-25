
#include "background.h"
#include <gl\GLAux.h>
#include "io.h"
#pragma warning(disable : 4996)

background::background()
    : m_uText(0)
{
}

background::~background()
{
    glDeleteTextures(1, &m_uText);
}

void background::Init()
{
    if (access("tex\\bk.bmp", 0) == -1)
        return;
    
    AUX_RGBImageRec* irec = auxDIBImageLoad("tex\\bk.bmp");

	// 加载贴图
    glGenTextures(1, &m_uText);
    glBindTexture(GL_TEXTURE_2D, m_uText);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, irec->sizeX, irec->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, irec->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// 线形滤波
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	//释放资源
    if (irec)
    {
        if (irec->data)
            free(irec->data);
        free(irec);
    }
}

void background::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(FALSE);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, m_uText);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(0, 1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glDepthMask(TRUE);
}
