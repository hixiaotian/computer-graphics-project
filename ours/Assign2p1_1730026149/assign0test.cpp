

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
// GLM is a C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) 
// specification.
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/FreeImage.h>
#include <iomanip>


int mouseoldx, mouseoldy ; // For mouse motion
int windowWidth = 500, windowHeight = 500; //Width/Height of OpenGL window
GLdouble eyeloc = 2.0 ; // Where to look from; initially 0 -2, 2
GLfloat teapotloc = -0.5 ; // ** NEW ** where the teapot is located
GLfloat rotamount = 0.0; // ** NEW ** amount to rotate teapot by
GLint animate = 0 ; // ** NEW ** whether to animate or not
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
GLuint projectionPos, modelviewPos, colorPos; // Locations of uniform variables
// Uniform variables are used to communicate with your vertex or fragment shader from "outside". 
glm::mat4 projection, modelview; // The mvp matrices themselves
glm::mat4 identity(1.0f); // An identity matrix used for making transformation matrices

// Robot data.
int shoulder = 0, elbow = 0;
int movDeg = 1;
int slowDown[2] = { 0,20 };
float botLoc = -0.5;
float arm1[3] = { 0.3, 0.3, 1.2 };
float leg1[3] = { 0.4, 0.4, 1.5 };
float armJoint[3] = { 0.3, 0.3, 0.3 };
float legJoint[3] = { 0.4, 0.4, 0.4 };
float bodyHt = 2.0;
float elbowDirec = -1.0;

GLubyte woodtexture[256][256][3] ; // ** NEW ** texture (from grsites.com)
GLuint texNames[1] ; // ** NEW ** texture buffer
GLuint istex ;  // ** NEW ** blend parameter for texturing
GLuint islight ; // ** NEW ** for lighting
GLint texturing = 1 ; // ** NEW ** to turn on/off texturing
GLint lighting = 1 ; // ** NEW ** to turn on/off lighting

/* Variables to set uniform params for lighting fragment shader */
GLuint light0dirn ; 
GLuint light0color ; 
GLuint light1posn ; 
GLuint light1color ; 
GLuint light2posn;
GLuint light2color;
GLuint ambient ; 
GLuint diffuse ; 
GLuint specular ; 
GLuint shininess ; 

#include "shaders.h"
#include "geometry3.h"
 

/* New helper transformation function to transform vector by modelview */ 
void transformvec (const GLfloat input[4], GLfloat output[4]) {
	glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
	glm::vec4 outputvec = modelview * inputvec;
	output[0] = outputvec[0];
	output[1] = outputvec[1];
	output[2] = outputvec[2];
	output[3] = outputvec[3];

}

// Treat this as a destructor function. Delete any dynamically allocated memory here
void deleteBuffers() {
	glDeleteVertexArrays(numobjects + ncolors, VAOs);
	glDeleteVertexArrays(1, &teapotVAO);
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteVertexArrays(1, &cylinderVAO);
	glDeleteBuffers(numperobj*numobjects + ncolors, buffers);
	glDeleteBuffers(3, spherebuffers);
	glDeleteBuffers(3, cylinderbuffers);
}

void display(void)
{
	// clear all pixels  

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw white polygon (square) of unit length centered at the origin
	// Note that vertices must generally go counterclockwise
	// Change from the first program, in that I just made it white.
	// The old OpenGL code of using glBegin... glEnd no longer appears. 
	// The new version uses vertex buffer objects from init.   

	glUniform1i(islight, 0); // Turn off lighting (except on teapot, later)
	glUniform1i(istex, texturing);

	// Draw the floor
	// Start with no modifications made to the model matrix
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	glUniform3f(colorPos, 1.0f, 1.0f, 1.0f); // The floor is white
	drawtexture(FLOOR, texNames[0]); // Texturing floor 
	glUniform1i(istex, 0); // Other items aren't textured 

	// Now draw several cubes with different transforms, colors
	// We now maintain a stack for the modelview matrices. Changes made to the stack after pushing
	// are discarded once it is popped.

	/* New for Demo; add lighting effects */
	{

		const GLfloat light_specular[] = { 1, 0.5, 0, 1 };  // red
		const GLfloat light_specular1[] = { 0, 0.5, 1, 1 }; // blue
		const GLfloat light_direction[] = { 0.5, 0, 0, 0 }; // Dir light 0 in w 
		const GLfloat light_position1[] = { 0, -0.5, 0, 1 };

		GLfloat light0[4], light1[4];

		// Set Light and Material properties for the teapot
		// Lights are transformed by current modelview matrix. 
		// The shader can't do this globally. 
		// So we need to do so manually.  
		transformvec(light_direction, light0);
		transformvec(light_position1, light1);

		glUniform3fv(light0dirn, 1, light0);
		glUniform4fv(light0color, 1, light_specular);
		glUniform4fv(light1posn, 1, light1);
		glUniform4fv(light1color, 1, light_specular1);
		// glUniform4fv(light1color, 1, zero) ; 
	}

	pushMatrix(modelview);// Push basic modelview. Now is the 1st modelview.

	modelview = modelview * glm::translate(identity, glm::vec3(botLoc, 0.0, 0.1));// 1st modelview, move.
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	{
		const GLfloat one[] = { 1, 1, 1, 1 }; //rgba
		const GLfloat medium[] = { 0.5, 0.5, 0.5, 1 }; //rgba
		const GLfloat small[] = { 0.2f, 0.2f, 0.2f, 1 }; //rgba
		const GLfloat high[] = { 100 };
		const GLfloat zero[] = { 0.0, 0.0, 0.0, 1.0 }; //rgba
		const GLfloat light_specular2[] = { 0, 1, 0, 1 }; // blue
		const GLfloat light_position2[] = { 0.2, -0.2, 0.5, 1 };

		GLfloat light2[4];

		// Set Light and Material properties for the teapot
		// Lights are transformed by current modelview matrix. 
		// The shader can't do this globally. 
		// So we need to do so manually.  
		transformvec(light_position2, light2);
		glUniform4fv(light2posn, 1, light2);

		glUniform4fv(light2posn, 1, light2);
		glUniform4fv(light2color, 1, light_specular2);

		glUniform4fv(ambient, 1, small);
		glUniform4fv(diffuse, 1, medium);
		glUniform4fv(specular, 1, one);
		glUniform1fv(shininess, 1, high);

		glUniform1i(islight, lighting); // turn on lighting only for teapot. 
	}

	//================================================
	///* robot leg
	{

		/* Now draw the right first leg */
		pushMatrix(modelview);// Push 1st modelview. Now is the 2nd modelview.
		glUniform3fv(colorPos, 1, _cubecol[1]); // green
		// 2nd modelview, move and rotate.
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, - wd * leg1[1], 0.7 * 0.3)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)shoulder, glm::vec3(0.0, -1.0, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -leg1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);// Push 2nd modelview. Now is the 3rd modelview.
		modelview = modelview * glm::scale(identity, glm::vec3(leg1[0], leg1[1], leg1[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);// Pop out 2nd modelview. Now is the 2nd modelview.

		// Draw a sphere as an elbow.
		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(legJoint[0], legJoint[1], legJoint[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		/* Now draw the right second leg */
		glUniform3fv(colorPos, 1, _cubecol[2]); // blue
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.0)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)elbow, glm::vec3(0.0, elbowDirec, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -leg1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		// Draw a sphere as an shoe.
		pushMatrix(modelview);
		modelview = modelview * glm::translate(identity, glm::vec3(0.05, 0.0, 0.0));
		modelview = modelview * glm::scale(identity, glm::vec3(0.6, 0.5, 0.2));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(leg1[0], leg1[1], leg1[2]));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);

		popMatrix(modelview);

		/* Now draw the left first leg */
		pushMatrix(modelview);// Push 1st modelview. Now is the 2nd modelview.
		glUniform3fv(colorPos, 1, _cubecol[1]); // green
		// 2nd modelview, move and rotate.
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, + wd * leg1[1], 0.7 * 0.3)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)shoulder, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -leg1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);// Push 2nd modelview. Now is the 3rd modelview.
		modelview = modelview * glm::scale(identity, glm::vec3(leg1[0], leg1[1], leg1[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);// Pop out 2nd modelview. Now is the 2nd modelview.

		// Draw a sphere as an elbow.
		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(legJoint[0], legJoint[1], legJoint[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		/* Now draw the left second leg */
		glUniform3fv(colorPos, 1, _cubecol[2]); // blue
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.0)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)elbow, glm::vec3(0.0, elbowDirec, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -leg1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		// Draw a sphere as an shoe.
		pushMatrix(modelview);
		modelview = modelview * glm::translate(identity, glm::vec3(0.05, 0.0, 0.0));
		modelview = modelview * glm::scale(identity, glm::vec3(0.6, 0.5, 0.2));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(leg1[0], leg1[1], leg1[2]));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);

		popMatrix(modelview);
	}


	// Body
	modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.2));// 1st modelview, move.
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	glUniform3fv(colorPos, 1, _cubecol[0]); // red
	
	pushMatrix(modelview);// Push basic modelview. Now is the 1st modelview.
	modelview = modelview * glm::scale(identity, glm::vec3(1.0, 1.0, bodyHt));// 1st modelview, move.
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	popMatrix(modelview);// Push basic modelview. Now is the 1st modelview.

	drawcylinder();


	//================================================
	///* robot arm
	{
		/* Now draw the right first arm */
		pushMatrix(modelview);// Push 1st modelview. Now is the 2nd modelview.
		glUniform3fv(colorPos, 1, _cubecol[1]); // green
		// 2nd modelview, move and rotate.
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, - wd - wd * arm1[1], 0.7 * 0.3)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)shoulder, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -arm1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);// Push 2nd modelview. Now is the 3rd modelview.
		modelview = modelview * glm::scale(identity, glm::vec3(arm1[0], arm1[1], arm1[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);// Pop out 2nd modelview. Now is the 2nd modelview.

		// Draw a sphere as an elbow.
		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(armJoint[0], armJoint[1], armJoint[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		/* Now draw the right second arm */
		glUniform3fv(colorPos, 1, _cubecol[2]); // blue
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.0)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)elbow, glm::vec3(0.0, -elbowDirec, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -arm1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(arm1[0], arm1[1], arm1[2]));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);
		
		popMatrix(modelview);

		/* Now draw the left first arm */
		pushMatrix(modelview);// Push 1st modelview. Now is the 2nd modelview.
		glUniform3fv(colorPos, 1, _cubecol[1]); // green
		// 2nd modelview, move and rotate.
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, +wd + wd * arm1[1], 0.7 * 0.3)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)shoulder, glm::vec3(0.0, -1.0, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -arm1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);// Push 2nd modelview. Now is the 3rd modelview.
		modelview = modelview * glm::scale(identity, glm::vec3(arm1[0], arm1[1], arm1[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);// Pop out 2nd modelview. Now is the 2nd modelview.

		// Draw a sphere as an elbow.
		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(armJoint[0], armJoint[1], armJoint[2]));// 3rd modelview, scale.
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawsphere();
		popMatrix(modelview);

		/* Now draw the left second arm */
		glUniform3fv(colorPos, 1, _cubecol[2]); // blue
		modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.0)) * glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat)elbow, glm::vec3(0.0, -elbowDirec, 0.0)) * glm::translate(identity, glm::vec3(0.0, 0.0, -arm1[2] * ht));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

		pushMatrix(modelview);
		modelview = modelview * glm::scale(identity, glm::vec3(arm1[0], arm1[1], arm1[2]));
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		drawcylinder();
		popMatrix(modelview);

		popMatrix(modelview);
		//*/
		//===================================================
	}

	modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.3));
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	/* New for Demo; add lighting effects */
	drawsphere();

	popMatrix(modelview);

	// Draw a teapot
	//  ** NEW ** Put a teapot in the middle that animates 
	glUniform3f(colorPos, 0.0f, 1.0f, 1.0f);
	//  ** NEW ** Put a teapot in the middle that animates
	pushMatrix(modelview);
	modelview = modelview * glm::translate(identity, glm::vec3(teapotloc, 0.0, 0.0));

	//  The following two transforms set up and center the teapot 
	//  Remember that transforms right-multiply the modelview matrix (top of the stack)
	modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1));
	modelview = modelview * glm::rotate(glm::mat4(1.0f), rotamount * glm::pi<float>() / 180.0f, glm::vec3(0.0, 0.0, 1.0));
	modelview = modelview * glm::rotate(identity, glm::pi<float>() / 2.0f, glm::vec3(1.0, 0.0, 0.0));
	float size = 0.235f; // Teapot size
	modelview = modelview * glm::scale(identity, glm::vec3(size, size, size));
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	drawteapot();
	popMatrix(modelview);

	// 2nd pillar 
	pushMatrix(modelview);
	modelview = modelview * glm::translate(identity, glm::vec3(0.4, -0.4, 0.1));
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	glUniform3fv(colorPos, 1, _cubecol[1]);
	
	drawsphere();

	popMatrix(modelview);

	glutSwapBuffers();
	glFlush();
}

// ** NEW ** in this assignment, is an animation of a teapot 
// Hitting p will pause this animation; see keyboard callback

void animation(void) {
	teapotloc = teapotloc + 0.00025 ;
	rotamount = rotamount + 0.25;
	if (teapotloc > 0.5) teapotloc = -0.5 ;
	if (rotamount > 360.0) rotamount = 0.0;

	if (slowDown[0] <= slowDown[1]) {
		slowDown[0]++;
	} else {
		botLoc += 0.003;
		if (botLoc > 0.5)
			botLoc = -0.5;
		if (shoulder > 30 || shoulder < -30)
			movDeg = -movDeg;
		if (shoulder == 0) elbowDirec = -elbowDirec;
		shoulder = (shoulder + movDeg) % 360;
		elbow = (elbow + movDeg) % 360;
		slowDown[0] = 0;
	}

	glutPostRedisplay() ;  
}

void moveTeapot() {
	rotamount = 45.0;
	teapotloc = -0.05;
}

// Defines a Mouse callback to zoom in and out 
// This is done by modifying gluLookAt         
// The actual motion is in mousedrag           
// mouse simply sets state for mousedrag       
void mouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			// Do Nothing ;
		}
		else if (state == GLUT_DOWN) {
			mouseoldx = x ; mouseoldy = y ; // so we can move wrt x , y 
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{ // Reset gluLookAt
		eyeloc = 2.0 ;
		modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
		// Send the updated matrix to the shader
		glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
		glutPostRedisplay();
	}
}

void mousedrag(int x, int y) {
	int yloc = y - mouseoldy  ;    // We will use the y coord to zoom in/out
	eyeloc  += 0.005*yloc ;         // Where do we look from
	if (eyeloc < 0) eyeloc = 0.0 ;
	mouseoldy = y ;

	/* Set the eye location */
	modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
	// Send the updated matrix to the shader
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
	glutPostRedisplay();
}

void printHelp() {
	std::cout << "\nAvailable commands:\n"
		  << "press 'h' to print this message again.\n"
		  << "press Esc to quit.\n"
		  << "press 'o' to save a screenshot to \"./screenshot.png\".\n"
		  << "press 'i' to move teapot into position for Assign2 screenshot.\n"
		  << "press 'p' to start/stop teapot and robot animation.\n"
		  << "press 't' to turn texturing on/off.\n"
		  << "press 's' to turn shading on/off.\n";
}

void saveScreenshot() {
	int pix = windowWidth * windowHeight;
	BYTE *pixels = new BYTE[3*pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,windowWidth,windowHeight,GL_BGR,GL_UNSIGNED_BYTE,pixels);
	
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, windowWidth * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

	std::cout << "Saving screenshot: screenshot.png\n";

	FreeImage_Save(FIF_PNG, img, "screenshot.png", 0);
	delete pixels;
}

// Defines what to do when various keys are pressed 
void keyboard (unsigned char key, int x, int y) 
{
	switch (key) {
		case 'h':
			printHelp();
			break;
		case 'o':
			saveScreenshot();
			break;
		case 'i':
			moveTeapot();			
			eyeloc = 2.0f;
			// Immediately update the modelview matrix
			modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
			// Send the updated matrix to the shader
			glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
			texturing = 1;
			lighting = 1;
			animate = 0;
			glutIdleFunc(NULL);
			glutPostRedisplay();
			break;
		case 27:  // Escape to quit
			exit(0) ;
			break ;
		case 'p': // ** NEW ** to pause/restart animation
			animate = !animate ;
			if (animate) glutIdleFunc(animation) ;
			else glutIdleFunc(NULL) ;
			break ;
		case 't': // ** NEW ** to turn on/off texturing ; 
			texturing = !texturing ;
			glutPostRedisplay() ; 
			break ;
		case 's':
			lighting = !lighting;
			glutPostRedisplay();
		break; default:
			break ;
	}
}

/* Reshapes the window appropriately */
void reshape(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	// Think about the rationale for this choice for glm::perspective 
	// What would happen if you changed near and far planes? 
	projection = glm::perspective(30.0f / 180.0f * glm::pi<float>(), (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);
	glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
}

void checkOpenGLVersion() {
	const char *version_p = (const char *)glGetString(GL_VERSION);
	float version = 0.0f;

	if(version_p != NULL)
		version = atof(version_p);

	if(version < 3.1f) {
		std::cout << std::endl << "*****************************************" << std::endl;

		if(version_p != NULL) {
			std::cout << "WARNING: Your OpenGL version is not supported." << std::endl;
			std::cout << "We detected version " << std::fixed << std::setprecision(1) << version;
			std::cout << ", but at least version 3.1 is required." << std::endl << std::endl;
		} else {
			std::cout << "WARNING: Your OpenGL version could not be detected." << std::endl << std::endl;
		}

		std::cout << "Please update your graphics drivers BEFORE posting on the forum. If this" << std::endl
				  << "doesn't work, ensure your GPU supports OpenGL 3.1 or greater." << std::endl;

		std::cout << "If you receive a 0xC0000005: Access Violation error, this is likely the reason." << std::endl;

		std::cout << std::endl;

		std::cout << "Additional OpenGL Info:" << std::endl;
		std::cout << "(Please include with support requests)" << std::endl;
		std::cout << "GL_VERSION: ";
		std::cout << glGetString(GL_VERSION) << std::endl;
		std::cout << "GL_VENDOR: ";
		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << "GL_RENDERER: ";
		std::cout << glGetString(GL_RENDERER) << std::endl;

		std::cout << std::endl << "*****************************************" << std::endl;
		std::cout << std::endl << "Select terminal and press <ENTER> to continue." << std::endl;
		std::cin.get();
		std::cout << "Select OpenGL window to use commands below." << std::endl;
	}
}

void init (void) 
{
	//Warn students about OpenGL version before 0xC0000005 error
	checkOpenGLVersion();

	printHelp();

	/* select clearing color 	*/
	glClearColor (0.0, 0.0, 0.0, 0.0);

	/* initialize viewing values  */
	projection = glm::mat4(1.0f); // The identity matrix
	modelview = glm::lookAt(glm::vec3(0.0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
	 
	 
	// Initialize the shaders

	vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
	fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
	GLuint program = glCreateProgram() ;
	shaderprogram = initprogram(vertexshader, fragmentshader) ;
	GLint linked;
	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &linked) ;  

	// * NEW * Set up the shader parameter mappings properly for lighting.
	islight = glGetUniformLocation(shaderprogram,"islight") ;        
	//  glGetUniformLocation - return the location of a uniform variable
	light0dirn = glGetUniformLocation(shaderprogram,"light0dirn") ;       
	light0color = glGetUniformLocation(shaderprogram,"light0color") ;       
	light1posn = glGetUniformLocation(shaderprogram,"light1posn") ;       
	light1color = glGetUniformLocation(shaderprogram,"light1color") ;    
	light2posn = glGetUniformLocation(shaderprogram, "light2posn");
	light2color = glGetUniformLocation(shaderprogram, "light2color");
	ambient = glGetUniformLocation(shaderprogram,"ambient") ;       
	diffuse = glGetUniformLocation(shaderprogram,"diffuse") ;       
	specular = glGetUniformLocation(shaderprogram,"specular") ;       
	shininess = glGetUniformLocation(shaderprogram,"shininess") ;  

	// Get the positions of other uniform variables
	projectionPos = glGetUniformLocation(shaderprogram, "projection"); 	
	modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
	colorPos = glGetUniformLocation(shaderprogram, "color");

	// Now create the buffer objects to be used in the scene later
	glGenVertexArrays(numobjects + ncolors, VAOs);
	glGenVertexArrays(1, &teapotVAO);
	glGenVertexArrays(1, &sphereVAO);
	glGenVertexArrays(1, &cylinderVAO);
	glGenBuffers(numperobj * numobjects + ncolors + 1, buffers); // 1 extra buffer for the texcoords
	glGenBuffers(3, teapotbuffers);
	glGenBuffers(3, spherebuffers);
	glGenBuffers(3, cylinderbuffers);
	// Initialize texture
	inittexture("wood.ppm", shaderprogram) ; 

	// Initialize objects
	initobject(FLOOR, (GLfloat *)floorverts, sizeof(floorverts), (GLfloat *)floorcol, sizeof(floorcol), (GLubyte *)floorinds, sizeof(floorinds), GL_TRIANGLES);
	initcubes(CUBE, (GLfloat *)cubeverts, sizeof(cubeverts), (GLubyte *)cubeinds, sizeof(cubeinds), GL_TRIANGLES);
	loadteapot();
	initsphere();
	initcylinder();

	// Enable the depth test
	glEnable(GL_DEPTH_TEST) ;
	glDepthFunc (GL_LESS) ; // The default option
}


int main(int argc, char** argv)
{
	FreeImage_Initialise(); // FreeImage is an Open Source library project for developers who would 
	//like to support popular graphics image formats like PNG, BMP, JPEG, TIFF and others as needed 
	//by today's multimedia applications. 

	glutInit(&argc, argv); // GLUT (OpenGL Utility Toolkit, now freeglut) takes care of all the 
	//system-specific chores required for creating windows, initializing OpenGL contexts, and 
	//handling input events, to allow for trully portable OpenGL programs.

	// Requests the type of buffers (Single, RGB).
	// Think about what buffers you would need...

	// Request the depth if needed, later swith to double buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize (windowWidth, windowHeight); 
	glutCreateWindow ("Simple Demo with Shaders");

	GLenum err = glewInit() ; //The OpenGL Extension Wrangler Library (GLEW) is 
	//a cross-platform open-source C/C++ extension loading library. 
	//GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported 
	//on the target platform.
	if (GLEW_OK != err) { 
		std::cerr << "Error: " << glewGetString(err) << std::endl; 
	} 

	init(); // Always initialize first

	// Now, we define callbacks and functions for various tasks.
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape) ;
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse) ;
	glutMotionFunc(mousedrag) ;

	glutMainLoop(); // Start the main code
	FreeImage_DeInitialise();
	return 0;   /* ANSI C requires main to return int. */
}
