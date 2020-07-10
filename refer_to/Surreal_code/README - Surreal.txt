**************************************************************************
||									||	                                                                        
||	Authors:	Dixon Koesdjojo <dtk2001@columbia.edu>		||
||			Makiko Yasui <my277@columbia.edu>		||
||									||			
||	Project:	Surreal						||	
||									||					
||	Date:		5/28/02						||			
||									||
||	Description:	CS4160 Computer Graphics, Columbia University	||
||			Spring 2003, Assignment 3			||
||			Surreal - OpenGL Scene Animation		||
||									||				
**************************************************************************

Files:  	Main.cpp   
		Init.cpp   
		Camera.cpp 
		Camera.h   
		Main.h     
		Surreal.dsp  		
		Surreal.dsw  
		Textures files

Libraries:      opengl32.lib and glu32.lib gluax.lib winmm.lib glut.lib

Controls:	W - Move the camera forward
		S - Move the camera backward
		A - Move the camera left
		D - Move the camera right
		UP_ARROW - Move the camera up
		DOWN_ARROW - Move the camera down
		LEFT_ARROW - Move the camera left
		RIGHT_ARROW - Move the camera right
		Q - Pan camera left
		E - Pan camera right
		Z - Zoom in
		X - Zoom out
		O - Rotate Left
		P - Rotate Right
		I - Rotate Up
		K - Rotate Down
		R - Reset camera
		T - Get camera vectors info
		B - Save camera position
		1-0 - Switch to saved camera position
		H - Turn objects animation on/off
		M - Turn mouse control on/off
		Mouse - Rotate camera (if turned on)

Surreal features:

1. Scenes definition
- A museum with 3 rooms seperated by walls
- An outdoor scene surrounding the museum
- A secret surreal scene 

2. Ground plane
- Museum floor
- Hills and plane
- Street path

3. Perspective projection is used in all three scenes

4. Double buffering is used for all rendering

5. Depth buffering is used to perform hidden surfaces

6. Lighting and Shading
- Lighting, we have ambient light for all three scenes and a spotlight for the
secret surreal scene.
- Shading, we used all five types of materials defined in OpenGL for shading.

7. Texture Mapping
We used a lot of texture mapping for the objects, we applied the different 
types of texturing (GL_REPLACE, GL_MODULATE, GL_BLEND) and also applied them
to 3D shapes, i.e. spheres and nurbs.

8. Mouse-Based Camera motion
We implemented all posibilities of camera movement:
- Translating (Up-Down-Left-Right-Forward-Backward)
- Rotating (Horizontal-Vertical-Panning), not dependant on the main xyz axis.
- Zooming (In-Out)
- Mouse-based control for rotation
- Camera reset
- Camera information saving

9. Procedural Wireframe Model
We built basic shapes wireframe model, including: boxes, cylinders, planes, 
rotating steps, from scratch without using the built-in primitives object.
And computed all the normals for each planes of the objects created.

10. Object Animations
- Girl animation: Walking, moving her arms, moving her legs, moving her head.
- Bouncing ball
- Collision detection bouncing balls
- Shadow clock
- Fountain
- Atoms orbit

11. Scene Animation
The scene animation was performed by positioning the camera based on time. 
And having four acting sequences for the girl.

12. Display Lists
We used many display lists to optimize the overall performance.

13. Reflections
We implemented two reflective planes, both reflect animated objects.

14. Shadows
We implemented a shadow clock.

15. Multipass rendering
We used stencil buffer to do multipass rendering for reflections and shadows

16. Animated Textures
We implemented a slideshows.

17. Curved Surfaces
We implemented some NURBS surfaces, including the outdoor hills, the glass
windows, and the roof tent.

18. Culling
We implemented occlusion culling. We divided the scenes into smaller areas 
and determined which objects to be rendered accordingly. And also, we computed
whether or not certain objects are in the camera's view frustum.

19. Particle Effects
We created a fountain with random flowing water particles. The particle system
is configurable for different effects, i.e. we could have rain simulation.

20. Collision Detection
We created an aquarium with three moving balls with different direction vectors,
when the ball hits the wall or another ball, it will rebound accoring to the
physical properties.

