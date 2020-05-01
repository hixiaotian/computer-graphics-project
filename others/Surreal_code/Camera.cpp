#include "Main.h"
#include "Camera.h"

float dTime = 0.0f;

char cameraDebug[50];

/*
 * This returns a perpendicular vector from 2 given vectors by taking the cross product.												
 */
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


/*
 * This returns the magnitude of a vector
 */
float Magnitude(CVector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}


/*
 * This returns a normalize vector (A vector exactly of length 1)
 */
CVector3 Normalize(CVector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	
    static float previousTime		= 0.0f;	
	static float frameTime = 0.0f;	
    float currentTime = timeGetTime() * 0.001f;				

 	dTime = currentTime - frameTime;
	frameTime = currentTime;

    ++framesPerSecond;

    if( currentTime - previousTime > 1.0f )
    {
		// Here we set the lastTime to the currentTime
	    previousTime = currentTime;
		
		// Reset the frames per second
        framesPerSecond = 0;
    }
}


/*
 * This is the class constructor
 */
CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		// Init a vVector to 0 0 0 for our position
	CVector3 vView = CVector3(0.0, 1.0, 0.0);		// Init a starting view vVector  
	CVector3 vUp   = CVector3(0.0, 0.0, 1.0);		// Init a standard up vVector 

	vCameraPosition	= vZero;					// Init the position to zero
	vCameraView		= vView;					// Init the view to a std starting view
	vCameraUp	= vUp;						// Init the UpVector
	vCameraRight = CVector3(1.0, 0.0, 0.0);
	
	savedCameras = 0;
	
	control = false;
	kSpeed = 5.0;
	timeBased = true;

	objectsAnimationOn = true;
	mouseControlOn = false;
}	


/*
 * This function sets the camera's position and view and up vVector.
 */
void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vNewPosition	= CVector3(positionX, positionY, positionZ);
	CVector3 vNewView		= CVector3(viewX, viewY, viewZ);
	CVector3 vNewUpVector	= CVector3(upVectorX, upVectorY, upVectorZ);

	vCameraPosition = vNewPosition;	
	vCameraView     = vNewView;		
	vCameraUp = vNewUpVector;	

	UpdateRightVector();
}

/*
 * This updates the camera's view and strafe vector
 */
void CCamera::Update() 
{
	if (control)
	{
		// Mouse movement?
		if (mouseControlOn)
			MouseInput();

		// Keyboard movement?
		KeyboardInput();
	}

	// For time based camera
	CalculateFrameRate();
}


/*
 * This allows us to look around using the mouse
 */
void CCamera::MouseInput()
{
	float degreeSensitivity = 45;
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = SCREEN_WIDTH  >> 1;				// This is a binary shift to get half the width
	int middleY = SCREEN_HEIGHT >> 1;				// This is a binary shift to get half the height
	float angleX = 0.0f;							// This is the direction for looking up or down
	float angleY = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
	
	GetCursorPos(&mousePos);						
	
	// Never moved, just return
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) 
		return;

	// Set mouse position to the middle of the window
	SetCursorPos(middleX, middleY);							

	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleX = (float)((middleX - mousePos.x)) / 1000.0f * degreeSensitivity;		
	angleY = (float)((middleY - mousePos.y)) / 1000.0f * degreeSensitivity;		

	RotateH(angleX);
	RotateV(angleY);
}


/*
 * This function handles the input faster than in the WinProc()
 */
void CCamera::KeyboardInput()
{
	float speed;
	
	if (timeBased)
		speed = kSpeed * dTime;
	else
		speed = kSpeed;

	if (GetKeyState('W') & 0x80) 
	{				
		ForwardBackward(speed);				
	}

	if (GetKeyState('S') & 0x80) 
	{			
		ForwardBackward(-speed);				
	}

	if ((GetKeyState('A') & 0x80) || (GetKeyState(VK_LEFT) & 0x80))
	{			
		LeftRight(-speed);
	}

	if ((GetKeyState('D') & 0x80) || (GetKeyState(VK_RIGHT) & 0x80))
	{			
		LeftRight(speed);
	}

	if (GetKeyState(VK_UP) & 0x80) 
	{			
		UpDown(speed);
	}

	if (GetKeyState(VK_DOWN) & 0x80) 
	{			
		UpDown(-speed);
	}
	
	if (GetKeyState('Q') & 0x80) 
	{
		Panning(-5.0f);
	}

	if (GetKeyState('E') & 0x80) 
	{
		Panning(5.0f);
	}

	if (GetKeyState('O') & 0x80) 
	{
		RotateH(5.0);
	}

	if (GetKeyState('P') & 0x80) 
	{
		RotateH(-5.0);
	}

	if (GetKeyState('I') & 0x80) 
	{
		RotateV(10.0);
	}

	if (GetKeyState('K') & 0x80) 
	{
		RotateV(-10.0);
	}

	if (GetKeyState('Z') & 0x80) 
	{				
		Zoom(speed);				
	}

	if (GetKeyState('X') & 0x80) 
	{			
		Zoom(-speed);				
	}

	if (GetKeyState('R') & 0x80) 
	{			
		Reset();				
	}

	if (GetKeyState('T') & 0x80) 
	{
		sprintf(cameraDebug,"pos:<%f, %f, %f>  view:<%f, %f, %f>  up:<%f, %f, %f>", 
			vCameraPosition.x, vCameraPosition.y, vCameraPosition.z, 
			vCameraView.x, vCameraView.y, vCameraView.z, 
			vCameraUp.x, vCameraUp.y, vCameraUp.z);
		SetWindowText(g_hWnd, cameraDebug);
	}

	if (GetKeyState('B') & 0x80) 
	{
		if (savedCameras < 10)
		{
			vSavedPositions[savedCameras] = CVector3(vCameraPosition.x, vCameraPosition.y, vCameraPosition.z);
			vSavedViews[savedCameras] = CVector3(vCameraView.x, vCameraView.y, vCameraView.z);
			vSavedUps[savedCameras] = CVector3(vCameraUp.x, vCameraUp.y, vCameraUp.z);
			savedCameras++;

			MessageBox(NULL, "Camera is saved", "Confirm", MB_OK | MB_ICONQUESTION);
		}
	}

	if (GetKeyState('1') & 0x80) 
	{
		if (savedCameras >= 1)
			PositionCamera(vSavedPositions[0].x, vSavedPositions[0].y, vSavedPositions[0].z,
				vSavedViews[0].x, vSavedViews[0].y, vSavedViews[0].z,
				vSavedUps[0].x, vSavedUps[0].y, vSavedUps[0].z);
	}

	if (GetKeyState('2') & 0x80) 
	{
		if (savedCameras >= 2)
			PositionCamera(vSavedPositions[1].x, vSavedPositions[1].y, vSavedPositions[1].z,
				vSavedViews[1].x, vSavedViews[1].y, vSavedViews[1].z,
				vSavedUps[1].x, vSavedUps[1].y, vSavedUps[1].z);
	}

	if (GetKeyState('3') & 0x80)
	{
		if (savedCameras >= 3)
			PositionCamera(vSavedPositions[2].x, vSavedPositions[2].y, vSavedPositions[2].z,
				vSavedViews[2].x, vSavedViews[2].y, vSavedViews[2].z,
				vSavedUps[2].x, vSavedUps[2].y, vSavedUps[2].z);
	}

	if (GetKeyState('4') & 0x80) 
	{
		if (savedCameras >= 4)
			PositionCamera(vSavedPositions[3].x, vSavedPositions[3].y, vSavedPositions[3].z,
				vSavedViews[3].x, vSavedViews[3].y, vSavedViews[3].z,
				vSavedUps[3].x, vSavedUps[3].y, vSavedUps[3].z);
	}

	if (GetKeyState('5') & 0x80) 
	{
		if (savedCameras >= 5)
			PositionCamera(vSavedPositions[4].x, vSavedPositions[4].y, vSavedPositions[4].z,
				vSavedViews[4].x, vSavedViews[4].y, vSavedViews[4].z,
				vSavedUps[4].x, vSavedUps[4].y, vSavedUps[4].z);
	}

	if (GetKeyState('6') & 0x80) 
	{
		if (savedCameras >= 6)
			PositionCamera(vSavedPositions[5].x, vSavedPositions[5].y, vSavedPositions[5].z,
				vSavedViews[5].x, vSavedViews[5].y, vSavedViews[5].z,
				vSavedUps[5].x, vSavedUps[5].y, vSavedUps[5].z);
	}

	if (GetKeyState('7') & 0x80) 
	{
		if (savedCameras >= 7)
			PositionCamera(vSavedPositions[6].x, vSavedPositions[6].y, vSavedPositions[6].z,
				vSavedViews[6].x, vSavedViews[6].y, vSavedViews[6].z,
				vSavedUps[6].x, vSavedUps[6].y, vSavedUps[6].z);
	}

	if (GetKeyState('8') & 0x80) 
	{
		if (savedCameras >= 8)
			PositionCamera(vSavedPositions[7].x, vSavedPositions[7].y, vSavedPositions[7].z,
				vSavedViews[7].x, vSavedViews[7].y, vSavedViews[7].z,
				vSavedUps[7].x, vSavedUps[7].y, vSavedUps[7].z);
	}

	if (GetKeyState('9') & 0x80) 
	{
		if (savedCameras >= 9)
			PositionCamera(vSavedPositions[8].x, vSavedPositions[8].y, vSavedPositions[8].z,
				vSavedViews[8].x, vSavedViews[8].y, vSavedViews[8].z,
				vSavedUps[8].x, vSavedUps[8].y, vSavedUps[8].z);
	}

	if (GetKeyState('0') & 0x80) 
	{
		if (savedCameras >= 10)
			PositionCamera(vSavedPositions[9].x, vSavedPositions[9].y, vSavedPositions[9].z,
				vSavedViews[9].x, vSavedViews[9].y, vSavedViews[9].z,
				vSavedUps[9].x, vSavedUps[9].y, vSavedUps[9].z);
	}

	if (GetKeyState('H') & 0x80) 
	{
		objectsAnimationOn = !objectsAnimationOn;
		MessageBox(NULL, "Objects animation is switched", "Confirm", MB_OK | MB_ICONQUESTION);
	}

	if (GetKeyState('M') & 0x80) 
	{
		mouseControlOn = !mouseControlOn;
		MessageBox(NULL, "Mouse control is switched", "Confirm", MB_OK | MB_ICONQUESTION);
	}

}

/*
 * This updates the camera according to the 
 */
void CCamera::Look()
{
	gluLookAt(vCameraPosition.x, vCameraPosition.y, vCameraPosition.z,	
			  vCameraView.x,	 vCameraView.y,     vCameraView.z,	
			  vCameraUp.x, vCameraUp.y, vCameraUp.z);
}


void CCamera::UpdateRightVector()
{
	vCameraRight = Normalize(Cross(vCameraView - vCameraPosition, vCameraUp));
}

void CCamera::Zoom(float speed)
{
	CVector3 vVector = vCameraView - vCameraPosition;
	vVector = Normalize(vVector);
	
	vCameraPosition.x += vVector.x * speed;		
	vCameraPosition.y += vVector.y * speed;		
	vCameraPosition.z += vVector.z * speed;		
	vCameraView.x += vVector.x * speed;		
	vCameraView.y += vVector.y * speed;		
	vCameraView.z += vVector.z * speed;	
}

void CCamera::ForwardBackward(float speed)
{
	CVector3 vVector = vCameraView - vCameraPosition;

	vVector = Normalize(vVector);
	
	vCameraPosition.x += vVector.x * speed;	
	vCameraPosition.y += vVector.y * speed;	
	vCameraView.x += vVector.x * speed;		
	vCameraView.y += vVector.y * speed;	
}

void CCamera::LeftRight(float speed)
{
	// Add the right vector to position
	vCameraPosition.x += vCameraRight.x * speed;
	vCameraPosition.y += vCameraRight.y * speed;

	// Add the right vector to view
	vCameraView.x += vCameraRight.x * speed;
	vCameraView.y += vCameraRight.y * speed;
}

void CCamera::UpDown(float speed)
{
	// Add the up vector to position
	vCameraPosition.x += vCameraUp.x * speed;
	vCameraPosition.y += vCameraUp.y * speed;
	vCameraPosition.z += vCameraUp.z * speed;

	// Add the up vector to view
	vCameraView.x += vCameraUp.x * speed;
	vCameraView.y += vCameraUp.y * speed;
	vCameraView.z += vCameraUp.z * speed;
}

void CCamera::RotateH(float angle)
{
	float x = vCameraUp.x;
	float y = vCameraUp.y;
	float z = vCameraUp.z;

	CVector3 vView = vCameraView - vCameraPosition;		
	CVector3 vNewView = CalculateRotation(angle, x, y, z, vView);

	vCameraView = vCameraPosition + vNewView;

	UpdateRightVector();
}

void CCamera::RotateV(float angle)
{
	float x = vCameraRight.x;
	float y = vCameraRight.y;
	float z = vCameraRight.z;

	CVector3 vView = vCameraView - vCameraPosition;		
	CVector3 vNewView = CalculateRotation(angle, x, y, z, vView);

	vCameraView = vCameraPosition + vNewView;

	vCameraUp = Normalize(Cross(vCameraView - vCameraPosition, vCameraRight)*-1);
}


/*
 * This pans the camera view
 */
void CCamera::Panning(float angle)
{
	CVector3 vVector = vCameraView - vCameraPosition;

	float x = vVector.x;
	float y = vVector.y;
	float z = vVector.z;

	CVector3 vNewUp;
	CVector3 vUp = vCameraUp;

	vNewUp = CalculateRotation(angle, x, y, z, vUp);

	vCameraUp = Normalize(vNewUp);
	UpdateRightVector();
}

void CCamera::Reset()
{
	PositionCamera(	vCameraPosition.x, vCameraPosition.y, vCameraPosition.z,
					vCameraPosition.x+1, vCameraPosition.y, vCameraPosition.z,
					0, 0, 1);
}

CVector3 CCamera::CalculateRotation(float angle, float x, float y, float z, CVector3 vRot)
{
	CVector3 vResult;

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle*PIdiv180);
	float sinTheta = (float)sin(angle*PIdiv180);

	// Find the new x position for the new rotated point
	vResult.x  = (cosTheta + (1 - cosTheta) * x * x)		* vRot.x;
	vResult.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vRot.y;
	vResult.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vRot.z;

	// Find the new y position for the new rotated point
	vResult.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vRot.x;
	vResult.y += (cosTheta + (1 - cosTheta) * y * y)		* vRot.y;
	vResult.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vRot.z;

	// Find the new z position for the new rotated point
	vResult.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vRot.x;
	vResult.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vRot.y;
	vResult.z += (cosTheta + (1 - cosTheta) * z * z)		* vRot.z;

	return vResult;
}

void CCamera::TurnControlOn()
{
	if (!control)
		control = true;
}

void CCamera::TurnControlOff()
{
	if (control)
		control = false;
}