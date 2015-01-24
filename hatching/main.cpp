#ifdef _WIN32

#include <windows.h>        // windows API header (required by gl.h)
#include "glew/include/GL/glew.h" // Support for shaders
#include <GL/gl.h>          // OpenGL header
#include <GL/glu.h>         // OpenGL Utilities header
#include <GL/glut.h>        // OpenGL Utility Toolkit header


#else

#include "glew/include/GL/glew.h"		// Support for shaders
#include <GL/gl.h>          // OpenGL header
#include <GL/glu.h>         // OpenGL Utilities header
#include <GL/glut.h>        // OpenGL Utility Toolkit header
#include <stdlib.h>

#endif

#include <cstdio>
#include <cmath>

#include "glsl.h" // GLSL Helper functions
#include "Matrix4x4.h" //class to represent matrices
#include "Vertex.h" //class to represent vertices
#include "Trackball.h" // trackball
#include "Texture.h" // Structure to represent a texture
#include <cstdio>
#include <cmath>

//path of the shaders
#ifdef _WIN32
#define SRC_PATH	""
#else
#define SRC_PATH	""
#endif

#include <iostream>
using namespace std;

/* --------------------- Main Window ------------------- */

int          g_MainWindow; // glut Window Id
int          g_W=640;      // window width
int          g_H=480;      // window width

/* --------------------- Geometry ------------------- */

//Vertex Array Object
GLuint VertexArrayID;
// This will identify our vertex buffer
GLuint vertexbuffer;
// This will identify our normal buffer
GLuint normalbuffer;
// This will identify our texture coordinate buffer
GLuint uvbuffer;

float c = 1;


// This will identify the texture
//GLuint texture;

// This will identify the texture
Texture treeTexture;

//---- IFS representation ---
typedef struct s_point
{
  float p[3]; //position
  float n[3]; //normal
  float uv[2]; //texture coordinate
} t_point;

t_point        *g_Verticies     = NULL; //list of vertices
unsigned short *g_Indices       = NULL; //list of faces, 3 indices per face
unsigned int    g_NumVerticies  = 0; //number of vertices
unsigned int    g_NumIndices    = 0; //number of indices, i.e. 3 times the number of faces


/*---------------------- Shaders -------------------*/
GLuint g_glslProgram;


/* -------------------------------------------------------- */

void mainKeyboard(unsigned char key, int x, int y) 
{
	if (key == 'q') {
		exit (0);
	} else if (key == ' ') {
		printf("spacebar pressed\n");
	} 

	printf("key '%c' pressed\n",key);
}

/* -------------------------------------------------------- */

void mainMouse(int btn, int state, int x, int y) 
{
	if (state == GLUT_DOWN) {
		trackballButtonPressed(btn,x,y);
	} else if (state == GLUT_UP) {
		trackballButtonReleased(btn);
	}
}

/* -------------------------------------------------------- */

void mainMotion(int x,int y)
{
	trackballUpdate(x,y);
}

/* -------------------------------------------------------- */

void mainReshape(int w,int h)
{
	printf("Resizing window to %d,%d\n",w,h);
	g_W=w;
	g_H=h;
	// set viewport to the entire window
	glViewport(0,0,g_W,g_H);
}


/* -------------------------------------------------------- */

void mainRender()
{
	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST); //[Q1c]
	
	// use our shader
	glUseProgramObjectARB(g_glslProgram);
	
	
	
	//--- Camera ---//
	// View: get trackball transform
	m4x4f View = trackballMatrix();
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m4x4f Projection = perspectiveMatrix<float>(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Model matrix : an identity matrix (model will be at the origin)
	m4x4f Model;
	Model.eqIdentity();
	
	// Our ModelViewProjection : multiplication of our 3 matrices
	m4x4f MVP = Projection*View*Model; 
	MVP = MVP.transpose();
	
	// Get a handle for our "MVP" uniform (identify the model-view-projection matrix in the shader)
	GLuint MatrixID = glGetUniformLocation(g_glslProgram, "MVP");
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(MVP[0]));

	// send ModelView to the shader 
	m4x4f MV = View*Model; 
	// Get a handle for our "MV" uniform 
	GLuint MVmatrixID = glGetUniformLocation(g_glslProgram, "MV"); 
	// Send our transformation to the currently bound shader, in the "MV" uniform 
	glUniformMatrix4fv(MVmatrixID, 1, GL_FALSE, &(MV[0]));


	// Get a handle for our "ViewMat" uniform (identify the view matrix in the shader)
	GLuint MatrixViewID1 = glGetUniformLocation(g_glslProgram, "ViewMat");
	// Send our transformation to the currently bound shader, in the "ViewMat" uniform
	glUniformMatrix4fv(MatrixViewID1, 1, GL_FALSE, &(View[0]));
	

	GLuint countID  = glGetUniformLocation(g_glslProgram, "count");
	glUniform1f(countID, c);
	c += 0.01;
	
	
	GLuint TextureID  = glGetUniformLocation(g_glslProgram, "myTextureSampler");
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, treeTexture.texID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0); 
	
	
	
	//--- Geometry ---//
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	  0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size, 3 coordinates per vertex (x,y,z)
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	);
	
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
	    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	    3,                                // size, 3 coordinate per normal
	    GL_FLOAT,                         // type
	    GL_FALSE,                         // normalized? No, it will be normalized in the shader
	    0,                                // stride
	    (void*)0                          // array buffer offset
	);
	
	// 3nd attribute buffer : UVs [Q2b]
	// ...
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
	    2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	    2,                                // size, 2
	    GL_FLOAT,                         // type
	    GL_FALSE,                         // normalized? No, it will be normalized in the shader
	    0,                                // stride
	    (void*)0                          // array buffer offset
	);


	
	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, g_NumIndices); // Starting from vertex 0; 3 vertices total -> 1 triangle. [Q1b] Update to draw more vertices
 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	
	// swap - this call exchanges the back and front buffer
	// swap is synchronized on the screen vertical sync
	glutSwapBuffers();
}

/* -------------------------------------------------------- */

void idle( void )
{
	// whenever the application has free time, ask for a screen refresh
	glutPostRedisplay();

}

/* -------------------------------------------------------- */

void loadShaders()
{
	const char *fp_code=loadStringFromFile(SRC_PATH "SimpleFragmentShader.fp");
	const char *vp_code=loadStringFromFile(SRC_PATH "SimpleVertexShader.vp");
	g_glslProgram = createGLSLProgram(vp_code,fp_code);
	delete [](fp_code);
	delete [](vp_code);
}

/* -------------------------------------------------------- */

// Load an IFS model
void loadIFS(const char *filename)
{
  // open file
  FILE *f=fopen(filename,"rb");
  if (f == NULL) {
    // error?
    cerr << "[loadIFS] Cannot load " << filename << endl;
  }
  // read vertices
  fread(&g_NumVerticies,sizeof(unsigned int),1,f);
  g_Verticies = new t_point[g_NumVerticies];
  fread(g_Verticies,sizeof(t_point),g_NumVerticies,f);
  // read indices
  fread(&g_NumIndices,sizeof(unsigned int),1,f);
  g_Indices = new unsigned short[g_NumIndices];
  fread(g_Indices,sizeof(unsigned short),g_NumIndices,f);
  // close file
  fclose(f);
  // print mesh info
  cerr << g_NumVerticies << " points " << g_NumIndices/3 << " triangles" << endl;
  // done.
}

/* -------------------------------------------------------- */

void createGeometry()
{
	loadIFS("test.mesh");
	
	// Position
	GLfloat* g_vertex_buffer_data;
	g_vertex_buffer_data = new GLfloat[3*g_NumIndices];
	for(int i=0; i<g_NumIndices; i++){
	    g_vertex_buffer_data[i*3] = g_Verticies[g_Indices[i]].p[0];
	    g_vertex_buffer_data[i*3+1] = g_Verticies[g_Indices[i]].p[1];
	    g_vertex_buffer_data[i*3+2] = g_Verticies[g_Indices[i]].p[2];
	}
	
	// Normals
	GLfloat* g_normal_buffer_data;
	g_normal_buffer_data = new GLfloat[3*g_NumIndices];
	for(int i=0; i<g_NumIndices; i++){
	    g_normal_buffer_data[i*3] = g_Verticies[g_Indices[i]].n[0];
	    g_normal_buffer_data[i*3+1] = g_Verticies[g_Indices[i]].n[1];
	    g_normal_buffer_data[i*3+2] = g_Verticies[g_Indices[i]].n[2];
	  
	}
	
	// UVs
	GLfloat* g_uv_buffer_data;
	g_uv_buffer_data = new GLfloat[2*g_NumIndices];
	for(int i=0; i<g_NumIndices; i++){
	    g_uv_buffer_data[i*2] = g_Verticies[g_Indices[i]].uv[0];
	    g_uv_buffer_data[i*2+1] = g_Verticies[g_Indices[i]].uv[1];
	}
	
	

	//--- Send the geometry to OpenGL
	// We need a vertex array object
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Generate 1 vertex buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// Make the buffer active
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, 3*g_NumIndices*sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);
	//same for normals...
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*g_NumIndices*sizeof(GLfloat), g_normal_buffer_data, GL_STATIC_DRAW);
	//same for uv...
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, 2*g_NumIndices*sizeof(GLfloat), g_uv_buffer_data, GL_STATIC_DRAW);
	
	
	//clean up
	delete [] g_vertex_buffer_data;
	delete [] g_normal_buffer_data;
	delete [] g_uv_buffer_data;

}

/* -------------------------------------------------------- */
bool LoadTreeTextures()
{

    // first of all we call the tga file loader. It doesn't do anything special: it fills the Texture struct with information about
    // the image (height, width, bits per pixel). The important thing is do load the image corectly in the structure you give it
    if (LoadTGA(&treeTexture, "hatches.tga"))
    {

        // This tells opengl to create 1 texture and put it's ID in the given integer variable
        // OpenGL keeps a track of loaded textures by numbering them: the first one you load is 1, second is 2, ...and so on.
        glGenTextures(1, &treeTexture.texID);
        // Binding the texture to GL_TEXTURE_2D is like telling OpenGL that the texture with this ID is now the current 2D texture in use
        // If you draw anything the used texture will be the last binded texture
        glBindTexture(GL_TEXTURE_2D, treeTexture.texID);
        // This call will actualy load the image data into OpenGL and your video card's memory. The texture is allways loaded into the current texture
        // you have selected with the last glBindTexture call
        // It asks for the width, height, type of image (determins the format of the data you are giveing to it) and the pointer to the actual data
        glTexImage2D(GL_TEXTURE_2D, 0, treeTexture.bpp / 8, treeTexture.width, treeTexture.height, 0, treeTexture.type, GL_UNSIGNED_BYTE, treeTexture.imageData);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glEnable(GL_TEXTURE_2D);
        if (treeTexture.imageData)
        {

            // You can now free the image data that was allocated by LoadTGA
            // You don't want to keep a few Mb of worthless data on heap. It's worthless because OpenGL stores the image someware else after
            // you call glTexImage2D (usualy in you video card)
            free(treeTexture.imageData); 

        }

        return true;

    } // Return The Status
    else return false;

}

/* -------------------------------------------------------- */

void cleanUp()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}

/* -------------------------------------------------------- */

int
main(int argc, char **argv) 
{
	///
	///  glut Window
	///
	// main glut init
	glutInit(&argc, argv);
	// initial window size
	glutInitWindowSize(g_W,g_H); 
	// init display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// create main window
	g_MainWindow=glutCreateWindow("TP2");
	// set main window as current window
	glutSetWindow(g_MainWindow);
	/// setup glut callbacks
	// mouse (whenever a button is pressed)
	glutMouseFunc(mainMouse);
	// motion (whenever the mouse is moved *while* a button is down)
	glutMotionFunc(mainMotion);
	// keyboard (whenever a character key is pressed)
	glutKeyboardFunc(mainKeyboard);
	// display  (whenerver the screen needs to be painted)
	glutDisplayFunc(mainRender);
	// reshape (whenever the window size changes)
	glutReshapeFunc(mainReshape);
	// idle (whenever the application as some free time)
	glutIdleFunc(idle);

	///
	/// Shaders, geometry, texture and camera
	///
	
	//need to init Glew before anything else
	glewInit();
	
	loadShaders();
	
	//Send the geometry to OpenGL
	createGeometry();
	
	// Trackball init (controls the camera)
	trackballInit(g_W,g_H);
	trackballTranslation() = V3F(0.f,0.f,-3.f);
	


	//load texture
	if(!LoadTreeTextures()){
	    printf("Error while loading the texture.");
	    #ifdef WINDOWS
	    return; 
	    #else
	    return 0;
	    #endif
	}

	// print a small documentation
	printf("[q]     - quit\n");

	// enter glut main loop - this *never* returns
	glutMainLoop();
	
	cleanUp();
}

/* -------------------------------------------------------- */

