#include "Main.h"
#include "Camera.h"

float dTime = 0.0f;

char cameraDebug[50];

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;										 
}


float Magnitude(CVector3 vNormal)
{
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}


CVector3 Normalize(CVector3 vVector)
{
	float magnitude = Magnitude(vVector);				
	vVector = vVector / magnitude;		
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
	    previousTime = currentTime;
		
        framesPerSecond = 0;
    }
}


CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		
	CVector3 vView = CVector3(0.0, 1.0, 0.0);		
	CVector3 vUp   = CVector3(0.0, 0.0, 1.0);		

	vCameraPosition	= vZero;					
	vCameraView		= vView;				
	vCameraUp	= vUp;			
	vCameraRight = CVector3(1.0, 0.0, 0.0);
	
	savedCameras = 0;
	
	control = false;
	kSpeed = 5.0;
	timeBased = true;

	objectsAnimationOn = true;
	mouseControlOn = false;
}	


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

void CCamera::Update() 
{
	if (control)
	{
		if (mouseControlOn)
			MouseInput();

		KeyboardInput();
	}

	CalculateFrameRate();
}


void CCamera::MouseInput()
{
	float degreeSensitivity = 45;
	POINT mousePos;						
	int middleX = SCREEN_WIDTH  >> 1;			
	int middleY = SCREEN_HEIGHT >> 1;			
	float angleX = 0.0f;	
	float angleY = 0.0f;						
	static float currentRotX = 0.0f;
	
	GetCursorPos(&mousePos);						
	
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) 
		return;

	SetCursorPos(middleX, middleY);							

	angleX = (float)((middleX - mousePos.x)) / 1000.0f * degreeSensitivity;		
	angleY = (float)((middleY - mousePos.y)) / 1000.0f * degreeSensitivity;		

	RotateH(angleX);
	RotateV(angleY);
}


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
		RotateH(3.0);
	}

	if (GetKeyState('E') & 0x80) 
	{
		RotateH(-3.0);
	}

	if (GetKeyState('R') & 0x80) 
	{
		RotateV(3.0);
	}

	if (GetKeyState('F') & 0x80) 
	{
		RotateV(-3.0);
	}

	if (GetKeyState('Z') & 0x80) 
	{				
		Zoom(speed);				
	}

	if (GetKeyState('X') & 0x80) 
	{			
		Zoom(-speed);				
	}

	if (GetKeyState('P') & 0x80) 
	{			
		Reset();				
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
	vCameraPosition.x += vCameraRight.x * speed;
	vCameraPosition.y += vCameraRight.y * speed;

	vCameraView.x += vCameraRight.x * speed;
	vCameraView.y += vCameraRight.y * speed;
}

void CCamera::UpDown(float speed)
{
	vCameraPosition.x += vCameraUp.x * speed;
	vCameraPosition.y += vCameraUp.y * speed;
	vCameraPosition.z += vCameraUp.z * speed;

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

	float cosTheta = (float)cos(angle*PIdiv180);
	float sinTheta = (float)sin(angle*PIdiv180);

	vResult.x  = (cosTheta + (1 - cosTheta) * x * x)		* vRot.x;
	vResult.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vRot.y;
	vResult.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vRot.z;

	vResult.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vRot.x;
	vResult.y += (cosTheta + (1 - cosTheta) * y * y)		* vRot.y;
	vResult.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vRot.z;

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