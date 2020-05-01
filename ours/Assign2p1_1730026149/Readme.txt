Some major changes in the original code:
===========================================
assign0test.cpp:

Add some data for robot:
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
...

Draw the robot and a new point light moving with the robot in display().

Add initialization for cylinder and light2 in init().

===========================================
geometry3.h:

Add some cylinder value:
GLuint cylinderVAO;
GLuint cylinderbuffers[3];
std::vector <glm::vec3> cylinderVertices;
std::vector <glm::vec3> cylinderNormals;
std::vector <unsigned int> cylinderIndices;
...

Add functions:
void buildCylinderData()
void initcylinder()
void drawcylinder()
...

===========================================
light.frag.glsl:

Add some value for light2:
uniform vec4 light2posn ; 
uniform vec4 light2color ;
...

Add the light computation for light2 in main funciton.
