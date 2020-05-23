#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "ObjLoader.h"
#include "Main.h"			// This includes our header file
#include "Camera.h"

#define STOPPED 0			// Possible camera states when animating our scene
#define SWITCHED 1
#define ZOOMING 2
#define ROTATING 3
#define TRANSLATING 4
#define FINAL 5

// Debugging Text
char debugText[50];

// Window and camera setting variables
CCamera g_Camera;										// This is our global camera object
bool  g_bFullScreen = TRUE;								// Set full screen as default
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()
bool animationOn = TRUE;

// General polygon variables
GLUquadricObj* quadric = gluNewQuadric();
int polySubdivisions = 20;

// Reflection object variables
float height = 1.3f;									// Height of ball from floor
float dHeight = 0.2;									// Change in height
float maxHeight = 3.0;									// Maximum height a ball can achieve
float minHeight = 1.3;									// Minimum height a ball can achieve
float platformHeight = 0.3;								// Height of the platform

// Shadow variables
float floorShadow[4][4];								// Matrix used to calculate our projected floor shadow
float groundplane[] = { 0.0f, 0.0f, 1.0f, 1.0f };		// Normal of the floor
float degreeLong = 0;									// Degree of the long hand
float degreeShort = 0;									// Degree of the short hand
float dDegreeLong = -1;									// Change in the degree of long hand
float dDegreeShort = -0.0167;							// Change ih the degree of short hand

float lightPos1[4] = { 0, 0, 0, 0 };						// Position of the spotlight used in VG scene
float lightSpotDir1[3] = { 0, 0, -1 };					// Direction of the spotlight
float lightSpotCutoff1 = 15.0;							// Size of the spotlight
float lightSpotExp1 = 2.0;								// Fade of the spotlight

float lightPos3[4] = { 33, 34, 8, 1.0 };

UINT textures[MAX_TEXTURES];							// Textures variable
														// This will reference to our texture data stored 
														// with OpenGL UINT is an unsigned int (only positive numbers)

// Animation variables
float cameraSpeed = 0.4;								// Speed of the camera
int cameraCurrent = 0;									// Current frame of the camera
int cameraNumOfSeq = 40;								// Number of states of the camera during the animation

int cameraStates[] =									// Camera states
{														// 1 --> stop
	1, 2, 3, 2, 3,										// 2 --> zoom
	3, 3, 2, 3, 3,										// 3 --> rotate
	2, 3, 3, 0, 2,										// 4 --> translate
	3, 0, 2, 0, 3,										// 0 --> switch camera position
	0, 1, 3, 0, 1,
	0, 3, 0, 1, 0,
	2, 0, 3, 3, 3,
	4, 3, 2, 0, 0
};


float cameraTimes[] =									// Time of each seqence
{
	5,	// Starting point, look at Surreal
	6.8,	// Zoom in
	2,	// Face door
	6.8,	// Zoom into the museum, through door
	3,	// Look right

	6,	// Look left
	3,	// Look right
	1.1,	// Zoom to flat box, wants to look up
	3,	// Look up at ceilings
	3,	// Look down

	2.8,	// Zoom in to Dali
	3,	// Look left
	0.5,// Look back right at painting 
	2,	// Stop
	0.3,	// Zoom in

	2,	// Look left
	6,	// Stop
	7.0,	// Zoom out to cinema
	5,	// Wait for girl
	4,	// Rotate right, look at slideshow
	10,	// Stop, in cinema

	0,	// Switch to bird-eye
	11,	// Rotate right
	18,	// Stop
	0,	// Switch to 3rd person, look at bouncing balls
	16,	// Stop

	3,	// Look left
	3,	// Stop, wait until girl passes by
	0,	// Switch, behind girl
	3,	// Stop, wait until girl jump through painting
	1.9,	// Zoom into Van Gogh

	1,	// Voila! Stop
	1,	// Look left
	3,	// Look right
	1,	// Look left
	1.2,	// Go up

	3,	// Look up
	3.0,	// Fly! 
	0,	// Production
	10  // THE END
};

CVector3 vNull = CVector3();							// Null vector

CVector3 cameraPositions[] =
{
	CVector3(-2.8, -24.1, 4.658), CVector3(14.647, -14.795, 4.658), vNull, CVector3(15.299, 4.572, 4.658), vNull,
	vNull, vNull, CVector3(20.155, 9.979, 4.658), vNull, vNull,
	CVector3(32.511, 23.737, 4.658), vNull, vNull, vNull, CVector3(32.153, 26.263, 4.658),
	vNull, vNull, CVector3(63.287, 6.865, 4.658), vNull, vNull, vNull,
	CVector3(44.325, 26.121, 8.415), vNull, vNull, CVector3(5.992, 11.537, 3.402), vNull,
	vNull, vNull, CVector3(15.880, 14.715, 4.981), vNull, CVector3(-0.140, 14.715, 4.981),
	vNull, vNull, vNull, vNull, CVector3(-0.140, 14.715, 10.49),
	vNull, CVector3(-50.439, 23.584, 39.985), CVector3(-55, 25, 40), vNull
};

CVector3 cameraViews[] =
{
	CVector3(12.2, -16.1, 4.658), CVector3(29.647, -6.795, 4.658), vNull, CVector3(15.871, 21.563, 4.658), vNull,
	vNull, vNull, CVector3(31.154, 22.627, 4.658), vNull, vNull,
	CVector3(43.871, 36.384, 4.658), vNull, vNull, vNull, CVector3(29.765, 43.095, 4.658),
	vNull, vNull, CVector3(48.858, 15.255, 4.658), vNull, vNull, vNull,
	CVector3(41.465, 10.06, 3.635), vNull, vNull, CVector3(5.992, 10.537, 3.402), vNull,
	vNull, vNull, CVector3(14.880, 14.715, 4.981), vNull, CVector3(-1.14, 14.715, 4.981),
	vNull, vNull, vNull, vNull, CVector3(-1.14, 14.715, 10.49),
	vNull, CVector3(-51.292, 23.735, 40.485), CVector3(-56, 25, 40), vNull
};

CVector3 cameraUps[] =
{
	CVector3(0.0, 0.0, 1.0), CVector3(0.0, 0.0, 1.0), vNull, CVector3(0.0, 0.0, 1.0), vNull,
	vNull, vNull, CVector3(0.0, 0.0, 1.0), vNull, vNull,
	CVector3(0.0, 0.0, 1.0), vNull, vNull, vNull, CVector3(0.0, 0.0, 1.0),
	vNull, vNull, CVector3(0.0, 0.0, 1.0), vNull, vNull, vNull,
	CVector3(-0.185, -0.249, 0.985), vNull, vNull, CVector3(0.0, 0.0, 1.0), vNull,
	vNull, vNull, CVector3(0.0, 0.0, 1.0), vNull, CVector3(0.0, 0.0, 1.0),
	vNull, vNull, vNull, vNull, CVector3(0.0, 0.0, 1.0),
	vNull, CVector3(0.492, -0.087, 0.866), CVector3(0.0, 0.0, 1.0), vNull
};

float cameraRotationAngles[] =
{
	0, 0, 60, 0, -60,
	120, -100, 0, 30, -30,
	0, 70, -20, 0, 0,
	50, 0, 0, 0, -30, 0,
	0, -50, 0, 0, 0,
	100, 0, 0, 0, 0,
	0, 20, -40, 10, 0,
	45, 0, 0, 0
};

int cameraRotationAxis[] =
{
	0, 0, 1, 0, 1,
	1, 1, 0, 2, 2,
	0, 1, 1, 0, 0,
	1, 0, 0, 0, 1, 0,
	0, 1, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	2, 0, 0, 0
};


int cameraDirections[] =
{
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	1, 0, 0, 0, 1,
	0, 0, -1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 1, 0, 0
};


int cameraTranslations[] =
{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 1,
	0, 0, 0, 0
};

float cameraStartTime = -1;
bool cameraResetStartTime = true;
float cameraCurrentAngle = 0;
int cameraStart = 1;
float cameraTotalDistance = 0;
float cameraCurrentDistance = 0;
bool cameraCalculateDistance = false;

// variables for orbitz system;
int orbitzCurrentFrame = 0;

// variables for particle effect
CParticle* particles;
int particlesTotal;
float particleLifetime = 2037;
float particleRadius = 0.02;
int particleSubdivisions = 5;
float particleAngle = 0;
bool particlesIsNotInitialized = true;

// variables for girl camera action
int currentAct = 0;
int girlNumAct;
CVector3* girlAct;
CVector3* girlHeadAct;
int* girlStateAct;
float* girlTimeAct;

int girlNumAct1 = 2;
CVector3 girlAct1[] = { CVector3(22, 10, 3.365), CVector3(7, 12, 3.365) };
CVector3 girlHeadAct1[] = { CVector3(0, 0, 0), CVector3(0, 0, 0) };
int girlStateAct1[] = { -1, 0 };
float girlTimeAct1[] = { 4 };

int girlNumAct2 = 13;
CVector3 girlAct2[] = {
						CVector3(5, 9, 3.365), CVector3(8, 9.5, 3.365), CVector3(12, 11, 3.365),
						CVector3(16, 14, 3.365), CVector3(19, 18, 3.365), CVector3(22, 21, 3.365),
						CVector3(30, 22, 3.365), CVector3(34, 24, 3.365), CVector3(38, 26, 3.365),
						CVector3(41, 30, 3.365), CVector3(42, 34, 3.365), CVector3(41, 38, 3.365),
						CVector3(40, 40, 3.365)
};
CVector3 girlHeadAct2[] = {
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0)
};
int girlStateAct2[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0 };
float girlTimeAct2[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

int girlNumAct3 = 65;
CVector3 girlAct3[] = {
						CVector3(14, 42, 3.365), CVector3(10, 42, 3.365), CVector3(4, 40, 3.365),
						CVector3(3, 36, 3.365), CVector3(5, 32, 3.365), CVector3(10, 32, 3.365),
						CVector3(14, 32.1, 3.365), CVector3(18, 32.2, 3.365), CVector3(22, 32, 3.365),
						CVector3(26, 31, 3.365), CVector3(30, 30, 3.365), CVector3(34, 28, 3.365),
						CVector3(37, 24, 3.365), CVector3(39, 20, 3.365), CVector3(43, 18, 3.365),
						CVector3(47, 16, 3.365), CVector3(48.5, 14, 3.365), CVector3(52, 14.2, 3.365),
						CVector3(54, 17, 3.365), CVector3(54.2, 20, 3.365), // Stop at cinema

						CVector3(54.2, 20, 3.365), // Turn, then Start walking again	
						CVector3(54.2, 20, 3.365), CVector3(53, 22, 3.365),
						CVector3(50, 21, 3.365), CVector3(48, 19, 3.365), CVector3(46, 17, 3.365),

						CVector3(45, 16, 3.365), CVector3(42, 14, 3.365),


						CVector3(40, 11, 3.365), CVector3(38, 9, 3.365), CVector3(36, 8, 3.365),
						CVector3(32, 7.8, 3.365), CVector3(28, 8, 3.365),

						CVector3(26, 7, 3.365), CVector3(22, 10, 3.365), CVector3(20, 12, 3.365),
						CVector3(19, 15, 3.365), CVector3(18, 18, 3.365), CVector3(16, 20, 3.365),
						CVector3(14, 22, 3.365), CVector3(10, 23, 3.365), CVector3(7, 22, 3.365),
						CVector3(5, 20, 3.365), CVector3(4, 17, 3.365), CVector3(3, 14, 3.365),
						CVector3(4, 12, 3.365),
						CVector3(5, 9, 3.365), CVector3(5, 9, 3.365), CVector3(5, 9, 3.365),
						CVector3(5, 9, 3.365),
						// After looking at bouncing balls.
						CVector3(9, 9, 3.365), CVector3(12, 8, 3.365), CVector3(16, 6, 3.365),
						CVector3(20, 8, 3.365), CVector3(24, 9, 3.365), CVector3(28, 9.1, 3.365),
						CVector3(25, 9, 3.365), CVector3(21, 8.9, 3.365), CVector3(18, 12, 3.365),
						CVector3(14, 12.1, 3.365), CVector3(10, 12, 3.365), CVector3(8, 13, 3.365),
						CVector3(4, 13.5, 3.365), CVector3(0, 13.9, 3.365), CVector3(-5, 14, 3.365),
};
CVector3 girlHeadAct3[] = {
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(50, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0),
						// After looking at bouncing balls
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 45), CVector3(0, 0, -45), CVector3(0, 0, 45),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0)
};
int girlStateAct3[] =
{
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, 0,
	0, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, 0, 0, 0, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1
	- 1, -1, -1, -1, -1
};
float girlTimeAct3[] =
{
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 5,
	5, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 5, 5, 5, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1
};



int girlNumAct4 = 15;
CVector3 girlAct4[] = {
						CVector3(-5, 14, 3.365), CVector3(-8, 19, 3.365), CVector3(-15, 24, 3.365),
						CVector3(-20, 27, 3.365), CVector3(-25, 27.5, 3.365), CVector3(-29, 27.4, 3.365),
						CVector3(-33, 26.5, 3.365), CVector3(-37, 26.7, 3.365), CVector3(-41, 27, 3.365),
						CVector3(-45, 26.5, 3.365), CVector3(-46, 25, 3.365), CVector3(-48, 23, 3.365),
						CVector3(-48, 19, 3.365), CVector3(-47.5, 14, 3.365), CVector3(-47.4, 10, 3.365),
};
CVector3 girlHeadAct4[] = {
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
						CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
};
int girlStateAct4[] =
{
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, 0
};
float girlTimeAct4[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };




bool firstTime = true;
float initialTime = 0;

// variables for girl
float girlSkin[] = { 0.98, 0.84, 0.78, 1.0 };
float girlDress[] = { 0.8, 0.0, 0.3, 1.0 };
int girlDirection = 1;
float girlArmRotation = 0;
float girlLegRotation = 0;
int girlX = 10;
int girlY = 11;
int girlZ = 5;
int girlNumOfSeq = 59;
CVector3 girlPositions[] = { CVector3(40, 11, 3.365), CVector3(36, 8, 3.365), CVector3(31.5, 7.5, 3.365),
								CVector3(26, 7, 3.365), CVector3(22, 8, 3.365), CVector3(19, 12, 3.365),
								CVector3(17, 16, 3.365), CVector3(16, 20, 3.365), CVector3(14, 22, 3.365),
								CVector3(10, 23, 3.365), CVector3(6, 22, 3.365), CVector3(4, 19, 3.365),
								CVector3(3, 15, 3.365), CVector3(4, 11, 3.365), CVector3(5, 9, 3.365),
								CVector3(5, 9, 3.365), CVector3(5, 9, 3.365),
								CVector3(5, 9, 3.365), CVector3(8, 9.5, 3.365), CVector3(12, 11, 3.365),
								CVector3(16, 14, 3.365), CVector3(19, 18, 3.365), CVector3(22, 21, 3.365),
								CVector3(30, 22, 3.365), CVector3(34, 24, 3.365), CVector3(34, 24, 3.365),
								CVector3(36, 27, 3.365), CVector3(37, 30, 3.365), CVector3(38, 34, 3.365),
								CVector3(37, 38, 3.365), CVector3(34, 41, 3.365), CVector3(28, 41.5, 3.365),
								CVector3(22, 41, 3.365), CVector3(16, 41.5, 3.365), CVector3(10, 42, 3.365),
								CVector3(6, 41.5, 3.365), CVector3(4, 40, 3.365), CVector3(3, 38, 3.365),
								CVector3(3.5, 34, 3.365), CVector3(5, 32.5, 3.365), CVector3(9, 32, 3.365),
								CVector3(16, 31.5, 3.365), CVector3(22, 31, 3.365), CVector3(28, 30.5, 3.365),
								CVector3(33, 28, 3.365), CVector3(36, 22, 3.365), CVector3(39, 20, 3.365),
								CVector3(43, 18, 3.365), CVector3(47, 16.5, 3.365), CVector3(52, 17, 3.365),
								CVector3(56, 18, 3.365), CVector3(56, 18, 3.365), CVector3(56, 18, 3.365),
								CVector3(56.5, 20, 3.365), CVector3(55, 22, 3.365), CVector3(52, 21.5, 3.365),
								CVector3(50, 19, 3.365), CVector3(47, 16, 3.365), CVector3(42, 14, 3.365) };
CVector3 girlHeadTurns[] = { CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, -45),
							CVector3(0, 0, 45), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, -40), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(40, 0, 0),
							CVector3(0, 0, -30), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0) };
float initialOrientation = 270;
CVector3 initialHeadTurn = CVector3(0, 0, 0);
int girlStates[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
						-2, -2, 0, 0, 60, -1, -2, -2, -2, -2, -2, -2,
						30, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
						-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
						-2, -2, 175, 0, -2, -2, -2, -2, -2, -2, -2 };
int girlCurrent = 0;
CVector3 currentHeadTurn = CVector3(0, 0, 0);

bool girlResetStartTime = true;
float girlStartTime;
float girlCurrentTime;
float girlResetArmLegStartTime;
int girlResetArmLeg = 0;

//slides variables
bool slidesResetStartTime = true;
float slidesStartTime;
float slidesCurrentTime;
float slidesChangeTime = 500.0;
float slidesStayTime = 5000.0;
int slidesCounter = 20;
int slidesNum = 20;

//ball variables
int ballNum = 3;
float ballRadius = 0.2;
CVector3 ballPositions[] = { CVector3(4.5, 5.5, 3), CVector3(5, 5.75, 3.3), CVector3(6, 6, 3.6) };
CVector3 ballDirections[] = { CVector3(1, 2, 3), CVector3(4, 5, 6), CVector3(9, 8, 7) };
float ballColors[3][3] = { {0.815, 0.435, 0.713}, {0.54, 0.345, 0.906}, {0.345, 0.905, 0.722} };
float ballSpeed = 0.5;
float tableTopHeight = 2.32;

//door variables
float doorHandleRadius = 0.08;
float doorFrameSize = 0.1;
float doorHeight = 7;
float doorWidth = 2.5;
float doorCenterX = 16;
float doorCenterY = 2;
float doorOffsetX = 0.3;
float doorOffsetY = 0.3;
float doorOffsetHandleZ = 0.8;
float doorOffsetZ = 0.6;

//teapot variables
float teapotAmbient[] = { 1, 0.5, 0.5, 0.6 };
float teapotDiffuse[] = { 1, 0.5, 0.7, 1.0 };
float teapotSpecular[] = { 1, 0.5, 0.5, 1.0 };
float teapotShine[] = { 100 };

//nurbs variables
int numOfKnots = 8;
int numOfCtrlPtsU = 5;
int numOfCtrlPtsV = 5;
int uDegree = 4;
int vDegree = 4;
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };


// Hills variables
GLfloat ctrlPts1[5][5][3];
GLfloat ctrlPts2[5][5][3];
GLUnurbsObj* nurb1;
GLUnurbsObj* nurb2;

// Roof variables
GLUnurbsObj* nurb3;
GLfloat ctrlPts3[5][5][3];

// Ceiling variables
GLUnurbsObj* ceilingNurbs[16];
GLfloat ceilingCtrlPts[16][4][4][3];

//Fountain variables
float fountainX = 6;
float fountainY = -14;
float rBase = 3.0;
float hBase = 0.4;
float rBaseStand = 1.8;
float hBaseStand = 1.2;
float rBaseStandTorusMin = 0.2;
float rBaseStandTorusMaj = 1.7;
float rMainBottom = 4.0;
float rMainTop = 4.25;
float hMain = 0.8;
float rMainBottomTorusMin = 2.5;
float rMainBottomTorusMaj = 1.5;
float rMainTopTorusMin = 0.2;
float rMainTopTorusMaj = 4.15;
float rMainStand = 1.0;
float hMainStand = 1.0;
float rTopBottom = 1.3;
float rTopTop = 1.4;
float hTop = 0.3;
float rTopTorusMin = 0.05;
float rTopTorusMaj = 1.35;
float rPipe = 0.1;
float hPipe = 0.2;

// Trees outside variables
float treeLeavesRadius = 1.9;
float treeBarkRadiusTop = 1.0;
float treeBarkRadiusBottom = 1.2;
float treeHeight = 6.5;
float treeLeavesHeight = 6.5;
float treeLeavesOffsetX = 1.0;
float treeLeavesOffsetZ = 1.1;
float treeLeavesVarianceZ = 0.5;
float treeLeavesVarianceAngle = 8.0;

// Wall variables
float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };
float wallShine[] = { 100 };

// Chair variables
float chairRadius = 0.05;
float chairHeight = 1.5;
float chairBackHeight = 1.5;
float chairBackPlaneHeight = 0.5;
float chairSeatHeight = 0.1;
float chairSeatWidth = 1.3;
float chairBackPlaneOffset = 0.9;

// Table variables
float tableRadiusTop = 1.4;
float tableRadiusBottom = 1.45;
float tableCylinderHeight = 0.15;
float tableHeight = 2.3;
float tableLegRadius = 0.05;
float tableLegHeight = 2.75;
float tableLegSphere = 0.05;
float tableLegRotY = 30;
float tableLegRotZ = 120;

// Teapot in VG scene variable
float teapotVGHeight = 0.1;
float teapotVGSize = 0.23;

// Default material
float defaultAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
float defaultDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float defaultSpecular[] = { 0.1, 0.1, 0.1, 1.0 };
float defaultEmission[] = { 0.01, 0.01, 0.01, 1.0 };
float defaultShine[] = { 10 };

// Light material variables
float lightBulbRadius = 0.25;
float lightConeRadiusBottom = 0.4;
float lightConeRadiusTop = 0.25;
float lightConeHeight = 0.5;
float lightInnerBallRadius = 0.25;

float lightBulbAmbient[] = { 0.9, 0.9, 0.9, 1.0 };
float lightBulbDiffuse[] = { 0.5, 0.5, 0.5, 1.0 };
float lightBulbSpecular[] = { 0.6, 0.6, 0.6, 1.0 };
float lightBulbEmission[] = { 0.33, 0.33, 0.33, 1.0 };
float lightBulbShine[] = { 10 };

float lightConeAmbient[] = { 0.19225, 0.19225, 0.19225, 1.0 };
float lightConeDiffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
float lightConeSpecular[] = { 0.508273, 0.508273, 0.508273, 1.0 };
float lightConeEmission[] = { 0.0, 0.0, 0.0, 0.0 };
float lightConeShine[] = { 51.2 };

float lightConeInnerSpecular[] = { 0.8, 0.8, 0.8, 1.0 };
float lightConeInnerEmission[] = { 0.1, 0.1, 0.1, 1.0 };
float lightConeInnerShine[] = { 80 };

// Floor material variables
float pearlAmbient[] = { 1.0, 1.0, 1.0, 0.922 };
float pearlDiffuse[] = { 0.0829, 0.0829, 0.0829, 0.0922 };
float pearlSpecular[] = { 0.6, 0.6, 0.6, 0.922 };
float pearlShine[] = { 50 };

// Glass material variables
float glassAmbient[] = { 0.1, 0.1, 0.5, 0.922 };
float glassDiffuse[] = { 1.0, 0.829, 0.829, 0.922 };
float glassSpecular[] = { 0.8, 0.8, 0.8, 0.922 };
float glassShine[] = { 63.8 };

// Bouncing ball material variables
float bouncingBallAmbient[] = { 0.8, 0.6, 1.0, 1.0 };
float bouncingBallDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float bouncingBallSpecular[] = { 0.5, 0.5, 0.5, 1.0 };
float bouncingBallShine[] = { 20 };

// ### bath room variables
float bathroomI_x = 11.0;
float bathroomI_y = 0.0;
float bathroomI_z = 0.0;
float caseHeight = 3.0;
float basinHeight = 5.1;

// Define some colors
float pureWhite[4] = { 1.0, 1.0, 1.0, 1.0 };			// Pure white
float white[4] = { 0.8, 0.9, 1.0, 1.0 };				// White
float gray[4] = { 0.7, 0.7, 0.7, 1.0 };				// Gray
float lightGray[4] = { 0.9, 0.9, 0.9, 1.0 };			// Light Gray
float teapotColor[4] = { 0.6, 0.99, 0.89, 1.0 };		// Green
float black[4] = { 0.0, 0.0, 0.0, 1.0 };				// Black
float silver[4] = { 0.875, 0.875, 0.875, 1.0 };		// Silver
float bulbColor[4] = { 1.0, 0.893, 0.651, 0.5 };			// Pinkish
float pink[4] = { 1.0, 0.863, 0.641, 1.0 };			// Yellow
float darkBrown[4] = { 0.22, 0.11, 0.035 };
float cone[4] = { 0.902, 0.608, 0.169, 1.0 };
float orbitz1[4] = { 1.0, 0.5, 0.0, 1.0 };
float orbitz2[4] = { 1.0, 0.341, 0.184, 1.0 };
float orbitz3[4] = { 0.286, 0.671, 0.624, 1.0 };
float brown[4] = { 0.77, 0.51, 0.38, 1.0 };			// Brown
float glass[4] = { 0.95, 0.95, 0.97, 0.3 };			// Glass	
float water[4] = { 0.0, 0.0, 1.0, 0.45 };				// Water
float wallGreen[4] = { 0.12, 0.58, 0.32, 1.0 };		// Dark Green
float hillColor2[4] = { 0.07058, 0.6275, 0.005882, 1.0 };	 // Dark green
float nurbRoofColor[4] = { 0.83, 0.8, 0.39, 1.0 };	// Yellowish brown
float treeGreen1[4] = { 0.23, 0.63, 0.28, 1.0 };
float treeGreen2[4] = { 0.21, 0.58, 0.26, 1.0 };
float treeGreen3[4] = { 0.17, 0.45, 0.20, 1.0 };
float treeGreen4[4] = { 0.14, 0.39, 0.18, 1.0 };
float tableColor[4] = { 0.63, 0.79, 0.785, 1.0 };
float tableLegColor[4] = { 0.195, 0.27, 0.27, 1.0 };
float museumFloorColor[4] = { 0.86, 0.96, 0.905, 0.8 };	// Very light green


// display lists
GLuint girlHeadList;
GLuint girlLeftArmList;
GLuint girlRightArmList;
GLuint girlLeftDressConeList;
GLuint girlRightDressConeList;
GLuint girlLeftShoulderList;
GLuint girlRightShoulderList;
GLuint girlLegList;
GLuint doorList;
GLuint teapotMuseumList;
GLuint hillsList;
GLuint nurbRoofList;
GLuint unitCubeList;
GLuint lightList;
GLuint treeOutsideList;
GLuint particleList;
GLuint chairList;
GLuint tableList;
GLuint tablechairList;
GLuint treesList;
GLuint tree2DList;
GLuint forestList;
// ### display list for bathroom
GLuint basinList;

// ### define the file paths where onject files are stored
string basinPath = "data/basin.obj";
string casePath = "data/case.obj";
string cubePath = "data/cube.obj";
string stoolPath = "data/stool.obj";
string bathroomDoorPath = "data/bathroom_door.obj";

// ### objects are loaded here
ObjLoader basinObj = ObjLoader(basinPath);
ObjLoader caseObj = ObjLoader(casePath);
ObjLoader cubeObj = ObjLoader(cubePath);
ObjLoader stoolObj = ObjLoader(stoolPath);
ObjLoader bathroomDoorObj = ObjLoader(bathroomDoorPath);

/*
 * This returns the magnitude of a vector
 */
float VMagnitude(CVector3 vNormal)
{
	return (float)sqrt((vNormal.x * vNormal.x) +
		(vNormal.y * vNormal.y) +
		(vNormal.z * vNormal.z));
}

/*
 * This returns a normalize vector (A vector exactly of length 1)
 */
CVector3 VNormalize(CVector3 vVector)
{
	float magnitude = VMagnitude(vVector);
	vVector = vVector / magnitude;
	return vVector;
}


/*
 * This function initializes the app window
 */
void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	// Init our camera position
							// Position        View		   Up Vector
	g_Camera.PositionCamera(10.0, 10.0, 5.0, 20.0, 10.0, 5.0, 0.0, 0.0, 1.0);		// Default Cam
	//g_Camera.PositionCamera(-2.8, -24.1, 6.0,	12.2, -16.1, 6.0,	0.0, 0.0, 1.0);		// Animation Cam, we changed this
	//g_Camera.PositionCamera(32.511, 23.737, 4.658,	30.123, 40.568, 4.658,	0,0,1);	// Anim, state 12.
	//g_Camera.PositionCamera(44.325, 26.121, 8.415,	41.465, 10.06, 3.635,	-0.185, -0.249, 0.985); // Anim state 15

	//Anim debug
	int debugCam = 38;
	//g_Camera.PositionCamera(cameraPositions[debugCam].x, cameraPositions[debugCam].y, cameraPositions[debugCam].z, cameraViews[debugCam].x, cameraViews[debugCam].y, cameraViews[debugCam].z, cameraUps[debugCam].x, cameraUps[debugCam].y, cameraUps[debugCam].z);

	//g_Camera.PositionCamera(20.0, -2.0, 3.0,   5.0, -10.0, 3.0,   0.0, 0.0, 1.0);		// Fountain Cam	
	//g_Camera.PositionCamera(42, 36, 3,	34, 36, 3, 0, 0, 1);	// Bubble Cam
	//g_Camera.PositionCamera(10.0, 10.0, 3.0,   0.0, 0.0, 0.0,   0.0, 0.0, 1.0);
	//g_Camera.PositionCamera(-0.1, 15.0, 4.0,   -10.0, 15.0, 4.0,   0.0, 0.0, 1.0);		// VG

	//camera viewing from top
	//g_Camera.PositionCamera(28.0, 10.0, 40.0,   28.0, 10.0, 0.0,   -1.0, 0.0, 0.0);


	CreateTexture(textures, "Textures/reddoor.bmp", 0);			// Load textures, white.bmp currently not used
	CreateTexture(textures, "Textures/wood256.bmp", 1);
	CreateTexture(textures, "Textures/vgsky.bmp", 2);
	CreateTexture(textures, "Textures/cafewall.bmp", 3);
	CreateTexture(textures, "Textures/cafe_terrace.bmp", 4);
	CreateTexture(textures, "Textures/waterlilies.bmp", 5);
	CreateTexture(textures, "Textures/flower.bmp", 6);
	CreateTexture(textures, "Textures/microscopicsymphony.bmp", 7);
	CreateTexture(textures, "Textures/highnotes.bmp", 8);
	CreateTexture(textures, "Textures/water.bmp", 9);
	CreateTexture(textures, "Textures/argenteuil.bmp", 10);
	CreateTexture(textures, "Textures/memory.bmp", 11);
	CreateTexture(textures, "Textures/glass.bmp", 12);
	CreateTexture(textures, "Textures/shapes.bmp", 13);
	CreateTexture(textures, "Textures/white.bmp", 14);
	CreateTexture(textures, "Textures/fountain.bmp", 15);
	CreateTexture(textures, "Textures/vase.bmp", 16);
	CreateTexture(textures, "Textures/beach.bmp", 17);
	CreateTexture(textures, "Textures/production.bmp", 18);
	CreateTexture(textures, "Textures/whitebricks.bmp", 19);

	CreateTexture(textures, "Textures/slide0.bmp", 20);
	CreateTexture(textures, "Textures/slide1.bmp", 21);
	CreateTexture(textures, "Textures/slide2.bmp", 22);
	CreateTexture(textures, "Textures/slide3.bmp", 23);
	CreateTexture(textures, "Textures/slide4.bmp", 24);
	CreateTexture(textures, "Textures/slide5.bmp", 25);
	CreateTexture(textures, "Textures/slide6.bmp", 26);
	CreateTexture(textures, "Textures/slide7.bmp", 27);
	CreateTexture(textures, "Textures/slide8.bmp", 28);
	CreateTexture(textures, "Textures/slide9.bmp", 29);
	CreateTexture(textures, "Textures/slide10.bmp", 30);
	CreateTexture(textures, "Textures/slide11.bmp", 31);
	CreateTexture(textures, "Textures/slide12.bmp", 32);
	CreateTexture(textures, "Textures/slide13.bmp", 33);
	CreateTexture(textures, "Textures/slide14.bmp", 34);
	CreateTexture(textures, "Textures/slide15.bmp", 35);
	CreateTexture(textures, "Textures/slide16.bmp", 36);
	CreateTexture(textures, "Textures/slide17.bmp", 37);
	CreateTexture(textures, "Textures/slide18.bmp", 38);
	CreateTexture(textures, "Textures/slide19.bmp", 39);

	CreateTexture(textures, "Textures/background4.bmp", 40);
	CreateTexture(textures, "Textures/outsidebackground.bmp", 41);
	CreateTexture(textures, "Textures/rightBuildingFront.bmp", 42);
	CreateTexture(textures, "Textures/rightBuildingSide.bmp", 43);
	CreateTexture(textures, "Textures/frontRoad.bmp", 44);
	CreateTexture(textures, "Textures/cafeBuildingWall.bmp", 45);
	CreateTexture(textures, "Textures/floorFrontside.bmp", 46);
	CreateTexture(textures, "Textures/floorRightside.bmp", 47);
	CreateTexture(textures, "Textures/floorTop.bmp", 48);
	CreateTexture(textures, "Textures/doorFrameTop.bmp", 49);
	CreateTexture(textures, "Textures/doorFrameSides.bmp", 50);
	CreateTexture(textures, "Textures/backRoadLeft.bmp", 51);
	CreateTexture(textures, "Textures/backRoadRight.bmp", 52);
	CreateTexture(textures, "Textures/tree2D.bmp", 53);
	CreateTexture(textures, "Textures/surreal.bmp", 54);
	CreateTexture(textures, "Textures/top.bmp", 55);
	CreateTexture(textures, "Textures/left.bmp", 56);
	CreateTexture(textures, "Textures/right.bmp", 57);
	CreateTexture(textures, "Textures/front.bmp", 58);
	CreateTexture(textures, "Textures/back.bmp", 59);
	CreateTexture(textures, "Textures/patioside.bmp", 60);
	CreateTexture(textures, "Textures/envball.bmp", 61);

	/*         Textures of Charm          */
	CreateTexture(textures, "Textures/whitewall.bmp", 62);
	CreateTexture(textures, "Textures/woodh256.bmp", 63);
	CreateTexture(textures, "Textures/ground_tex1.bmp", 64);
	CreateTexture(textures, "Textures/house_wall_tex1.bmp", 65);
	CreateTexture(textures, "Textures/roof_tex1.bmp", 66);
	/*************************************/

	// hills
	nurb1 = gluNewNurbsRenderer();
	gluNurbsProperty(nurb1, GLU_SAMPLING_TOLERANCE, 50.0);
	gluNurbsProperty(nurb1, GLU_CULLING, GL_TRUE);
	gluNurbsProperty(nurb1, GLU_DISPLAY_MODE, GLU_FILL);

	nurb2 = gluNewNurbsRenderer();
	gluNurbsProperty(nurb2, GLU_SAMPLING_TOLERANCE, 50.0);
	gluNurbsProperty(nurb2, GLU_CULLING, GL_TRUE);
	gluNurbsProperty(nurb2, GLU_DISPLAY_MODE, GLU_FILL);

	// ceiling
	nurb3 = gluNewNurbsRenderer();
	gluNurbsProperty(nurb3, GLU_SAMPLING_TOLERANCE, 50.0);
	gluNurbsProperty(nurb3, GLU_CULLING, GL_TRUE);
	gluNurbsProperty(nurb3, GLU_DISPLAY_MODE, GLU_FILL);

	int u, v;
	for (int i = 0; i < 16; i++)
	{
		ceilingNurbs[i] = gluNewNurbsRenderer();
		gluNurbsProperty(ceilingNurbs[i], GLU_SAMPLING_TOLERANCE, 50.0);
		gluNurbsProperty(ceilingNurbs[i], GLU_CULLING, GL_TRUE);
		gluNurbsProperty(ceilingNurbs[i], GLU_DISPLAY_MODE, GLU_FILL);

		// ceiling control points
		for (u = 0; u < 4; u++)
		{
			for (v = 0; v < 4; v++)
			{
				ceilingCtrlPts[i][u][v][0] = u;
				ceilingCtrlPts[i][u][v][1] = v;
				if (u == 1 && v == 2 || u == 2 && v == 1)
					ceilingCtrlPts[i][u][v][2] = 3;
				else
					ceilingCtrlPts[i][u][v][2] = 0;
			}
		}
	}

	// hills control points
	for (u = 0; u < numOfCtrlPtsU; u++)
	{
		for (v = 0; v < numOfCtrlPtsV; v++)
		{
			ctrlPts1[u][v][0] = u;
			ctrlPts1[u][v][1] = u + v;
			ctrlPts2[u][v][0] = u;
			ctrlPts2[u][v][1] = u + v;
			if (u == 1 && v == 2)
			{
				ctrlPts1[u][v][2] = 4.5;
				ctrlPts2[u][v][2] = 5;
			}
		}
	}

	// roof control points
	for (u = 0; u < numOfCtrlPtsU; u++)
	{
		for (v = 0; v < numOfCtrlPtsV; v++)
		{
			ctrlPts3[u][v][0] = u;
			ctrlPts3[u][v][1] = v * 2;
			if (u == 2 && v == 1)
				ctrlPts3[u][v][2] = -1;
			if (u == 3 && v == 3)
				ctrlPts3[u][v][2] = 1;
			if (u == 3 && v == 0)
				ctrlPts3[u][v][2] = 1;
		}
	}


	// Create display lists

	// Girl's head
	girlHeadList = glGenLists(1);
	glNewList(girlHeadList, GL_COMPILE);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidSphere(0.58, polySubdivisions, polySubdivisions);
	glutSolidCone(0.3175, 0.714, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glPopMatrix();
	glEndList();

	// Girl's left arm
	girlLeftArmList = glGenLists(2);
	glNewList(girlLeftArmList, GL_COMPILE);
	glRotatef(40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -1.27);
	gluCylinder(quadric, 0.149, 0.149, 1.27, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's right arm
	girlRightArmList = glGenLists(3);
	glNewList(girlRightArmList, GL_COMPILE);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -1.27);
	gluCylinder(quadric, 0.149, 0.149, 1.27, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's left dress cone
	girlLeftDressConeList = glGenLists(4);
	glNewList(girlLeftDressConeList, GL_COMPILE);
	glRotatef(40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -0.222);
	glutSolidCone(0.222, 0.222, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's right dress cone
	girlRightDressConeList = glGenLists(5);
	glNewList(girlRightDressConeList, GL_COMPILE);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -0.222);
	glutSolidCone(0.222, 0.222, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's left shoulder
	girlLeftShoulderList = glGenLists(6);
	glNewList(girlLeftShoulderList, GL_COMPILE);
	glTranslatef(-0.53, 0, 0);
	glutSolidSphere(0.222, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's right shoulder
	girlRightShoulderList = glGenLists(7);
	glNewList(girlRightShoulderList, GL_COMPILE);
	glTranslatef(0.53, 0, 0);
	glutSolidSphere(0.222, polySubdivisions, polySubdivisions);
	glEndList();

	// Girl's leg
	girlLegList = glGenLists(8);
	glNewList(girlLegList, GL_COMPILE);
	glTranslatef(0, 0, -1.588);
	gluCylinder(quadric, 0.191, 0.191, 1.588, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	// Door handle
	doorList = glGenLists(9);
	glNewList(doorList, GL_COMPILE);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 + doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 + doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, 2 * doorOffsetHandleZ, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY + doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, 2 * doorOffsetHandleZ, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 + doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 + doorOffsetHandleZ);
	glutSolidSphere(doorHandleRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, 2 * doorOffsetHandleZ, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetHandleZ);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, 2 * doorOffsetHandleZ, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetZ);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, doorOffsetY * 2, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 - doorOffsetZ);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, doorOffsetY * 2, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX - doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 + doorOffsetZ);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, doorOffsetY * 2, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(doorCenterX + doorOffsetX, doorCenterY - doorOffsetY, doorHeight / 2 + doorOffsetZ);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, doorHandleRadius / 2, doorHandleRadius / 2, doorOffsetY * 2, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glEndList();

	// Teapot in museum
	teapotMuseumList = glGenLists(10);
	glNewList(teapotMuseumList, GL_COMPILE);
	glColor4f(teapotColor[0], teapotColor[1], teapotColor[2], teapotColor[3]);
	glPushMatrix();
	glTranslatef(44.3, 7.4, 3.3);
	glRotatef(90, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, teapotAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, teapotDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, teapotSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, teapotShine);
	glutSolidTeapot(0.38);
	glPopMatrix();
	glEndList();

	// Hills outside the museum
	hillsList = glGenLists(11);
	glNewList(hillsList, GL_COMPILE);
	glColor4f(hillColor2[0], hillColor2[1], hillColor2[2], hillColor2[3]);

	// First hill
	glPushMatrix();
	glScalef(5, 5, 5);
	glRotatef(-45, 0.0, 0, 1.0);
	gluBeginSurface(nurb1);
	gluNurbsSurface(nurb1, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts1[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb1);
	glPopMatrix();

	// Second hill
	glPushMatrix();
	glTranslatef(-15, 0, 0);
	glScalef(5, 5, 5);
	glRotatef(-45, 0.0, 0, 1.0);
	gluBeginSurface(nurb2);
	gluNurbsSurface(nurb2, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts2[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb2);
	glPopMatrix();
	glEndList();

	// Roof in van gogh's painting
	nurbRoofList = glGenLists(12);
	glNewList(nurbRoofList, GL_COMPILE);
	glPushMatrix();
	glRotatef(90, 0.0, 0.0, 1.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glColor4f(nurbRoofColor[0], nurbRoofColor[1], nurbRoofColor[2], nurbRoofColor[3]);
	gluBeginSurface(nurb3);
	gluNurbsSurface(nurb3, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts3[0][0][0], 4, 4, GL_MAP2_TEXTURE_COORD_2);
	gluNurbsSurface(nurb3, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts3[0][0][0], 4, 4, GL_MAP2_NORMAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	gluNurbsSurface(nurb3, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts3[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb3);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();

	// Lights inside the museum
	lightList = glGenLists(14);
	glNewList(lightList, GL_COMPILE);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	glutSolidSphere(lightInnerBallRadius, polySubdivisions, polySubdivisions);
	glTranslatef(0, 0, -lightConeHeight);
	gluCylinder(quadric, lightConeRadiusBottom, lightConeRadiusTop, lightConeHeight, polySubdivisions, polySubdivisions);
	gluDisk(quadric, lightConeRadiusBottom - 0.02, lightConeRadiusBottom, polySubdivisions, polySubdivisions);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeInnerSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeInnerEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeInnerShine);
	glColor4f(bulbColor[0], bulbColor[1], bulbColor[2], bulbColor[3] + 0.2);
	gluCylinder(quadric, lightConeRadiusBottom - 0.02, lightConeRadiusTop - 0.02, lightConeHeight, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightBulbAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightBulbDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightBulbSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightBulbEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightBulbShine);
	glColor4f(bulbColor[0], bulbColor[1], bulbColor[2], bulbColor[3]);
	glutSolidSphere(lightBulbRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glEndList();

	// ### create basin list
	basinList = glGenLists(15);
	glNewList(basinList, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, 0, basinHeight);
	basinObj.Draw();
	glPopMatrix();

	// Particles for the fountain
	particleList = glGenLists(16);
	glNewList(particleList, GL_COMPILE);
	glPushMatrix();
	glutSolidSphere(particleRadius, particleSubdivisions, particleSubdivisions);
	glPopMatrix();
	glEndList();

	// Chairs in VG scene
	chairList = glGenLists(17);
	glNewList(chairList, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, textures[60]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	glTranslatef(chairRadius, 0, 0);
	gluCylinder(quadric, chairRadius, chairRadius, chairHeight + chairBackHeight + chairSeatHeight, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(chairRadius, 0, chairHeight + chairBackHeight + chairSeatHeight);
	gluDisk(quadric, 0, chairRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(chairSeatWidth - chairRadius, 0, 0);
	gluCylinder(quadric, chairRadius, chairRadius, chairHeight + chairBackHeight + chairSeatHeight, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(chairSeatWidth - chairRadius, 0, chairHeight + chairBackHeight + chairSeatHeight);
	gluDisk(quadric, 0, chairRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(chairRadius, chairSeatWidth - chairRadius, 0);
	gluCylinder(quadric, chairRadius, chairRadius, chairHeight, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(chairSeatWidth - chairRadius, chairSeatWidth - chairRadius, 0);
	gluCylinder(quadric, chairRadius, chairRadius, chairHeight, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, chairHeight + chairSeatHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(chairSeatWidth, 0, chairHeight + chairSeatHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(chairSeatWidth, chairSeatWidth, chairHeight + chairSeatHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, chairSeatWidth, chairHeight + chairSeatHeight);

	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, chairHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(chairSeatWidth, 0, chairHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(chairSeatWidth, 0, chairHeight + chairSeatHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, chairHeight + chairSeatHeight);

	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(chairSeatWidth, 0, chairHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(chairSeatWidth, chairSeatWidth, chairHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(chairSeatWidth, chairSeatWidth, chairHeight + chairSeatHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(chairSeatWidth, 0, chairHeight + chairSeatHeight);

	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(chairSeatWidth, chairSeatWidth, chairHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, chairSeatWidth, chairHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, chairSeatWidth, chairHeight + chairSeatHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(chairSeatWidth, chairSeatWidth, chairHeight + chairSeatHeight);

	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, chairSeatWidth, chairHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, chairHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0, chairHeight + chairSeatHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, chairSeatWidth, chairHeight + chairSeatHeight);

	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, chairHeight + chairSeatHeight + chairBackPlaneOffset);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(chairSeatWidth - chairRadius, 0, chairHeight + chairSeatHeight + chairBackPlaneOffset);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(chairSeatWidth - chairRadius, 0, chairHeight + chairSeatHeight + chairBackPlaneOffset + chairBackPlaneHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 0, chairHeight + chairSeatHeight + chairBackPlaneOffset + chairBackPlaneHeight);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glEndList();

	// Tables in VG scene
	tableList = glGenLists(18);
	glNewList(tableList, GL_COMPILE);

	glColor4f(tableColor[0], tableColor[1], tableColor[2], tableColor[3]);

	glPushMatrix();
	glTranslatef(0, 0, tableHeight);
	gluCylinder(quadric, tableRadiusBottom, tableRadiusTop, tableCylinderHeight, polySubdivisions, polySubdivisions);
	gluDisk(quadric, 0, tableRadiusBottom, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glColor4f(tableLegColor[0], tableLegColor[1], tableLegColor[2], tableLegColor[3]);

	glPushMatrix();
	glTranslatef(-tableHeight / (2 * sqrt(3)), 0, 0);
	glRotatef(tableLegRotY, 0, 1, 0);
	gluCylinder(quadric, tableLegRadius, tableLegRadius, tableLegHeight, polySubdivisions, polySubdivisions);
	glutSolidSphere(tableLegSphere, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-tableLegRotZ, 0, 0, 1);
	glTranslatef(-tableHeight / (2 * sqrt(3)), 0, 0);
	glRotatef(tableLegRotY, 0, 1, 0);
	gluCylinder(quadric, tableLegRadius, tableLegRadius, tableLegHeight, polySubdivisions, polySubdivisions);
	glutSolidSphere(tableLegSphere, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glRotatef(tableLegRotZ, 0, 0, 1);
	glTranslatef(-tableHeight / (2 * sqrt(3)), 0, 0);
	glRotatef(tableLegRotY, 0, 1, 0);
	gluCylinder(quadric, tableLegRadius, tableLegRadius, tableLegHeight, polySubdivisions, polySubdivisions);
	glutSolidSphere(tableLegSphere, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, tableHeight + tableCylinderHeight);
	gluDisk(quadric, 0, tableRadiusTop, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glEndList();

	// All chairs and tables in VG scene
	tablechairList = glGenLists(19);
	glNewList(tablechairList, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-13, 14.3, tableLegRadius);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17.5, 22.5, tableLegRadius);
	glRotatef(-37, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-21, 21, tableLegRadius);
	glRotatef(78, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-27, 21.5, tableLegRadius);
	glRotatef(12, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-31.5, 20.5, tableLegRadius);
	glRotatef(132, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-37.5, 21.5, tableLegRadius);
	glRotatef(43, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-42, 22, tableLegRadius);
	glRotatef(98, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, 13.5, 0.3 + tableLegRadius);
	glRotatef(100, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-26, 14, 0.3 + tableLegRadius);
	glRotatef(316, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-32, 13.8, 0.3 + tableLegRadius);
	glRotatef(85, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-39, 14.3, 0.3 + tableLegRadius);
	glRotatef(68, 0, 0, 1);
	glCallList(tableList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-14.5, 17, 0);
	glRotatef(-55, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17.5, 20, 0);
	glRotatef(-40, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-22, 19, 0);
	glRotatef(-5, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-28, 19.5, 0);
	glRotatef(-10, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-34, 21.5, 0);
	glRotatef(-60, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-38.5, 19, 0);
	glRotatef(-8, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-42.5, 20, 0);
	glRotatef(-6, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-18, 12, 0.3);
	glRotatef(45, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-18.5, 16, 0.3);
	glRotatef(165, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-21.7, 12.5, 0.3);
	glRotatef(-47, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.5, 13, 0.3);
	glRotatef(42, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-28.3, 13, 0.3);
	glRotatef(-50, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30, 16, 0.3);
	glRotatef(170, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-34, 11.7, 0.3);
	glRotatef(-35, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-37, 15, 0.3);
	glRotatef(121, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-41, 12.5, 0.3);
	glRotatef(-36, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-41, 16, 0.3);
	glRotatef(-100, 0, 0, 1);
	glCallList(chairList);
	glPopMatrix();

	glEndList();

	// 3D trees outside the museum
	treesList = glGenLists(20);
	glNewList(treesList, GL_COMPILE);

	glPushMatrix();
	glTranslatef(55, -14, 0);
	glCallList(treeOutsideList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, -28, 0);
	glCallList(treeOutsideList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(13, -25, 0);
	glCallList(treeOutsideList);
	glPopMatrix();

	glEndList();

	// 2D trees in VG scene
	tree2DList = glGenLists(21);
	glNewList(tree2DList, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[53]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPushMatrix();
	glTranslatef(0, 0, 5);
	glRotatef(10, 0, 0, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.31f);
	glVertex3f(-8, 0, 4.96);

	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1.0f, 0.31f);
	glVertex3f(8, 0, 4.96);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0, 16);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 5);
	glRotatef(55, 0, 0, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.31f);
	glVertex3f(-8, 0, 4.96);

	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1.0f, 0.31f);
	glVertex3f(8, 0, 4.96);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0, 16);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 5);
	glRotatef(100, 0, 0, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.31f);
	glVertex3f(-8, 0, 4.96);

	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1.0f, 0.31f);
	glVertex3f(8, 0, 4.96);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0, 16);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 5);
	glRotatef(145, 0, 0, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.31f);
	glVertex3f(-8, 0, 4.96);

	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1.0f, 0.31f);
	glVertex3f(8, 0, 4.96);

	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0, 0, 16);

	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, textures[60]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glutSolidCone(0.7, 12, polySubdivisions, polySubdivisions);

	glDisable(GL_TEXTURE_2D);
	glEndList();

	// 2D trees in VG scene
	forestList = glGenLists(22);
	glNewList(forestList, GL_COMPILE);
	glPushMatrix();
	glTranslatef(-35, 32, 0);
	glRotatef(23, 0, 0, 1);
	glCallList(tree2DList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-28, 32, 0);
	glRotatef(-17, 0, 0, 1);
	glCallList(tree2DList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-21, 32, 0);
	glRotatef(92, 0, 0, 1);
	glCallList(tree2DList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-13, 31, 0);
	glRotatef(123, 0, 0, 1);
	glCallList(tree2DList);
	glPopMatrix();

	glEndList();
}


/*
 * The MAIN Loop
 */
WPARAM MainLoop()
{
	MSG msg;

	static char strTitle[100] = { 0 };
	sprintf(strTitle, "  :: Surreal ::    by Makiko Yasui & Dixon Koesdjojo");
	SetWindowText(g_hWnd, strTitle);

	while (1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
			TranslateMessage(&msg);						// Find out what the message does
			DispatchMessage(&msg);						// Execute the message
		}
		else											// if there wasn't a message
		{
			// We just need to call Update() from the camera object to
			// deal with the input and updating.
			g_Camera.Update();							// Update the camera information
			//RenderFrameRate();
			RenderScene();								// Render the scene every frame
		}
	}

	// Free all allocated nurbs
	gluDeleteNurbsRenderer(nurb1);
	gluDeleteNurbsRenderer(nurb2);
	gluDeleteNurbsRenderer(nurb3);
	for (int i = 0; i < 16; i++)
	{
		gluDeleteNurbsRenderer(ceilingNurbs[i]);
	}
	DeInit();											// Free all the app's memory allocated

	return(msg.wParam);									// Return from the program
}


/*
 * Controls the camera during scene animation.
 */
void CameraAction()
{
	float cameraCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	float dTime;

	if (cameraResetStartTime)
	{
		cameraStartTime = glutGet(GLUT_ELAPSED_TIME);
		cameraCurrentAngle = 0;
		cameraResetStartTime = false;
		cameraCalculateDistance = true;
		cameraCurrentDistance = 0;
	}

	dTime = cameraCurrentTime - cameraStartTime;

	float progress = dTime / (cameraTimes[cameraCurrent] * 1000);
	bool zoomingDone = false;
	bool translatingDone = false;

	if (progress > 1.0)
		progress = 1.0;

	if (cameraStates[cameraCurrent] == SWITCHED)	// camera is stopped, camera switched
	{
		g_Camera.PositionCamera(cameraPositions[cameraCurrent].x, cameraPositions[cameraCurrent].y, cameraPositions[cameraCurrent].z,
			cameraViews[cameraCurrent].x, cameraViews[cameraCurrent].y, cameraViews[cameraCurrent].z,
			cameraUps[cameraCurrent].x, cameraUps[cameraCurrent].y, cameraUps[cameraCurrent].z);
	}
	else if (cameraStates[cameraCurrent] == ZOOMING) // camera is zooming
	{
		if (progress <= 1.0)
		{
			if (cameraCalculateDistance)
			{
				cameraTotalDistance = (float)sqrt(
					(cameraPositions[cameraCurrent].x - g_Camera.Position().x) * (cameraPositions[cameraCurrent].x - g_Camera.Position().x) +
					(cameraPositions[cameraCurrent].y - g_Camera.Position().y) * (cameraPositions[cameraCurrent].y - g_Camera.Position().y) +
					(cameraPositions[cameraCurrent].z - g_Camera.Position().z) * (cameraPositions[cameraCurrent].z - g_Camera.Position().z)
				);
				cameraCalculateDistance = false;
			}

			float dDistance = progress * cameraTotalDistance - cameraCurrentDistance;
			cameraCurrentDistance += dDistance;

			if (cameraCurrentDistance < cameraTotalDistance)
				g_Camera.Zoom(cameraDirections[cameraCurrent] * dDistance);
			else
				g_Camera.PositionCamera(
					cameraPositions[cameraCurrent].x, cameraPositions[cameraCurrent].y, cameraPositions[cameraCurrent].z,
					cameraViews[cameraCurrent].x, cameraViews[cameraCurrent].y, cameraViews[cameraCurrent].z,
					cameraUps[cameraCurrent].x, cameraUps[cameraCurrent].y, cameraUps[cameraCurrent].z
				);
		}
	}
	/*
		{
			CVector3 vVector = g_Camera.View() - g_Camera.Position();

			if ((cameraDirections[cameraCurrent] > 0) && (((vVector.x > 0 && g_Camera.Position().x < cameraPositions[cameraCurrent].x) || (vVector.x < 0 && g_Camera.Position().x > cameraPositions[cameraCurrent].x))
				|| ((vVector.y > 0 && g_Camera.Position().y < cameraPositions[cameraCurrent].y) || (vVector.y < 0 && g_Camera.Position().y > cameraPositions[cameraCurrent].y))
				|| ((vVector.z > 0 && g_Camera.Position().z < cameraPositions[cameraCurrent].z) || (vVector.z < 0 && g_Camera.Position().z > cameraPositions[cameraCurrent].z)))
			)
			{
				if (cameraCurrent == cameraNumOfSeq - 3)
					cameraSpeed = 3.7;
				else if (cameraCurrent < 4)
					cameraSpeed = 0.7;
				else if (cameraCurrent >= 16 && cameraCurrent <= 19)
					cameraSpeed = 0.001;
				else
					cameraSpeed = 0.3;


				g_Camera.Zoom(cameraDirections[cameraCurrent] * cameraSpeed);
			}
			else if ((cameraDirections[cameraCurrent] < 0) && (((vVector.x > 0 && g_Camera.Position().x > cameraPositions[cameraCurrent].x) || (vVector.x < 0 && g_Camera.Position().x < cameraPositions[cameraCurrent].x))
				|| ((vVector.y > 0 && g_Camera.Position().y > cameraPositions[cameraCurrent].y) || (vVector.y < 0 && g_Camera.Position().y < cameraPositions[cameraCurrent].y))
				|| ((vVector.z > 0 && g_Camera.Position().z > cameraPositions[cameraCurrent].z) || (vVector.z < 0 && g_Camera.Position().z < cameraPositions[cameraCurrent].z)))
			)
			{
				g_Camera.Zoom(cameraDirections[cameraCurrent] * cameraSpeed);
			}
			else
				zoomingDone = true;
		}
	*/
	else if (cameraStates[cameraCurrent] == ROTATING)	// Camera is rotating
	{
		if (progress <= 1.0)
		{
			float dAngle = progress * cameraRotationAngles[cameraCurrent] - cameraCurrentAngle;
			cameraCurrentAngle += dAngle;

			if (cameraRotationAxis[cameraCurrent] == 1) // rotate about the up vector -> horizontal rotation
				g_Camera.RotateH(dAngle);
			else if (cameraRotationAxis[cameraCurrent] == 2) // rotate about the right vector -> vertical rotation
				g_Camera.RotateV(dAngle);
		}
	}
	else if (cameraStates[cameraCurrent] == TRANSLATING)	// Camera is translating
	{
		if (cameraTranslations[cameraCurrent] == 1)		// Translating Up-Down
/*
		{
			if (progress <= 1.0)
			{
				if (cameraCalculateDistance)
				{
					cameraTotalDistance = (float)sqrt(
							(cameraPositions[cameraCurrent].x - g_Camera.Position().x) * (cameraPositions[cameraCurrent].x - g_Camera.Position().x) +
							(cameraPositions[cameraCurrent].y - g_Camera.Position().y) * (cameraPositions[cameraCurrent].x - g_Camera.Position().y) +
							(cameraPositions[cameraCurrent].z - g_Camera.Position().z) * (cameraPositions[cameraCurrent].x - g_Camera.Position().z)
						);
					cameraCalculateDistance = false;
				}

				float dDistance = progress * cameraTotalDistance - cameraCurrentDistance;
				cameraCurrentDistance += dDistance;

				g_Camera.UpDown(cameraDirections[cameraCurrent] * dDistance);
			}
		}
*/
		{
			CVector3 vVector = g_Camera.UpVector();

			if (((vVector.x > 0 && g_Camera.Position().x < cameraPositions[cameraCurrent].x) || (vVector.x < 0 && g_Camera.Position().x > cameraPositions[cameraCurrent].x))
				|| ((vVector.y > 0 && g_Camera.Position().y < cameraPositions[cameraCurrent].y) || (vVector.y < 0 && g_Camera.Position().y > cameraPositions[cameraCurrent].y))
				|| ((vVector.z > 0 && g_Camera.Position().z < cameraPositions[cameraCurrent].z) || (vVector.z < 0 && g_Camera.Position().z > cameraPositions[cameraCurrent].z)))
			{
				g_Camera.UpDown(cameraDirections[cameraCurrent] * cameraSpeed);
			}
			else
				translatingDone = true;
		}


		else if (cameraTranslations[cameraCurrent] == 2)		// Translating Left-Right
/*
		{
			if (progress <= 1.0)
			{
				if (cameraCalculateDistance)
				{
					cameraTotalDistance = (float)sqrt(
							(cameraPositions[cameraCurrent].x - g_Camera.Position().x) * (cameraPositions[cameraCurrent].x - g_Camera.Position().x) +
							(cameraPositions[cameraCurrent].y - g_Camera.Position().y) * (cameraPositions[cameraCurrent].x - g_Camera.Position().y) +
							(cameraPositions[cameraCurrent].z - g_Camera.Position().z) * (cameraPositions[cameraCurrent].x - g_Camera.Position().z)
						);
					cameraCalculateDistance = false;
				}

				float dDistance = progress * cameraTotalDistance - cameraCurrentDistance;
				cameraCurrentDistance += dDistance;

				g_Camera.LeftRight(cameraDirections[cameraCurrent] * dDistance);
			}
		}
*/
		{
			CVector3 vVector = g_Camera.UpVector();

			if (((vVector.x > 0 && g_Camera.Position().x < cameraPositions[cameraCurrent].x) || (vVector.x < 0 && g_Camera.Position().x > cameraPositions[cameraCurrent].x))
				|| ((vVector.y > 0 && g_Camera.Position().y < cameraPositions[cameraCurrent].y) || (vVector.y < 0 && g_Camera.Position().y > cameraPositions[cameraCurrent].y))
				|| ((vVector.z > 0 && g_Camera.Position().z < cameraPositions[cameraCurrent].z) || (vVector.z < 0 && g_Camera.Position().z > cameraPositions[cameraCurrent].z)))
			{
				g_Camera.LeftRight(cameraDirections[cameraCurrent] * cameraSpeed);
			}
			else
				translatingDone = true;
		}

	}
	/*
		if ((cameraStates[cameraCurrent] == STOPPED || cameraStates[cameraCurrent] == SWITCHED || cameraStates[cameraCurrent] == ROTATING) && progress >= 1.0
			|| (cameraStates[cameraCurrent] == ZOOMING && zoomingDone)
			|| (cameraStates[cameraCurrent] == TRANSLATING && translatingDone) )
	*/
	if (((cameraStates[cameraCurrent] == STOPPED ||
		cameraStates[cameraCurrent] == SWITCHED ||
		cameraStates[cameraCurrent] == ROTATING ||
		cameraStates[cameraCurrent] == ZOOMING) && progress >= 1.0)
		|| (cameraStates[cameraCurrent] == TRANSLATING && translatingDone))
	{
		cameraCurrent++;
		if (cameraCurrent >= cameraNumOfSeq)
		{
			glEnable(GL_LIGHT1);
			DrawPlane(1, 0, 0, -65, 23, 38, -65, 27, 38, -65, 27, 42, -65, 23, 42, 2, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
			animationOn = false;
			g_Camera.TurnControlOn();
		}
		cameraResetStartTime = true;
	}
}


/*
 * This function renders the entire scene.
 */
void RenderScene()
{
	if (animationOn)
	{
		g_Camera.TurnControlOff();
		CameraAction();
	}
	else
		g_Camera.TurnControlOn();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLoadIdentity();  // reset matrix
	g_Camera.Look();



	if (g_Camera.Position().x < 0)
	{
		if (!animationOn)
		{
			glDisable(GL_LIGHT0);
			lightPos1[0] = g_Camera.Position().x;
			lightPos1[1] = g_Camera.Position().y;
			lightPos1[2] = g_Camera.Position().z;
			lightPos1[3] = 1.0;
			glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
			glEnable(GL_LIGHT1);
		}
		else if (cameraCurrent < 3)
		{
			RenderMuseumScene();
			glEnable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
		}
		//RenderVanGoghPaintingScene();
	}
	else
	{
		RenderMuseumScene();
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glEnable(GL_LIGHT0);
	}

	//DrawCameraVector();

	SwapBuffers(g_hDC);			// double buffer
}


/*
 * This function handles the window messages.
 */
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;

	switch (uMsg)
	{
	case WM_SIZE:										// If the window is resized
		if (!g_bFullScreen)								// Do this only if we are NOT in full screen
		{
			SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);				// Get the window rectangle
		}
		break;

	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:
		switch (wParam) {								// Check if we hit a key
		case VK_ESCAPE:								// If we hit the escape key
			PostQuitMessage(0);						// Send a QUIT message to the window
			break;
		}
		break;

	case WM_CLOSE:										// If the window is being closes
		PostQuitMessage(0);								// Send a QUIT Message to the window
		break;

	default:											// Return by default
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;										// Return by default
}


void DrawShadows()
{
	if (g_Camera.objectsAnimationOn)
	{
		degreeLong += dDegreeLong;
		degreeShort += dDegreeShort;
	}

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	// for shadow, we don't need color or depth
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	// Draw the floor
	DrawReflectiveFloor2();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


	// Draw the floor
	DrawReflectiveFloor2();

	glColor4f(0.0, 0.0, 0.0, 0.5f);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// Calculate the projected shadow
	glPushMatrix();
	glTranslatef(0, 0, 1.29);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	shadowMatrix(floorShadow, groundplane, lightPos3);
	glMultMatrixf((float*)floorShadow);
	DrawShadowBall();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);


	glPushMatrix();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawShadowBall();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	glColor4f(pureWhite[0], pureWhite[1], pureWhite[2], pureWhite[3]);
	DrawReflectiveBall2();
	DrawSidesofReflectivePlane2();
}

void DrawReflections()
{

	// Clip Plane Equations
	double eqn[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	DrawReflectiveFloor1();
	glEnable(GL_DEPTH_TEST);
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glPushMatrix();

	glScalef(1.0f, 1.0f, -1.0f);
	glPushMatrix();
	glTranslatef(0.0, 0.0, height - 0.6);
	DrawReflectiveBall1();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.3);
	DrawReflectiveCones();
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawReflectiveFloor1();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glPushMatrix();
	glTranslatef(0.0f, 0.0, height);
	DrawReflectiveBall1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0, platformHeight);
	DrawReflectiveCones();
	glPopMatrix();
	DrawSidesofReflectivePlane1();


	if (g_Camera.objectsAnimationOn)
	{
		height += dHeight;
		if (height > maxHeight || height < minHeight)
		{
			dHeight = -dHeight;
			if (height < minHeight)
				height = minHeight;
		}
	}
}

/*
 * Render museum scene using occlusion culling.
 */
void RenderMuseumScene()
{
	// Start drawing the scene
	// DrawingManyLights();

	DrawShadows();

	if (animationOn)
		ChooseGirlAct();  // draw girl for scene animation
	else
		DrawGirl();   // draw girl first

	//if (g_Camera.Position().z < 12)
	//{
	//	if (InRoomI())
	//	{
	//		// In Room I, never renders Room III
	//		RenderRoomI();
	//		RenderOutdoor();

	//		if (FacingXPos())
	//		{
	//			RenderRoomIIB();
	//			RenderRoomIV();

	//			if (FacingYPos())
	//				RenderRoomIIA();
	//		}			

	//	}

	//	else if (InRoomIIA())
	//	{
	//		RenderRoomIIA();

	//		if (FacingXNeg())
	//		{
	//			RenderRoomIII();
	//			
	//			if (FacingYNeg())
	//			{
	//				RenderRoomI();
	//				RenderRoomIIB();
	//				RenderOutdoor();
	//			}
	//		}
	//				
	//		else if (FacingYNeg())
	//		{
	//			RenderRoomIV();
	//			RenderRoomIIB();
	//		}
	//	}

	//	else if (InRoomIIB())
	//	{
	//		RenderRoomIIB();

	//		if (FacingXNeg())
	//		{
	//			RenderRoomI();

	//			if (FacingYPos())
	//			{
	//				RenderRoomIIA();
	//				RenderRoomIII();
	//				sprintf(debugText, "not rendering outdoor");
	//				SetWindowText(g_hWnd, debugText);
	//				
	//				if (FacingYNeg())
	//				{
	//					sprintf(debugText, "rendering outdoor");
	//					SetWindowText(g_hWnd, debugText);
	//					RenderOutdoor();
	//				}
	//			}
	//			else
	//			{
	//				sprintf(debugText, "rendering outdoor");
	//				SetWindowText(g_hWnd, debugText);
	//				RenderOutdoor();
	//			}
	//		}

	//		else if (FacingXPos())
	//		{
	//			RenderRoomIV();
	//	
	//			if (FacingYPos())
	//			{
	//				RenderRoomIIA();
	//				//RenderOutdoor();
	//			}
	//		}
	//	}

	//	else if (InRoomIII())
	//	{
	//		// In Room III, never renders Room I
	//		RenderRoomIII();

	//		if (FacingXPos())
	//		{
	//			RenderRoomIIA();

	//			if (FacingYNeg())
	//			{
	//				RenderRoomIIB();
	//				RenderRoomIV();
	//			}
	//		}

	//	}

	//	else if (InRoomIV())
	//	{
	//		RenderRoomIV();

	//		if (FacingXNeg())
	//		{
	//			RenderRoomI();
	//			RenderRoomIIB();

	//			if (FacingYPos())
	//			{
	//				RenderRoomIIA();
	//				RenderRoomIII();
	//			}
	//			else
	//				RenderOutdoor();
	//		}

	//	}

	//	else
	//	{
	//		if (FacingYPos())
	//		{
	//			RenderRoomI();
	//			RenderRoomIIA();
	//			RenderRoomIIB();
	//		}

	//		RenderOutdoor();
	//	}

	//}

	//else
	//{
	//	RenderOutdoor();
	//	RenderRoomI();
	//	RenderRoomIIA();
	//	RenderRoomIIB();
	//	RenderRoomIII();
	//}
	RenderOutdoor();
	// draw the whole bathroom
	glPushMatrix();
	glTranslatef(20, 0, 0);
	drawBathroom();
	drawBed();
	drawHouse();
	drawTelevision(0.0, 0.0, 1, 9.6, 5.4, 9.6, 5.4, 180.0, black);
	glPopMatrix();

	//DrawPearlFloor();
	//DrawWalls(66, 46);

	// Draw walls that separate the rooms
	//DrawWhiteBox(0, 2, 0, 12, 3, 10, wallGreen, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(20, 2, 0, 46, 3, 10, wallGreen, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine); // Thin wall, next is the plane for the Surreal name

	//glColor4f(white[0], white[1], white[2], white[3]);
	//DrawPlane(0, -1, 0, 20, 0, 0, 50, 0, 0, 50, 0, 10, 20, 0, 10, 54, 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawPlane(0, -1, 0, 50, 0, 0, 66, 0, 0, 66, 0, 10, 50, 0, 10, -1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawPlane(0, -1, 0, 0, 0, 0, 12, 0, 0, 12, 0, 10, 0, 0, 10, -1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw walls that separate the rooms
	//DrawWhiteBox(45, 5, 0, 1, 6, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(0, 25, 0, 30, 1, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(45, 21, 0, 1, 25, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(46, 25, 0, 20, 21, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(12, 0, 0, 4 - doorWidth, doorCenterY, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(16 + doorWidth, 0, 0, 4 - doorWidth, doorCenterY, 10, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawWhiteBox(16 - doorWidth, 0, doorHeight, 2 * doorWidth, doorCenterY, 10 - doorHeight, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);


	// Draw transparent objects
	// Draw ceilings
	//DrawCeiling();

	// DrawEntranceDoors();

	//DrawWhiteBox(3.1, 5.1, 2.3, 5.8, 1.8, 2, glass, -1, 1, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	//DrawWhiteBox(43.6, 6.85, 2.8, 1.3, 1.3, 1.5, glass, -1, 1, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
}


void DrawGirl()
{
	if (!g_Camera.objectsAnimationOn)
	{
		GirlHead(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
		GirlBody(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 2.222); //1.143);
		GirlLeg(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 1.586, initialOrientation);
		GirlArm(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 0.635, initialOrientation);
	}
	else
	{
		if (girlResetArmLeg == 1)
		{
			if (girlDirection > 0 && girlLegRotation >= 0 || girlDirection < 0 && girlLegRotation <= 0)
			{
				girlResetArmLeg = 0;
				girlArmRotation = 0;
				girlLegRotation = 0;
			}

			GirlHead(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
			GirlBody(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 2.222); //1.143);
			GirlLeg(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 1.586, initialOrientation);
			GirlArm(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 0.635, initialOrientation);
		}
		else
		{
			if (girlResetStartTime)
			{
				girlStartTime = glutGet(GLUT_ELAPSED_TIME);
				girlResetStartTime = false;
			}

			float progress = 0.0;
			float destOrientation;
			CVector3 currentDestVector;
			currentDestVector = girlPositions[(girlCurrent + 1) % girlNumOfSeq] - girlPositions[girlCurrent];

			float distance = sqrt(currentDestVector.x * currentDestVector.x + currentDestVector.y * currentDestVector.y);

			girlCurrentTime = glutGet(GLUT_ELAPSED_TIME);
			float dTime = girlCurrentTime - girlStartTime;

			float girlTime;
			if (girlStates[girlCurrent] == -2)
				girlTime = distance * 200;
			else if (girlStates[girlCurrent] == -1)
				girlTime = distance * 750;
			else
				girlTime = 3000;

			progress = dTime / girlTime;
			if (progress > 1.0)
				progress = 1.0;

			CVector3 currentPos;
			float currentOrientation;

			if (girlStates[girlCurrent] < 0)	//walking or running state
			{
				// current position
				currentPos = (currentDestVector * progress) + girlPositions[girlCurrent];

				if (currentDestVector.y != 0)
				{
					destOrientation = atan(currentDestVector.y / currentDestVector.x) * (180 / 3.14) + 90;
					if (currentDestVector.x < 0)
						destOrientation += 180;
				}
				else
					destOrientation = initialOrientation;
				if (progress * 1.08 < 1.0 && destOrientation - initialOrientation < -180)
					currentOrientation = ((destOrientation - initialOrientation + 360) * progress * 1.08) + initialOrientation;
				else if (progress * 1.08 < 1.0)
					currentOrientation = ((destOrientation - initialOrientation) * progress * 1.08) + initialOrientation;
				else
					currentOrientation = destOrientation;
			}
			else if (girlStates[girlCurrent] == 0)	//stopping state
			{
				currentPos = girlPositions[girlCurrent];
				currentOrientation = initialOrientation;
				destOrientation = initialOrientation;
			}
			else if (girlStates[girlCurrent] > 0)	//turning state
			{
				currentPos = girlPositions[girlCurrent];
				destOrientation = girlStates[girlCurrent];
				if (progress * 1.08 < 1.0 && destOrientation - initialOrientation < -180)
					currentOrientation = ((destOrientation - initialOrientation + 360) * progress * 1.08) + initialOrientation;
				else if (progress * 1.08 < 1.0)
					currentOrientation = ((destOrientation - initialOrientation) * progress * 1.08) + initialOrientation;
				else
					currentOrientation = destOrientation;
			}

			float destHeadTurnX = 0;
			float destHeadTurnY = 0;
			float destHeadTurnZ = 0;

			if (progress <= 0.5)
			{
				initialHeadTurn.x = girlHeadTurns[girlCurrent].x * 2 * progress;
				initialHeadTurn.y = girlHeadTurns[girlCurrent].y * 2 * progress;
				initialHeadTurn.z = girlHeadTurns[girlCurrent].z * 2 * progress;
			}
			else if (progress > 0.5)
			{
				initialHeadTurn.x = girlHeadTurns[girlCurrent].x - girlHeadTurns[girlCurrent].x * 2 * (progress - 0.5);
				initialHeadTurn.y = girlHeadTurns[girlCurrent].y - girlHeadTurns[girlCurrent].y * 2 * (progress - 0.5);
				initialHeadTurn.z = girlHeadTurns[girlCurrent].z - girlHeadTurns[girlCurrent].z * 2 * (progress - 0.5);
			}

			GirlHead(currentPos.x, currentPos.y, currentPos.z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, currentOrientation);
			GirlBody(currentPos.x, currentPos.y, currentPos.z - 2.222);
			GirlLeg(currentPos.x, currentPos.y, currentPos.z - 1.586, currentOrientation);
			GirlArm(currentPos.x, currentPos.y, currentPos.z - 0.635, currentOrientation);

			if (progress >= 1.0)
			{
				girlResetStartTime = true;
				if (g_Camera.objectsAnimationOn)
				{
					if (++girlCurrent > girlNumOfSeq - 1)
						girlCurrent = 0;
					if (girlStates[girlCurrent] == 0)
					{
						girlDirection *= -1;	//change direction
						girlResetArmLeg = 1;	//reset arm and leg positions
					}
				}


				initialOrientation = destOrientation;
				initialHeadTurn = CVector3(0, 0, 0);

			}
		}
	}
}

void GirlHead(float x, float y, float z, float hrx, float hry, float hrz, float rz)
{
	glColor4f(girlSkin[0], girlSkin[1], girlSkin[2], girlSkin[3]);
	float radius = 0.58;
	float noseRadius = 0.3175;
	float noseHeight = 0.714;

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glRotatef(hrz, 0.0, 0.0, 1.0);
	glRotatef(hry, 0.0, 1.0, 0.0);
	glRotatef(hrx, 1.0, 0.0, 0.0);
	glCallList(girlHeadList);
	glPopMatrix();
}

void GirlBody(float x, float y, float z)
{
	glColor4f(girlDress[0], girlDress[1], girlDress[2], girlDress[3]);
	float radius = 1.2;
	float height = 2.54;

	float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
	float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
	float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };

	float wallShine[] = { 0 };

	glPushMatrix();
	glTranslatef(x, y, z);

	glutSolidCone(radius, height, polySubdivisions, polySubdivisions);
	glPopMatrix();
}


void GirlResetArmLegAngle()
{

	if (girlDirection > 0 && girlLegRotation >= 0 || girlDirection < 0 && girlLegRotation <= 0)
	{
		girlResetArmLeg = 0;
		girlArmRotation = 0;
		girlLegRotation = 0;
	}

	GirlHead(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
	GirlBody(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 2.222); //1.143);
	GirlLeg(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 1.586, initialOrientation);
	GirlArm(girlPositions[girlCurrent].x, girlPositions[girlCurrent].y, girlPositions[girlCurrent].z - 0.635, initialOrientation);
}


void GirlArm(float x, float y, float z, float rz)
{
	float angle = 2;

	if (girlResetArmLeg == 0 && ((animationOn && girlStateAct[girlCurrent] == 0) || (girlStates[girlCurrent] == 0)))
		angle = 0;
	else if ((animationOn && girlStateAct[girlCurrent] == -2) || (girlStates[girlCurrent] == -2))
		angle = 6;


	float length = 1.27;
	float shoulderSize = 0.222;
	float armRadius = 0.149;
	float handRadius = 0.191;

	if (g_Camera.objectsAnimationOn)
		girlArmRotation += -girlDirection * angle;

	glColor4f(girlSkin[0], girlSkin[1], girlSkin[2], girlSkin[3]);

	// Left arm & hand
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(-0.53, 0, 0);
	glRotatef(girlArmRotation, 1.0, 0.0, 0.0);
	glCallList(girlLeftArmList);

	glPopMatrix();

	// Right arm & hand
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(0.53, 0, 0);
	glRotatef(-girlArmRotation, 1.0, 0.0, 0.0);
	glCallList(girlRightArmList);

	glPopMatrix();

	glColor4f(girlDress[0], girlDress[1], girlDress[2], girlDress[3]);

	// Left dress cone
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(-0.53, 0, 0);
	glRotatef(girlArmRotation, 1.0, 0.0, 0.0);
	glCallList(girlLeftDressConeList);
	glPopMatrix();

	// Left shoulder
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glCallList(girlLeftShoulderList);
	glPopMatrix();

	// Right dress cone
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(0.53, 0, 0);
	glRotatef(-girlArmRotation, 1.0, 0.0, 0.0);
	glCallList(girlRightDressConeList);
	glPopMatrix();

	// Right shoulder
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glCallList(girlRightShoulderList);
	glPopMatrix();
}

void GirlLeg(float x, float y, float z, float rz)
{
	float max = 50.0;
	float min = -50.0;

	float angle = 2;
	float length = 1.588;
	float legRadius = 0.191;

	if (girlResetArmLeg == 0 && ((animationOn && girlStateAct[girlCurrent] == 0) || girlStates[girlCurrent] == 0))
		angle = 0;
	else if ((animationOn && girlStateAct[girlCurrent] == -2) || girlStates[girlCurrent] == -2)
		angle = 6;

	if (g_Camera.objectsAnimationOn)
		girlLegRotation += girlDirection * angle;

	if (((animationOn && girlStateAct[girlCurrent] != 0) || (girlStates[girlCurrent] != 0)) && ((girlDirection > 0 && girlLegRotation > max) || (girlDirection < 0 && girlLegRotation < min)))
	{
		girlLegRotation -= girlDirection * angle;
		girlDirection *= -1;		// Change the direction
	}

	glColor4f(girlSkin[0], girlSkin[1], girlSkin[2], girlSkin[3]);

	// Left leg & foot
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(-0.32, 0, 0);
	glRotatef(girlLegRotation, 1.0, 0.0, 0.0);
	glCallList(girlLegList);
	glPopMatrix();

	// Right leg & foot
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(0.32, 0, 0);
	glRotatef(-girlLegRotation, 1.0, 0.0, 0.0);
	glCallList(girlLegList);
	glPopMatrix();
}

void DrawCameraVector()
{
	glBegin(GL_LINES);
	// Up Vector
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glLineWidth(3.0);
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(
		g_Camera.View().x + g_Camera.UpVector().x,
		g_Camera.View().y + g_Camera.UpVector().y,
		g_Camera.View().z + g_Camera.UpVector().z);

	// Right Vector
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glLineWidth(3.0);
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(
		g_Camera.View().x + g_Camera.Right().x,
		g_Camera.View().y + g_Camera.Right().y,
		g_Camera.View().z + g_Camera.Right().z);

	// View Vector
	glColor4f(0.0, 1.0, 0.0, 1.0);
	glLineWidth(3.0);
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(
		2 * g_Camera.View().x - g_Camera.Position().x,
		2 * g_Camera.View().y - g_Camera.Position().y,
		2 * g_Camera.View().z - g_Camera.Position().z);


	// X
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(g_Camera.View().x + 5, g_Camera.View().y, g_Camera.View().z);
	// Y
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(g_Camera.View().x, g_Camera.View().y + 5, g_Camera.View().z);
	// Z
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z);
	glVertex3f(g_Camera.View().x, g_Camera.View().y, g_Camera.View().z + 5);

	glEnd();
}


void DrawOrbitz()
{
	float planetRadius = 0.35;
	float atomRadius = 0.11;
	float atomOrbitRadius = 0.59;
	float atom1Speed = 329;
	float atom2Speed = -333;
	float atom3Speed = 337;

	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glColor4f(white[0], white[1], white[2], white[3]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[14]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	glTranslatef(44.25, 23.25, 3.85);
	gluSphere(quadric, planetRadius, polySubdivisions, polySubdivisions);
	glDisable(GL_TEXTURE_2D);

	glRotatef(-23, 1, 0, 0);
	glPushMatrix();
	glPushMatrix();
	glColor4f(orbitz1[0], orbitz1[1], orbitz1[2], orbitz1[3]);
	glRotatef(0, 1, 0, 0);
	gluDisk(quadric, atomOrbitRadius - 0.05, atomOrbitRadius, polySubdivisions, polySubdivisions);
	glRotatef(orbitzCurrentFrame * atom1Speed, 0, 0, 1);
	glTranslatef(atomOrbitRadius, 0, 0);
	glutSolidSphere(atomRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glColor4f(orbitz2[0], orbitz2[1], orbitz2[2], orbitz2[3]);
	glRotatef(60, 1, 0, 0);
	gluDisk(quadric, atomOrbitRadius - 0.05, atomOrbitRadius, polySubdivisions, polySubdivisions);
	glRotatef(orbitzCurrentFrame * atom2Speed, 0, 0, 1);
	glTranslatef(atomOrbitRadius, 0, 0);
	glutSolidSphere(atomRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glColor4f(orbitz3[0], orbitz3[1], orbitz3[2], orbitz3[3]);
	glRotatef(120, 1, 0, 0);
	gluDisk(quadric, atomOrbitRadius - 0.05, atomOrbitRadius, polySubdivisions, polySubdivisions);
	glRotatef(orbitzCurrentFrame * atom3Speed, 0, 0, 1);
	glTranslatef(atomOrbitRadius, 0, 0);
	glutSolidSphere(atomRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	if (g_Camera.objectsAnimationOn)
		orbitzCurrentFrame++;
}

void gluBox(float width, float length, float height)
{
	glBegin(GL_QUADS);
	//Top
	glNormal3f(0, 0, 1);
	glVertex3f(-width / 2, -length / 2, height);
	glVertex3f(width / 2, -length / 2, height);
	glVertex3f(width / 2, length / 2, height);
	glVertex3f(-width / 2, length / 2, height);

	//Bottom
	glNormal3f(0, 0, -1);
	glVertex3f(-width / 2, -length / 2, 0);
	glVertex3f(width / 2, -length / 2, 0);
	glVertex3f(width / 2, length / 2, 0);
	glVertex3f(-width / 2, length / 2, 0);

	//East
	glNormal3f(1, 0, 0);
	glVertex3f(width / 2, -length / 2, 0);
	glVertex3f(width / 2, length / 2, 0);
	glVertex3f(width / 2, length / 2, height);
	glVertex3f(width / 2, -length / 2, height);

	//West
	glNormal3f(-1, 0, 0);
	glVertex3f(-width / 2, length / 2, 0);
	glVertex3f(-width / 2, -length / 2, 0);
	glVertex3f(-width / 2, -length / 2, height);
	glVertex3f(-width / 2, length / 2, height);

	//North
	glNormal3f(0, 1, 0);
	glVertex3f(-width / 2, length / 2, 0);
	glVertex3f(width / 2, length / 2, 0);
	glVertex3f(width / 2, length / 2, height);
	glVertex3f(-width / 2, length / 2, height);

	//South
	glNormal3f(0, -1, 0);
	glVertex3f(-width / 2, -length / 2, 0);
	glVertex3f(width / 2, -length / 2, 0);
	glVertex3f(width / 2, -length / 2, height);
	glVertex3f(-width / 2, -length / 2, height);

	glEnd();
}

void DrawBouncingBalls()
{
	float minX = 3.1;
	float maxX = 8.9;
	float minY = 5.1;
	float maxY = 6.9;
	float minZ = tableTopHeight;
	float maxZ = 4.3;

	glMaterialfv(GL_FRONT, GL_AMBIENT, bouncingBallAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bouncingBallDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bouncingBallSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, bouncingBallShine);

	for (int i = 0; i < ballNum; i++)
	{
		CVector3 vBall = VNormalize(ballDirections[i]);

		if (g_Camera.objectsAnimationOn)
		{
			ballPositions[i].x += vBall.x * ballSpeed;
			ballPositions[i].y += vBall.y * ballSpeed;
			ballPositions[i].z += vBall.z * ballSpeed;
		}

		if (ballPositions[i].x < minX + ballRadius)
		{
			ballPositions[i].x = minX + ballRadius;
			ballDirections[i].x = -ballDirections[i].x;
		}
		else if (ballPositions[i].x > maxX - ballRadius)
		{
			ballPositions[i].x = maxX - ballRadius;
			ballDirections[i].x = -ballDirections[i].x;
		}
		if (ballPositions[i].y < minY + ballRadius)
		{
			ballPositions[i].y = minY + ballRadius;
			ballDirections[i].y = -ballDirections[i].y;
		}
		else if (ballPositions[i].y > maxY - ballRadius)
		{
			ballPositions[i].y = maxY - ballRadius;
			ballDirections[i].y = -ballDirections[i].y;
		}
		if (ballPositions[i].z < minZ + ballRadius)
		{
			ballPositions[i].z = minZ + ballRadius;
			ballDirections[i].z = -ballDirections[i].z;
		}
		else if (ballPositions[i].z > maxZ - ballRadius)
		{
			ballPositions[i].z = maxZ - ballRadius;
			ballDirections[i].z = -ballDirections[i].z;
		}

		for (int j = i + 1; j < ballNum; j++)
		{
			float distance = VMagnitude(ballPositions[i] - ballPositions[j]);
			if (distance < 2 * ballRadius)
			{
				CVector3 temp = ballDirections[i];
				ballDirections[i] = ballDirections[j];
				ballDirections[j] = temp;
			}
		}
	}

	double eqn[] = { 0.0f, 0.0f, 0.0, 0.0f };
	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	DrawReflectiveTableTop();

	glEnable(GL_DEPTH_TEST);
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_CLIP_PLANE0);

	glClipPlane(GL_CLIP_PLANE0, eqn);
	glPushMatrix();

	glScalef(1.0f, 1.0f, -1.0f);
	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[0].z - 2 * tableTopHeight);
	DrawBall(0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[1].z - 2 * tableTopHeight);
	DrawBall(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[2].z - 2 * tableTopHeight);
	DrawBall(2);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	(GL_LIGHTING);

	glPushMatrix();
	glColor4f(0.01f, 0.01f, 0.01f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawReflectiveTableTop();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[0].z);
	DrawBall(0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[1].z);
	DrawBall(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, ballPositions[2].z);
	DrawBall(2);
	glPopMatrix();
}

void DrawBall(int i)
{
	glPushMatrix();
	glColor4f(ballColors[i][0], ballColors[i][1], ballColors[i][2], 0.5);
	glTranslatef(ballPositions[i].x, ballPositions[i].y, 0);
	glutSolidSphere(ballRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
}

void DrawReflectiveTableTop()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(3.1, 5.1, tableTopHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(8.9, 5.1, tableTopHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(8.9, 6.9, tableTopHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.1, 6.9, tableTopHeight);

	glEnd();
}

void DrawSlides()
{
	DrawWhiteBox(51, 24.8, 1.25, 10, 0.2, 7.5, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	if (slidesResetStartTime)
	{
		slidesStartTime = glutGet(GLUT_ELAPSED_TIME);
		slidesResetStartTime = false;
	}
	slidesCurrentTime = glutGet(GLUT_ELAPSED_TIME);
	float dTime = slidesCurrentTime - slidesStartTime;
	if (slidesCounter % 5 == 0)
	{
		if (dTime > slidesStayTime)
		{
			slidesCounter++;
			slidesResetStartTime = true;
		}
	}
	else if (dTime > slidesChangeTime)
	{
		slidesCounter++;
		slidesResetStartTime = true;
	}

	if (slidesCounter == 40)
		slidesCounter = 20;

	DrawPlane(0, -1, 0, 51, 24.75, 1.25, 61, 24.75, 1.25, 61, 24.75, 8.75, 51, 24.75, 8.75, slidesCounter, 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}

void DrawStepObject(float x, float y, float z, float radius, float r, float height, float color[])
{
	float degree = 3.14 * 2 / polySubdivisions;
	float theta = degree;
	float rCosTheta = radius;
	float rSinTheta = 0;
	float nextRSinTheta;
	float nextRCosTheta;
	float offset = height / polySubdivisions;

	glColor4f(color[0], color[1], color[2], color[3]);

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(r, 0.0, 0.0, 1.0);


	for (int i = 0; i < polySubdivisions; i++) {

		theta += degree;
		nextRCosTheta = radius * cos(theta);
		nextRSinTheta = radius * sin(theta);

		glBegin(GL_TRIANGLES);

		// Surface facing up
		glNormal3f(0, 0, 1);
		glVertex3f(rCosTheta, rSinTheta, i * offset);
		glVertex3f(nextRCosTheta, nextRSinTheta, i * offset);
		glVertex3f(0, 0, i * offset);

		glEnd();

		glBegin(GL_QUADS);
		glNormal3f(radius * cos(theta - theta / 2), radius * sin(theta - theta / 2), 0);
		glVertex3f(rCosTheta, rSinTheta, i * offset - z);
		glVertex3f(nextRCosTheta, nextRSinTheta, i * offset - z);
		glVertex3f(nextRCosTheta, nextRSinTheta, i * offset);
		glVertex3f(rCosTheta, rSinTheta, i * offset);
		glEnd();

		// Surface facing sideways
		if (i != polySubdivisions - 1)
		{
			glBegin(GL_QUADS);
			glNormal3f(nextRSinTheta, nextRCosTheta, 0);
			glVertex3f(0, 0, i * offset - z);
			glVertex3f(nextRCosTheta, nextRSinTheta, i * offset - z);
			glVertex3f(nextRCosTheta, nextRSinTheta, (i + 1) * offset);
			glVertex3f(0, 0, (i + 1) * offset);

			glEnd();
		}
		rCosTheta = nextRCosTheta;
		rSinTheta = nextRSinTheta;
	}
	glPopMatrix();
}


void DrawEntranceDoors()
{

	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);

	//Vertical frames
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	//Horizontal frames
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	//Draw handles
	glCallList(doorList);

	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}


void DrawCylinder(float x, float y, float z, float radius, float height, float color[])
{
	int subdivision = 24;
	double degree = 3.14 * 2 / subdivision;
	double theta = 0;
	double rCosTheta = radius;
	double rSinTheta = 0;
	double nextRSinTheta;
	double nextRCosTheta;

	glColor4f(color[0], color[1], color[2], color[3]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// Translate the frame to the specified location
	glTranslatef(x, y, z);

	for (int i = 1; i < subdivision; i++)
	{
		theta += degree;
		nextRCosTheta = radius * cos(theta);
		nextRSinTheta = radius * sin(theta);

		// Top
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		glVertex3f(0, 0, height);
		glVertex3f(rCosTheta, rSinTheta, height);
		glVertex3f(nextRCosTheta, nextRSinTheta, height);
		glEnd();

		// Bottom
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, -1);
		glVertex3f(0, 0, 0);
		glVertex3f(rCosTheta, rSinTheta, 0);
		glVertex3f(nextRCosTheta, nextRSinTheta, 0);
		glEnd();

		// Side
		glBegin(GL_QUADS);
		glNormal3f((radius * cos(theta - degree / 2)), (radius * sin(theta - degree / 2)), 0);
		glVertex3f(rCosTheta, rSinTheta, 0);
		glVertex3f(nextRCosTheta, nextRSinTheta, 0);
		glVertex3f(nextRCosTheta, nextRSinTheta, height);
		glVertex3f(rCosTheta, rSinTheta, height);
		glEnd();

		rCosTheta = nextRCosTheta;
		rSinTheta = nextRSinTheta;
	}

	// Top
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, height);
	glVertex3f(rCosTheta, rSinTheta, height);
	glVertex3f(radius, 0, height);
	glEnd();

	// Bottom
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(rCosTheta, rSinTheta, 0);
	glVertex3f(radius, 0, 0);
	glEnd();

	// Side
	glBegin(GL_QUADS);
	glNormal3f(radius * cos(theta - theta / 2), radius * sin(theta - theta / 2), 0);
	glVertex3f(rCosTheta, rSinTheta, 0);
	glVertex3f(radius, 0, 0);
	glVertex3f(radius, 0, height);
	glVertex3f(rCosTheta, rSinTheta, height);
	glEnd();

	glPopMatrix();


}


void DrawFrame(float x, float y, float z, float width, float height, float innerWidth, float innerHeight, float angle, float color[])
{
	float thickness = 0.2;
	float offset = 0.2;
	float frameH = (height - innerHeight - 2 * offset) / 2;
	float frameW = (width - innerWidth - 2 * offset) / 2;

	glPushMatrix();
	// Translate the frame to the specified location
	glTranslatef(x, y, z);
	// Rotate the frame so that it is against the wall
	glRotatef(angle, 0.0, 0.0, 1.0);

	// Draw the boxes on all four sides
	DrawWhiteBox(0, 0, 0, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0, height - frameH, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(width - frameW, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw the slanted quadrilaterals
	DrawPlane(0, 0.75 * thickness, thickness / 4 * sqrt(3.0), frameW, 0, frameH, width - frameW, 0, frameH, width - frameW, thickness, thickness * sqrt(3.0) + frameH, frameW, thickness, thickness * sqrt(3.0) + frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0.75 * -thickness, -(thickness / 4 * sqrt(3.0)), frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, 0, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(thickness * sqrt(3.0) / 4, 0.75 * thickness, 0, frameW, 0, frameH, thickness * sqrt(3.0) + frameW, thickness, frameH, thickness * sqrt(3.0) + frameW, thickness, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-(thickness / 4 * sqrt(3.0)), 0.75 * -thickness, 0, width - frameW - thickness * sqrt(3.0), thickness, frameH, width - frameW, 0, frameH, width - frameW, 0, height - frameH, width - frameW - thickness * sqrt(3.0), thickness, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPopMatrix();
}

void DrawPainting(float x, float y, float z, float width, float height, float angle, int textureNum)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0, 0.0, 1.0);

	DrawPlane(0, 1, 0, 0, 0, 0, width, 0, 0, width, 0, height, 0, 0, height, textureNum, 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPopMatrix();
}


/*
 * This function draws a white box, with xyz as the front lower left coordinate
 */
void DrawWhiteBox(float x, float y, float z, float width, float length, float height, float color[], int textureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[])
{
	// Top
	DrawPlane(0, 0, 1, x, y, z + height, x + width, y, z + height, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

	// W
	DrawPlane(-1, 0, 0, x, y, z, x, y + length, z, x, y + length, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

	// E
	DrawPlane(1, 0, 0, x + width, y, z, x + width, y + length, z, x + width, y + length, z + height, x + width, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

	// N
	DrawPlane(0, 1, 0, x, y + length, z, x + width, y + length, z, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

	// S
	DrawPlane(0, -1, 0, x, y, z, x + width, y, z, x + width, y, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

	// Bottom
	DrawPlane(0, 0, -1, x, y, z, x + width, y, z, x + width, y + length, z, x, y + length, z, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

}

/*
 * This function draws a quadrilateral plane given the corner vertices
 */
void DrawPlane(float xn, float yn, float zn, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int textureNum, int blend, float color[], float ambient[], float diffuse[], float specular[], float emission[], float shine[])
{

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	glColor4f(color[0], color[1], color[2], color[3]);

	// If texture is specified, apply texture
	if (textureNum >= 0) {
		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
		glBindTexture(GL_TEXTURE_2D, textures[textureNum]);
	}

	// Blend or modulate or do neither
	if (blend == 1) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (blend == 2)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glNormal3f(xn, yn, zn);

	if (textureNum >= 0)
		glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x0, y0, z0);

	if (textureNum >= 0)
		glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x1, y1, z1);

	if (textureNum >= 0)
		glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x2, y2, z2);

	if (textureNum >= 0)
		glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x3, y3, z3);

	glEnd();

	if (blend == 1)
		glDisable(GL_BLEND);
	else if (blend == 2)
		glDisable(GL_MODULATE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (textureNum >= 0)
		glDisable(GL_TEXTURE_2D);
}


void DrawPearlFloor()
{
	// Floor plane = xy-plane;
	DrawPlane(0, 0, 1, 0, 2, 0, 66, 2, 0, 66, 46, 0, 0, 46, 0, -1, 0, pureWhite, pearlAmbient, pearlDiffuse, pearlSpecular, defaultEmission, pearlShine);
}


/*
 * This function draw the walls
 */
void DrawWalls(float x, float y)
{
	float z = 10;
	float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
	float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
	float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };
	float wallShine[] = { 100 };

	DrawPlane(1, 0, 0, 0, y, 0, 0, 0, 0, 0, 0, z, 0, y, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-1, 0, 0, x, 0, 0, x, y, 0, x, y, z, x, 0, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, -1, 0, x, y, 0, 0, y, 0, 0, y, z, x, y, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

}


void DrawCeiling()
{
	DrawWhiteBox(0, 0, 10, 34, 10, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 10, 10, 6, 36, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(6, 14, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(34, 0, 10, 32, 46, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawWhiteBox(6, 22, 10, 28, 8, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(6, 34, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(6, 42, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(10, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(18, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(26, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(10, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(18, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(26, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawWhiteBox(10, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(18, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(26, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(10, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(18, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(26, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Ceiling windows, using nurbs
	for (int i = 0; i < 16; i++)
	{
		glColor4f(water[0], water[1], water[2], water[3]);
		glPushMatrix();
		if (i < 4)
			glTranslatef(6 + 8 * (i % 4), 10, 12);
		else if (i < 8)
			glTranslatef(6 + 8 * (i % 4), 18, 12);
		else if (i < 12)
			glTranslatef(6 + 8 * (i % 4), 30, 12);
		else if (i < 16)
			glTranslatef(6 + 8 * (i % 4), 38, 12);

		glScalef(1.333, 1.333, 1.333);
		gluBeginSurface(ceilingNurbs[i]);
		gluNurbsSurface(ceilingNurbs[i], numOfKnots, knots, numOfKnots, knots, 3 * 4, 3, &ceilingCtrlPts[i][0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(ceilingNurbs[i]);
		glPopMatrix();
	}
	glDisable(GL_BLEND);
}

void DrawALight(float rotX, float rotY, float rotZ, float x, float y, float z)
{
	float lightHangerheight = 10 - z;
	float lightHangerRadius = 0.05;
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rotZ, 0.0, 0.0, 1.0);
	glRotatef(rotY, 0.0, 1.0, 0.0);
	glRotatef(rotX, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);  // do we need all this?  isn't it in lightList?
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	gluCylinder(quadric, lightHangerRadius, lightHangerRadius, lightHangerheight, polySubdivisions, polySubdivisions);
	glCallList(lightList);
	glPopMatrix();
}


void DrawingManyLights()
{
	DrawALight(0, 0, 0, 12, 16, 8);
	DrawALight(0, 0, 0, 20, 16, 8);
	DrawALight(0, 0, 0, 28, 16, 8);

	DrawALight(0, 0, 0, 12, 32, 8);
	DrawALight(0, 0, 0, 20, 32, 8);
	DrawALight(0, 0, 0, 28, 36, 8);
	DrawALight(0, 0, 0, 12, 40, 8);
	DrawALight(0, 0, 0, 20, 40, 8);
}


void RenderVanGoghPaintingScene()
{
	float red[] = { 1, 0, 0 };
	float green[] = { 0, 1, 0 };
	float blue[] = { 0, 0, 1 };
	float white[] = { 1, 1, 1 };
	float yellow[] = { 0.6, 0.6, 0 };

	if (animationOn)
		ChooseGirlAct();  // draw girl for scene animation
	else
		DrawGirl();   // draw girl first

	DrawVGDoor();
	DrawVGPatio();
	DrawVGPatioRoof();
	DrawVGEndWall();
	DrawVGCafe();
	DrawVGFarBuilding();
	DrawVGGround();

	glCallList(forestList);

	glCallList(tablechairList);
}


void DrawVGGround()
{
	DrawPlane(0, 0, 1, 0, 10, 0, 0, 36, 0, -15, 36, 0, -15, 10, 0, 44, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -15, 10, 0, -15, 36, 0, -30, 36, 0, -30, 10, 0, 44, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -30, 10, 0, -30, 36, 0, -45, 36, 0, -45, 10, 0, 44, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -45, 0, 0, -45, 18, 0, -50, 18, 0, -50, 0, 0, 51, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -45, 18, 0, -45, 36, 0, -50, 36, 0, -50, 18, 0, 52, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}


void DrawVGDoor()
{
	DrawWhiteBox(-6.8, 10, 0, 0.8, 0.8, 7.8, gray, 50, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(-10.8, 10, 7, 4, 0.8, 0.8, gray, 49, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(-11.6, 10, 0, 0.8, 0.8, 7.8, gray, 50, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	DrawPlane(0, 1, 0, -6.8, 10.3, 0, -10.8, 10.3, 0, -10.8, 10.3, 7, -6.8, 10.3, 7, 0, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}


void DrawVGPatio()
{
	glColor4f(nurbRoofColor[0], nurbRoofColor[1], nurbRoofColor[2], nurbRoofColor[3]);
	DrawPlane(0, 0, 1, -15, 10, 0.3, -15, 18, 0.3, -45, 18, 0.3, -45, 10, 0.3, 48, 0, nurbRoofColor, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(1, 0, 0, -15, 10, 0, -15, 18, 0, -15, 18, 0.3, -15, 10, 0.3, 46, 0, nurbRoofColor, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, -15, 18, 0, -45, 18, 0, -45, 18, 0.3, -15, 18, 0.3, 47, 0, nurbRoofColor, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-1, 0, 0, -45, 18, 0, -45, 10, 0, -45, 10, 0.3, -45, 18, 0.3, 46, 0, nurbRoofColor, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}

void DrawVGPatioRoof()
{
	glColor4f(nurbRoofColor[0], nurbRoofColor[1], nurbRoofColor[2], nurbRoofColor[3]);
	glPushMatrix();
	glTranslatef(-12.4, 10, 11);
	glScalef(5.43, 2.67, 2.67);
	glCallList(nurbRoofList);
	glPopMatrix();
}

void DrawVGFarBuilding()
{
	DrawPlane(1, 0, 0, -42, 30, 0, -42, 36, 0, -42, 36, 30, -42, 30, 30, 42, 0, gray, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, -1, 0, -50, 30, 0, -42, 30, 0, -42, 30, 30, -50, 30, 30, 43, 0, gray, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -50, 30, 30, -42, 30, 30, -42, 36, 30, -50, 36, 30, -1, 0, gray, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}

void DrawVGCafe()
{
	DrawPlane(0, 1, 0, 0, 10, 0, -45, 10, 0, -45, 10, 30, 0, 10, 30, 45, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-1, 0, 0, -45, 10, 0, -45, 0, 0, -45, 0, 30, -45, 10, 30, -1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, -45, 0, 30, 0, 0, 30, 0, 10, 30, -45, 10, 30, -1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}


void DrawVGEndWall()
{
	DrawPlane(1, 0, 0, -50, 0, 0, -50, 36, 0, -50, 36, 40, -50, 0, 40, 40, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, -1, 0, 0, 36, 0, 0, 36, 40, -50, 36, 40, -50, 36, 0, 2, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, -45, 0, 0, -45, 0, 40, -50, 0, 40, -50, 0, 0, 2, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, -1, 0, 0, 40, 0, 36, 40, -50, 36, 40, -50, 0, 40, 2, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-1, 0, 0, 0, 0, 0, 0, 36, 0, 0, 36, 40, 0, 0, 40, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}


void InitializeParticleSystem()
{
	particlesTotal = 1000;
	particles = new CParticle[particlesTotal];
	float idleTime = 0;
	float dIdle = particleLifetime / particlesTotal;

	for (int i = 0; i < particlesTotal; i++)
	{
		InitializeAParticle(i, idleTime);
		idleTime += dIdle;
	}
}

void InitializeAParticle(int i, float idleTime)
{
	float sourceX = 0;
	float sourceY = 0;
	float sourceZ = 3.8;
	float sourceRadius = 0.01;

	float meanVelocityX = 1 * 1.7;
	float meanVelocityY = 0;
	float meanVelocityZ = 3.3 * 2.7;
	float meanVelocityVariant = 0.02;

	float meanAccelX = 0;
	float meanAccelY = 0;
	float meanAccelZ = -9.8;
	float meanAccelVariant = 0.07;

	float meanR = 1.0;
	float meanG = 1.0;
	float meanB = 1.0;
	float meanA = 0.8;
	float meanColorVariant = 0.05;

	float meanDR = 0.001;
	float meanDG = 0.001;
	float meanDB = 0.001;
	float meanDA = 0;
	float meanDColorVariant = 0.0001;

	float minLifetime = particleLifetime;
	float maxLifetime = particleLifetime + 1000;

	float pX = (sourceX - sourceRadius) + (2 * sourceRadius) * GEN_RANDOM_FLOAT;
	float pY = (sourceY - sourceRadius) + (2 * sourceRadius) * GEN_RANDOM_FLOAT;
	float pZ = (sourceZ - 1.2) + (8 * sourceRadius) * GEN_RANDOM_FLOAT;

	float pVX = (meanVelocityX - meanVelocityVariant) + (2 * meanVelocityVariant) * GEN_RANDOM_FLOAT;
	float pVY = (meanVelocityY - meanVelocityVariant) + (2 * meanVelocityVariant) * GEN_RANDOM_FLOAT;
	float pVZ = (meanVelocityZ - meanVelocityVariant) + (2 * meanVelocityVariant) * GEN_RANDOM_FLOAT;

	float pAX = (meanAccelX - meanAccelVariant) + (2 * meanAccelVariant) * GEN_RANDOM_FLOAT;
	float pAY = (meanAccelY - meanAccelVariant) + (2 * meanAccelVariant) * GEN_RANDOM_FLOAT;
	float pAZ = (meanAccelZ - meanAccelVariant) + (2 * meanAccelVariant) * GEN_RANDOM_FLOAT;

	float pR = (meanR - meanColorVariant) + (2 * meanColorVariant) * GEN_RANDOM_FLOAT;
	float pG = (meanG - meanColorVariant) + (2 * meanColorVariant) * GEN_RANDOM_FLOAT;
	float pB = (meanB - meanColorVariant) + (2 * meanColorVariant) * GEN_RANDOM_FLOAT;
	float pA = (meanA - meanColorVariant) + (2 * meanColorVariant) * GEN_RANDOM_FLOAT;

	float pDR = (meanDR - meanDColorVariant) + (2 * meanDColorVariant) * GEN_RANDOM_FLOAT;
	float pDG = (meanDG - meanDColorVariant) + (2 * meanDColorVariant) * GEN_RANDOM_FLOAT;
	float pDB = (meanDB - meanDColorVariant) + (2 * meanDColorVariant) * GEN_RANDOM_FLOAT;
	float pDA = (meanDA - meanDColorVariant) + (2 * meanDColorVariant) * GEN_RANDOM_FLOAT;


	float pLifetime = minLifetime + (maxLifetime - minLifetime) * GEN_RANDOM_FLOAT;

	int pAngle = (int)(3 * GEN_RANDOM_FLOAT) % 3;

	particles[i] = CParticle(pX, pY, pZ, pAngle, pVX, pVY, pVZ, pAX, pAY, pAZ, pR, pG, pB, pA, pDR, pDG, pDB, pDA, -1, idleTime, pLifetime, sourceRadius);
}


void DrawPraticleSystem()
{
	float fountainX = 6;
	float fountainY = -14;

	float numStreams = 3;
	float dAngle = 360 / numStreams;

	for (int i = 0; i < particlesTotal; i++)
	{
		float currentTime = glutGet(GLUT_ELAPSED_TIME);

		if (particles[i].startTime == -1)
			particles[i].startTime = currentTime + particles[i].idleTime;

		if (currentTime >= particles[i].startTime)
		{
			float dTime;
			float dRadius = 0.6;

			if (particles[i].lastTime == -1)
				dTime = 0;
			else
				dTime = currentTime - particles[i].lastTime;

			particles[i].lastTime = currentTime;

			float randomZ = particles[i].radius * GEN_RANDOM_FLOAT;
			float randomX = 0.1 * GEN_RANDOM_FLOAT;

			if (currentTime - particles[i].startTime < particles[i].lifeTime)
			{
				dTime /= 1000.0;

				particles[i].vPosition.x += particles[i].vVelocity.x * dTime + randomX;
				particles[i].vPosition.y += particles[i].vVelocity.y * dTime;
				particles[i].vPosition.z += particles[i].vVelocity.z * dTime - randomZ;

				particles[i].vVelocity.x += particles[i].vAcceleration.x * dTime;
				particles[i].vVelocity.y += particles[i].vAcceleration.y * dTime;
				particles[i].vVelocity.z += particles[i].vAcceleration.z * dTime;

				particles[i].color[0] += particles[i].dColor[0] * particles[i].lifeTime * dTime;
				particles[i].color[1] += particles[i].dColor[1] * particles[i].lifeTime * dTime;
				particles[i].color[2] += particles[i].dColor[2] * particles[i].lifeTime * dTime;
				particles[i].color[3] += particles[i].dColor[3] * particles[i].lifeTime * dTime;

				particles[i].radius = dRadius;

				if (particles[i].vPosition.z >= hBase + hBaseStand + hMain)
				{
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glColor4f(particles[i].color[0], particles[i].color[1], particles[i].color[2], particles[i].color[3]);

					glPushMatrix();
					glTranslatef(fountainX, fountainY, 0);
					glRotatef(dAngle * particles[i].angle, 0.0, 0.0, 1.0);
					glTranslatef(particles[i].vPosition.x - randomX - 0.25, particles[i].vPosition.y, particles[i].vPosition.z + randomZ);
					glCallList(particleList);
					glPopMatrix();
					glDisable(GL_BLEND);
				}
				else
					InitializeAParticle(i, 0);
			}

			else
				InitializeAParticle(i, 0);
		}
	}
}

void ChooseGirlAct()
{
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	bool dodraw = false;

	if (firstTime)
	{
		initialTime = currentTime;
		firstTime = false;
	}

	switch (currentAct)
	{
	case 0:
		if (currentTime >= 13000)
		{
			girlAct = girlAct1;
			girlNumAct = girlNumAct1;
			girlStateAct = girlStateAct1;
			girlTimeAct = girlTimeAct1;
			girlHeadAct = girlHeadAct1;
			dodraw = true;
		}
		break;
	case 1:
		if (currentTime >= 26500)
		{
			girlAct = girlAct2;
			girlNumAct = girlNumAct2;
			girlStateAct = girlStateAct2;
			girlTimeAct = girlTimeAct2;
			girlHeadAct = girlHeadAct2;
			dodraw = true;
		}
		break;
	case 2:
		if (currentTime >= 51000)
		{
			girlAct = girlAct3;
			girlNumAct = girlNumAct3;
			girlStateAct = girlStateAct3;
			girlTimeAct = girlTimeAct3;
			girlHeadAct = girlHeadAct3;
			dodraw = true;
		}
		break;
	case 3:
		if (currentTime >= 91000)
		{
			girlAct = girlAct4;
			girlNumAct = girlNumAct4;
			girlStateAct = girlStateAct4;
			girlTimeAct = girlTimeAct4;
			girlHeadAct = girlHeadAct4;
			dodraw = true;
		}
		break;
	default:
		break;
	}

	//	sprintf(debugText,"cumulative time: %f currentAct:%d  gc: %d", (currentTime - initialTime)/1000, currentAct, girlCurrent);
	//	SetWindowText(g_hWnd, debugText);

	if (dodraw) {
		DrawGirlActs();
		dodraw = false;
	}
}

void DrawGirlActs()
{
	if (girlResetArmLeg == 1)
	{
		if (girlDirection > 0 && girlLegRotation >= 0 || girlDirection < 0 && girlLegRotation <= 0)
		{
			girlResetArmLeg = 0;
			girlArmRotation = 0;
			girlLegRotation = 0;
		}

		GirlHead(girlAct[girlCurrent].x, girlAct[girlCurrent].y, girlAct[girlCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
		GirlBody(girlAct[girlCurrent].x, girlAct[girlCurrent].y, girlAct[girlCurrent].z - 2.222);
		GirlLeg(girlAct[girlCurrent].x, girlAct[girlCurrent].y, girlAct[girlCurrent].z - 1.586, initialOrientation);
		GirlArm(girlAct[girlCurrent].x, girlAct[girlCurrent].y, girlAct[girlCurrent].z - 0.635, initialOrientation);
	}
	else
	{
		if (girlResetStartTime)
		{
			girlStartTime = glutGet(GLUT_ELAPSED_TIME);
			girlResetStartTime = false;
		}

		float progress = 0.0;
		float destOrientation;
		CVector3 currentDestVector;

		if (girlCurrent < girlNumAct - 1)
		{
			currentDestVector = girlAct[girlCurrent + 1] - girlAct[girlCurrent];

			girlCurrentTime = glutGet(GLUT_ELAPSED_TIME);
			float dTime = girlCurrentTime - girlStartTime;

			progress = dTime / (girlTimeAct[girlCurrent] * 1000);

			if (progress > 1.0)
				progress = 1.0;

			CVector3 currentPos;
			float currentOrientation;

			if (girlStateAct[girlCurrent] < 0)	//walking or running state
			{
				// current position
				currentPos = (currentDestVector * progress) + girlAct[girlCurrent];

				if (currentDestVector.y != 0)
				{
					destOrientation = atan(currentDestVector.y / currentDestVector.x) * (180 / 3.14) + 90;
					if (currentDestVector.x < 0)
						destOrientation += 180;
				}
				else
					destOrientation = initialOrientation;
				if (progress <= 1.0 && destOrientation - initialOrientation < -180)
					currentOrientation = ((destOrientation - initialOrientation + 360) * progress) + initialOrientation;
				else if (progress <= 1.0)
					currentOrientation = ((destOrientation - initialOrientation) * progress) + initialOrientation;
				else
					currentOrientation = destOrientation;
			}
			else if (girlStateAct[girlCurrent] == 0)	//stopping state
			{
				currentPos = girlAct[girlCurrent];
				currentOrientation = initialOrientation;
				destOrientation = initialOrientation;
			}
			else if (girlStateAct[girlCurrent] > 0)	//turning state
			{
				currentPos = girlAct[girlCurrent];
				destOrientation = girlStates[girlCurrent];
				if (progress <= 1.0 && destOrientation - initialOrientation < -180)
					currentOrientation = ((destOrientation - initialOrientation + 360) * progress) + initialOrientation;
				else if (progress <= 1.0)
					currentOrientation = ((destOrientation - initialOrientation) * progress) + initialOrientation;
				else
					currentOrientation = destOrientation;
			}

			float destHeadTurnX = 0;
			float destHeadTurnY = 0;
			float destHeadTurnZ = 0;

			if (progress <= 0.5)
			{
				initialHeadTurn.x = girlHeadAct[girlCurrent].x * 2 * progress;
				initialHeadTurn.y = girlHeadAct[girlCurrent].y * 2 * progress;
				initialHeadTurn.z = girlHeadAct[girlCurrent].z * 2 * progress;
			}
			else if (progress > 0.5)
			{
				initialHeadTurn.x = girlHeadAct[girlCurrent].x - girlHeadAct[girlCurrent].x * 2 * (progress - 0.5);
				initialHeadTurn.y = girlHeadAct[girlCurrent].y - girlHeadAct[girlCurrent].y * 2 * (progress - 0.5);
				initialHeadTurn.z = girlHeadAct[girlCurrent].z - girlHeadAct[girlCurrent].z * 2 * (progress - 0.5);
			}

			GirlHead(currentPos.x, currentPos.y, currentPos.z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, currentOrientation);
			GirlBody(currentPos.x, currentPos.y, currentPos.z - 2.222);
			GirlLeg(currentPos.x, currentPos.y, currentPos.z - 1.586, currentOrientation);
			GirlArm(currentPos.x, currentPos.y, currentPos.z - 0.635, currentOrientation);

			if (progress >= 1.0)
			{
				girlResetStartTime = true;
				if (++girlCurrent >= girlNumAct - 1)
				{
					girlCurrent = 0;
					currentAct++;
				}

				if (girlStateAct[girlCurrent] == 0)
				{
					girlDirection *= -1;	//change direction
					girlResetArmLeg = 1;	//reset arm and leg positions
				}
				initialOrientation = destOrientation;
				initialHeadTurn = CVector3(0, 0, 0);

			}

		}
	}
}

void DrawShadowBall()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glPushMatrix();
	glTranslatef(33, 34, -0.6);
	glPushMatrix();
	glTranslatef(2.5, 0, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.04, 1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, 2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.25, 2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.04, 1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.5, 0, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.04, -1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.25, -2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, -2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.04, -1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glutSolidSphere(0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(33, 34, 0.2);
	glPushMatrix();
	glRotatef(degreeLong, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0.25, 0.5, 0);
	glVertex3f(0, 2.1, 0);
	glVertex3f(-0.25, 0.5, 0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotatef(degreeShort, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0.275, 0.5, 0);
	glVertex3f(0, 1.34, 0);
	glVertex3f(-0.275, 0.5, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void DrawReflectiveBall1()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(white[0], white[1], white[2], white[3]);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textures[61]);

	glPushMatrix();
	glTranslatef(26, 12.5, 0);
	gluSphere(quadric, 1.0, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glTranslatef(26, 12.5, 0);
	gluSphere(quadric, 1.0, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

}


void DrawReflectiveBall2()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glPushMatrix();
	glTranslatef(33, 34, 0.3);
	glPushMatrix();
	glTranslatef(2.5, 0, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.04, 1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, 2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.25, 2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.04, 1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.5, 0, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.04, -1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.25, -2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.25, -2.04, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.04, -1.25, 0);
	gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	gluDisk(quadric, 0, 0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glutSolidSphere(0.25, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(33, 34, 0.8);
	glPushMatrix();
	glRotatef(degreeLong, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0.25, 0.5, 0);
	glVertex3f(0, 2.1, 0);
	glVertex3f(-0.25, 0.5, 0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glRotatef(degreeShort, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0.275, 0.5, 0);
	glVertex3f(0, 1.34, 0);
	glVertex3f(-0.275, 0.5, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


void DrawReflectiveObjects()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	glColor4f(1.0, 0.0, 0.0, 1.0);

	glPushMatrix();
	glTranslatef(29, 15, 0);
	gluCylinder(quadric, 1.0, 0, 4, polySubdivisions, polySubdivisions);
	glPopMatrix();
}

void DrawReflectiveFloor1()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(23, 10, platformHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(31, 10, platformHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(31, 18, platformHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(23, 18, platformHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void DrawReflectiveFloor2()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4f(pureWhite[0], pureWhite[1], pureWhite[2], pureWhite[3]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(30, 31, platformHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(30, 37, platformHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(36, 37, platformHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(36, 31, platformHeight);

	glEnd();
}


void DrawSidesofReflectivePlane1()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(23, 18, 0);

	glTexCoord2f(1, 0);
	glVertex3f(23, 10, 0);

	glTexCoord2f(1, 1);
	glVertex3f(23, 10, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(23, 18, platformHeight);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(31, 10, 0);

	glTexCoord2f(1, 0);
	glVertex3f(31, 18, 0);

	glTexCoord2f(1, 1);
	glVertex3f(31, 18, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(31, 10, platformHeight);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(23, 10, 0);

	glTexCoord2f(1, 0);
	glVertex3f(31, 10, 0);

	glTexCoord2f(1, 1);
	glVertex3f(31, 10, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(23, 10, platformHeight);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(31, 18, 0);

	glTexCoord2f(1, 0);
	glVertex3f(23, 18, 0);

	glTexCoord2f(1, 1);
	glVertex3f(23, 18, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(31, 18, platformHeight);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawSidesofReflectivePlane2()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(30, 37, 0);

	glTexCoord2f(1, 0);
	glVertex3f(30, 31, 0);

	glTexCoord2f(1, 1);
	glVertex3f(30, 31, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(30, 37, platformHeight);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(36, 31, 0);

	glTexCoord2f(1, 0);
	glVertex3f(36, 37, 0);

	glTexCoord2f(1, 1);
	glVertex3f(36, 37, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(36, 31, platformHeight);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(30, 31, 0);

	glTexCoord2f(1, 0);
	glVertex3f(36, 31, 0);

	glTexCoord2f(1, 1);
	glVertex3f(36, 31, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(30, 31, platformHeight);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(36, 37, 0);

	glTexCoord2f(1, 0);
	glVertex3f(30, 37, 0);

	glTexCoord2f(1, 1);
	glVertex3f(30, 37, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(36, 37, platformHeight);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawReflectiveCones()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	glColor4f(cone[0], cone[1], cone[2], cone[3]);
	glPushMatrix();
	glTranslatef(27, 15, 0);
	gluCylinder(quadric, 0.6, 0.3, 2, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.5, 16, 0.2966);
	glRotatef(98.63, 1, 0, 0);
	gluCylinder(quadric, 0.3, 0, 2, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(28.5, 13.5, 0);
	gluCylinder(quadric, 1.0, 0, 4, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(28.8, 15.8, 0);
	gluCylinder(quadric, 0.8, 0, 3.5, polySubdivisions, polySubdivisions);
	glPopMatrix();
}


void shadowMatrix(float shadowMat[4][4], float groundPlane[4], float lightPos[4])
{
	float dot = groundPlane[0] * lightPos[0] + groundPlane[1] * lightPos[1] + groundPlane[3] * lightPos[2] + groundPlane[3] * lightPos[3];

	shadowMat[0][0] = dot - lightPos[0] * groundPlane[0];
	shadowMat[1][0] = 0.0 - lightPos[0] * groundPlane[1];
	shadowMat[2][0] = 0.0 - lightPos[0] * groundPlane[2];
	shadowMat[3][0] = 0.0 - lightPos[0] * groundPlane[3];

	shadowMat[0][1] = 0.0 - lightPos[1] * groundPlane[0];
	shadowMat[1][1] = dot - lightPos[1] * groundPlane[1];
	shadowMat[2][1] = 0.0 - lightPos[1] * groundPlane[2];
	shadowMat[3][1] = 0.0 - lightPos[1] * groundPlane[3];

	shadowMat[0][2] = 0.0 - lightPos[2] * groundPlane[0];
	shadowMat[1][2] = 0.0 - lightPos[2] * groundPlane[1];
	shadowMat[2][2] = dot - lightPos[2] * groundPlane[2];
	shadowMat[3][2] = 0.0 - lightPos[2] * groundPlane[3];

	shadowMat[0][3] = 0.0 - lightPos[3] * groundPlane[0];
	shadowMat[1][3] = 0.0 - lightPos[3] * groundPlane[1];
	shadowMat[2][3] = 0.0 - lightPos[3] * groundPlane[2];
	shadowMat[3][3] = dot - lightPos[3] * groundPlane[3];

}



bool IntersectAt(float y, float minX, float maxX)
{
	CVector3 pos = g_Camera.Position();
	CVector3 view = g_Camera.View();
	float angle = 45;

	float cosAlpha = (float)cos(angle * PIdiv180);
	float sinAlpha = (float)sin(angle * PIdiv180);

	float cosBetha = (float)cos(-angle * PIdiv180);
	float sinBetha = (float)sin(-angle * PIdiv180);

	CVector3 vTheta = view - pos;

	if (vTheta.y > 0)
		return false;

	CVector3 vAlpha;
	CVector3 vBetha;

	vAlpha.x = vTheta.x * cosAlpha - vTheta.y * sinAlpha;
	vAlpha.y = vTheta.x * sinAlpha + vTheta.y * cosAlpha;
	vAlpha.z = vTheta.z;

	vBetha.x = vTheta.x * cosBetha - vTheta.y * sinBetha;
	vBetha.y = vTheta.x * sinBetha + vTheta.y * cosBetha;
	vBetha.z = vTheta.z;

	float ratioAlpha = vAlpha.y / vAlpha.x;
	float newXAlpha = (y - pos.y) / ratioAlpha + pos.x;

	float ratioBetha = vBetha.y / vBetha.x;
	float newXBetha = (y - pos.y) / ratioBetha + pos.x;


	//sprintf(debugText, "vA.y:%f  vB.y:%f  newXAlpha:%f  newXBetha:%f", vAlpha.y, vBetha.y, newXAlpha, newXBetha);
	//SetWindowText(g_hWnd, debugText);

	if (vAlpha.y < 0 && vBetha.y < 0)
	{
		if ((newXAlpha < minX && newXBetha < minX) || (newXAlpha > maxX&& newXBetha > maxX))
			return false;
		else
			return true;
	}
	else if (vBetha.y >= 0)
	{
		if (newXAlpha >= minX)
			return true;
		else
			return false;
	}
	else if (vAlpha.y >= 0)
	{
		if (newXBetha <= maxX)
			return true;
		else
			return false;
	}
	else
		return false;

}


bool FacingXPos()
{
	CVector3 pos = g_Camera.Position();
	CVector3 view = g_Camera.View();
	float angle = 45;

	float cosAlpha = (float)cos(angle * PIdiv180);
	float sinAlpha = (float)sin(angle * PIdiv180);

	float cosBetha = (float)cos(-angle * PIdiv180);
	float sinBetha = (float)sin(-angle * PIdiv180);

	CVector3 vTheta = view - pos;

	CVector3 vAlpha;
	CVector3 vBetha;

	vAlpha.x = vTheta.x * cosAlpha - vTheta.y * sinAlpha;
	vAlpha.y = vTheta.x * sinAlpha + vTheta.y * cosAlpha;
	vAlpha.z = vTheta.z;

	vBetha.x = vTheta.x * cosBetha - vTheta.y * sinBetha;
	vBetha.y = vTheta.x * sinBetha + vTheta.y * cosBetha;
	vBetha.z = vTheta.z;

	return (vAlpha.x > 0 || vBetha.x > 0);
}

bool FacingXNeg()
{
	CVector3 pos = g_Camera.Position();
	CVector3 view = g_Camera.View();
	float angle = 45;

	float cosAlpha = (float)cos(angle * PIdiv180);
	float sinAlpha = (float)sin(angle * PIdiv180);

	float cosBetha = (float)cos(-angle * PIdiv180);
	float sinBetha = (float)sin(-angle * PIdiv180);

	CVector3 vTheta = view - pos;

	CVector3 vAlpha;
	CVector3 vBetha;

	vAlpha.x = vTheta.x * cosAlpha - vTheta.y * sinAlpha;
	vAlpha.y = vTheta.x * sinAlpha + vTheta.y * cosAlpha;
	vAlpha.z = vTheta.z;

	vBetha.x = vTheta.x * cosBetha - vTheta.y * sinBetha;
	vBetha.y = vTheta.x * sinBetha + vTheta.y * cosBetha;
	vBetha.z = vTheta.z;

	return (vAlpha.x < 0 || vBetha.x < 0);
}

bool FacingYPos()
{
	CVector3 pos = g_Camera.Position();
	CVector3 view = g_Camera.View();
	float angle = 45;

	float cosAlpha = (float)cos(angle * PIdiv180);
	float sinAlpha = (float)sin(angle * PIdiv180);

	float cosBetha = (float)cos(-angle * PIdiv180);
	float sinBetha = (float)sin(-angle * PIdiv180);

	CVector3 vTheta = view - pos;

	CVector3 vAlpha;
	CVector3 vBetha;

	vAlpha.x = vTheta.x * cosAlpha - vTheta.y * sinAlpha;
	vAlpha.y = vTheta.x * sinAlpha + vTheta.y * cosAlpha;
	vAlpha.z = vTheta.z;

	vBetha.x = vTheta.x * cosBetha - vTheta.y * sinBetha;
	vBetha.y = vTheta.x * sinBetha + vTheta.y * cosBetha;
	vBetha.z = vTheta.z;

	return (vAlpha.y > 0 || vBetha.y > 0);
}

bool FacingYNeg()
{
	CVector3 pos = g_Camera.Position();
	CVector3 view = g_Camera.View();
	float angle = 45;

	float cosAlpha = (float)cos(angle * PIdiv180);
	float sinAlpha = (float)sin(angle * PIdiv180);

	float cosBetha = (float)cos(-angle * PIdiv180);
	float sinBetha = (float)sin(-angle * PIdiv180);

	CVector3 vTheta = view - pos;

	CVector3 vAlpha;
	CVector3 vBetha;

	vAlpha.x = vTheta.x * cosAlpha - vTheta.y * sinAlpha;
	vAlpha.y = vTheta.x * sinAlpha + vTheta.y * cosAlpha;
	vAlpha.z = vTheta.z;

	vBetha.x = vTheta.x * cosBetha - vTheta.y * sinBetha;
	vBetha.y = vTheta.x * sinBetha + vTheta.y * cosBetha;
	vBetha.z = vTheta.z;

	return (vAlpha.y < 0 || vBetha.y < 0);
}

bool InRoomI()
{
	float minX = 0;
	float maxX = 30;
	float minY = 5;
	float maxY = 25;

	return (g_Camera.Position().x >= minX && g_Camera.Position().x <= maxX
		&& g_Camera.Position().y >= minY && g_Camera.Position().y <= maxY);
}

bool InRoomIIA()
{
	float minX = 30;
	float maxX = 46;
	float minY = 25;
	float maxY = 46;

	return (g_Camera.Position().x >= minX && g_Camera.Position().x <= maxX
		&& g_Camera.Position().y >= minY && g_Camera.Position().y <= maxY);
}

bool InRoomIIB()
{
	float minX = 30;
	float maxX = 46;
	float minY = 5;
	float maxY = 25;

	return (g_Camera.Position().x >= minX && g_Camera.Position().x <= maxX
		&& g_Camera.Position().y >= minY && g_Camera.Position().y <= maxY);
}

bool InRoomIII()
{
	float minX = 0;
	float maxX = 30;
	float minY = 25;
	float maxY = 46;

	return (g_Camera.Position().x >= minX && g_Camera.Position().x <= maxX
		&& g_Camera.Position().y >= minY && g_Camera.Position().y <= maxY);
}

bool InRoomIV()		// Cinema
{
	float minX = 46;
	float maxX = 66;
	float minY = 5;
	float maxY = 25;

	return (g_Camera.Position().x >= minX && g_Camera.Position().x <= maxX
		&& g_Camera.Position().y >= minY && g_Camera.Position().y <= maxY);
}

void RenderOutdoor()
{
	//if ((g_Camera.Position().z >= 10) || ((cameraCurrent < 5 || cameraCurrent == 26) && (g_Camera.Position().y < 5 || IntersectAt(5, 12, 20))))
	//{
		// Draw hills
		//glPushMatrix();
		//glTranslatef(15, -40, 0);
		//glCallList(hillsList);
		//glPopMatrix();
		//glPushMatrix();
		//glTranslatef(25, -30, 0);
		//glCallList(hillsList);
		//glPopMatrix();

		// Outdoor floor
	DrawPlane(0, 0, 1, 100, -100, 0, 100, 100, 0, -100, 100, 0, -100, -100, 0, -1, 1, hillColor2, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawPlane(0, 0, 1, -1, -54, 0, 100, -54, 0, 100, -5, 0, -1, -5, 0, -1, 1, hillColor2, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//DrawPlane(0, 0, 1, -1, -5, 0, 66, -5, 0, 66, 2, 0, -1, 2, 0, -1, 0, hillColor2, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, textures[53]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DrawPlane(0, 0, -1, 99, -53, 50, -1, -53, 50, -1, 47, 50, 99, 47, 50, 55, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	if (!animationOn)
		DrawPlane(1, 0, 0, -1, -53, 0, -1, 47, 0, -1, 47, 50, -1, -53, 50, 56, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-1, 0, 0, 99, 47, 0, 99, -53, 0, 99, -53, 50, 99, 47, 50, 57, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, 99, -53, 0, -1, -53, 0, -1, -53, 50, 99, -53, 50, 58, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, -1, 0, -1, 47, 0, 99, 47, 0, 99, 47, 50, -1, 47, 50, 59, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glDisable(GL_TEXTURE_2D);

	glCallList(treesList);

	if (particlesIsNotInitialized)
	{
		InitializeParticleSystem();
		particlesIsNotInitialized = false;
	}
	if (g_Camera.objectsAnimationOn)
		DrawPraticleSystem();
	//}
}

// This is the room with the bouncing balls
void RenderRoomI()
{
	// Room I			
	DrawReflections();

	DrawFrame(30, 5.3, 2, 4.936, 6.664, 3.9488, 5.3312, 180.0, brown);
	DrawPainting(29.5064, 5.15, 2.6664, 3.9488, 5.3312, 180.0, 6);

	DrawFrame(4.5, 24.8, 2.0, 11.8, 6.175, 12, 6.375, 0, brown);
	DrawPainting(4.55, 24.9, 2.05, 12, 6.375, 0, 5);

	DrawFrame(19.95, 24.8, 2.95, 6.7512, 5.0628, 6.4512, 4.7628, 0, brown);
	DrawPainting(20.1, 24.9, 3.1, 6.4512, 4.7628, 0, 10);

	DrawFrame(0.3, 11.5, 1.3, 6.176, 7.814, 5.976, 7.614, 90, brown);
	DrawPainting(0.2, 11.7, 1.4, 5.976, 7.614, 90, 4);

	//	if //((g_Camera.Position().y > 0 && g_Camera.Position().y <= 7) || 
	//		(IntersectAt(5.1, 3.1, 8.9) || IntersectAt(6.9, 3.1, 8.9))//)
	//	{
	DrawBouncingBalls();

	DrawWhiteBox(4, 5, 0, 1, 1.5, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(7, 5, 0, 1, 1.5, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(3, 5, 2, 6, 2, 0.3, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	//	}
}

// This is the room with the step object
void RenderRoomIIA()
{
	DrawFrame(27, 45.8, 2.0, 7.692, 5.998, 7.392, 5.698, 0, brown);
	DrawPainting(27.1, 45.95, 2.15, 7.392, 5.698, 0, 17);

	DrawWhiteBox(43.0, 44.0, 0.0, 2.0, 2.0, 0.5, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawFrame(45, 30.15, 1.0, 11.52, 8.3662, 11.22, 7.9662, 90, brown);
	DrawPainting(44.9, 30.25, 1.2, 11.223, 8.0, 90, 11);

	DrawStepObject(44, 45, 0.5, 0.6, 30, 8, bulbColor);
}

// This is the room with the teapot
void RenderRoomIIB()
{
	DrawOrbitz();
	DrawWhiteBox(43.5, 22.5, 0.0, 1.5, 1.5, 2.8, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawFrame(42, 5.3, 2.3, 7.2, 6.02, 7, 5.82, 180, brown);
	DrawPainting(41.9, 5.15, 2.4, 7, 5.82, 180, 12);

	glCallList(teapotMuseumList);
	DrawWhiteBox(43.85, 7.0, 2.8, 0.8, 0.8, 0.2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(43.5, 6.75, 0, 1.5, 1.5, 2.8, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}

// This is the room with the hanging wall
void RenderRoomIII()
{
	DrawWhiteBox(6.5, 34.5, 0.0, 17, 2, 0.5, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawWhiteBox(7.5, 35.75, 2, 15, 0.5, 5, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);	// hanging wall
	DrawCylinder(9.375, 36, 7, 0.15, 3, white);
	DrawCylinder(13.125, 36, 7, 0.15, 3, white);
	DrawCylinder(16.875, 36, 7, 0.15, 3, white);
	DrawCylinder(20.625, 36, 7, 0.15, 3, white);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	DrawPlane(-1, 0, 0, 10, 35.75, 2.67, 10, 35.55, 2.67, 10, 35.55, 6.33, 10, 35.75, 6.33, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(1, 0, 0, 15, 35.75, 2.67, 15, 35.55, 2.67, 15, 35.55, 6.33, 15, 35.75, 6.33, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, 10, 35.75, 6.33, 10, 35.55, 6.33, 15, 35.55, 6.33, 15, 35.75, 6.33, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, -1, 10, 35.75, 2.67, 10, 35.55, 2.67, 15, 35.55, 2.67, 10, 35.75, 2.67, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPainting(10, 35.53, 2.67, 5, 3.66, 0, 7);

	DrawPlane(-1, 0, 0, 18, 35.75, 2.5, 18, 35.55, 2.5, 18, 35.55, 6.4, 18, 35.75, 6.4, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(1, 0, 0, 21.003, 35.75, 2.5, 21.003, 35.55, 2.5, 21.003, 35.55, 6.4, 21.003, 35.75, 6.4, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, 18, 35.75, 6.4, 18, 35.55, 6.4, 21.003, 35.55, 6.4, 21.003, 35.75, 6.4, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, -1, 18, 35.75, 2.5, 18, 35.55, 2.5, 21.003, 35.55, 2.5, 18, 35.75, 2.5, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPainting(18, 35.53, 2.5, 3.003, 3.9, 0, 8);

	DrawPlane(0, -1, 0, 0, 31.302, 1.5, 0.2, 31.302, 1.5, 0.2, 31.302, 8.628, 0, 31.302, 8.628, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, 0, 40.698, 1.5, 0.2, 40.698, 1.5, 0.2, 40.698, 8.628, 0, 40.698, 8.628, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, 1, 0, 31.302, 8.628, 0.2, 31.302, 8.628, 0.2, 40.698, 8.628, 0, 40.698, 8.628, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0, -1, 0, 31.302, 1.5, 0.2, 31.302, 1.5, 0.2, 40.698, 1.5, 0, 40.698, 1.5, -1, 0, pureWhite, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPainting(0.2, 31.302, 1.5, 9.396, 7.128, 90, 13);

	DrawFrame(13, 45.8, 1.0, 5.445, 8.07, 5.145, 7.77, 0, brown);
	DrawPainting(13.15, 45.9, 1.15, 5.145, 7.77, 0, 15);

	DrawFrame(17, 36.5, 2.4, 4.08, 4.04, 3.68, 3.64, 180, brown);
	DrawPainting(16.8, 36.43, 2.6, 3.68, 3.64, 180, 16);
}

// This is the room with the slideshow
void RenderRoomIV()
{
	// Cinema
	DrawWhiteBox(52.5, 8.5, 1, 7, 2.5, 0.5, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(53.25, 8.75, 0, 1, 2, 1, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(57.75, 8.75, 0, 1, 2, 1, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawSlides();
}

// ### function used to draw bathroom part I
void drawBathroomI() {
	glPushMatrix();
	glTranslatef(bathroomI_x, bathroomI_y, bathroomI_z);

	glPushMatrix();
	glTranslatef(3.2, 3.2, 0);

	// drawBathroomLight(0, 0, 0, 18, 16, 8);

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-0.5, 1.4, 1.2);
	glCallList(basinList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, -3, 0);
	drawCaseSurface();
	drawSidesOfCase();
	glPopMatrix();
	glPopMatrix();
	drawBathroomWall();

	glPopMatrix();
}

// function used to draw the 2nd part of bathroom
void drawBathroomII() {
	drawShowerRoom();
	// add the stool to the scene
	glPushMatrix();
	glTranslatef(23, 13, 0.1);
	glScalef(0.6, 0.6, 0.6);
	stoolObj.Draw();
	glPopMatrix();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glScalef(1.8, 1.8, 2.4);
	glTranslatef(10.6, -29, 0.1);
	//glScalef(0.6, 0.6, 0.6);
	// add bath room door to the scene
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	bathroomDoorObj.Draw();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_MODULATE);
	glPopMatrix();
}

// function used to draw the whole bathroom
void drawBathroom() {
	drawBathroomI();
	drawBathroomII();
}

void drawSidesOfCase()
{
	glColor4f(1.0, 0.3, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, 6, 0);

	glTexCoord2f(1, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1, 1);
	glVertex3f(0, 0, 3);

	glTexCoord2f(0, 1);
	glVertex3f(0, 6, 3);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(3, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(3, 6, 0);

	glTexCoord2f(1, 1);
	glVertex3f(3, 6, 3);

	glTexCoord2f(0, 1);
	glVertex3f(3, 0, 3);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(1, 0);
	glVertex3f(3, 0, 0);

	glTexCoord2f(1, 1);
	glVertex3f(3, 0, 3);

	glTexCoord2f(0, 1);
	glVertex3f(0, 0, 3);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(3, 6, 0);

	glTexCoord2f(1, 0);
	glVertex3f(0, 6, 0);

	glTexCoord2f(1, 1);
	glVertex3f(0, 6, 3);

	glTexCoord2f(0, 1);
	glVertex3f(3, 6, 3);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCaseSurface()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	// glColor4f(pureWhite[0], pureWhite[1], pureWhite[2], pureWhite[3]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, caseHeight);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 6, caseHeight);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3, 6, caseHeight);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3, 0, caseHeight);
	glDisable(GL_TEXTURE_2D);

	glEnd();
}

void drawBathroomWall() {

	// 1st wall
	glPushMatrix();
	glTranslatef(-2, 0, 0.1);
	DrawWhiteBox(0, 0, 0, 2, 15, 9, wallGreen, 19, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	// 2nd wall
	glPushMatrix();
	glTranslatef(10, -2, 0.1);
	glRotatef(90, 0, 0, 1);
	DrawWhiteBox(0, 0, 0, 2, 12, 9, wallGreen, 19, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	// 3rd wall
	glPushMatrix();
	glTranslatef(15, 15, 0.1);
	glRotatef(90, 0, 0, 1);
	DrawWhiteBox(0, 0, 0, 2, 17, 9, wallGreen, 19, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	// 4th wall
	glPushMatrix();
	glTranslatef(15, -2, 0.1);
	DrawWhiteBox(0, 0, 0, 2, 19, 9, wallGreen, 19, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// bathroom floor
	DrawPlane(0, 0, 1, 0, 0, 0.1, 0, 15, 0.1, 15, 15, 0.1, 15, 0, 0.1, 1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);\

	// bathroom ceiling
	glPushMatrix();
	glTranslatef(-2, -2, 9);
	DrawWhiteBox(0, 0, 0, 19, 19, 2, gray, 19, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}

void drawShowerRoom()
{

	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);

	// The 1st glass of shower room
	// Vertical frames of shower room
	glPushMatrix();
	glScalef(0.8, 0.6, 1.0);
	glTranslatef(1, 11.5, 0.1);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// glasses
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// The 2nd glass of shower room
// Vertical frames of shower room
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 0.6, 1.0);
	glTranslatef(0, -33.1, 0.1);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);


	// glasses
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// The 3rd glass of shower room
// Vertical frames of shower room
	glPushMatrix();
	glRotatef(40, 0, 0, 1);
	glScalef(0.8, 0.7, 1.0);
	glTranslatef(8, -7.5, 0.1);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawWhiteBox(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	//Draw handles
	glCallList(doorList);

	// glasses
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}

// function used to draw the light of bathroom
void drawBathroomLight(float angleX, float angleY, float angleZ, float x, float y, float z)
{
	// parameters for cylinder
	float lightRadius = 0.05;
	float lightheight = 10 - z;
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angleX, 0.0, 0.0, 1.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);  // do we need all this?  isn't it in lightList?
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	gluCylinder(quadric, lightRadius, lightRadius, lightheight, 20, 20);
	glCallList(lightList);
	glPopMatrix();
}

void drawHouse() {
	drawHouseWalls();
}

void Charm_drawWall(float base_height, float length, float height) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[65]);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, base_height);

	glTexCoord2f(0.0, height / 4.37);
	glVertex3f(0, 0, base_height + height);

	glTexCoord2f(length/ 19.78, height / 4.37);
	glVertex3f(0, length, base_height + height);

	glTexCoord2f(length/ 19.78, 0.0);
	glVertex3f(0, length, base_height);
	glEnd();

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(2, 0, base_height);

	glTexCoord2f(0.0, height / 4.37);
	glVertex3f(2, 0, base_height + height);

	glTexCoord2f(length / 19.78, height / 4.37);
	glVertex3f(2, length, base_height + height);

	glTexCoord2f(length / 19.78, 0.0);
	glVertex3f(2, length, base_height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Charm_drawFloor(float width, float length, float height) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[64]);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, height);

	glTexCoord2f(width/15, 0);
	glVertex3f(width, 0, height);

	glTexCoord2f(width/15, length/15);
	glVertex3f(width, length, height);

	glTexCoord2f(0, length/15);
	glVertex3f(0, length, height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Charm_drawRoof(float base_height, float width, float length, float height) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[66]);

	if (height >= 2)
		height -= 2;
	for (int i = 0; i < 2; i++, height+=6) {
		// First roof
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 1, -width / (5 * height));

		glTexCoord2f(0, 0);
		glVertex3f(0, 0, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + height);

		glTexCoord2f(width / 13.23, 0.0);
		glVertex3f(width / 5, 0, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + height);

		glTexCoord2f(0, 0);
		glVertex3f(width * 4 / 5, 0, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + height);

		glTexCoord2f(width / 13.23, 0);
		glVertex3f(width, 0, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + height);

		glEnd();

		// Second roof.
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 1, width / (5 * height));

		glTexCoord2f(0, 0);
		glVertex3f(0, length, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + height);

		glTexCoord2f(width / 13.23, 0.0);
		glVertex3f(width / 5, length, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + height);

		glTexCoord2f(0, 0);
		glVertex3f(width * 4 / 5, length, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + height);

		glTexCoord2f(width / 13.23, 0);
		glVertex3f(width, length, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + height);

		glEnd();

		glBegin(GL_TRIANGLES);

		glNormal3f(1, 0, -width / (5 * height));
		glTexCoord2f(width / 10, 0);
		glVertex3f(0, 0, base_height);

		glTexCoord2f(width / 20, width / 10);
		glVertex3f(width / 5, length / 2, base_height + height);

		glTexCoord2f(0, 0);
		glVertex3f(0, length, base_height);

		glEnd();

		glBegin(GL_TRIANGLES);

		glNormal3f(1, 0, width / (5 * height));
		glTexCoord2f(width / 10, 0);
		glVertex3f(width, 0, base_height);

		glTexCoord2f(width / 20, width / 10);
		glVertex3f(width * 4 / 5, length / 2, base_height + height);

		glTexCoord2f(0, 0);
		glVertex3f(width, length, base_height);

		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

float Charm_doorWidth = 16, Charm_doorHeight = 10, Charm_wallWidth = 60, Charm_wallLength = 60, Charm_wallHeight = 20;
float Charm_house_x = 0, Charm_house_y = -Charm_wallLength / 2, Charm_house_z = 0;

float Charm_doorLeft = (Charm_wallLength - Charm_doorWidth) / 2;
float Charm_doorRight = Charm_doorLeft + Charm_doorWidth;


void drawHouseWalls() {

	glPushMatrix();
	glTranslatef(Charm_house_x, Charm_house_y, Charm_house_z);
	//glTranslatef(50, -Charm_wallLength/2, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	// 1st wall
	glPushMatrix();

	Charm_drawWall(Charm_doorHeight, Charm_wallLength, Charm_wallHeight - Charm_doorHeight);

	glPushMatrix();
	Charm_drawWall(0, (Charm_wallLength - Charm_doorWidth) / 2, Charm_doorHeight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, Charm_doorRight, 0);
	Charm_drawWall(0, (Charm_wallLength - Charm_doorWidth) / 2, Charm_doorHeight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, Charm_doorLeft - 2, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawWall(0, 2, Charm_doorHeight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, Charm_doorRight, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawWall(0, 2, Charm_doorHeight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, Charm_doorLeft, Charm_doorHeight + 2);
	glRotatef(90, 0, 1, 0);
	Charm_drawWall(0, Charm_doorWidth, 2);
	glPopMatrix();

	glPopMatrix();

	// 2nd wall
	glPushMatrix();
	glTranslatef(Charm_wallWidth, 0, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawWall(0, Charm_wallWidth, Charm_wallHeight);
	glPopMatrix();

	// 3rd wall
	glPushMatrix();
	glTranslatef(Charm_wallWidth, Charm_wallLength - 2, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawWall(0, Charm_wallWidth, Charm_wallHeight);
	glPopMatrix();

	// 4th wall
	glPushMatrix();
	glTranslatef(Charm_wallWidth - 2, 0, 0);
	Charm_drawWall(0, Charm_wallLength, Charm_wallHeight);
	glPopMatrix();

	// Floor
	DrawPlane(0,0,1,0,0,0.01,0, Charm_wallLength,0.01, Charm_wallWidth, Charm_wallLength,0.01, Charm_wallWidth,0,0.01,64,0,gray,defaultAmbient,defaultDiffuse,defaultSpecular,defaultEmission,defaultShine);
	//Charm_drawFloor(60, 60, 0.01);

	// House roof
	glPushMatrix();
	glTranslatef(-5, -5, 0);
	Charm_drawRoof(Charm_wallHeight - 1, Charm_wallWidth + (Charm_wallWidth/6), Charm_wallLength + (Charm_wallLength/6),4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, Charm_doorLeft, 0);
	DrawWhiteBox(0, 0,0,0.5,0.5,Charm_doorHeight, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0.5, Charm_doorHeight - 0.5, 0.5, (Charm_doorWidth / 2) - 1, 0.5, white, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0.5, 0, 0.5, (Charm_doorWidth / 2) - 1, 0.5, white, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, (Charm_doorWidth / 2) - 0.5, 0, 0.5, 0.5, Charm_doorHeight, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	
	glTranslatef(0, Charm_doorWidth/2 - 0.5, Charm_doorHeight/2);

	for (int i = 0; i < 180;i++) {
		glRotatef(1,1,0,0);
		glPushMatrix();
		glTranslatef(0, 0, Charm_doorHeight / 5);
		DrawWhiteBox(0, 0, 0, 0.5, PI/90, Charm_doorHeight / 20, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
		glPopMatrix();
	}
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, Charm_doorRight, 0);
	glRotatef(145, 0, 0, 1);
	DrawWhiteBox(0, 0, 0, 0.5, 0.5, Charm_doorHeight, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0.5, Charm_doorHeight - 0.5, 0.5, (Charm_doorWidth / 2) - 1, 0.5, white, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0.5, 0, 0.5, (Charm_doorWidth / 2) - 1, 0.5, white, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, (Charm_doorWidth / 2) - 0.5, 0, 0.5, 0.5, Charm_doorHeight, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPushMatrix();
	glTranslatef(0, Charm_doorWidth / 2 - 0.5, Charm_doorHeight / 2);
	for (int i = 0; i < 180; i++) {
		glRotatef(1, 1, 0, 0);
		glPushMatrix();
		glTranslatef(0, 0, Charm_doorHeight / 5);
		DrawWhiteBox(0, 0, 0, 0.5, PI / 90, Charm_doorHeight / 20, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
		glPopMatrix();
	}
	glPopMatrix();
	DrawPlane(1, 0, 0, 0.1, 0, 0, 0.1, 0, Charm_doorHeight, 0.1, (Charm_doorWidth / 2), Charm_doorHeight, 0.1, (Charm_doorWidth / 2), 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	DrawPlane(1, 0, 0, 0.4, 0, 0, 0.4, 0, Charm_doorHeight, 0.4, (Charm_doorWidth / 2), Charm_doorHeight, 0.4, (Charm_doorWidth / 2), 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	


	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, Charm_doorLeft, 0);
	DrawPlane(1, 0, 0, 0.1, 0, 0, 0.1, 0, Charm_doorHeight, 0.1, (Charm_doorWidth / 2), Charm_doorHeight, 0.1, (Charm_doorWidth / 2), 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	DrawPlane(1, 0, 0, 0.4, 0, 0, 0.4, 0, Charm_doorHeight, 0.4, (Charm_doorWidth / 2), Charm_doorHeight, 0.4, (Charm_doorWidth / 2), 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_REPLACE);
}

//void DrawWhiteBox(float x, float y, float z, float width, float length, float height, float color[], int textureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[])
//{
//	// Top
//	DrawPlane(0, 0, 1, x, y, z + height, x + width, y, z + height, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//	// W
//	DrawPlane(-1, 0, 0, x, y, z, x, y + length, z, x, y + length, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//	// E
//	DrawPlane(1, 0, 0, x + width, y, z, x + width, y + length, z, x + width, y + length, z + height, x + width, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//	// N
//	DrawPlane(0, 1, 0, x, y + length, z, x + width, y + length, z, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//	// S
//	DrawPlane(0, -1, 0, x, y, z, x + width, y, z, x + width, y, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//	// Bottom
//	DrawPlane(0, 0, -1, x, y, z, x + width, y, z, x + width, y + length, z, x, y + length, z, textureNum, blend, color, ambient, diffuse, specular, emission, shine);
//
//}


// function used to draw the sofa mat
void drawMat()
{
	DrawWhiteBox(0, 0, 0, 3, 3, 0.6, white, 62, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
}
// function used to draw the sofa seat
void drawSofaseat()
{
	// Bottom mat
	glPushMatrix();
	glTranslatef(0, 0.0, 0.3);
	drawMat();
	glPopMatrix();
	// Second mat
	glPushMatrix();
	glScalef(1.0, 0.8, 1.0);
	glTranslatef(0, 0.75, 0.9);
	drawMat();
	glPopMatrix();
	// Back mat
	glPushMatrix();
	glTranslatef(0, 0.8, 1.0);
	glRotatef(100, 1.0, 0.0, 0.0);
	drawMat();
	glPopMatrix();
}

// funtion used to draw the connect seat
void drawConnectSeat()
{

	// Bottom mat
	glPushMatrix();
	glTranslatef(0, 0.0, 0.3);
	drawMat();
	glPopMatrix();
	// Second mat
	glPushMatrix();
	glScalef(0.8, 0.8, 1.0);
	glTranslatef(0, 0.75, 0.9);
	drawMat();
	glPopMatrix();
	// Back mat 1
	glPushMatrix();
	glTranslatef(0, 0.8, 1.0);
	glRotatef(100, 1.0, 0.0, 0.0);
	drawMat();
	glPopMatrix();
	// Back mat 2
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(10, 1.0, 0.0, 0.0);
	glTranslatef(0.5, 1.38, 1.99);
	glScalef(0.9, 1.0, 1.0);
	drawMat();
	glPopMatrix();
}
// funtion used to draw the sofa
void drawSofa()
{
	glPushMatrix();
	//glColor3f(0.22, 0.11, 0.035);
	glScalef(0.8, 0.8, 0.8);
	// First Seat
	glPushMatrix();
	drawSofaseat();
	glPopMatrix();
	// Second Seat
	glPushMatrix();
	glTranslatef(3.0, 0.0, 0.0);
	drawSofaseat();
	glPopMatrix();
	// Third Seat
	glPushMatrix();
	glTranslatef(6.0, 0.0, 0.0);
	drawSofaseat();
	glPopMatrix();
	// Connect Seat
	glPushMatrix();
	glTranslatef(-3.0, 3.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	drawConnectSeat();
	glPopMatrix();
	// Fourth Seat
	glPushMatrix();
	glTranslatef(-3.0, 6.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);
	drawSofaseat();
	glPopMatrix();
	// Second connect
	glPushMatrix();
	glTranslatef(0.0, 9.0, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawConnectSeat();
	glPopMatrix();
	glPopMatrix();
}

// function used to draw the tea table surface
void drawTeatableSurface()
{
	// First Frame of the teatable
	DrawWhiteBox(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	// Second Frame of the teatable
	glPushMatrix();
	glRotatef(-90, 0.0, 0.0, 1.0);
	DrawWhiteBox(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Third Frame of the teatable
	glPushMatrix();
	glTranslatef(0.0, -10.0, 0.0);
	DrawWhiteBox(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Fourth Frame of the teatable
	glPushMatrix();
	glRotatef(-90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 10.0, 0.0);
	DrawWhiteBox(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Table surface
	glPushMatrix();
	glTranslatef(0.0, -10.0, 0.0);
	DrawPlane(0, 0, 1, 0, 0, 0.1, 0, 10, 0.1, 10, 10, 0.1, 10, 0, 0.1, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}

// function used to draw the tea table leg
void drawTeatableLeg()
{
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, 0.3);
	gluCylinder(quadric, 0.3, 0.3, 1.4, 30, 10);
	gluSphere(quadric, 0.3, 30, 10);
}

// function used to draw the teatable
void drawTeatable()
{
	// Table surface
	glPushMatrix();
	glTranslatef(0, 0, 1.7);
	drawTeatableSurface();
	glPopMatrix();
	// First Leg
	glPushMatrix();
	glTranslatef(2.5, -2.5, 0);
	drawTeatableLeg();
	glPopMatrix();
	// Second leg
	glPushMatrix();
	glTranslatef(7.5, -2.5, 0);
	drawTeatableLeg();
	glPopMatrix();
	// Third leg
	glPushMatrix();
	glTranslatef(7.5, -7.5, 0);
	drawTeatableLeg();
	glPopMatrix();
	// Fourth leg
	glPushMatrix();
	glTranslatef(2.5, -7.5, 0);
	drawTeatableLeg();
	glPopMatrix();
}
double imageIndex = 63.0;
// function used to draw the television
void drawTelevision(float x, float y, float z, float width, float height, float innerWidth, float innerHeight, float angle, float color[])
{
	if (g_Camera.objectsAnimationOn)
	{
		degreeLong += dDegreeLong;
		degreeShort += dDegreeShort;
		imageIndex = imageIndex + 0.01;
		if ((int)imageIndex == 67) {
			imageIndex = 63.0;
		}
	}

	float thickness = 0.2;
	float offset = 0.2;
	float frameH = (height - innerHeight - 2 * offset) / 2;
	float frameW = (width - innerWidth - 2 * offset) / 2;

	glPushMatrix();
	// Translate the frame to the specified location
	glTranslatef(x, y, z);
	// Rotate the frame so that it is against the wall
	glRotatef(angle, 0.0, 0.0, 1.0);

	// Draw the boxes on all four sides
	DrawWhiteBox(0, 0, 0, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0, height - frameH, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(0, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawWhiteBox(width - frameW, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw the slanted quadrilaterals
	DrawPlane(0, 0.75 * thickness, thickness / 4 * sqrt(3.0), frameW, 0, frameH, width - frameW, 0, frameH, width - frameW, thickness, thickness * sqrt(3.0) + frameH, frameW, thickness, thickness * sqrt(3.0) + frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(0, 0.75 * -thickness, -(thickness / 4 * sqrt(3.0)), frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, 0, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(thickness * sqrt(3.0) / 4, 0.75 * thickness, 0, frameW, 0, frameH, thickness * sqrt(3.0) + frameW, thickness, frameH, thickness * sqrt(3.0) + frameW, thickness, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawPlane(-(thickness / 4 * sqrt(3.0)), 0.75 * -thickness, 0, width - frameW - thickness * sqrt(3.0), thickness, frameH, width - frameW, 0, frameH, width - frameW, 0, height - frameH, width - frameW - thickness * sqrt(3.0), thickness, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw the back plane
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, -0.3);
	DrawPlane(0, 0, 1, 0, 0, 0.1, 0, height, 0.1, width, height, 0.1, width, 0, 0.1, -1, 1, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	// Draw the image on TV
	glPushMatrix();
	//glRotatef(degreeLong, 0, 0, 1);
	DrawPlane(0, 0, 1, 0, 0, 0, width, 0, 0, width, 0, height, 0, 0, height, int(imageIndex), 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	glPopMatrix();


}

// function used to draw the lamp
void drawLamp()
{
	glColor3f(0, 0, 0);
	gluCylinder(quadric, 1.2, 1.2, 0.4, 30, 10);
	gluDisk(quadric, 0, 1.2, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glTranslatef(0, 0, 0.4);
	gluDisk(quadric, 0, 1.2, polySubdivisions, polySubdivisions);
	glPopMatrix();
	gluCylinder(quadric, 0.2, 0.2, 7, 30, 10);
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 6);
	gluCylinder(quadric, 2, 1, 2, 30, 10);
	glPopMatrix();
}



// funtion used to draw the sofa
void drawBed()
{
	glPushMatrix();
	//glColor3f(0.22, 0.11, 0.035);
	//glScalef(0.8, 0.8, 0.8);
	//draw base
	glPushMatrix();
	glTranslatef(0, 0.0, 0.6);
	drawBedBase();
	glPopMatrix();
	//draw quilt
	glPushMatrix();
	glTranslatef(0, 0.0, 1);
	DrawWhiteBox(0, 0, 0, 5, 8, 0.6, white, 2, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw head
	glPushMatrix();
	DrawWhiteBox(0, -0.6, 0, 5, 0.6, 2.5, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw left leg
	glPushMatrix();
	glTranslatef(0, 7.4, 0.0);
	DrawWhiteBox(0, 0, 0, 0.6, 0.6, 0.6, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw right leg
	glPushMatrix();
	glTranslatef(4.4, 7.4, 0);
	DrawWhiteBox(0, 0, 0, 0.6, 0.6, 0.6, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw zhentou
	//glPushMatrix();
	//glTranslatef(1.5, 2.4, 1);
	//gluCylinder(quadric, 0.25, 0.25, 0.1, polySubdivisions, polySubdivisions);
	//glPopMatrix();

	gluQuadricNormals(quadric, GL_SMOOTH);
	glColor4f(cone[0], cone[1], cone[2], cone[3]);
	glPushMatrix();
	glTranslatef(1, 1, 2);
	glScalef(0.8, 0.8, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	//glRotatef(60, 0.0, 0.0, 1.0);
	gluCylinder(quadric, 1, 1, 1, polySubdivisions, polySubdivisions);
	glutSolidSphere(1, polySubdivisions, polySubdivisions);

	glPushMatrix();
	glTranslatef(0, 0, 1);
	glutSolidSphere(1, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.5, 1, 2);
	glScalef(0.8, 0.8, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	//glRotatef(60, 0.0, 0.0, 1.0);
	gluCylinder(quadric, 1, 1, 1, polySubdivisions, polySubdivisions);
	glutSolidSphere(1, polySubdivisions, polySubdivisions);

	glPushMatrix();
	glTranslatef(0, 0, 1);
	glutSolidSphere(1, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

// function used to draw the sofa seat
void drawBedBase()
{
	// Bottom mat
	glPushMatrix();
	glTranslatef(0, 0.0, 0);
	DrawWhiteBox(0, 0, 0, 5, 8, 0.4, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}

