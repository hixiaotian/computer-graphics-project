#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <gl\glaux.h>
#include <gl\glut.h>

#include <mmsystem.h>	

#define SCREEN_WIDTH 800								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 600								// We want our screen height 600 pixels
#define SCREEN_DEPTH 16									// We want 16 bits per pixel

#define GEN_RANDOM_FLOAT (((float)rand())/RAND_MAX)

#define MAX_TEXTURES 62									// This says how many texture we will be using



// This is our basic 3D point/vector class
struct CVector3
{
public:
	
	// A default constructor
	CVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
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

extern bool  g_bFullScreen;									// Set full screen as default
extern HWND  g_hWnd;										// This is the handle for the window
extern RECT  g_rRect;										// This holds the window dimensions
extern HDC   g_hDC;											// General HDC - (handle to device context)
extern HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
extern HINSTANCE g_hInstance;								// This holds our window hInstance

extern bool animationOn;								// Animation is either on or off

extern UINT g_Texture[MAX_TEXTURES];					// This is our texture data array
extern float lightPos1[4];						// This is the position of our light
extern float lightSpotDir1[3];
extern float lightSpotCutoff1;
extern float lightSpotExp1;

void RenderMuseumScene();
void DrawWhiteBox(float x, float y, float z, float width, float length, float height, float color[], int textureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[]);
void DrawCylinder(float x, float y, float z, float radius, float height, float color[]);
void DrawFrame(float x, float y, float z, float width, float height, float innerWidth, float innerHeight, float angle, float color[]);
void DrawPainting(float x, float y, float z, float width, float height, float angle, int textureNum);
void DrawPlane(float xn, float yn, float zn, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int textureNum, int blend, float color[], float ambient[], float diffuse[], float specular[], float emission[], float shine[]);
void DrawStepObject(float x, float y, float z, float radius, float r, float height, float color[]);
void DrawEntranceDoors();
void DrawBouncingBalls();
void DrawBall(int i);
void DrawReflectiveTableTop();
void DrawFloor();
void DrawPearlFloor();
void DrawWalls(float x, float y);
void DrawCeiling();
void DrawSlides();
void DrawALight(float rotX, float rotY, float rotZ, float x, float y, float z);
void DrawingManyLights();

void DrawOrbitz();
void gluBox(float width, float length, float height);

void DrawGirlActs();
void ChooseGirlAct();
void GirlActReset();
void SimpleAnimation();
void DrawGirl();
void GirlHead(float x, float y, float z, float hrx, float hry, float hrz, float rz);
void GirlBody(float x, float y, float z);
void GirlArm(float x, float y, float z, float rz);
void GirlLeg(float x, float y, float z, float rz);
void GirlResetArmLegAngle();

void RenderVanGoghPaintingScene();
void DrawBuildingA(float x, float y, float color[]);
void DrawBuildingB(float x, float y, float color[]);
void DrawGridPlane(float width, float length, float color[]);
void DrawVGDoor();
void DrawVGPatio();
void DrawVGPatioRoof();
void DrawVGEndWall();
void DrawVGCafe();
void DrawVGFarBuilding();
void DrawVGGround();

void InitializeParticleSystem();
void InitializeAParticle(int i, float idleTime);
void DrawPraticleSystem();

void DrawShadows();
void DrawReflections();
void DrawReflectiveBall1();
void DrawReflectiveBall2();
void DrawReflectiveFloor1();
void DrawReflectiveFloor2();
void DrawReflectiveObjects();
void DrawSidesofReflectivePlane1();
void DrawSidesofReflectivePlane2();
void DrawReflectiveCones();
void DrawShadowBall();

void shadowMatrix(float shadowMat[4][4], float groundPlane[4], float lightPos[4]);


void RenderOutdoor();
void RenderRoomI();
void RenderRoomIIA();
void RenderRoomIIB();
void RenderRoomIII();
void RenderRoomIV();
bool InRoomI();
bool InRoomIIA();
bool InRoomIIB();
bool InRoomIII();
bool InRoomIV();
bool FacingXPos();
bool FacingXNeg();
bool FacingYPos();
bool FacingYNeg();
bool IntersectAt(float y, float minX, float maxX);

void DrawCameraVector();

// This is our MAIN() for windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// This controls our main program loop
WPARAM MainLoop();

// This loads a texture into openGL from a file (IE, "bitmap.bmp")
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);

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

	
#endif 
