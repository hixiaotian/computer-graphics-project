// 3dsReader.h: interface for the C3dsReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_)
#define AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "glStructures.h"	//数据结构定义
#include "TriList.h"		//CTriList类说明
#include "TriObject.h"		//CTriObject说明
#include "basicRead.h"		//基本的数据读入函数定义
class C3dsReader  
{
public:
	C3dsReader();
	virtual ~C3dsReader();
	int ReadKFTrackTag(unsigned long fileSize, unsigned long tagStart, unsigned long tagSize, FILE * fp,  char* nodeName, tVector* pivot, Chunk3DS chunk);
	int ReadKFObjectNode( unsigned long fileSize, unsigned long nodeStart, unsigned long nodeSize, FILE* fp);
	int ReadKFDATA( unsigned long fileSize, unsigned long kfdataStart, unsigned long kfdataSize, FILE* fp);
	int Read3DSChunk(FILE* fp, Chunk3DS& chunk);
	int Read3DSString(FILE* fp, char* name, int len = 256);
	int ReadPercentage(FILE* fp, float& value);
	int ReadColor(FILE* fp, float& red, float& green, float& blue);
	int ReadPointArray (CTriObject*	newchild, unsigned long fileSize, FILE *fp);
	int ReadFaceArray (CTriObject* newchild, unsigned long fileSize, FILE* fp);
	int ReadMeshMatGroup(CTriObject* newchild, MaterialDict* matdict, unsigned long fileSize, FILE* fp);
	int ReadTriObject(MaterialDict* matdict, unsigned long fileSize, FILE* fp, unsigned long triStart, unsigned long triSize, char* groupName);
	int ReadNamedObject(MaterialDict* matdict, unsigned long fileSize, unsigned long namedStart, unsigned long namedSize, FILE* fp);
	int ReadMatEntry(MaterialDict* matdict, unsigned long fileSize, unsigned long matStart, unsigned long matSize, FILE* fp);
	int ReadMDATA(MaterialDict* matdict, unsigned long fileSize, unsigned long mdataStart, unsigned long mdataSize, FILE* fp);
	int Read3DSFile(unsigned long fileSize, unsigned long fileStart, unsigned long fileLen, FILE* fp);
	int Is3DSFile(FILE* fp);
	BOOL Reader(char* filename, CTriList* _list);
private:
	CTriList* DaList;

};

#endif // !defined(AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_)
