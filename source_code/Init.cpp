#include "Main.h"
#include <string>
using namespace std;

void CreateTexture(UINT textureArray[], char strFileName[], int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)
		return;
	
	pBitmap = auxDIBImageLoad(strFileName);
	
	if(pBitmap == NULL)
		exit(0);

	glGenTextures(1, &textureArray[textureID]);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);



	if (pBitmap)
	{
		if (pBitmap->data)
		{
			free(pBitmap->data);
		}

		free(pBitmap);
	}
}

void ChangeToFullScreen()
{
	DEVMODE dmSettings;

	memset(&dmSettings,0,sizeof(dmSettings));

	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= SCREEN_WIDTH;	
	dmSettings.dmPelsHeight	= SCREEN_HEIGHT;
	
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}


HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASS));	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;			
	wndclass.lpfnWndProc = WinProc;						
	wndclass.hInstance = hInstance;						
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	
	wndclass.lpszClassName = "UIC_CG_project";			

	RegisterClass(&wndclass);
	
	if(bFullScreen && !dwStyle) 
	{							
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen();
		ShowCursor(FALSE);
	}
	else if(!dwStyle)
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;

	RECT rWindow;
	rWindow.left	= 0;
	rWindow.right	= width;
	rWindow.top	    = 0;
	rWindow.bottom	= height;

	AdjustWindowRect( &rWindow, dwStyle, false);

	hWnd = CreateWindow("UIC_CG_project", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;	

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);	

	SetFocus(hWnd);	

	return hWnd;
}


bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;	

    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			
    pfd.iPixelType = PFD_TYPE_RGBA;				
    pfd.cColorBits = SCREEN_DEPTH;		
    pfd.cDepthBits = SCREEN_DEPTH;		
    pfd.cAccumBits = 0;			
    pfd.cStencilBits = 1;
 
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;
}


void SizeOpenGLScreen(int width, int height)
{
	if (height==0)	
	{
		height=1;
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();							

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();					
}


void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);	
							
    if (!bSetupPixelFormat(g_hDC))			
        PostQuitMessage (0);		

    g_hRC = wglCreateContext(g_hDC);
    wglMakeCurrent(g_hDC, g_hRC);	


	glEnable(GL_DEPTH_TEST);		

	glClearColor(0, 0, 0, 1);


	float lightAmbience0[4] = {0.3, 0.3, 0.3, 0.5};
	float lightDiffuse0[4] = {0.5, 0.5, 0.5, 1.0};		
	float lightSpecular0[4] = {0.5, 0.5, 0.5, 0.5};	

	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbience0);		
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse0);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
	glEnable(GL_LIGHT0);							


	float lightAmbience1[4] = {0.3, 0.3, 0.3, 1.0};	
	float lightDiffuse1[4] = {1.0, 1.0, 1.0, 1.0};		
	float lightSpecular1[4] = {0.5, 0.5, 0.5, 0.5};	
	float lightSpotDir1[3] = {0, 0, -1};
	float lightSpotCutoff1 = 15.0;
	float lightSpotExp1 = 2.0;

	glLightfv(GL_LIGHT1, GL_AMBIENT,  lightAmbience1);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightDiffuse1);	
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lightSpotCutoff1);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lightSpotExp1);

	
	float lightAmbience2[4] = {0.7, 0.5, 0.6, 1.0};		
	float lightDiffuse2[4] = {0.1f, 0.1f, 0.1f, 0.1};		
	float lightSpecular2[4] = {0.2f, 0.2f, 0.2f, 0.2f};	

	glLightfv(GL_LIGHT2, GL_AMBIENT,  lightAmbience2);		
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  lightDiffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular2);

	float lightSpotCutoff2 = 30.0;
	float lightSpotExp2 = 5.0;

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lightSpotCutoff2);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lightSpotExp2);


	float lightSpotDir3[3] = {0, 0, -1};
	float lightSpotCutoff3 = 15.0;
	float lightSpotExp3 = 2.0;
	float lightAmbience3[4] = {0.6, 0.6, 0.75, 1.0};	
	float lightDiffuse3[4] = {0.1f, 0.1f, 0.1f, 0.1};		
	float lightSpecular3[4] = {0.2f, 0.2f, 0.2f, 0.2f};

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);								
	
	glEnable(GL_COLOR_MATERIAL);		
	glShadeModel(GL_SMOOTH);
	
	SizeOpenGLScreen(width, height);				
}


void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);					
		wglDeleteContext(g_hRC);			
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);		
		
	if(g_bFullScreen)				
	{
		ChangeDisplaySettings(NULL,0);	
		ShowCursor(TRUE);							
	}

	UnregisterClass("UIC_CG_project", g_hInstance);		

	PostQuitMessage (0);							
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	HWND hWnd;

	animationOn = false;

	g_bFullScreen = false;

	std::string title = "UIC CG project";
	hWnd = CreateMyWindow(const_cast<char*>(title.c_str()), SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_bFullScreen, hInstance);


	if(hWnd == NULL) return TRUE;

	Init(hWnd);													

	return MainLoop();						
}
