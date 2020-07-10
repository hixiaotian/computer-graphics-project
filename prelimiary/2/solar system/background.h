#pragma once
#include "windows.h"
#include <gl/GL.h>

// ”Ó÷Ê±≥æ∞
class background  
{
public :
    void Init();
    void draw();

	background();
	virtual ~background();
private :
    // Œ∆¿Ì
    GLuint m_uText;
};

