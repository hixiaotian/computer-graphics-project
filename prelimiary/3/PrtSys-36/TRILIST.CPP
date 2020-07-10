// TriList.cpp: implementation of the CTriList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dsObject.h"
#include "TriList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriList::CTriList()
{
	free = 0;
	numobjects = 0;
	maxobjects = 20000;
}

CTriList::~CTriList()
{
	for (unsigned long i=0; i <numobjects; i++)
	{
		delete objects[i];
	}
}

BOOL CTriList::add(CTriObject * _object)
{
	if (numobjects <= maxobjects)
	{
		objects[free] = _object;
		free ++;
		numobjects++;
		return TRUE;
	}
	else return FALSE;
}

void CTriList::drawGL(BOOL &Isline,GLfloat &scale)
{
	for (unsigned long i=0; i <numobjects; i++)
	{
		objects[i]->drawGL(Isline,scale);
	}	
}



void CTriList::Init()
{
	free = 0;
	numobjects = 0;
	maxobjects = 20000;
}

void CTriList::doAfterMath()
{
	for (unsigned long i=0; i <numobjects; i++)
	{
		objects[i]->applyNormals();
	}
}

void CTriList::removeAllObjects()
{
	for (unsigned long i=0; i <numobjects; i++)
	{
		delete objects[i];
	}
	numobjects = 0;
	free = 0;
}

CTriObject* CTriList::getObjectByName(char * name)
{
	char* objname;
	for (unsigned long i=0; i <numobjects; i++)
	{
		objects[i]->getName(objname);
		if(strcmp(name, objname) == 0) return objects[i];
	}
	
	return NULL;
}
