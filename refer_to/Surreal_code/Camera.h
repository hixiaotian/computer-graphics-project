#ifndef _CAMERA_H
#define _CAMERA_H

#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)


// This is our camera class
class CCamera 
{
	public:

		CCamera();	

		void PositionCamera(float positionX, float positionY, float positionZ,
			 				float viewX,     float viewY,     float viewZ,
							float upVectorX, float upVectorY, float upVectorZ);
		void Look();
		void Update();

		void KeyboardInput();
		void MouseInput(); 

		void UpdateRightVector();
	
		void Zoom(float speed);
		void ForwardBackward(float speed);
		void LeftRight(float speed);
		void UpDown(float speed);

		void RotateV(float angle);
		void RotateH(float angle);
		void Panning(float angle);

		CVector3 CalculateRotation(float angle, float x, float y, float z, CVector3 vRot);

		void Reset();

		void TurnControlOn();
		void TurnControlOff();

		CVector3 Position() 
		{	
			return vCameraPosition;		
		}

		CVector3 View()		
		{	
			return vCameraView;			
		}

		CVector3 UpVector() 
		{	
			return vCameraUp;		
		}

		CVector3 Right()	
		{	
			return vCameraRight;			
		}

		bool objectsAnimationOn;

	private:

		CVector3 vCameraPosition;					
		CVector3 vCameraView;						
		CVector3 vCameraUp;		
		CVector3 vCameraRight;

		CVector3 vSavedPositions[10];
		CVector3 vSavedViews[10];
		CVector3 vSavedUps[10];
		int savedCameras;

		bool control;
		bool mouseControlOn;

		bool timeBased;
		float kSpeed;	
};


#endif
