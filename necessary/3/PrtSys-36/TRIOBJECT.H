// TriObject.h: interface for the CTriObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIOBJECT_H__922F85A8_F87E_4F9B_B49D_5B1175163166__INCLUDED_)
#define AFX_TRIOBJECT_H__922F85A8_F87E_4F9B_B49D_5B1175163166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "glStructures.h"

class CTriObject  
{
public:
	CTriObject();
	virtual ~CTriObject();

	void setId(unsigned long _id);
	void drawGL(BOOL &Isline,GLfloat &scale);
	void applyNormals();

	//基本操作
	void setX( float* _x, unsigned long num )		{x = _x; numvertices = num;};
	void setY( float* _y, unsigned long num )		{y = _y; numvertices = num;};
	void setZ( float* _z, unsigned long num )		{z = _z; numvertices = num;};
	void getX( float*& _x, unsigned long& num )	{_x = x; num = numvertices;};
	void getY( float*& _y, unsigned long& num )	{_y = y; num = numvertices;};
	void getZ( float*& _z, unsigned long& num )	{_z = z; num = numvertices;};

	void setXN( float* _nx, unsigned long num )	{nx = _nx; numnormals = num;};
	void setYN( float* _ny, unsigned long num )	{ny = _ny; numnormals = num;};
	void setZN( float* _nz, unsigned long num )	{nz = _nz; numnormals = num;};
	void getXN( float*& _nx, unsigned long& num ) {_nx = nx; num = numnormals;};
	void getYN( float*& _ny, unsigned long& num ) {_ny = ny; num = numnormals;};
	void getZN( float*& _nz, unsigned long& num ) {_nz = nz; num = numnormals;};
	
	void setFaces( unsigned long* _faces, unsigned long num)	{faces = _faces; numfaces = num; matfaces = new unsigned long[num/3];};
	void getFaces( unsigned long*& _faces, unsigned long& num)	{_faces = faces; num = numfaces;};

	unsigned long	 addMaterial( tMaterial* _material);
	void addMaterialFace(unsigned long entry, unsigned long index) {matfaces[entry] = index; nummatfacesapplied++; if(nummatfacesapplied < numfaces/3) materialsapplied = FALSE; else materialsapplied = TRUE; };
	
	void setName(char * _name) {name = new char[strlen(_name)]; strcpy(name,_name);};
	void getName(char *& _name) {_name = name;};

	void setPivotPoint(tVector* _pivot) {pivot[0] = _pivot->x; pivot[1] = _pivot->y; pivot[2] = _pivot->z;}
	void getPivotPoint(tVector* _pivot) {_pivot->x = pivot[0]; _pivot->y = pivot[1]; _pivot->z = pivot[2];}


private:
	void CalcNormal(unsigned long entry , float out[3]);
	void ReduceToUnit(float vector[3]);
	BOOL isDataAlive();
	BOOL normalapplied;
	BOOL materialsapplied;

	unsigned long			id;
	unsigned long			i;
	float		value, valuepos;
	//基本的材质成员变量
	float*		x;
	float*		y;
	float*		z;
	unsigned long			numvertices;
	float*		nx;
	float*		ny;
	float*		nz;
	unsigned long			numnormals;
	unsigned long*		faces;
	unsigned long			numfaces;
	tMaterial*	materials;
	unsigned long			nummaterials;
	unsigned long*		matfaces;
	unsigned long			nummatfacesapplied;
	char*		name;
	float		pivot[3];
	float		pivotrot[4];

};

#endif // !defined(AFX_TRIOBJECT_H__922F85A8_F87E_4F9B_B49D_5B1175163166__INCLUDED_)
