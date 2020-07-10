#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#include "ObjLoader.h"
#include "Main.h"
#include "Camera.h"
#define STOPPED 0	
#define SWITCHED 1
#define ZOOMING 2
#define ROTATING 3
#define TRANSLATING 4
#define FINAL 5

char debugText[50];
CCamera g_Camera;										
bool  g_bFullScreen = TRUE;								
HWND  g_hWnd;										
RECT  g_rRect;									
HDC   g_hDC;									
HGLRC g_hRC;										
HINSTANCE g_hInstance;								
bool animationOn = TRUE;
GLUquadricObj* quadric = gluNewQuadric();
int polySubdivisions = 20;
float height = 1.3f;							
float dHeight = 0.2;						
float maxHeight = 3.0;								
float minHeight = 1.3;							
float platformHeight = 0.3;				
float floorShadow[4][4];								
float groundplane[] = { 0.0f, 0.0f, 1.0f, 1.0f };	
float degreeLong = 0;								
float degreeShort = 0;						
float dDegreeLong = -1;						
float dDegreeShort = -0.0167;
float lightPos1[4] = { 0, 0, 0, 0 };
float lightPos3[4] = { -5, 5, 10, 1.0 };
float lightSpotDir1[3] = { 0, 0, -1 };			
float lightSpotCutoff1 = 15.0;				
float lightSpotExp1 = 2.0;					
UINT textures[MAX_TEXTURES];		
float cameraSpeed = 0.4;			
int cameraCurrent = 0;							
int cameraNumOfSeq = 40;		
CVector3 vNull = CVector3();				
float cameraStartTime = -1;
bool cameraResetStartTime = true;
float cameraCurrentAngle = 0;
int cameraStart = 1;
float cameraTotalDistance = 0;
float cameraCurrentDistance = 0;
bool cameraCalculateDistance = false;
int orbitzCurrentFrame = 0;
CParticle* particles;
int particlesTotal;
float particleLifetime = 2037;
float particleRadius = 0.08;
int particleSubdivisions = 5;
float particleAngle = 0;
bool particlesIsNotInitialized = true;
int currentAct = 0;
int robotNumAct;
CVector3* robotAct;
CVector3* robotHeadAct;
int* robotStateAct;
float* robotTimeAct;
int robotNumAct1 = 13;

bool firstTime = true;
float initialTime = 0;

float robotSkin[] = { 0.98, 0.84, 0.78, 1.0 };
float robotDress[] = { 0.8, 0.0, 0.3, 1.0 };
int robotDirection = 1;
float robotArmRotation = 0;
float robotLegRotation = 0;
int robotX = 10;
int robotY = 11;
int robotZ = 5;
int robotNumOfSeq = 59;


CVector3 robotPositions[] = { CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365),
							CVector3(10, 10, 5.365), CVector3(10, 5, 5.365), CVector3(10, 0, 5.365), CVector3(10, -5, 5.365), CVector3(10, -10, 5.365),  CVector3(10, -10, 5.365),
							CVector3(10, -5, 5.365), CVector3(10, 0, 5.365), CVector3(10, 5, 5.365), CVector3(10, 10, 5.365)
};

CVector3 robotHeadTurns[] = { CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0), CVector3(0, 0, 0), CVector3(0, 0, 0),
							CVector3(0, 0, 0) };

float initialOrientation = 270;
CVector3 initialHeadTurn = CVector3(0, 0, 0);
int robotStates[] = {   -2, -2, -2, -2, 0, -2, -2, -2, -2, 0, 
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0,
						-2, -2, -2, -2, 0, -2, -2, -2, -2, 0
};
int robotCurrent = 0;
CVector3 currentHeadTurn = CVector3(0, 0, 0);

bool robotResetStartTime = true;
float robotStartTime;
float robotCurrentTime;
float robotResetArmLegStartTime;
int robotResetArmLeg = 0;

float tableTopHeight = 2.32;


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


int numOfKnots = 8;
int numOfCtrlPtsU = 5;
int numOfCtrlPtsV = 5;
int uDegree = 4;
int vDegree = 4;
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };



GLfloat ctrlPts1[5][5][3];
GLfloat ctrlPts2[5][5][3];
GLUnurbsObj* nurb1;
GLUnurbsObj* nurb2;


GLUnurbsObj* nurb3;
GLfloat ctrlPts3[5][5][3];


GLUnurbsObj* ceilingNurbs[16];
GLfloat ceilingCtrlPts[16][4][4][3];

// Firework variables
float fireworkX = 6;
float fireworkY = -14;
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


float treeLeavesRadius = 0.6;
float treeBarkRadiusTop = 0.3;
float treeBarkRadiusBottom = 0.36;
float treeHeight = 2.1;
float treeLeavesHeight = 2.1;
float treeLeavesOffsetX = 0.3;
float treeLeavesOffsetZ = 0.33;
float treeLeavesVarianceZ = 0.15;
float treeLeavesVarianceAngle = 0.24;


float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };
float wallShine[] = { 100 };



float defaultAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
float defaultDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float defaultSpecular[] = { 0.1, 0.1, 0.1, 1.0 };
float defaultEmission[] = { 0.01, 0.01, 0.01, 1.0 };
float defaultShine[] = { 10 };


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

float pearlAmbient[] = { 1.0, 1.0, 1.0, 0.922 };
float pearlDiffuse[] = { 0.0829, 0.0829, 0.0829, 0.0922 };
float pearlSpecular[] = { 0.6, 0.6, 0.6, 0.922 };
float pearlShine[] = { 50 };

float glassAmbient[] = { 0.1, 0.1, 0.5, 0.922 };
float glassDiffuse[] = { 1.0, 0.829, 0.829, 0.922 };
float glassSpecular[] = { 0.8, 0.8, 0.8, 0.922 };
float glassShine[] = { 63.8 };

float bathroomI_x = 11.0;
float bathroomI_y = 0.0;
float bathroomI_z = 0.0;
float caseHeight = 3.0;
float basinHeight = 5.1;

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
float hillColor[4] = { 0.05058, 0.5275, 0.005882, 1.0 };	 // Dark green
float nurbRoofColor[4] = { 0.83, 0.8, 0.39, 1.0 };	// Yellowish brown
float treeGreen1[4] = { 0.23, 0.63, 0.28, 1.0 };
float treeGreen2[4] = { 0.21, 0.58, 0.26, 1.0 };
float treeGreen3[4] = { 0.17, 0.45, 0.20, 1.0 };
float treeGreen4[4] = { 0.14, 0.39, 0.18, 1.0 };
float tableColor[4] = { 0.63, 0.79, 0.785, 1.0 };
float tableLegColor[4] = { 0.195, 0.27, 0.27, 1.0 };



GLuint robotHeadList;
GLuint robotLeftArmList;
GLuint robotRightArmList;
GLuint robotLeftDressConeList;
GLuint robotRightDressConeList;
GLuint robotLeftShoulderList;
GLuint robotRightShoulderList;
GLuint robotLegList;
GLuint doorList;

GLuint lightList;
GLuint particleList;

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


float VMagnitude(CVector3 vNormal)
{
	return (float)sqrt((vNormal.x * vNormal.x) +
		(vNormal.y * vNormal.y) +
		(vNormal.z * vNormal.z));
}


CVector3 VNormalize(CVector3 vVector)
{
	float magnitude = VMagnitude(vVector);
	vVector = vVector / magnitude;
	return vVector;
}



void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	// Init our camera position
							// Position        View		   Up Vector
	g_Camera.PositionCamera(-55.0, 24.0, 20.0, -20, 10.0, 18.0, 0.0, 0.0, 1.0);		// Default Cam
	//g_Camera.PositionCamera(0.0, 0.0, 5.0, 20.0, 10.0, 5.0, 0.0, 0.0, 1.0);		// Default Cam
	int debugCam = 38;

	CreateTexture(textures, "Textures/wood256.bmp", 1);
	CreateTexture(textures, "Textures/bed.bmp", 2);
	CreateTexture(textures, "Textures/flower.bmp", 6);
	CreateTexture(textures, "Textures/whitebricks.bmp", 19);
	CreateTexture(textures, "Textures/earth.bmp", 54);
	CreateTexture(textures, "Textures/top.bmp", 55);
	CreateTexture(textures, "Textures/left.bmp", 56);
	CreateTexture(textures, "Textures/right.bmp", 57);
	CreateTexture(textures, "Textures/front.bmp", 58);
	CreateTexture(textures, "Textures/back.bmp", 59);

	/*         Textures of Charm          */
	CreateTexture(textures, "Textures/whitewall.bmp", 62);
	CreateTexture(textures, "Textures/woodh256.bmp", 63);
	CreateTexture(textures, "Textures/ground_tex1.bmp", 64);
	CreateTexture(textures, "Textures/house_wall_tex1.bmp", 65);
	CreateTexture(textures, "Textures/roof_tex1.bmp", 66);

	/*************************************/
		/*         Textures of Jason         */
	CreateTexture(textures, "Textures/sofa.bmp", 67);
	CreateTexture(textures, "Textures/t1.bmp", 68);
	CreateTexture(textures, "Textures/t2.bmp", 69);
	CreateTexture(textures, "Textures/t3.bmp", 70);
	CreateTexture(textures, "Textures/t4.bmp", 71);
	CreateTexture(textures, "Textures/p1.bmp", 72);
	CreateTexture(textures, "Textures/p2.bmp", 73);
	CreateTexture(textures, "Textures/p3.bmp", 74);
	CreateTexture(textures, "Textures/p1.bmp", 75);
	CreateTexture(textures, "Textures/Steve/Head/Top.bmp", 77);
	CreateTexture(textures, "Textures/Steve/Head/West.bmp", 78);
	CreateTexture(textures, "Textures/Steve/Head/East.bmp", 79);
	CreateTexture(textures, "Textures/Steve/Head/North.bmp", 80);
	CreateTexture(textures, "Textures/Steve/Head/South.bmp", 81);
	CreateTexture(textures, "Textures/Steve/Head/Bottom.bmp", 82);
	CreateTexture(textures, "Textures/Steve/Body/Top.bmp", 83);
	CreateTexture(textures, "Textures/Steve/Body/West.bmp", 84);
	CreateTexture(textures, "Textures/Steve/Body/East.bmp", 85);
	CreateTexture(textures, "Textures/Steve/Body/North.bmp", 86);
	CreateTexture(textures, "Textures/Steve/Body/South.bmp", 87);
	CreateTexture(textures, "Textures/Steve/Body/Bottom.bmp", 88);
	CreateTexture(textures, "Textures/Steve/Hand/Top.bmp", 89);
	CreateTexture(textures, "Textures/Steve/Hand/West.bmp", 90);
	CreateTexture(textures, "Textures/Steve/Hand/East.bmp", 91);
	CreateTexture(textures, "Textures/Steve/Hand/North.bmp", 92);
	CreateTexture(textures, "Textures/Steve/Hand/South.bmp", 93);
	CreateTexture(textures, "Textures/Steve/Hand/Bottom.bmp", 94);
	CreateTexture(textures, "Textures/Steve/Leg/Top.bmp", 95);
	CreateTexture(textures, "Textures/Steve/Leg/West.bmp", 96);
	CreateTexture(textures, "Textures/Steve/Leg/East.bmp", 97);
	CreateTexture(textures, "Textures/Steve/Leg/North.bmp", 98);
	CreateTexture(textures, "Textures/Steve/Leg/South.bmp", 99);
	CreateTexture(textures, "Textures/Steve/Leg/Bottom.bmp", 100);
	/*************************************/

		/*         Textures of Elliot         */
	CreateTexture(textures, "Textures/firework.bmp", 76);
	CreateTexture(textures, "Textures/box1.bmp", 21);
	CreateTexture(textures, "Textures/box2.bmp", 22);
	CreateTexture(textures, "Textures/box3.bmp", 23);
	CreateTexture(textures, "Textures/display.bmp", 24);
	CreateTexture(textures, "Textures/sky1.bmp", 25);
	CreateTexture(textures, "Textures/cylinder.bmp", 26);

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

	// robot's head
	robotHeadList = glGenLists(1);
	glNewList(robotHeadList, GL_COMPILE);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidSphere(0.58, polySubdivisions, polySubdivisions);
	glutSolidCone(0.3175, 0.714, polySubdivisions, polySubdivisions);
	glPushMatrix();
	glPopMatrix();
	glEndList();

	// robot's left arm
	robotLeftArmList = glGenLists(2);
	glNewList(robotLeftArmList, GL_COMPILE);
	glRotatef(40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -1.27);
	gluCylinder(quadric, 0.149, 0.149, 1.27, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	// robot's right arm
	robotRightArmList = glGenLists(3);
	glNewList(robotRightArmList, GL_COMPILE);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -1.27);
	gluCylinder(quadric, 0.149, 0.149, 1.27, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	// robot's leg
	robotLegList = glGenLists(8);
	glNewList(robotLegList, GL_COMPILE);
	glTranslatef(0, 0, -1.588);
	gluCylinder(quadric, 0.191, 0.191, 1.588, polySubdivisions, polySubdivisions);
	glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

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

	// Particles for the firwork
	particleList = glGenLists(16);
	glNewList(particleList, GL_COMPILE);
	glPushMatrix();
	glutSolidSphere(particleRadius, particleSubdivisions, particleSubdivisions);
	glPopMatrix();
	glEndList();

	// ### create basin list
	basinList = glGenLists(15);
	glNewList(basinList, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, 0, basinHeight);
	glDisable(GL_TEXTURE_2D);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	basinObj.Draw();
	glPopMatrix();

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


	glEndList();
}

WPARAM MainLoop()
{
	MSG msg;
	static char strTitle[100] = { 0 };
	sprintf_s(strTitle, "CG project - a lovely house");
	SetWindowText(g_hWnd, strTitle);

	while (1)							
	{				
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	
				break;
			TranslateMessage(&msg);			
			DispatchMessage(&msg);	
		}
		else						
		{
			g_Camera.Update();			
			RenderScene();
		}
	}

	gluDeleteNurbsRenderer(nurb1);
	gluDeleteNurbsRenderer(nurb2);
	gluDeleteNurbsRenderer(nurb3);

	for (int i = 0; i < 16; i++)
	{
		gluDeleteNurbsRenderer(ceilingNurbs[i]);
	}
	DeInit();									

	return(msg.wParam);	
}

void RenderScene()
{
	g_Camera.TurnControlOn();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity(); 
	g_Camera.Look();
	RenderInside();
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	SwapBuffers(g_hDC);	
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;

	switch (uMsg)
	{
	case WM_SIZE:								
		if (!g_bFullScreen)					
		{
			SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));
			GetClientRect(hWnd, &g_rRect);		
		}
		break;
	case WM_PAINT:									
		BeginPaint(hWnd, &ps);						
		EndPaint(hWnd, &ps);				
		break;
	case WM_KEYDOWN:
		switch (wParam) {						
		case VK_ESCAPE:						
			PostQuitMessage(0);				
			break;
		}
		break;
	case WM_CLOSE:							
		PostQuitMessage(0);				
		break;
	default:							
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return lRet;					
}

void RenderInside()
{

	Drawrobot();
	ChooserobotAct();
	RenderOutdoor();
	glTranslated(-20, 0, 0);
	drawHouse();

}


void Drawrobot()
{
	if (!g_Camera.objectsAnimationOn)
	{
		robotHead(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
		robotBody(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 2.222); 
		robotLeg(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 1.586, initialOrientation);
		robotArm(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 0.635, initialOrientation);
	}
	else
	{
		if (robotResetArmLeg == 1)
		{
			if (robotDirection > 0 && robotLegRotation >= 0 || robotDirection < 0 && robotLegRotation <= 0)
			{
				robotResetArmLeg = 0;
				robotArmRotation = 0;
				robotLegRotation = 0;
			}

			robotHead(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
			robotBody(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 2.222); 
			robotLeg(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 1.586, initialOrientation);
			robotArm(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 0.635, initialOrientation);
		}
		else
		{
			if (robotResetStartTime)
			{
				robotStartTime = glutGet(GLUT_ELAPSED_TIME);
				robotResetStartTime = false;
			}

			float progress = 0.0;
			float destOrientation;
			CVector3 currentDestVector;
			currentDestVector = robotPositions[(robotCurrent + 1) % robotNumOfSeq] - robotPositions[robotCurrent];

			float distance = sqrt(currentDestVector.x * currentDestVector.x + currentDestVector.y * currentDestVector.y);

			robotCurrentTime = glutGet(GLUT_ELAPSED_TIME);
			float dTime = robotCurrentTime - robotStartTime;

			float robotTime = 1000;

			progress = dTime / robotTime;
			if (progress > 1.0)
				progress = 1.0;

			CVector3 currentPos;
			float currentOrientation;

			if (robotStates[robotCurrent] < 0)
			{
				
				currentPos = (currentDestVector * progress) + robotPositions[robotCurrent];

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
			else if (robotStates[robotCurrent] == 0)	
			{
				currentPos = robotPositions[robotCurrent];
				currentOrientation = initialOrientation;
				destOrientation = initialOrientation;
			}
			else if (robotStates[robotCurrent] > 0)	
			{
				currentPos = robotPositions[robotCurrent];
				destOrientation = robotStates[robotCurrent];
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
				initialHeadTurn.x = robotHeadTurns[robotCurrent].x * 2 * progress;
				initialHeadTurn.y = robotHeadTurns[robotCurrent].y * 2 * progress;
				initialHeadTurn.z = robotHeadTurns[robotCurrent].z * 2 * progress;
			}
			else if (progress > 0.5)
			{
				initialHeadTurn.x = robotHeadTurns[robotCurrent].x - robotHeadTurns[robotCurrent].x * 2 * (progress - 0.5);
				initialHeadTurn.y = robotHeadTurns[robotCurrent].y - robotHeadTurns[robotCurrent].y * 2 * (progress - 0.5);
				initialHeadTurn.z = robotHeadTurns[robotCurrent].z - robotHeadTurns[robotCurrent].z * 2 * (progress - 0.5);
			}

			robotHead(currentPos.x, currentPos.y, currentPos.z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, currentOrientation);
			robotBody(currentPos.x, currentPos.y, currentPos.z - 2.222);
			robotLeg(currentPos.x, currentPos.y, currentPos.z - 1.586, currentOrientation);
			robotArm(currentPos.x, currentPos.y, currentPos.z - 0.635, currentOrientation);

			if (progress >= 1.0)
			{
				robotResetStartTime = true;
				if (g_Camera.objectsAnimationOn)
				{
					if (++robotCurrent > robotNumOfSeq - 1)
						robotCurrent = 0;
					if (robotStates[robotCurrent] == 0)
					{
						robotDirection *= -1;	
						robotResetArmLeg = 1;	
					}
				}


				initialOrientation = destOrientation;
				initialHeadTurn = CVector3(0, 0, 0);

			}
		}
	}
}


void robotHead(float x, float y, float z, float hrx, float hry, float hrz, float rz)
{
	glColor4f(robotSkin[0], robotSkin[1], robotSkin[2], robotSkin[3]);
	float radius = 0.58;
	float noseRadius = 0.3175;
	float noseHeight = 0.714;

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glRotatef(hrz, 0.0, 0.0, 1.0);
	glRotatef(hry, 0.0, 1.0, 0.0);
	glRotatef(hrx, 1.0, 0.0, 0.0);
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.5, -0);
	DrawRobotPart(0, 0, 0, 1.6, 1.6, 1.6, white, 77, 79, 78, 81, 80, 82, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	glTranslatef(0, 0, -2);
	DrawRobotPart(0, 0, 0, 1.6, 0.8, 2, white, 83, 84, 85, 86, 87, 88, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPopMatrix();
}

void robotBody(float x, float y, float z)
{
	glColor4f(robotDress[0], robotDress[1], robotDress[2], robotDress[3]);
	float radius = 1.2;
	float height = 2.54;

	float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
	float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
	float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };

	float wallShine[] = { 0 };

	glPushMatrix();
	glTranslatef(x, y, z);

	glRotatef(90, 0, 0, 1);
	glTranslatef(-1.7, -1, 0);

	glPopMatrix();
}


void DrawRobotPart(float x, float y, float z, float width, float length, float height, float color[], int ToptextureNum, int WtextureNum, int EtextureNum, int NtextureNum, int StextureNum, int BottomtextureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[])
{

	DrawSurface(0, 0, 1, x, y, z + height, x + width, y, z + height, x + width, y + length, z + height, x, y + length, z + height, ToptextureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(-1, 0, 0, x, y, z, x, y + length, z, x, y + length, z + height, x, y, z + height, WtextureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(1, 0, 0, x + width, y, z, x + width, y + length, z, x + width, y + length, z + height, x + width, y, z + height, EtextureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, 1, 0, x, y + length, z, x + width, y + length, z, x + width, y + length, z + height, x, y + length, z + height, NtextureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, -1, 0, x, y, z, x + width, y, z, x + width, y, z + height, x, y, z + height, StextureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, 0, -1, x, y, z, x + width, y, z, x + width, y + length, z, x, y + length, z, BottomtextureNum, blend, color, ambient, diffuse, specular, emission, shine);

}


void robotResetArmLegAngle()
{

	if (robotDirection > 0 && robotLegRotation >= 0 || robotDirection < 0 && robotLegRotation <= 0)
	{
		robotResetArmLeg = 0;
		robotArmRotation = 0;
		robotLegRotation = 0;
	}

	robotHead(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
	robotBody(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 2.222); //1.143);
	robotLeg(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 1.586, initialOrientation);
	robotArm(robotPositions[robotCurrent].x, robotPositions[robotCurrent].y, robotPositions[robotCurrent].z - 0.635, initialOrientation);
}


void robotArm(float x, float y, float z, float rz)
{
	float angle = 2;

	if (robotResetArmLeg == 0 && ((animationOn && robotStateAct[robotCurrent] == 0) || (robotStates[robotCurrent] == 0)))
		angle = 0;
	else if ((animationOn && robotStateAct[robotCurrent] == -2) || (robotStates[robotCurrent] == -2))
		angle = 6;


	float length = 1.27;
	float shoulderSize = 0.222;
	float armRadius = 0.149;
	float handRadius = 0.191;

	if (g_Camera.objectsAnimationOn)
		robotArmRotation += -robotDirection * angle;

	glColor4f(robotSkin[0], robotSkin[1], robotSkin[2], robotSkin[3]);

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(-0.4, 0.4, 0.5);
	glRotatef(robotArmRotation, 1.0, 0.0, 0.0);
	glTranslatef(-0.4, -0.4, -2.4);
	DrawRobotPart(0, 0, 0, 0.8, 0.8, 2.4, white, 89, 90, 91, 92, 93, 94, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(2.0, 0.4, 0.5);
	glRotatef(-robotArmRotation, 1.0, 0.0, 0.0);
	glTranslatef(-0.4, -0.4, -2.4);
	DrawRobotPart(0, 0, 0, 0.8, 0.8, 2.4, white, 89, 90, 91, 92, 93, 94, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glColor4f(robotDress[0], robotDress[1], robotDress[2], robotDress[3]);

}

void robotLeg(float x, float y, float z, float rz)
{
	float max = 30.0;
	float min = -30.0;

	float angle = 2;
	float length = 1.588;
	float legRadius = 0.191;

	if (robotResetArmLeg == 0 && ((animationOn && robotStateAct[robotCurrent] == 0) || robotStates[robotCurrent] == 0))
		angle = 0;
	else if ((animationOn && robotStateAct[robotCurrent] == -2) || robotStates[robotCurrent] == -2)
		angle = 6;

	if (g_Camera.objectsAnimationOn)
		robotLegRotation += robotDirection * angle;

	if (((animationOn && robotStateAct[robotCurrent] != 0) || (robotStates[robotCurrent] != 0)) && ((robotDirection > 0 && robotLegRotation > max) || (robotDirection < 0 && robotLegRotation < min)))
	{
		robotLegRotation -= robotDirection * angle;
		robotDirection *= -1;		
	}

	glColor4f(robotSkin[0], robotSkin[1], robotSkin[2], robotSkin[3]);

	
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(0.3, 0.4, -0.3);
	glRotatef(robotLegRotation, 1.0, 0.0, 0.0);
	glTranslatef(-0.4, -0.4, -2.4);
	DrawRobotPart(0, 0, 0, 0.8, 0.8, 2.4, white, 95, 96, 97, 98, 99, 100, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rz, 0.0, 0.0, 1.0);
	glTranslatef(1.3, 0.4, -0.3);
	glRotatef(-robotLegRotation, 1.0, 0.0, 0.0);
	glTranslatef(-0.4, -0.4, -2.4);
	DrawRobotPart(0, 0, 0, 0.8, 0.8, 2.4, white, 95, 96, 97, 98, 99, 100, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
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

void drawHuaHua(float x, float y, float z, float width, float height, float angle, int textureNum)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0, 0.0, 1.0);

	DrawSurface(0, 1, 0, 0, 0, 0, width, 0, 0, width, 0, height, 0, 0, height, textureNum, 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPopMatrix();
}

void DrawCuboid(float x, float y, float z, float width, float length, float height, float color[], int textureNum, int blend, float ambient[], float diffuse[], float specular[], float emission[], float shine[])
{
	DrawSurface(0, 0, 1, x, y, z + height, x + width, y, z + height, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(-1, 0, 0, x, y, z, x, y + length, z, x, y + length, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(1, 0, 0, x + width, y, z, x + width, y + length, z, x + width, y + length, z + height, x + width, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, 1, 0, x, y + length, z, x + width, y + length, z, x + width, y + length, z + height, x, y + length, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, -1, 0, x, y, z, x + width, y, z, x + width, y, z + height, x, y, z + height, textureNum, blend, color, ambient, diffuse, specular, emission, shine);


	DrawSurface(0, 0, -1, x, y, z, x + width, y, z, x + width, y + length, z, x, y + length, z, textureNum, blend, color, ambient, diffuse, specular, emission, shine);

}

void DrawSurface(float xn, float yn, float zn, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int textureNum, int blend, float color[], float ambient[], float diffuse[], float specular[], float emission[], float shine[])
{

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	glColor4f(color[0], color[1], color[2], color[3]);

	if (textureNum >= 0) {
		glEnable(GL_TEXTURE_2D);							
		glBindTexture(GL_TEXTURE_2D, textures[textureNum]);
	}


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



void DrawWalls(float x, float y)
{
	float z = 10;
	float wallAmbient[] = { 1, 0.5, 0.5, 0.6 };
	float wallDiffuse[] = { 1, 0.5, 0.7, 1.0 };
	float wallSpecular[] = { 1, 0.5, 0.5, 1.0 };
	float wallShine[] = { 100 };

	DrawSurface(1, 0, 0, 0, y, 0, 0, 0, 0, 0, 0, z, 0, y, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(-1, 0, 0, x, 0, 0, x, y, 0, x, y, z, x, 0, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, -1, 0, x, y, 0, 0, y, 0, 0, y, z, x, y, z, -1, 0, wallGreen, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

}


void DrawCeiling()
{
	DrawCuboid(0, 0, 10, 34, 10, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, 10, 10, 6, 36, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(6, 14, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(34, 0, 10, 32, 46, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawCuboid(6, 22, 10, 28, 8, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(6, 34, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(6, 42, 10, 28, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(10, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(18, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(26, 30, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(10, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(18, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(26, 38, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	DrawCuboid(10, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(18, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(26, 18, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(10, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(18, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(26, 10, 10, 4, 4, 2, white, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

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
	float sourceZ = 2.8;
	float sourceRadius = 0.1;

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
	float meanColorVariant = 0.5;

	float meanDR = 0.001;
	float meanDG = 0.001;
	float meanDB = 0.001;
	float meanDA = 0;
	float meanDColorVariant = 0.0001;

	float minLifetime = particleLifetime;
	float maxLifetime = particleLifetime + 1000;

	float pX = (sourceX - sourceRadius) + (2 * sourceRadius) * GEN_RANDOM_FLOAT;
	float pY = (sourceY - sourceRadius) + (2 * sourceRadius) * GEN_RANDOM_FLOAT;
	float pZ = (sourceZ)+(90 * sourceRadius) * GEN_RANDOM_FLOAT;

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


void drawParticleSystem()
{
	float fireworkX = 0;
	float fireworkY = 0;

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

				particles[i].color[0] += particles[i].dColor[0] * particles[i].lifeTime * dTime * 0.1;
				particles[i].color[1] += particles[i].dColor[1] * particles[i].lifeTime * dTime * 0.1;
				particles[i].color[2] += particles[i].dColor[2] * particles[i].lifeTime * dTime * 0.1;
				particles[i].color[3] += particles[i].dColor[3] * particles[i].lifeTime * dTime * 0.1;

				particles[i].radius = dRadius;

				if (particles[i].vPosition.z >= hBase + hBaseStand + hMain)
				{
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glColor4f(particles[i].color[0], particles[i].color[1], particles[i].color[2], particles[i].color[3]);
					glPushMatrix();
					glTranslatef(fireworkX, fireworkY, 0);
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

void ChooserobotAct()
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
	default:
		break;
	}

	if (dodraw) {
		DrawrobotActs();
		dodraw = false;
	}
}

void DrawrobotActs()
{
	if (robotResetArmLeg == 1)
	{
		if (robotDirection > 0 && robotLegRotation >= 0 || robotDirection < 0 && robotLegRotation <= 0)
		{
			robotResetArmLeg = 0;
			robotArmRotation = 0;
			robotLegRotation = 0;
		}

		robotHead(robotAct[robotCurrent].x, robotAct[robotCurrent].y, robotAct[robotCurrent].z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, initialOrientation);
		robotBody(robotAct[robotCurrent].x, robotAct[robotCurrent].y, robotAct[robotCurrent].z - 2.222);
		robotLeg(robotAct[robotCurrent].x, robotAct[robotCurrent].y, robotAct[robotCurrent].z - 1.586, initialOrientation);
		robotArm(robotAct[robotCurrent].x, robotAct[robotCurrent].y, robotAct[robotCurrent].z - 0.635, initialOrientation);
	}
	else
	{
		if (robotResetStartTime)
		{
			robotStartTime = glutGet(GLUT_ELAPSED_TIME);
			robotResetStartTime = false;
		}

		float progress = 0.0;
		float destOrientation;
		CVector3 currentDestVector;

		if (robotCurrent < robotNumAct - 1)
		{
			currentDestVector = robotAct[robotCurrent + 1] - robotAct[robotCurrent];

			robotCurrentTime = glutGet(GLUT_ELAPSED_TIME);
			float dTime = robotCurrentTime - robotStartTime;

			progress = dTime / (robotTimeAct[robotCurrent] * 1000);

			if (progress > 1.0)
				progress = 1.0;

			CVector3 currentPos;
			float currentOrientation;

			if (robotStateAct[robotCurrent] < 0)	
			{

				currentPos = (currentDestVector * progress) + robotAct[robotCurrent];

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
			else if (robotStateAct[robotCurrent] == 0)
			{
				currentPos = robotAct[robotCurrent];
				currentOrientation = initialOrientation;
				destOrientation = initialOrientation;
			}
			else if (robotStateAct[robotCurrent] > 0)	
			{
				currentPos = robotAct[robotCurrent];
				destOrientation = robotStates[robotCurrent];
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
				initialHeadTurn.x = robotHeadAct[robotCurrent].x * 2 * progress;
				initialHeadTurn.y = robotHeadAct[robotCurrent].y * 2 * progress;
				initialHeadTurn.z = robotHeadAct[robotCurrent].z * 2 * progress;
			}
			else if (progress > 0.5)
			{
				initialHeadTurn.x = robotHeadAct[robotCurrent].x - robotHeadAct[robotCurrent].x * 2 * (progress - 0.5);
				initialHeadTurn.y = robotHeadAct[robotCurrent].y - robotHeadAct[robotCurrent].y * 2 * (progress - 0.5);
				initialHeadTurn.z = robotHeadAct[robotCurrent].z - robotHeadAct[robotCurrent].z * 2 * (progress - 0.5);
			}

			robotHead(currentPos.x, currentPos.y, currentPos.z, initialHeadTurn.x + 90, initialHeadTurn.y + 90, initialHeadTurn.z + 90, currentOrientation);
			robotBody(currentPos.x, currentPos.y, currentPos.z - 2.222);
			robotLeg(currentPos.x, currentPos.y, currentPos.z - 1.586, currentOrientation);
			robotArm(currentPos.x, currentPos.y, currentPos.z - 0.635, currentOrientation);

			if (progress >= 1.0)
			{
				robotResetStartTime = true;
				if (++robotCurrent >= robotNumAct - 1)
				{
					robotCurrent = 0;
					currentAct++;
				}

				if (robotStateAct[robotCurrent] == 0)
				{
					robotDirection *= -1;	
					robotResetArmLeg = 1;	
				}
				initialOrientation = destOrientation;
				initialHeadTurn = CVector3(0, 0, 0);

			}

		}
	}
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


void RenderOutdoor()
{
	DrawSurface(0, 0, 1, 70, -70, 0, 70, 70, 0, -70, 70, 0, -70, -70, 0, -1, 1, hillColor, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glEnable(GL_TEXTURE_2D);						
	glBindTexture(GL_TEXTURE_2D, textures[55]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	DrawSurface(0, 0, 1, 70, -70, 70, 70, 70, 70, -70, 70, 70, -70, -70, 70, 55, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(1, 0, 0, -70, -70, 0, -70, 70, 0, -70, 70, 70, -70, -70, 70, 56, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(-1, 0, 0, 70, 70, 0, 70, -70, 0, 70, -70, 70, 70, 70, 70, 57, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, 70, -70, 0, -70, -70, 0, -70, -70, 70, 70, -70, 70, 58, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, -1, 0, -70, 70, 0, 70, 70, 0, 70, 70, 70, -70, 70, 70, 59, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glDisable(GL_TEXTURE_2D);
}


/***********************  By Marcus Liu  **************************/
// ### function used to draw bathroom part I
void drawBathroomI() {
	glPushMatrix();
	glTranslatef(bathroomI_x, bathroomI_y, bathroomI_z);

	glPushMatrix();
	glTranslatef(3.2, 3.2, 0);


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
	glRotatef(70, 0, 0, 1);
	glScalef(1.8, 1.8, 2.4);
	glTranslatef(3, -35, 0.1);
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
	//xiaotiantian
	glDisable(GL_LIGHT3);
	drawBathroomI();
	drawBathroomII();
	glEnable(GL_LIGHT3);
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
	glTranslatef(-2, -2, 0);
	Charm_drawRoomWall(0, 19, 15);

	glPopMatrix();
	// 2nd wall
	glPushMatrix();
	glTranslatef(10, -2, 0);

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 12, 15);
	glPopMatrix();

	glTranslatef(-2, 0, 0);
	Charm_drawRoomWall(0, 2, 15);

	glPopMatrix();
	// 3rd wall
	glPushMatrix();
	glTranslatef(15, 15, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 17, 15);

	glPopMatrix();
	// 4th wall
	glPushMatrix();
	glTranslatef(15, -2, 0);
	Charm_drawRoomWall(0, 19, 15);
	glTranslatef(2, 0, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 2, 15);

	glPopMatrix();

	// bathroom floor
	DrawSurface(0, 0, 1, 0, 0, 0.1, 0, 15, 0.1, 15, 15, 0.1, 15, 0, 0.1, 1, 0, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine); \

		// bathroom ceiling
		glPushMatrix();

	glPushMatrix();
	glTranslatef(-2, -2, 11);
	glRotatef(90, 0, 1, 0);
	Charm_drawRoomWall(0, 19, 19);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, -2, 9);
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 5, 6);
	glPopMatrix();

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
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// glasses
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// The 2nd glass of shower room
// Vertical frames of shower room
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 0.6, 1.0);
	glTranslatef(0, -33.1, 0.1);
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);


	// glasses
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// The 3rd glass of shower room
// Vertical frames of shower room
	glPushMatrix();
	glRotatef(40, 0, 0, 1);
	glScalef(0.8, 0.7, 1.0);
	glTranslatef(8, -7.5, 0.1);
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX + doorWidth - doorFrameSize, doorCenterY - doorFrameSize / 2, 0, doorFrameSize, doorFrameSize, doorHeight, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	// Horizontal frames of shower room
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX - doorWidth, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), 0, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	DrawCuboid(doorCenterX, doorCenterY - (doorFrameSize / 2), doorHeight - doorFrameSize, doorWidth, doorFrameSize, doorFrameSize, white, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);

	//Draw handles
	glCallList(doorList);

	// glasses
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX, doorCenterY + 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, 0, doorCenterX + doorWidth, doorCenterY + 0.01, doorHeight, doorCenterX, doorCenterY + 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX - doorWidth, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX, doorCenterY - 0.01, doorHeight, doorCenterX - doorWidth, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, doorCenterX, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, 0, doorCenterX + doorWidth, doorCenterY - 0.01, doorHeight, doorCenterX, doorCenterY - 0.01, doorHeight, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
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


/***********************  By Charm Zhang  **************************/

void drawChair(float size, float height) {
	glPushMatrix();
	DrawCuboid(0, 0, 0, size/10, size/10, height/2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(size / 2, 0, 0, size / 10, size / 10, height / 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(size / 2, size / 2, 0, size / 10, size / 10, height / 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, size / 2, 0, size / 10, size / 10, height / 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, 0, height / 2, size * 3 / 5, size * 3 / 5, size / 10, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, 0, (height / 2) + (size / 10), size * 3 / 5, size / 10, size * 3 / 5, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}


float Charm_wallWidth = 60, Charm_wallLength = 60, Charm_wallHeight = 30;
float Charm_secondFloor = 15;
float Charm_win1Height = 10;
float Charm_house_x = 20, Charm_house_y = -Charm_wallLength / 2, Charm_house_z = 1;

void drawHouse() {
	glTranslatef(Charm_house_x, Charm_house_y, Charm_house_z);
	glPushMatrix();

	//draw fractals

	glColor4f(0.9451, 0.4863, 0.4039, 0);
	glPushMatrix();
	glTranslatef(46, 35, 11);
	glPushMatrix();
	glTranslatef(0, 0, 2);
	gluCylinder(quadric, 0.1, 0.1, 2, 30, 10);
	glPopMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	drawFractals();
	glPopMatrix();

	//second floor shadow
	glPushMatrix();
	glTranslatef(49, 27, 16);
	DrawCuboid(0, 0, 0, 9, 9, 0.5, gray, 22, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glTranslatef(3, 6, 0.5);
	drawShadowSystem();
	drawShadowCylinder();
	glPushMatrix();
	glTranslatef(0, 0, 2.85);
	glColor4f(water[0], water[1], water[2], water[3]);
	gluDisk(quadric, 0, 0.7, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	// This chair is used to test the position.

	//draw che suo
	glPushMatrix();
	glTranslatef(31, 42, 0);
	drawBathroom();
	glPopMatrix();

	//draw Wo shi
	glPushMatrix();

	glColor4f(white[0], white[1], white[2], white[3]);
	glPushMatrix();
	glTranslatef(57.5, 1, 0);
	Charm_drawRoomWall(0, 25, 15);
	
	glPushMatrix();
	glTranslatef(-27, 0, 0);
	Charm_drawRoomWall(0, 27, 15);
	glPopMatrix();

	glPushMatrix();

	glPushMatrix();
	glTranslatef(-21, 25, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 6, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 25, 0);

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 16, 15);
	glPopMatrix();

	glTranslatef(-16, 0, 0);
	Charm_drawRoomWall(0, 2, 10);

	glPushMatrix();
	glTranslatef(0, 0, 10);

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	Charm_drawRoomWall(0, 6, 5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	Charm_drawRoomWall(0, 2, 6);
	glPopMatrix();

	glPopMatrix();

	glTranslatef(-8, 0, 0);
	Charm_drawRoomWall(0, 2, 10);

	glPopMatrix();

	glPopMatrix();


	glPopMatrix();

	glPushMatrix();
	glTranslatef(57, 13, 0);
	glRotatef(90, 0, 0, 1);
	drawBed();


	glPushMatrix();
	glTranslatef(-7, 4, 0);
	drawHuaPen();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	//draw ke ting
	glPushMatrix();
	glTranslatef(5.5, 3, -0.3);
	glScalef(1.3, 1.3, 1.3);
	drawSofa();

	//draw lamp
	glPushMatrix();
	glTranslatef(9.5, 2, 0);
	glScalef(0.8, 0.8, 0.8);
	drawLamp();
	glPopMatrix();

	// Draw Painting 1
	drawHuaHua(29.5064, 0, 5, 6.40, 3.1, 180.0, 73);
	// Draw Painting 2
	drawHuaHua(15.5064, 42, 5, 6.40, 4.12, 180.0, 72);
	// Draw Painting 3
	drawHuaHua(5.5064, 42, 5, 6.40, 4.12, 180.0, 6);
	// Draw Painting 4
	drawHuaHua(7.5064, 0, 5, 6.40, 4.12, 180.0, 74);
	// Draw Painting 5
	drawHuaHua(10.5064, 42, 16, 6.40, 4.12, 180.0, 74);
	// Draw Painting 6
	drawHuaHua(9.5064, 0, 16, 6.40, 4.12, 180.0, 25);

	glPopMatrix();
	

	//draw tele
	glPushMatrix();
	glTranslatef(30, 22, 2);
	glRotatef(90, 0, 0, 1);
	glScalef(1.5, 1.5, 1.5);
	drawTelevision(0.0, 0.0, 1, 9.6, 5.4, 9.6, 5.4, 180.0, black);
	glPopMatrix();

	/******************************************/
	glPushMatrix();
	glTranslatef(25, 2, 16);

	Charm_drawFloor(25, 33, 0.01);

	glPushMatrix();
	glTranslatef(15, 12.5, 6.5);
	glRotatef(90, 1, 0, 0);

	drawStorageLight(0, 0, 0, 8, 8, 8);
	glPopMatrix();
	

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	DrawSurface(0, 1, 0, 2.1, 0, 0, 2.1, 25, 0, 2.1, 25, 13, 2.1, 0, 13, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, 32.9, 0, 0, 32.9, 25, 0, 32.9, 25, 13, 32.9, 0, 13, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	Charm_drawWall(0, 25, 13);


	// left
	glPushMatrix();
	glRotated(90, 0, 0, 1);
	glTranslatef(23, -33, 0);
	DrawSurface(0, 1, 0, -0.1, 0, 0, -0.1, 20, 0, -0.1, 20, 13, -0.1, 0, 13, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 1, 0, -22.9, 0, 0, -22.9, 33, 0, -22.9, 33, 13, -22.9, 0, 13, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPushMatrix();
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-20, 0, 0);
	Charm_drawWall(0, 2, 10);

	glTranslatef(-10, 0, 0);
	Charm_drawWall(0, 2, 10);
	glPopMatrix();

	Charm_drawWall(0, 20, 13);

	// top
	glPushMatrix();
	glTranslatef(0, 20, 0);
	Charm_drawWall(10, 8, 3);
	DrawSurface(0, 1, 0, -0.1, 0, 10, -0.1, 0, 13, -0.1, 8, 13, -0.1, 8, 10, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	// right
	DrawSurface(0, 1, 0, -0.1, 28, 0, -0.1, 33, 0, -0.1, 33, 13, -0.1, 28, 13, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glTranslatef(0, 28, 0);
	Charm_drawWall(0, 5, 13);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 13);
	glRotated(90, 0, 1, 0);
	DrawSurface(0, 1, 0, 2.1, 0, 0, 2.1, 25, 0, 2.1, 25, 33, 2.1, 0, 33, -1, 0, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	Charm_drawWall(0, 25, 33);

	glTranslatef(1, 23, 5);
	Charm_drawWall(0, 2, 8);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_LIGHT4);

	glPushMatrix();
	glTranslatef(40, 35, -1);
	DrawALight(0, 0, 0, 12, 16, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(32, -1, 5);
	DrawALight(0, 0, 0, 12, 16, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(54, 22, 0);
	DrawCuboid(0, 0, 0, 4, 4, 3, gray, 21, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	
	/******************************************/

	//draw the stair

	glPushMatrix();
	glTranslatef(28, 45, 0);
	drawTotalStair();
	glPopMatrix();

	//draw the firework
	glPushMatrix();
	glTranslatef(20, 32, 0);
	drawFirework();

	if (particlesIsNotInitialized)
	{
		InitializeParticleSystem();
		particlesIsNotInitialized = false;
	}
	if (g_Camera.objectsAnimationOn)
		drawParticleSystem();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, -10, -1);
	glRotatef(180, 0, 0, 1);
	drawMountain();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, 0, -1);
	glRotatef(240, 0, 0, 1);
	glScalef(1.1, 1.1, 0.8);
	drawMountain();
	glTranslatef(10, -30, -1);
	drawMountain();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-70, 80, -1);
	glScalef(1.2, 1, 1.1);
	glRotatef(300, 0, 0, 1);
	drawMountain();
	glPopMatrix();


	//second floor chairs and table
	glPushMatrix();
	glTranslatef(19, 11, 16);
	glRotatef(90, 0, 0, 1);
	drawChair(4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, 12.5, 16);
	glRotatef(270, 0, 0, 1);
	drawChair(4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19, 17, 16);
	glRotatef(90, 0, 0, 1);
	drawChair(4, 4);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(8, 18.5, 16);
		glRotatef(270, 0, 0, 1);
		drawChair(4, 4);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(12, 13.5, 16);
		DrawCuboid(0, 0, 0, 3, 3, 3, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
		glTranslatef(0, -4, 3);
		DrawCuboid(0, 0, 0, 3, 11, 0.5, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	//second floor light
	glPushMatrix();
		glTranslatef(35, 21, 20);
		DrawALight(0, 0, 0, 12, 16, 8);
	glPopMatrix();
	
	Charm_drawHouseShell();
	//draw guizi 
	glPushMatrix();
	glTranslatef(-14, 32, 0);
	drawGuiZi();
	glPopMatrix();
	

	glPopMatrix();
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

	glColor4f(black[0], black[1], black[2], black[3]);

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

	float curr_height = 2;

	for (int i = 0; i < 2; i++, curr_height += height) {
		// First roof
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 1, -width / (5 * height));

		glTexCoord2f(0, 0);
		glVertex3f(0, 0, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + curr_height);

		glTexCoord2f(width / 13.23, 0.0);
		glVertex3f(width / 5, 0, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + curr_height);

		glTexCoord2f(0, 0);
		glVertex3f(width * 4 / 5, 0, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + curr_height);

		glTexCoord2f(width / 13.23, 0);
		glVertex3f(width, 0, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + curr_height);

		glEnd();

		// Second roof.
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 1, width / (5 * height));

		glTexCoord2f(0, 0);
		glVertex3f(0, length, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + curr_height);

		glTexCoord2f(width / 13.23, 0.0);
		glVertex3f(width / 5, length, base_height);

		glTexCoord2f(width / 13.23, length / 5.55);
		glVertex3f(width / 5, length / 2, base_height + curr_height);

		glTexCoord2f(0, 0);
		glVertex3f(width * 4 / 5, length, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + curr_height);

		glTexCoord2f(width / 13.23, 0);
		glVertex3f(width, length, base_height);

		glTexCoord2f(0, length / 5.55);
		glVertex3f(width * 4 / 5, length / 2, base_height + curr_height);

		glEnd();

		glBegin(GL_TRIANGLES);

		glNormal3f(1, 0, -width / (5 * height));
		glTexCoord2f(width / 10, 0);
		glVertex3f(0, 0, base_height);

		glTexCoord2f(width / 20, width / 10);
		glVertex3f(width / 5, length / 2, base_height + curr_height);

		glTexCoord2f(0, 0);
		glVertex3f(0, length, base_height);

		glEnd();

		glBegin(GL_TRIANGLES);

		glNormal3f(1, 0, width / (5 * height));
		glTexCoord2f(width / 10, 0);
		glVertex3f(width, 0, base_height);

		glTexCoord2f(width / 20, width / 10);
		glVertex3f(width * 4 / 5, length / 2, base_height + curr_height);

		glTexCoord2f(0, 0);
		glVertex3f(width, length, base_height);

		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

void Charm_drawWoodLoop(float x, float y, float z,float inR, float outR, float width) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	for (int i = 90; i < 270; i++) {
		glBegin(GL_QUAD_STRIP);

		glTexCoord2f(0, 0.25 * PI * outR * i / 1080);
		glVertex3f(x + width, y - inR * sin(PI * i / 180), z - inR * cos(PI * i / 180));

		glTexCoord2f(0, 0.25 * PI * outR * (i + 1) / 1080);
		glVertex3f(x + width, y - inR * sin(PI * (i + 1) / 180), z - inR * cos(PI * (i + 1) / 180));

		glTexCoord2f(1, 0.25 * PI * outR * i / 1080);
		glVertex3f(x, y - inR * sin(PI * i / 180), z - inR * cos(PI * i / 180));

		glTexCoord2f(1, 0.25 * PI * outR * (i + 1) / 1080);
		glVertex3f(x, y - inR * sin(PI * (i + 1) / 180), z - inR * cos(PI * (i + 1) / 180));

		glTexCoord2f(0, 0.25 * PI * outR * i / 1080);
		glVertex3f(x, y - outR * sin(PI * i / 180), z - outR * cos(PI * i / 180));

		glTexCoord2f(0, 0.25 * PI * outR * (i + 1) / 1080);
		glVertex3f(x, y - outR * sin(PI * (i + 1) / 180), z - outR * cos(PI * (i + 1) / 180));

		glTexCoord2f(1, 0.25 * PI * outR * i / 1080);
		glVertex3f(x + width, y - outR * sin(PI * i / 180), z - outR * cos(PI * i / 180));

		glTexCoord2f(1, 0.25 * PI * outR * (i + 1) / 1080);
		glVertex3f(x + width, y - outR * sin(PI * (i + 1) / 180), z - outR * cos(PI * (i + 1) / 180));

		glTexCoord2f(0, 0.25 * PI * outR * i / 1080);
		glVertex3f(x + width, y - inR * sin(PI * i / 180), z - inR * cos(PI * i / 180));

		glTexCoord2f(0, 0.25 * PI * outR * (i + 1) / 1080);
		glVertex3f(x + width, y - inR * sin(PI * (i + 1) / 180), z - inR * cos(PI * (i + 1) / 180));

		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

void Charm_drawHalfCircleGlass(float xn, float yn, float zn, float x, float y, float z, float r) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(glass[0], glass[1], glass[2], glass[3]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_POLYGON);
	glNormal3f(xn, yn, zn);
	for (int i = 90; i < 270; i++) 
		glVertex3f(x, y - r * sin(PI * i / 180), z - r * cos(PI * i / 180));
	
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Charm_drawHouseShell() {

	glPushMatrix();// 0 -> 1

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// 1st wall
	glPushMatrix();// 1 -> 2

	// 1
	glPushMatrix();// 2 -> 3
	glTranslatef(0, 0, -1);
	Charm_drawWall(0, 3.75, 16);

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 3.75, 1);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 1, 10, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 11.25, 0);

	glPushMatrix();// 4 -> 5
	glTranslatef(0, 0, 1);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 1, 10, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 4 <- 5

	Charm_drawWall(0, 5.75, 16);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 43, 0);
	Charm_drawWall(0, 5.75, 16);

	glPushMatrix();// 4 -> 5
	glTranslatef(0, 5.75, 1);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 1, 10, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 4 <- 5

	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 56.25, 0);
	Charm_drawWall(0, 3.75, 16);

	glPushMatrix();// 4 -> 5
	glTranslatef(0, 0, 1);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 1, 10, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 4 <- 5

	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 3.75, 11);
	Charm_drawWall(0, 7.5, 5);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 8.5, 1, gray, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 48.75, 11);
	Charm_drawWall(0, 7.5, 5);
	DrawCuboid(-0.2, -0.5, 0, 2.4, 8.5, 1, gray, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 3.75, 0);
	Charm_drawWall(0, 7.5, 1);
	DrawCuboid(-0.2, 0.5, 1, 2.4, 6.5, 1, gray, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 48.75, 0);
	Charm_drawWall(0, 7.5, 1);
	DrawCuboid(-0.2, 0.5, 1, 2.4, 6.5, 1, gray, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPopMatrix();// 2 <- 3

	// 2
	glPushMatrix();// 2 -> 3
	glTranslatef(-20, 15, -1);
	Charm_drawWall(0, 30, 1);

	glPushMatrix();// 3 -> 4
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -20, 0);
	Charm_drawWall(0, 20, 1);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-30, 0, 0);
	Charm_drawWall(0, 20, 1);
	glPopMatrix();// 3 <- 4

	Charm_drawFloor(20, 30, 1);

	glPopMatrix();// 2 <- 3

	// 2.5
	glPushMatrix();// 2 -> 3
	glTranslatef(0, 0, -1);

	glPushMatrix();// 3 -> 4
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -60, 0);
	Charm_drawWall(0, 60, 1);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-60, 0, 0);
	Charm_drawWall(0, 60, 1);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glRotatef(180, 0, 0, 1);
	glTranslatef(-60, -60, 0);
	Charm_drawWall(0, 60, 1);
	glPopMatrix();// 3 <- 4

	glPopMatrix();// 2 <- 3

	// 3
	/**************************************************************/

	glPushMatrix();// 2 -> 3
	glTranslatef(-10, 15, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[65]);

	float doorCircleCenter_x = 0, doorCircleCenter_y = 15, doorCircleCenter_z = 7, Charm_doorR = 5, Charm_doorHeight = 12;// , window_r = 5, depth = 0;
	float door_edge_xl, door_edge_yl, door_edge_xr, door_edge_yr;
	float depth = 0;

	for (int j = 0; j < 2; j++, depth += 2) {
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 0, 1);

		for (int i = 0; i < 180; i++) {
			if (i >= 90) {
				door_edge_xl = doorCircleCenter_y - Charm_doorR * sin(i * PI / 180);
			}
			else {
				door_edge_xl = doorCircleCenter_y - Charm_doorR;
			}
			door_edge_yl = doorCircleCenter_z - Charm_doorR * cos(i * PI / 180);

			glTexCoord2f(0, door_edge_yl / 4.37);
			glVertex3f(depth, 0, door_edge_yl);

			glTexCoord2f(door_edge_xl / 19.78, door_edge_yl / 4.37);
			glVertex3f(depth, door_edge_xl, door_edge_yl);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 0, 1);

		for (int i = 0; i < 180; i++) {
			if (i >= 90) {
				door_edge_xr = doorCircleCenter_y + Charm_doorR * sin(i * PI / 180);
			}
			else {
				door_edge_xr = doorCircleCenter_y + Charm_doorR;
			}
			door_edge_yr = doorCircleCenter_z - Charm_doorR * cos(i * PI / 180);

			glTexCoord2f(0, door_edge_yr / 4.37);
			glVertex3f(depth, 30, door_edge_yr);

			glTexCoord2f((30 - door_edge_xr) / 19.78, door_edge_yr / 4.37);
			glVertex3f(depth, door_edge_xr, door_edge_yr);
		}
		glEnd();
	}
	/*****************************************************************/
	Charm_drawWoodLoop(doorCircleCenter_x - 0.2, doorCircleCenter_y, doorCircleCenter_z, Charm_doorR - 0.5, Charm_doorR + 0.5, 2.4);

	DrawCuboid(doorCircleCenter_x - 0.2, doorCircleCenter_y - Charm_doorR - 0.5, 0, 2.4, 1, 7, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(doorCircleCenter_x - 0.2, doorCircleCenter_y + Charm_doorR - 0.5, 0, 2.4, 1, 7, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	Charm_drawWall(0, 10, 2);

	glPushMatrix();// 3 -> 4
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -12, 0);
	Charm_drawWall(0, 12, 15);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glTranslatef(0, 20, 0);
	Charm_drawWall(0, 10, 2);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-30, 0, 0);
	Charm_drawWall(0, 12, 15);
	glPopMatrix();// 3 <- 4

	glTranslatef(0, 0, Charm_doorHeight);
	Charm_drawWall(0, 30, 3);

	glPopMatrix();// 2 <- 3

	// 4
	glPushMatrix();// 2 -> 3
	glTranslatef(-20, 0, 15);
	Charm_drawWall(0, 60, 1);

	glPushMatrix();// 3 -> 4
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -20, 0);
	Charm_drawWall(0, 20, 1);
	glPopMatrix();// 3 <- 4

	glPushMatrix();// 3 -> 4
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-60, 0, 0);
	Charm_drawWall(0, 20, 1);
	glPopMatrix();// 3 <- 4

	Charm_drawFloor(52, 60, 1);
	Charm_drawFloor(52, 60, 0);
	DrawSurface(1, 0, 0, 52, 51, 0, 52, 51, 1, 52, 60, 1, 52, 60, 0, 64, 0, gray, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(52, 0, 0);
	Charm_drawFloor(8, 53, 1);
	Charm_drawFloor(8, 53, 0);
	DrawSurface(0, 1, 0, 0, 53, 0, 0, 53, 1, 8, 53, 1, 8, 53, 0, 64, 0, gray, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(8, 0, 0);
	Charm_drawFloor(18, 60, 1);
	Charm_drawFloor(18, 60, 0);
	glPopMatrix();// 2 <- 3

	//5
	glPushMatrix();// 2 -> 3
	/***************************************************/
	glTranslatef(0, 0, 15);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	glColor4f(wallGreen[0], wallGreen[1], wallGreen[2], wallGreen[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[65]);

	float Charm_winR = Charm_doorR, Charm_winHeight = 10;
	float win_edge_xl, win_edge_yl, win_edge_xr, win_edge_yr;
	float winCircleCenter_x = doorCircleCenter_x, winCircleCenter_y = doorCircleCenter_y + 15, winCircleCenter_z = doorCircleCenter_z - 1;
	depth = 0;
	for (int j = 0; j < 2; j++, depth += 2) {
		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 0, 1);

		for (int i = 0; i < 180; i++) {
			if (i >= 90) {
				win_edge_xl = winCircleCenter_y - Charm_winR * sin(i * PI / 180);
			}
			else {
				win_edge_xl = winCircleCenter_y - Charm_winR;
			}
			win_edge_yl = winCircleCenter_z - Charm_winR * cos(i * PI / 180);

			glTexCoord2f(0, win_edge_yl / 4.37);
			glVertex3f(depth, 0, win_edge_yl);

			glTexCoord2f(win_edge_xl / 19.78, win_edge_yl / 4.37);
			glVertex3f(depth, win_edge_xl, win_edge_yl);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);

		glNormal3f(0, 0, 1);

		for (int i = 0; i < 180; i++) {
			if (i >= 90) {
				win_edge_xr = winCircleCenter_y + Charm_winR * sin(i * PI / 180);
			}
			else {
				win_edge_xr = winCircleCenter_y + Charm_winR;
			}
			win_edge_yr = winCircleCenter_z - Charm_winR * cos(i * PI / 180);

			glTexCoord2f(0, win_edge_yr / 4.37);
			glVertex3f(depth, 60, win_edge_yr);

			glTexCoord2f((60 - win_edge_xr) / 19.78, win_edge_yr / 4.37);
			glVertex3f(depth, win_edge_xr, win_edge_yr);
		}
		glEnd();
	}
	/***************************************************/

	glPushMatrix();// 3 -> 4
	Charm_drawWall(11, 60, 5);
	Charm_drawWoodLoop(winCircleCenter_x - 0.2, winCircleCenter_y, winCircleCenter_z, Charm_winR - 0.5, Charm_winR + 0.5, 2.4);
	glTranslatef(0, 0, 1);
	DrawCuboid(winCircleCenter_x - 0.2, winCircleCenter_y - Charm_winR - 0.5, 0, 2.4, 1, 5, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(winCircleCenter_x - 0.2, winCircleCenter_y + Charm_winR - 0.5, 0, 2.4, 1, 5, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 3 <- 4

	glPopMatrix();// 2 <- 3

	// 6
	glPushMatrix();// 2 -> 3
	glTranslatef(0, 0, -1);
	DrawCylinder(-16, 5, 0, 1.5, 16, white);
	glTranslatef(0, 15, 0);
	DrawCylinder(-16, 5, 0, 1.5, 16, white);
	glPopMatrix();// 2 <- 3

	glPushMatrix();// 2 -> 3
	glTranslatef(0, 60, -1);
	DrawCylinder(-16, -5, 0, 1.5, 16, white);
	glTranslatef(0, -15, 0);
	DrawCylinder(-16, -5, 0, 1.5, 16, white);
	glPopMatrix();// 2 <- 3

	//Second floor
	glPushMatrix();// 2 -> 3

	glTranslatef(-19, 1, 16);
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 10; i++) {
			DrawCuboid(2 * i, 58 * j, 0, 0.3, 0.3, 2, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
		}
		DrawCuboid(0, 58 * j, 2, 19, 0.3, 0.3, gray, 63, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	}
	for (int i = 1; i < 29; i++) {
		DrawCuboid(0, 2 * i, 0, 0.3, 0.3, 2, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	}
	DrawCuboid(0, 0.3, 2, 0.3, 57.7, 0.3, gray, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();// 2 <- 3

	glPopMatrix();// 1 <- 2


	// 2nd wall
	glPushMatrix();// 1 -> 2
	glTranslatef(Charm_wallWidth, 0, 0);
	glRotatef(90, 0, 0, 1);
	Charm_drawWall(0, Charm_wallWidth, Charm_wallHeight);
	glPopMatrix();// 1 <- 2

	// 3rd wall
	glPushMatrix();// 1 -> 2
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-Charm_wallWidth, 0, 0);
	Charm_drawWall(0, Charm_wallWidth, Charm_wallHeight);
	glPopMatrix();// 1 <- 2

	// 4th wall
	glPushMatrix();// 1 -> 2
	glRotatef(180, 0, 0, 1);
	glTranslatef(-Charm_wallLength, -Charm_wallWidth, 0);
	Charm_drawWall(0, Charm_wallLength, Charm_wallHeight);
	glPopMatrix();// 1 <- 2

	// Floor
	DrawSurface(0,0,1,0,0,0,0, Charm_wallLength,0, Charm_wallWidth, Charm_wallLength,0, Charm_wallWidth,0,0,64,0,gray,defaultAmbient,defaultDiffuse,defaultSpecular,defaultEmission,defaultShine);
	//Charm_drawFloor(60, 60, 0.01);

	//draw cha ji
	glPushMatrix();// 1 -> 2
	glTranslatef(6, 9, 0);
	glScalef(0.8, 0.8, 0.8);
	glRotatef(90, 0, 0, 1);
	drawTeatable();
	glPopMatrix();// 1 <- 2

	// House roof
	glPushMatrix();// 1 -> 2
	glTranslatef(-5, -5, 0);
	Charm_drawRoof(Charm_wallHeight - 1, Charm_wallWidth + (Charm_wallWidth/6), Charm_wallLength + (Charm_wallLength/6),10);
	glPopMatrix();// 1 <- 2

	glTranslatef(5, 30, 5);
	glPopMatrix();// 0 <- 1

	glPushMatrix();// 0 -> 1
	glTranslatef(0, 3.75, 0);
	DrawSurface(1, 0, 0, 0.1, 0, 0, 0.1, 0, 10, 0.1, 7.5, 10, 0.1, 7.5, 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);
	
	glTranslatef(0, 25 - 3.75, 0);

	glTranslatef(0, 23.75, 0);
	DrawSurface(1, 0, 0, 0.1, 0, 0, 0.1, 0, 10, 0.1, 7.5, 10, 0.1, 7.5, 0, -1, 1, glass, glassAmbient, glassDiffuse, glassSpecular, defaultEmission, glassShine);

	glPopMatrix();// 0 <- 1

	glDisable(GL_REPLACE);
}

void Charm_drawRoomWall(float base_height, float length, float height) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, defaultShine);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glColor4f(white[0], white[1], white[2], white[3]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[19]);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, base_height);

	glTexCoord2f(0.0, height / 2);
	glVertex3f(0, 0, base_height + height);

	glTexCoord2f(length / 2, height / 2);
	glVertex3f(0, length, base_height + height);

	glTexCoord2f(length / 2, 0.0);
	glVertex3f(0, length, base_height);
	glEnd();

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(2, 0, base_height);

	glTexCoord2f(0.0, height / 2);
	glVertex3f(2, 0, base_height + height);

	glTexCoord2f(length / 2, height / 2);
	glVertex3f(2, length, base_height + height);

	glTexCoord2f(length / 2, 0.0);
	glVertex3f(2, length, base_height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

/***********************  By Jason Qiu  **************************/
// function used to draw the sofa mat
void drawMat()
{
	DrawCuboid(0, 0, 0, 3, 3, 0.6, white, 67, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
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
	DrawCuboid(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	// Second Frame of the teatable
	glPushMatrix();
	glRotatef(-90, 0.0, 0.0, 1.0);
	DrawCuboid(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Third Frame of the teatable
	glPushMatrix();
	glTranslatef(0.0, -10.0, 0.0);
	DrawCuboid(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Fourth Frame of the teatable
	glPushMatrix();
	glRotatef(-90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 10.0, 0.0);
	DrawCuboid(0, 0, 0, 10, 0.1, 0.1, black, -1, 0, lightConeAmbient, lightConeDiffuse, lightConeSpecular, lightConeEmission, lightConeShine);
	glPopMatrix();
	// Table surface
	glPushMatrix();
	glTranslatef(0.0, -10.0, 0.0);
	DrawSurface(0, 0, 1, 0, 0, 0.1, 0, 10, 0.1, 10, 10, 0.1, 10, 0, 0.1, -1, 1, glass, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
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
	// Table surface
	glPushMatrix();
	glTranslatef(0, 0, 1.7);
	drawTeatableSurface();
	glPopMatrix();
}
double imageIndex = 68.0;
// function used to draw the television
void drawTelevision(float x, float y, float z, float width, float height, float innerWidth, float innerHeight, float angle, float color[])
{
	if (g_Camera.objectsAnimationOn)
	{
		degreeLong += dDegreeLong;
		degreeShort += dDegreeShort;
		imageIndex = imageIndex + 0.01;
		if ((int)imageIndex == 72) {
			imageIndex = 68;
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
	DrawCuboid(0, 0, 0, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, 0, height - frameH, width, thickness, frameH, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(0, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawCuboid(width - frameW, 0, frameH, frameW, thickness, innerHeight + 2 * offset, color, -1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw the slanted quadrilaterals
	DrawSurface(0, 0.75 * thickness, thickness / 4 * sqrt(3.0), frameW, 0, frameH, width - frameW, 0, frameH, width - frameW, thickness, thickness * sqrt(3.0) + frameH, frameW, thickness, thickness * sqrt(3.0) + frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(0, 0.75 * -thickness, -(thickness / 4 * sqrt(3.0)), frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, thickness, height - frameH - thickness * sqrt(3.0), width - frameW, 0, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(thickness * sqrt(3.0) / 4, 0.75 * thickness, 0, frameW, 0, frameH, thickness * sqrt(3.0) + frameW, thickness, frameH, thickness * sqrt(3.0) + frameW, thickness, height - frameH, frameW, 0, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	DrawSurface(-(thickness / 4 * sqrt(3.0)), 0.75 * -thickness, 0, width - frameW - thickness * sqrt(3.0), thickness, frameH, width - frameW, 0, frameH, width - frameW, 0, height - frameH, width - frameW - thickness * sqrt(3.0), thickness, height - frameH, -1, 0, color, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	// Draw the back plane
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, -0.3);
	DrawSurface(0, 0, 1, 0, 0, 0.1, 0, height, 0.1, width, height, 0.1, width, 0, 0.1, -1, 1, black, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	// Draw the image on TV
	glPushMatrix();
	//glRotatef(degreeLong, 0, 0, 1);
	DrawSurface(0, 0, 1, 0, 0, 0, width, 0, 0, width, 0, height, 0, 0, height, int(imageIndex), 2, white, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	glPopMatrix();


}

// function used to draw the lamp
void drawLamp()
{

	float lightHangerheight = 10 - 8;
	float lightHangerRadius = 0.05;
	glPushMatrix();
	glTranslatef(0, 0, 7.5);
	glRotatef(0, 0.0, 0.0, 1.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	glRotatef(0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightConeAmbient);  // do we need all this?  isn't it in lightList?
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightConeSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, lightConeEmission);
	glMaterialfv(GL_FRONT, GL_SHININESS, lightConeShine);
	glColor4f(silver[0], silver[1], silver[2], silver[3]);
	//gluCylinder(quadric, lightHangerRadius, lightHangerRadius, lightHangerheight, polySubdivisions, polySubdivisions);
	glCallList(lightList);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	gluCylinder(quadric, 1.2, 1.2, 0.4, 30, 10);
	gluDisk(quadric, 0, 1.2, polySubdivisions, polySubdivisions);
	glPopMatrix();
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

void drawGuiZiSurface()
{

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glNormal3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(23, 15, platformHeight);//x,࠭

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(31, 15, platformHeight);//y,࠭

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(31, 18, platformHeight);//y,Ӥ

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(23, 18, platformHeight);//x,Ӥ
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void drawTeapot()
{
	glPushMatrix();
	glTranslatef(26, 16.5, -0.7);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(0.38);
	glPopMatrix();
}

void drawGuiZiSide()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(23, 18, 0);

	glTexCoord2f(1, 0);
	glVertex3f(23, 15, 0);

	glTexCoord2f(1, 1);
	glVertex3f(23, 15, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(23, 18, platformHeight);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(31, 15, 0);

	glTexCoord2f(1, 0);
	glVertex3f(31, 18, 0);

	glTexCoord2f(1, 1);
	glVertex3f(31, 18, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(31, 15, platformHeight);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(23, 15, 0);

	glTexCoord2f(1, 0);
	glVertex3f(31, 15, 0);

	glTexCoord2f(1, 1);
	glVertex3f(31, 15, platformHeight);

	glTexCoord2f(0, 1);
	glVertex3f(23, 15, platformHeight);

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

void drawGuiZi()
{
	glTranslatef(0, 0, 3);
	// Clip Plane Equations
	double eqn[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	drawGuiZiSurface();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);


	glPushMatrix();

	glScalef(1.0f, 1.0f, -1.0f);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.7);
	drawTeapot();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.3);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawGuiZiSurface();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glPushMatrix();
	glTranslatef(0.0f, 0.0, 1.3);
	drawTeapot();
	glPopMatrix();


	drawGuiZiSide();
	glPushMatrix();
	glTranslatef(25, 15.05, -2.8);
	DrawCuboid(0, 0, 0, 3, 2.9, 3, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 20, -2.8);
	DrawCuboid(0, 0, 0, 1.5, 1.5, 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 11.5, -2.8);
	DrawCuboid(0, 0, 0, 1.5, 1.5, 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 15.05, -2.8);
	DrawCuboid(0, 0, 0, 1.5, 1.5, 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 15.05, -2.8);
	DrawCuboid(0, 0, 0, 1.5, 1.5, 2, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

}

static GLfloat angle = 0.0f;
static GLfloat earth_angle = 60.0f;
void drawStorageLight(float rotX, float rotY, float rotZ, float x, float y, float z) {
	if (g_Camera.objectsAnimationOn)
	{
		angle += 1.0f;
		earth_angle += 1.0f;
		if (angle >= 360.0f)
			angle = 0.0f;
		if (earth_angle >= 360.0f)
			earth_angle = 0.0f;
	}
	glPushMatrix();

	// Define Sun's light
	{
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		GLfloat light_constant_attenuation[] = { 1.0 };
		GLfloat light_linear_attenuation[] = { 10.0 };
		GLfloat light_quadratic_attenuation[] = { 10.0 };
		glLightfv(GL_LIGHT3, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT3, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT3, GL_SPECULAR, sun_light_specular);

		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	glPushMatrix();
	// Define sun's texture and draw sun   
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_emission[] = { 0.4f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_mat_shininess = 0.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
		glutSolidSphere(1.5, 40, 32);
	}
	glPopMatrix();
	glPushMatrix();
	// Define Black ball's texture   
	{
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat earth_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
		glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(3.0f, 0.0f, 0.0f);
		glutSolidSphere(1, 40, 32);
	}
	glPopMatrix();
	glPushMatrix();
	// Define Earth texture and draw earth   
	{
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat earth_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat earth_mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
		//glRotatef(angle, 0.0f, -1.0f, 0.0f);
		//glTranslatef(6.0f, 0.0f, 0.0f);
		//glutSolidSphere(1, 40, 32);

		gluQuadricNormals(quadric, GL_SMOOTH);
		gluQuadricTexture(quadric, GL_TRUE);

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(white[0], white[1], white[2], white[3]);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textures[54]);

		glPushMatrix();
		glRotatef(earth_angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(5.0f, 0.0f, 0.0f);
		gluSphere(quadric, 1.0, polySubdivisions, polySubdivisions);
		glPopMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		glPushMatrix();
		//glTranslatef(26, 12.5, 0);
		glRotatef(earth_angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(5.0f, 0.0f, 0.0f);
		gluSphere(quadric, 1.0, polySubdivisions, polySubdivisions);
		glPopMatrix();

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);


	}
	glPopMatrix();
	glPopMatrix();
}


#define MAXCOLOR 7
float colortab[MAXCOLOR][4] =
{
	1,        0.6941,            0.1059,    0.0,    
	1,        0.6941,            0.1059,    0.0,    
	1,        0.6941,            0.1059,    0.0,    
	1,        0.6941,            0.1059,    0.0,    
	0.941,        0.3686,        0.1098,    0.0,   
	0.8039,        0.4980,        0.1961,    0.0,    
	1,        0.8745,        0.8745,    0.0        
};


void Square(float center[3], float size, float color[4])
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glTranslatef(center[0], center[1], center[2]);
	gluSphere(quadric, size, 5, 5);
	glPopMatrix();
}

void SurroundSquare(float center[3], float size, float color[4])
{
	float halfcenter[6][3] =
	{
		center[0] + size * 3 / 4.0, center[1], center[2],
		center[0] - size * 3 / 4.0, center[1], center[2],
		center[0] , center[1] + size * 3 / 4.0, center[2],
		center[0] , center[1] - size * 3 / 4.0, center[2],
		center[0] , center[1], center[2] + size * 3 / 4.0,
		center[0] , center[1], center[2] - size * 3 / 4.0,
	};

	for (int i = 0; i < 6; i++)
	{
		Square(halfcenter[i], size / 2, color);

	}
}

void Iteration(float center[3], float size, int nIter)
{
	if (nIter < 0)return;

	glColor4f(colortab[nIter][0], colortab[nIter][1], colortab[nIter][2], colortab[nIter][3]);
	SurroundSquare(center, size, colortab[nIter]);

	float halfcenter[6][3] =
	{
		center[0] + size * 3 / 4.0, center[1], center[2],
		center[0] - size * 3 / 4.0, center[1], center[2],
		center[0] , center[1] + size * 3 / 4.0, center[2],
		center[0] , center[1] - size * 3 / 4.0, center[2],
		center[0] , center[1], center[2] + size * 3 / 4.0,
		center[0] , center[1], center[2] - size * 3 / 4.0,
	};

	for (int i = 0; i < 3; i++)
	{
		Iteration(halfcenter[i], size / 2, nIter - 1);
	}
}

void drawFractals()
{
	float center[] = { 0.0f, 0.0f, 0.0f };
	float radius = 2;
	float color[] = { 1.0, 0.1, 0.0, 0 };
	Square(center, radius, color);
	Iteration(center, radius, 6);
}

/***********************  By Elliot Tian  **************************/
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
	DrawCuboid(0, 0, 0, 5, 8, 0.6, white, 2, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw head
	glPushMatrix();
	DrawCuboid(0, -0.6, 0, 5, 0.6, 2.5, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw left leg
	glPushMatrix();
	glTranslatef(0, 7.4, 0.0);
	DrawCuboid(0, 0, 0, 0.6, 0.6, 0.6, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
	//draw right leg
	glPushMatrix();
	glTranslatef(4.4, 7.4, 0);
	DrawCuboid(0, 0, 0, 0.6, 0.6, 0.6, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
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

void drawBedBase()
{
	// Bottom mat
	glPushMatrix();
	glTranslatef(0, 0.0, 0);
	DrawCuboid(0, 0, 0, 5, 8, 0.4, white, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();
}

void drawHuaPen()
{
	gluQuadricNormals(quadric, GL_SMOOTH);
	glColor4f(brown[0], brown[1], brown[2], brown[3]);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	gluCylinder(quadric, 0.8, 1, 1.2, polySubdivisions, polySubdivisions);
	glPopMatrix();

	glPushMatrix();
	//glCallList(treeOutsideList);
	drawHua();
	glPopMatrix();
}

void drawHua()
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight);
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(90 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen1[0], treeGreen1[1], treeGreen1[2], treeGreen1[3]);
	glRotatef(180 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(270 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight + treeLeavesHeight);
	glPushMatrix();
	glColor4f(treeGreen1[0], treeGreen1[1], treeGreen1[2], treeGreen1[3]);
	glRotatef(90 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(180 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(270 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glTranslatef(treeLeavesOffsetX, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight + treeLeavesOffsetZ);
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(70 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(130 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(190 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(250 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(310 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(10 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight + treeLeavesHeight - treeLeavesOffsetZ);
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(70 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(130 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(190 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(250 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(310 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 2, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(10 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX, 0.0, 0.0);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight + 2 * treeLeavesOffsetZ);
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(30 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen1[0], treeGreen1[1], treeGreen1[2], treeGreen1[3]);
	glRotatef(90 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(150 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(210 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(270 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(330 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, treeHeight + treeLeavesHeight - 2 * treeLeavesOffsetZ);
	glPushMatrix();
	glColor4f(treeGreen2[0], treeGreen2[1], treeGreen2[2], treeGreen2[3]);
	glRotatef(50 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(110 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(170 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(230 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen3[0], treeGreen3[1], treeGreen3[2], treeGreen3[3]);
	glRotatef(290 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPushMatrix();
	glColor4f(treeGreen4[0], treeGreen4[1], treeGreen4[2], treeGreen4[3]);
	glRotatef(350 + treeLeavesVarianceAngle * GEN_RANDOM_FLOAT, 0.0, 0.0, 1.0);
	glTranslatef(treeLeavesOffsetX * 3, 0.0, treeLeavesVarianceZ * GEN_RANDOM_FLOAT);
	glutSolidSphere(treeLeavesRadius, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glColor4f(darkBrown[0], darkBrown[1], darkBrown[2], darkBrown[3]);
	gluCylinder(quadric, treeBarkRadiusBottom, treeBarkRadiusTop, treeHeight, polySubdivisions, polySubdivisions);
	glPopMatrix();
}



void drawTotalStair() {
	drawStair();

	glPushMatrix();
	glTranslated(0, 8, 8);
	DrawCuboid(0, 0, 0, 4, 5, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);
	glPopMatrix();

	glPushMatrix();
	glRotated(270, 0, 0, 1);
	glTranslated(-13, 4, 8);
	drawStair();
	glPopMatrix();
}


void drawStair() {
	//glColor4f(white[0], white[1], white[2], white[3]);
	glPushMatrix();
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glTranslatef(0, 1, 1);
	DrawCuboid(0, 0, 0, 5, 1, 1, wallGreen, 1, 0, defaultAmbient, defaultDiffuse, defaultSpecular, defaultEmission, defaultShine);

	glPopMatrix();
}


void drawFirework() {
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4f(pureWhite[0], pureWhite[1], pureWhite[2], pureWhite[3]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[76]);
	glPushMatrix();
	gluCylinder(quadric, 1, 0.1, 3, polySubdivisions, polySubdivisions);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawMountain() {
	// First hill
	glColor4f(hillColor2[0], hillColor2[1], hillColor2[2], hillColor2[3]);
	glPushMatrix();
	glScalef(5, 5, 5);
	glRotatef(-45, 0.0, 0, 1.0);
	gluBeginSurface(nurb1);
	gluNurbsSurface(nurb1, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts1[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb1);
	glPopMatrix();

	// Second hill
	glPushMatrix();
	glTranslatef(-25, 10, 0);
	glScalef(10, 10, 10);
	glRotatef(-45, 0.0, 0, 1.0);
	gluBeginSurface(nurb2);
	gluNurbsSurface(nurb2, numOfKnots, knots, numOfKnots, knots, 3 * numOfCtrlPtsU, 3, &ctrlPts2[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb2);
	glPopMatrix();
	glEndList();
}

void drawShadowCylinder() {
	gluQuadricNormals(quadric, GL_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4f(pureWhite[0], pureWhite[1], pureWhite[2], pureWhite[3]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[26]);
	glPushMatrix();
	gluCylinder(quadric, 0.7, 0.85, 3, polySubdivisions, polySubdivisions);
	glPopMatrix();
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
	glDisable(GL_TEXTURE_2D);
}


void drawShadowSystem()
{
	if (g_Camera.objectsAnimationOn)
	{
		degreeLong += dDegreeLong;
		degreeShort += dDegreeShort;
	}

	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glColor4f(0.0, 0.0, 0.0, 0.5f);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslatef(-0.1, 0.1, 0.85);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	shadowMatrix(floorShadow, groundplane, lightPos3);
	glMultMatrixf((float*)floorShadow);
	DrawCylinder(0, 0, 0.35, 0.7, 3, black);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}