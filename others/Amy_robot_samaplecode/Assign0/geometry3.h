/**** Basic setup for defining and drawing objects ****/
// Moved to a header for the second OpenGL program

#ifndef __INCLUDEGEOMETRY
#define __INCLUDEGEOMETRY

#include <vector>

// May need to replace with absolute path on some systems
#define PATH_TO_TEAPOT_OBJ "teapot.obj"

const int numobjects = 2 ; // ** NEW ** number of objects for buffer 
const int numperobj  = 3 ;
const int ncolors = 4 ; 
GLuint VAOs[numobjects + ncolors], teapotVAO, sphereVAO, cubeVAO, cylinderVAO; // A VAO for each object
GLuint buffers[numperobj*numobjects+ncolors+1], teapotbuffers[3], spherebuffers[4], cubebuffers[3], cylinderbuffers[3]; // ** NEW ** List of buffers for geometric data 
GLuint objects[numobjects] ; // ** NEW ** For each object
GLenum PrimType[numobjects] ;
GLsizei NumElems[numobjects] ;

// For the geometry of the teapot
std::vector <glm::vec3> teapotVertices;
std::vector <glm::vec3> teapotNormals;
std::vector <unsigned int> teapotIndices;

// For the geometry of the sphere
std::vector <glm::vec3> sphereVertices;
std::vector <glm::vec3> sphereNormals;
std::vector <unsigned int> sphereIndices;
std::vector<float> spheretexCoords;

// For the geometry of the cylinder
std::vector <glm::vec3> cylinderVertices;
std::vector <glm::vec3> cylinderNormals;
std::vector <unsigned int> cylinderIndices;

// To be used as a matrix stack for the modelview.
std::vector <glm::mat4> modelviewStack;

// ** NEW ** Floor Geometry is specified with a vertex array
// ** NEW ** Same for other Geometry 

enum {Vertices, Colors, Elements} ; // For arrays for object 
enum {FLOOR, CUBE} ; // For objects, for the floor

const GLfloat floorverts[4][3] = {
	{ 0.5, 0.5, 0.0 },{ -0.5, 0.5, 0.0 },{ -0.5, -0.5, 0.0 },{ 0.5, -0.5, 0.0 }
};
const GLfloat floorcol[4][3] = {
	{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 }
};
const GLubyte floorinds[1][6] = { { 0, 1, 2, 0, 2, 3 } };
const GLfloat floortex[4][2] = {
	{ 1.0, 1.0 },{ 0.0, 1.0 },{ 0.0, 0.0 },{ 1.0, 0.0 }
};

// CUBES FOR ADDING TO THE SCENE

const GLfloat wd = 0.1 ; 
const GLfloat ht = 0.3 ; 
const GLfloat _cubecol[4][3] = { 
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0} } ;
const GLfloat cubeverts[8][3] = {
	{ -wd, -wd, 0.0 },{ -wd, wd, 0.0 },{ wd, wd, 0.0 },{ wd, -wd, 0.0 },
	{ -wd, -wd, ht },{ wd, -wd, ht },{ wd, wd, ht },{ -wd, wd, ht }
};
GLfloat cubecol[12][3];
const GLubyte cubeinds[12][3] = {
	{ 0, 1, 2 },{ 0, 2, 3 }, // BOTTOM 
	{ 4, 5, 6 },{ 4, 6, 7 }, // TOP 
	{ 0, 4, 7 },{ 0, 7, 1 }, // LEFT 
	{ 0, 3, 5 },{ 0, 5, 4 }, // FRONT
	{ 3, 2, 6 },{ 3, 6, 5 }, // RIGHT 
	{ 1, 7, 6 },{ 1, 6, 2 } // BACK
};

// Cube vertices
const GLfloat cubeVerts[24][3] = {
	// Front face
	{ -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },
	// Back face
	{ -0.5f, -0.5f, -0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
	// Left face
	{ -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ -0.5f, -0.5f, -0.5f },
	// Right face
	{ 0.5f, -0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
	// Top face
	{ 0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },
	// Bottom face
	{ 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f }
};

// Cube normals
const GLfloat cubeNorms[24][3] = {
	// Front face
	{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
	// Back face
	{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },
	// Left face
	{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },
	// Right face
	{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
	// Top face
	{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
	// Bottom face
	{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f }
};
// Cube indices
const GLuint cubeIndices[36] = {
	0, 1, 2, 0, 2, 3, // Front face
	4, 5, 6, 4, 6, 7, // Back face
	8, 9, 10, 8, 10, 11, // Left face
	12, 13, 14, 12, 14, 15, // Right face
	16, 17, 18, 16, 18, 19, // Top face
	20, 21, 22, 20, 22, 23 // Bottom face
};


const GLfloat radius = 0.1;
const int sectors = 36;
const int stacks = 18;
const GLfloat height = 0.3;
bool smooth;

void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) ;
void drawobject(GLuint object) ;
void initcubes(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type);
void drawcolor(GLuint object, GLuint color) ;
void inittexture (const char * filename, GLuint program) ;

void drawtexture(GLuint object, GLuint texture); 


void loadteapot();
void drawteapot();

void buildspheredata();
void initspheretexture(const char* filename, GLuint program); 
void initsphere();
void drawsphere();
void drawspheretexture(GLuint texture);

void initcylinder();
void drawcylinder();

void pushMatrix(glm::mat4);
void popMatrix(glm::mat4&);

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 

void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) {
	int offset = object * numperobj;
	glBindVertexArray(VAOs[object]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glBufferData(GL_ARRAY_BUFFER, sizevert, vert, GL_STATIC_DRAW);
	// Use layout location 0 for the vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors + offset]);
	glBufferData(GL_ARRAY_BUFFER, sizecol, col, GL_STATIC_DRAW);
	// Use layout location 1 for the colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds, GL_STATIC_DRAW);
	PrimType[object] = type;
	NumElems[object] = sizeind;
	// Prevent further modification of this VAO by unbinding it
	glBindVertexArray(0);
}

// Very basic code to read a ppm file
// And then set up buffers for texture coordinates
void inittexture (const char * filename, GLuint program) {
	// read the texture file and get texture data
	int width, height;
	int i, j, k;
	/*FILE * fp ;
	 assert(fp = fopen(filename,"rb")) ;
	 fscanf(fp,"%*s %*d %*d %*d%*c") ;
	 for (i = 0 ; i < 256 ; i++)
	   for (j = 0 ; j < 256 ; j++)
		 for (k = 0 ; k < 3 ; k++)
	   fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
	 fclose(fp) ;
	 */
	Image::Bmp bmp;
	if (!bmp.read(filename))
		return;     // exit if failed load image

	// get bmp info
	width = bmp.getWidth();
	height = bmp.getHeight();


	GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

	// We assume the image is 8-bit, 24-bit or 32-bit BMP
	GLenum format;
	int bpp = bmp.getBitCount();
	if (bpp == 8)
		format = GL_LUMINANCE;
	else if (bpp == 24)
		format = GL_RGB;
	else if (bpp == 32)
		format = GL_RGBA;
	else
		return;               // NOT supported, exit
		//spheretexture = bmp.getDataRGB();
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			for (k = 0; k < 3; k++)
				woodtexture[i][j][k] = bmp.getDataRGB()[(i * width + j) * 3 + k];
	

   // Set up Texture Coordinates
   glGenTextures(2, texNames) ; 
   glBindVertexArray(VAOs[FLOOR]);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[numobjects*numperobj+ncolors]) ; 
   glBufferData(GL_ARRAY_BUFFER, sizeof (floortex), floortex,GL_STATIC_DRAW);
   // Use layout location 2 for texcoords
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

   // Define a sampler.  See page 709 in red book, 7th ed.
   GLint texsampler;
   texsampler = glGetUniformLocation(program, "tex"); 

   glEnable(GL_TEXTURE_2D) ;
	glActiveTexture(GL_TEXTURE0) ; 

   glBindTexture (GL_TEXTURE_2D, texNames[0]) ; 
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                woodtexture) ;
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ; 
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ; 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);

   // build our texture mipmaps
   switch (bpp)
   {
   case 8:
	   gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, woodtexture);
	   break;
   case 24:
	   gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, woodtexture);
	   break;
   case 32:
	   gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, woodtexture);
	   break;
   }
   glBindVertexArray(0);
   glActiveTexture(GL_TEXTURE0); 
   glBindTexture(GL_TEXTURE_2D, 0);

   // Note that the value assigned to the texture sampler is n, where n is the active
   // texture number provided to glActiveTexture(). In this case, it's texture unit 0.
   glUniform1i(texsampler,0) ; 
   istex = glGetUniformLocation(program,"istex") ; 
}


void initspheretexture(const char* filename, GLuint program) {
	// read the texture file and get texture data
	int width, height;
	{
		int i, j, k;
		/*FILE * fp ;
		 assert(fp = fopen(filename,"rb")) ;
		 fscanf(fp,"%*s %*d %*d %*d%*c") ;
		 for (i = 0 ; i < 256 ; i++)
		   for (j = 0 ; j < 256 ; j++)
			 for (k = 0 ; k < 3 ; k++)
		   fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
		 fclose(fp) ;
		 */
		Image::Bmp bmp;
		if (!bmp.read(filename))
			return;     // exit if failed load image

		// get bmp info
		width = bmp.getWidth();
		height = bmp.getHeight();


		GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

		// We assume the image is 8-bit, 24-bit or 32-bit BMP
		GLenum format;
		int bpp = bmp.getBitCount();
		if (bpp == 8)
			format = GL_LUMINANCE;
		else if (bpp == 24)
			format = GL_RGB;
		else if (bpp == 32)
			format = GL_RGBA;
		else
			return;               // NOT supported, exit
			//spheretexture = bmp.getDataRGB();
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
				for (k = 0; k < 3; k++)
					spheretexture[i][j][k] = bmp.getDataRGB()[(i * width + j) * 3 + k];
	}
	

	// Set up Texture Coordinates
	//glGenTextures(1, spheretexNames);
	//glGenTextures(2, texNames);
	buildspheredata();
	// Done storing sphere data, now bind it
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereVertices.size(), &sphereVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereNormals.size(), &sphereNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spherebuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);


	glActiveTexture(GL_TEXTURE1);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texNames[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		spheretexture);	
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spheretexCoords), &spheretexCoords[0], GL_STATIC_DRAW);
	// Use layout location 2 for texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);




	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE1);	
	glBindTexture(GL_TEXTURE_2D, 0);
	// Define a sampler.  See page 709 in red book, 7th ed.
	GLint texsampler1;
	texsampler1 = glGetUniformLocation(program, "spheretex");
	// Note that the value assigned to the texture sampler is n, where n is the active
	// texture number provided to glActiveTexture(). In this case, it's texture unit 0.
	glUniform1i(texsampler1, 1);
	istex = glGetUniformLocation(program, "istex");

/*	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, spheretexNames[0]);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);


	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// copy texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		spheretexture);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// build our texture mipmaps
	switch (bpp)
	{
	case 8:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
		break;
	case 24:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
		break;
	case 32:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
		break;
	}
	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spheretexCoords), &spheretexCoords[0], GL_STATIC_DRAW);
	// Use layout location 2 for texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);






	// Define a sampler.  See page 709 in red book, 7th ed.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint texsampler;
	texsampler = glGetUniformLocation(program, "tex");
	// Note that the value assigned to the texture sampler is n, where n is the active
	// texture number provided to glActiveTexture(). In this case, it's texture unit 0.
	glUniform1i(texsampler, 0);
	istex = glGetUniformLocation(program, "istex");

	*/
}



// This function initializes a bunch of color cubes
void initcubes(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type) {
	for (int i = 0; i < ncolors; i++) {
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 3; k++)
				cubecol[j][k] = _cubecol[i][k];
		glBindVertexArray(VAOs[object + i]);
		int offset = object * numperobj;
		int base = numobjects * numperobj;
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
		glBufferData(GL_ARRAY_BUFFER, sizevert, vert, GL_STATIC_DRAW);
		// Use layout location 0 for the vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[base + i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubecol), cubecol, GL_STATIC_DRAW);
		// Use layout location 1 for the colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds, GL_STATIC_DRAW);
		PrimType[object] = type;
		NumElems[object] = sizeind;
		// Prevent further modification of this VAO by unbinding it
		glBindVertexArray(0);
	}
}



// Draws a solid cube
void solidCube(float size) {
	//glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview * glm::scale(mat4(1.0f), vec3(size, size, size)))[0][0]);
	glBindVertexArray(VAOs[CUBE]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 vertices to form 12 triangles for 1 cube
	glBindVertexArray(0);
}


// And a function to draw with them, similar to drawobject but with color
void drawcolor(GLuint object, GLuint color) {
	glBindVertexArray(VAOs[object + color]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}

// And a function to draw with textures, similar to drawobject
void drawtexture(GLuint object, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAOs[object]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Draws the teapot.
void drawteapot() {
	glBindVertexArray(teapotVAO);
	glDrawElements(GL_TRIANGLES, teapotIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// Draws the sphere.
void drawsphere() {
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// And a function to draw with textures, similar to drawobject
void drawspheretexture(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(sphereVAO);

	// interleaved array
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
	//glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
	//glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);

	glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Draws the cylinder.
void drawcylinder() {
	glBindVertexArray(cylinderVAO);
	glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawobject(GLuint object) {
	glBindVertexArray(VAOs[object]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}

void loadteapot() {
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(PATH_TO_TEAPOT_OBJ, "rb");

	if (fp == NULL) {
		std::cerr << "Error loading file: " << PATH_TO_TEAPOT_OBJ << std::endl;
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			teapotVertices.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			teapotNormals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if ((c1 == 'f'))
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			teapotIndices.push_back(fx - 1);
			teapotIndices.push_back(fy - 1);
			teapotIndices.push_back(fz - 1);
		}
	}

	fclose(fp); // Finished parsing
				// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.02f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < teapotVertices.size(); ++i) {
		glm::vec3 shiftedVertex = teapotVertices[i] - glm::vec3(0.0f, avgY, avgZ);
		teapotVertices[i] = shiftedVertex;
	}
	// Done loading teapot file, now bind it
	glBindVertexArray(teapotVAO);

	glBindBuffer(GL_ARRAY_BUFFER, teapotbuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * teapotVertices.size(), &teapotVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, teapotbuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * teapotNormals.size(), &teapotNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotbuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * teapotIndices.size(), &teapotIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void buildspheredata()
{
	const float PI = acos(-1);

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectors+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			sphereVertices.push_back(glm::vec3(x, y, z));

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			sphereNormals.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord between [0, 1]
			s = (float)j / sectors;
			t = (float)i / stacks;
			//addsphereTexCoord(s, t);
			spheretexCoords.push_back(s);
			spheretexCoords.push_back(t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				// k1---k2---k1+1
				sphereIndices.push_back(k1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k1+1);
			}

			if (i != (stacks - 1))
			{
				// k1+1---k2---k2+1
				sphereIndices.push_back(k1+1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k2 + 1);
			}

			/*
			// vertical lines for all stacks
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
			*/
		}
	}

	// generate interleaved vertex array as well
	//buildInterleavedVertices();
}


void buildcylinderdata()
{
	const float PI = acos(-1);

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz;   // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * PI / sectors;
	float stackStep = height / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{

		z = i * stackStep;              // r * sin(u)

		// add (sectors+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			if (i == 0)
			{
				x = 0.0;
				y = 0.0;
				z = (i + 1) * stackStep;
				nx = 0.0;
				ny = 0.0;
				nz = -1.0;
			}
			else if (i == stacks)
			{
				x = 0.0;
				y = 0.0;
				z = (i - 1) * stackStep;
				nx = 0.0;
				ny = 0.0;
				nz = 1.0;
			}
			else {
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position
				x = radius * cosf(sectorAngle);             // r * cos(v)
				y = radius * sinf(sectorAngle);             // r * sin(v)
				GLfloat mag = sqrt(x * x + y * y + z * z);
				mag = 1.0 / mag;
				// normalized vertex normal
				nx = x * mag;
				ny = y * mag;
				nz = 0;
			}

			cylinderVertices.push_back(glm::vec3(x, y, z));


			cylinderNormals.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord between [0, 1]
			//s = (float)j / sectors;
			//t = (float)i / stacks;
			//addTexCoord(s, t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				// k1---k2---k1+1
				cylinderIndices.push_back(k1);
				cylinderIndices.push_back(k2);
				cylinderIndices.push_back(k1 + 1);
			}

			if (i != (stacks - 1))
			{
				// k1+1---k2---k2+1
				cylinderIndices.push_back(k1 + 1);
				cylinderIndices.push_back(k2);
				cylinderIndices.push_back(k2 + 1);
			}

			/*
			// vertical lines for all stacks
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
			*/
		}
	}

	// generate interleaved vertex array as well
	//buildInterleavedVertices();
}

///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void addsphereTexCoord(float s, float t)
{
	spheretexCoords.push_back(s);
	spheretexCoords.push_back(t);
}


void initsphere() {
	int width = 2048, height = 1024;

	buildspheredata();
	// Done storing sphere data, now bind it
	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereVertices.size(), &sphereVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereNormals.size(), &sphereNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spherebuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);


	// Set up Texture Coordinates
	glGenTextures(1, spheretexNames);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, spheretexNames[0]);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	
	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// copy texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		spheretexture);
	//glGenerateMipmap(GL_TEXTURE_2D);

	/*// build our texture mipmaps
	switch (bpp)
	{
	case 8:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
		break;
	case 24:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
		break;
	case 32:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
		break;
	}*/
//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, spherebuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spheretexCoords), &spheretexCoords[0], GL_STATIC_DRAW);
	// Use layout location 2 for texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);


	



	// Define a sampler.  See page 709 in red book, 7th ed.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// This function initializes a cube
void initCube() {
	// Done defining the cube properties, now bind it to a VAO
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubebuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), &cubeVerts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, cubebuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), &cubeNorms[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spherebuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void initcylinder() {

	buildcylinderdata();
	// Done storing sphere data, now bind it
	glBindVertexArray(cylinderVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cylinderbuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cylinderVertices.size(), &cylinderVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderbuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cylinderNormals.size(), &cylinderNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderbuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cylinderIndices.size(), &cylinderIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



// This function pushes the specified matrix onto the modelview stack
void pushMatrix(glm::mat4 mat) {
	modelviewStack.push_back(glm::mat4(mat));
}

// This function pops a matrix from the modelview stack and assigns that to the matrix passed in
void popMatrix(glm::mat4& mat) {
	if (modelviewStack.size()) {
		mat = glm::mat4(modelviewStack.back());
		modelviewStack.pop_back();
	}
	else { // Just to prevent an unwanted error
		mat = glm::mat4(1.0f);
	}
}

#endif
