// TriObject.cpp: implementation of the CTriObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dsObject.h"
#include "TriObject.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriObject::CTriObject()
{
	x = y = z = nx = ny = nz = NULL;
	matfaces = faces = NULL;
	materials = NULL;
	numvertices = 0;
	numnormals = 0;
	numfaces = 0;
	nummatfacesapplied = 0;
	i = 0;
	nummaterials=0;
	pivot[0] = 0.0f;
	pivot[1] = 0.0f;
	pivot[2] = 0.0f;
	pivotrot[0] = 0.0f;
	pivotrot[1] = 0.0f;
	pivotrot[2] = 0.0f;
	pivotrot[3] = 0.0f;
	normalapplied = FALSE;
	materialsapplied = FALSE;
}

CTriObject::~CTriObject()
{
	delete [] x;
	delete [] y;
	delete [] z;
	delete [] nx;
	delete [] ny;
	delete [] nz;
	delete [] faces;
	delete [] matfaces;
	delete [] materials;
}

void CTriObject::applyNormals()
{
	if (numfaces == 0) return;
	delete [] nx;
	delete [] ny;
	delete [] nz;
	nx = new float[numfaces/3];
	ny = new float[numfaces/3];
	nz = new float[numfaces/3];
	if( nx==NULL || ny==NULL || nz==NULL )
	{
		delete [] nx;
		delete [] ny;
		delete [] nz;
		normalapplied = FALSE;
		return;
	}

	float normal[3];
	for ( unsigned long i=0 ; i<numfaces/3 ; i++ )	//get to work
	{
		CalcNormal(3*i, normal);
		ReduceToUnit(normal);
		nx[i] = normal[0];
		ny[i] = normal[1];
		nz[i] = normal[2];
	}
	
	normalapplied = TRUE;
}

void CTriObject::CalcNormal(unsigned long entry, float out [ 3 ])
{
	float v1[3],v2[3];

	// 根据空间三个点计算两个矢量值
	v1[0] = x[ faces[entry] ] - x[ faces[entry+1] ];
	v1[1] = y[ faces[entry] ] - y[ faces[entry+1] ];
	v1[2] = z[ faces[entry] ] - z[ faces[entry+1] ];
	
	v2[0] = x[ faces[entry+1] ] - x[ faces[entry+2] ];
	v2[1] = y[ faces[entry+1] ] - y[ faces[entry+2] ];
	v2[2] = z[ faces[entry+1] ] - z[ faces[entry+2] ];

	// 计算法向矢量
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void CTriObject::ReduceToUnit(float vector [ 3 ])
{
	float length;
	
	// 计算矢量的长度		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	if(length == 0.0f)
		length = 1.0f;

	// 单位化矢量
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

void CTriObject::drawGL(BOOL &Isline,GLfloat &scale)
{

	unsigned short mode;
	if(Isline)mode=GL_LINE_STRIP;
	else mode=GL_TRIANGLES;//_STRIP;



	glPushMatrix();
	if (normalapplied)
	{
		unsigned long j;
		glBegin(mode);
			for (i=0; i <numfaces/3; i++)
			{
				j = 3*i;
				if(materialsapplied) glColor4f( materials[matfaces[i]].diffuseColor[0], materials[matfaces[i]].diffuseColor[1], materials[matfaces[i]].diffuseColor[2], 1/materials[matfaces[i]].transparency );
				else glColor3f( 0.0f, 0.0f, 1.0f );
				::glNormal3f( nx[i], ny[i], nz[i]);
				::glVertex3f( x[faces[j]]*scale ,  y[faces[j]]*scale  ,  z[faces[j]]*scale);
				::glVertex3f( x[faces[j+1]]*scale,  y[faces[j+1]]*scale,  z[faces[j+1]]*scale);
				::glVertex3f( x[faces[j+2]]*scale,  y[faces[j+2]]*scale,  z[faces[j+2]]*scale);
			}
		glEnd();
	}
	else
	{
		glBegin(mode);
			for (i=0; i <numfaces; i+=3)
			{
				if(materialsapplied) glColor3f( materials[matfaces[i/3]].diffuseColor[0], materials[matfaces[i/3]].diffuseColor[1], materials[matfaces[i/3]].diffuseColor[2] );
				else glColor3f( 0.0f, 0.0f, 1.0f );
				glVertex3f( x[faces[i]]*scale  ,  y[faces[i]]*scale  ,  z[faces[i]]*scale);
				glVertex3f( x[faces[i+1]]*scale,  y[faces[i+1]]*scale,  z[faces[i+1]]*scale);
				glVertex3f( x[faces[i+2]]*scale,  y[faces[i+2]]*scale,  z[faces[i+2]]*scale);
			}
		glEnd();
	}

	glPopMatrix();	
}


BOOL CTriObject::isDataAlive()
{
	return ( x != NULL && y != NULL && z != NULL );
}


unsigned long CTriObject::addMaterial(tMaterial * _material)
{
	tMaterial * tmp;
	nummaterials++;
	
	if (nummaterials==1)
	{
		materials = new tMaterial[nummaterials];
		if (materials == NULL)
		{
			nummaterials = 0;
			materialsapplied = FALSE;
			return -1;
		}
	}
	else
	{	
		tmp = materials;	
		materials = new tMaterial[nummaterials];
		if (materials == NULL)
		{
			delete [] tmp;
			nummaterials = 0;
			materialsapplied = FALSE;
			return -1;
		}
		for (i=0; i<nummaterials-1;i++)	
		{
			materials[i] = tmp[i]; //将材质拷贝到新建的数组中
		}
	}

	//在数组中插入新材质
	materials[nummaterials-1].ambientColor[0] = _material->ambientColor[0];
	materials[nummaterials-1].ambientColor[1] = _material->ambientColor[1];
	materials[nummaterials-1].ambientColor[2] = _material->ambientColor[2];

	materials[nummaterials-1].diffuseColor[0] = _material->diffuseColor[0];
	materials[nummaterials-1].diffuseColor[1] = _material->diffuseColor[1];
	materials[nummaterials-1].diffuseColor[2] = _material->diffuseColor[2];

	materials[nummaterials-1].specularColor[0] = _material->specularColor[0];
	materials[nummaterials-1].specularColor[1] = _material->specularColor[1];
	materials[nummaterials-1].specularColor[2] = _material->specularColor[2];

	materials[nummaterials-1].emissiveColor[0] = _material->emissiveColor[0];
	materials[nummaterials-1].emissiveColor[1] = _material->emissiveColor[1];
	materials[nummaterials-1].emissiveColor[2] = _material->emissiveColor[2];

	materials[nummaterials-1].shininess = _material->shininess;
	materials[nummaterials-1].transparency = _material->transparency;
	
	materialsapplied = TRUE;

	return (nummaterials -1); 
}


void CTriObject::setId(unsigned long _id)
{
	id = _id;
}




