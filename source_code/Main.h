#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\gl.h>										
#include <gl\glu.h>										
#include <gl\glaux.h>
#include <gl\glut.h>

#include <mmsystem.h>	

#define SCREEN_WIDTH 1280								
#define SCREEN_HEIGHT 720								
#define SCREEN_DEPTH 16									

#define GEN_RANDOM_FLOAT (((float)rand())/RAND_MAX)

#define MAX_TEXTURES 100									




struct CVector3
{
public:
	
	
	CVector3() {}

	
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	
	CVector3 operator+(CVector3 vVector)
	{
		
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	 
	CVector3 operator-(CVector3 vVector)
	{
		
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	
	CVector3 operator*(float num)
	{
		
		return CVector3(x * num, y * num, z * num);
	}

	
	CVector3 operator/(float num)
	{
		
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;						
};


struct CParticle
{
public:
	
	CParticle() {}
	
	CParticle(float x, float y, float z, float _angle,
		float vx, float vy, float vz, 
		float ax, float ay, float az, 
		float r, float g, float b, float a, 
		float dr, float dg, float db, float da, 
		float _startTime, float _idleTime, float _lifeTime, float _radius)
	{
		vPosition = CVector3(x, y, z);
		angle = _angle;
		vVelocity = CVector3(vx, vy, vz);
		vAcceleration = CVector3(ax, ay, az);
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
		dColor[0] = dr;
		dColor[1] = dg;
		dColor[2] = db;
		dColor[3] = da;
		hasNotStarted = true;
		startTime = _startTime;
		idleTime = _idleTime;
		lifeTime = _lifeTime;
		lastTime = _startTime;
		radius = _radius;
	}

	CVector3 vPosition;
	float angle;
	CVector3 vVelocity;
	CVector3 vAcceleration;
	float color[4];
	float dColor[4];
	bool hasNotStarted;
	float startTime;
	float idleTime;
	float lifeTime;
	float lastTime;
	float radius;
};

extern bool  g_bFullScreen;									
extern HWND  g_hWnd;										
extern RECT  g_rRect;										
extern HDC   g_hDC;											
extern HGLRC g_hRC;											
extern HINSTANCE g_hInstance;								

extern bool animationOn;								

extern UINT g_Texture[MAX_TEXTURES];					
extern float lightPos1[4];						
extern float lightSpotDir1[3];
extern float lightSpotCutoff1;
extern float lightSpotExp1;

void RenderInside();
void RenderOutdoor();
void DrawCuboid(float x, float y, float z, float width, float length, float height, float color[], int textureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[]);
void DrawCylinder(float x, float y, float z, float radius, float height, float color[]);
void drawHuaHua(float x, float y, float z, float width, float height, float angle, int textureNum);
void DrawSurface(float xn, float yn, float zn, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int textureNum, int blend, float color[], float ambient[], float diffuse[], float specular[], float emission[], float shine[]);



void DrawFloor();
void DrawWalls(float x, float y);
void DrawCeiling();
void DrawSlides();
void DrawALight(float rotX, float rotY, float rotZ, float x, float y, float z);
void gluBox(float width, float length, float height);

void DrawrobotActs();
void ChooserobotAct();
void robotActReset();
void SimpleAnimation();
void Drawrobot();
void robotHead(float x, float y, float z, float hrx, float hry, float hrz, float rz);
void robotBody(float x, float y, float z);
void robotArm(float x, float y, float z, float rz);
void robotLeg(float x, float y, float z, float rz);
void robotResetArmLegAngle();

void InitializeParticleSystem();
void InitializeAParticle(int i, float idleTime);
void drawParticleSystem();

void shadowMatrix(float shadowMat[4][4], float groundPlane[4], float lightPos[4]);
void DrawCameraVector();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);


LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


WPARAM MainLoop();


void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);

/******************************                 Charm's texture                      *********************************/
void CreateWallRepeatTexture(UINT textureArray[], LPSTR strFileName, int textureID);

// This changes the screen to full screen mode
void ChangeToFullScreen();

// This is our own function that makes creating a window modular and easy
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// This allows us to configure our window for OpenGL and backbuffered
bool bSetupPixelFormat(HDC hdc);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

// This initializes the whole program
void Init(HWND hWnd);

// This draws everything to the screen
void RenderScene();

// This frees all our memory in our program
void DeInit();

/***********          functions from Marcus         *************/
// function used to draw bathroom
void drawBathroom();

// function used to draw bathroom partI
void drawBathroomI();

// function used to draw the sides of the case in bathroom I
void drawSidesOfCase();

// function used to draw the surface of the case
void drawCaseSurface();

// function used to draw bathroom wall
void drawBathroomWall();

// function used to draw shower room
void drawShowerRoom();

// function used to draw the 2nd part of bathroom
void drawBathroomII();

// function used to draw the light of bathroom
void drawBathroomLight(float angleX, float angleY, float angleZ, float x, float y, float z);

/***********          functions from Jason         *************/
// function used to draw the sofa mat
void drawMat();

// function used to draw the sofa seat
void drawSofaseat();

// funtion used to draw the connect seat
void drawConnectSeat();

// funtion used to draw the sofa
void drawSofa();

// function used to draw the tea table surface
void drawTeatableSurface();

// funtion used to draw the tea table leg
void drawTeatableLeg();

// function used to draw the teatable
void drawTeatable();

// function used to draw the television
void drawTelevision(float x, float y, float z, float width, float height, float innerWidth, float innerHeight, float angle, float color[]);

// funtion used to draw the Lamp
void drawLamp();

// funtion used to draw the surface of dining table
void drawGuiZiSurface();

void drawGuiZi();

void drawTeapot();
void drawGuiZiSide();

void drawSolar();
void drawStorageRoom();

void drawFractals();

void Iteration(float center[3], float size, int nIter);

void SurroundSquare(float center[3], float size, float color[4]);

void Square(float center[3], float size, float color[4]);

void DrawRobotPart(float x, float y, float z, float width, float length, float height, float color[], int ToptextureNum, int WtextureNum, int EtextureNum, int NtextureNum, int StextureNum, int BottomtextureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[]);

/***********          functions from Elliot         *************/
void drawBed();
void drawBedBase();
void drawHuaPen();
void drawHua();
void drawStair();
void drawTotalStair();
void drawFirework();
void drawMountain();
void drawShadowCylinder();
void drawShadowSystem();

/***********          functions from Charm         *************/
// function used to draw the house.
void drawHouse();

void Charm_drawHouseShell();

void Charm_drawWall(float base_height, float length, float height);

void Charm_drawFloor(float width, float length, float height);

void Charm_drawRoof(float base_height, float width, float length, float height);

void Charm_drawWoodLoop(float x, float y, float z, float inR, float outR, float width);

void Charm_drawHalfCircleGlass(float xn, float yn, float zn, float x, float y, float z, float r);

void Charm_drawRoomWall(float base_height, float length, float height);
/***********          functions from Jason         *************/
void drawStorageLight(float rotX, float rotY, float rotZ, float x, float y, float z);

/***********          functions from Marcus         *************/
void drawStorageRoom();

#endif 
