#include "Main.h"


/*
 * This creates a texture in OpenGL that we can texture map
 */
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);


	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}


/*
 * This changes the screen to FULL SCREEN
 */
void ChangeToFullScreen()
{
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= SCREEN_WIDTH;				// Selected Screen Width
	dmSettings.dmPelsHeight	= SCREEN_HEIGHT;			// Selected Screen Height
	
	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}


/*
 * This function creates a window, but doesn't have a message loop
 */
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
	wndclass.style = CS_HREDRAW | CS_VREDRAW;			// Regular drawing capabilities
	wndclass.lpfnWndProc = WinProc;						// Pass our function pointer as the window procedure
	wndclass.hInstance = hInstance;						// Assign our hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// An arrow for the cursor
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	// A white window
	wndclass.lpszClassName = "Surreal";					// Assign the class name

	RegisterClass(&wndclass);							// Register the class
	
	if(bFullScreen && !dwStyle) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen();							// Go to full screen
		ShowCursor(FALSE);								// Hide the cursor
	}
	else if(!dwStyle)									// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;							// Assign our global hInstance to the window's hInstance

	RECT rWindow;
	rWindow.left	= 0;								// Set Left Value To 0
	rWindow.right	= width;							// Set Right Value To Requested Width
	rWindow.top	    = 0;								// Set Top Value To 0
	rWindow.bottom	= height;							// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("Surreal", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;								// If we could get a handle, return NULL

	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

	SetFocus(hWnd);										// Sets Keyboard Focus To The Window	

	return hWnd;
}


/*
 * This function sets the pixel format for OpenGL.
 */
bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = SCREEN_DEPTH;						// Here we use our #define for the color bits
    pfd.cDepthBits = SCREEN_DEPTH;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 1;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;										// Return a success!
}


/*
 * This function resizes the viewport for OpenGL.
 */
void SizeOpenGLScreen(int width, int height)			// Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero error
	{
		height=1;										// Make the Height Equal One
	}

	glViewport(0,0,width,height);						// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  The closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


/*
 * This function handles all the initialization for OpenGL.
 */
void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
        PostQuitMessage (0);							// If there's an error, quit

    g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use


	// OUR CODE
	glEnable(GL_DEPTH_TEST);		

	glClearColor(0, 0, 0, 1);


	// LIGHT0 is the ambient light in museum scene
	float lightAmbience0[4] = {0.3, 0.3, 0.3, 0.5};
	float lightDiffuse0[4] = {0.5, 0.5, 0.5, 1.0};		
	float lightSpecular0[4] = {0.5, 0.5, 0.5, 0.5};	

	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbience0);		
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse0);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
	glEnable(GL_LIGHT0);							


	// LIGHT1 is the spotlight in VG scene
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

	/*
	// LIGHT2 is the light at the corner of VG scene
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


	// LIGHT3 is the light above the shadowed objects
	float lightSpotDir3[3] = {0, 0, -1};
	float lightSpotCutoff3 = 15.0;
	float lightSpotExp3 = 2.0;
	float lightAmbience3[4] = {0.6, 0.6, 0.75, 1.0};	
	float lightDiffuse3[4] = {0.1f, 0.1f, 0.1f, 0.1};		
	float lightSpecular3[4] = {0.2f, 0.2f, 0.2f, 0.2f};

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lightSpotCutoff1);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, lightSpotExp1);

	glLightfv(GL_LIGHT3, GL_AMBIENT,  lightAmbience3);	
	glLightfv(GL_LIGHT3, GL_DIFFUSE,  lightDiffuse3);	
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpecular3);

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lightSpotCutoff3);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, lightSpotExp3);
	*/

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);								
	
	glEnable(GL_COLOR_MATERIAL);		
	glShadeModel(GL_SMOOTH);
	
	SizeOpenGLScreen(width, height);				
}


/*
 * This function cleans up and then posts a quit message to the window
 */
void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory
		
	if(g_bFullScreen)									// If we were in full screen
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	UnregisterClass("Surreal", g_hInstance);			// Free the window class

	PostQuitMessage (0);								// Post a QUIT message to the window
}


/*
 * This function handles registering and creating the window.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	HWND hWnd;

	// Check if we want full screen or not
	if(MessageBox(NULL, "Do you want to see the animation?", "Options", MB_YESNO | MB_ICONQUESTION) == IDNO)
		animationOn = false;

	g_bFullScreen = false;
	
	// Create our window with our function we create that passes in the:
	// Name, width, height, any flags for the window, if we want fullscreen of not, and the hInstance
	hWnd = CreateMyWindow("Surreal", SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_bFullScreen, hInstance);

	// If we never got a valid window handle, quit the program
	if(hWnd == NULL) return TRUE;

	// INIT OpenGL
	Init(hWnd);													

	// Run our message loop and after it's done, return the result
	return MainLoop();						
}
