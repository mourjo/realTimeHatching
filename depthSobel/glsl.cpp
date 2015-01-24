/* -------------------------------------------------------- */
/* -------------------------------------------------------- */
/*

    TP OpenGL - Graphics and Sound Synthesis course

George Drettakis, Adrien Bousseau, Sylvain Lefebvre

mailto: FirstName.LastName@inria.fr

REVES / INRIA

History:
- Created 2006-10-10 (SL)
- Update 2013-02 (AB)

*/
/* -------------------------------------------------------- */
/*

Helper functions for GLSL

*/
/* -------------------------------------------------------- */

#include "glsl.h"

#ifdef	_WIN32
GLuint 
#else
GLhandleARB
#endif
loadGLSLShader(const char *prg,GLuint type)
{
	
#ifdef _WIN32
	int id;
#else
	GLhandleARB id;
#endif
	// create shader object
	id = glCreateShaderObjectARB(type);
	// setup string as source
	glShaderSourceARB(id,1,&prg,(const GLint*)NULL);
	// compile the shader
	glCompileShaderARB(id);
	// check for errors
	GLint compiled;
	glGetObjectParameterivARB(id,GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (!compiled) {
		// there was an error => display
		GLint maxLength;
		glGetObjectParameterivARB(id,
								  GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		GLcharARB *infoLog=new GLcharARB[maxLength+1];
		glGetInfoLogARB(id, maxLength, (GLsizei*)NULL, infoLog);
		fprintf(stderr,"\n\n**** GLSL shader failed to compile ****\n%s",infoLog);
		delete [](infoLog);
		// exit on error
		exit (0);
	}
	// done, return shader id
	return (id);
}

/* -------------------------------------------------------- */

/*
 createGLSLProgram
 
 Create a GLSL program and load shaders.
 
 * Inputs
 - vp_code   : null terminated string for vertex program
 - fp_code   : null terminated string for fragment program
 * Output
 - program GL id
 
 */
#ifdef	_WIN32
GLuint 
#else
GLhandleARB
#endif
createGLSLProgram(const char *vp_code,const char *fp_code)
{
	
	// create program
#ifdef _WIN32
	GLuint vp,fp;
	int id;
#else
	GLhandleARB vp,fp;
	GLhandleARB id;
#endif
	id = glCreateProgramObjectARB();
	// if vertex program code is given, compile it
	if (vp_code) {
		vp=loadGLSLShader(vp_code,GL_VERTEX_SHADER);
		glAttachObjectARB(id,vp);
	}
	// if fragment program code is given, compile it
	if (fp_code) {
		fp=loadGLSLShader(fp_code,GL_FRAGMENT_SHADER);
		glAttachObjectARB(id,fp);
	}
	// link compiled shaders
	glLinkProgramARB(id);
	// check link success
	GLint linked;
	glGetObjectParameterivARB(id,GL_OBJECT_LINK_STATUS_ARB, &linked);
	if (!linked) {
		// there was an error => display
		GLint maxLength;
		glGetObjectParameterivARB(id,GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		GLcharARB *infoLog=new GLcharARB[maxLength+1];
		glGetInfoLogARB(id, maxLength, (GLsizei*)NULL, infoLog);
		fprintf(stderr,"\n\n**** GLSL program failed to link ****\n%s",infoLog);
		delete [](infoLog);
		// exit on error
		exit (0);
	}
	// free compiled shaders (they are now embbeded into the program)
	if (vp_code) {
		glDeleteObjectARB(vp);
	}
	if (fp_code) {
		glDeleteObjectARB(fp);
	}
	// set as current program
	glUseProgramObjectARB(id);
	// done, return program id
	return (id);
}
/* -------------------------------------------------------- */

/*
 Load a string from a file
 
 Returned string must be deleted with delete[]()
*/
const char *loadStringFromFile(const char *fname)
{
  // open file
  FILE *f=fopen(fname,"rb");
  if (f == NULL) {
	  fprintf(stderr,"[ERROR] Cannot open file '%s'\n",fname);
	  exit (-1);
  }
  // get file size
  fseek(f,0,SEEK_END);    // goto end
  long fsize = ftell(f);  // get size
  fseek(f,0,SEEK_SET);    // goto begining
  // allocate string
  char *str=new char[fsize+1];
  // read
  fread(str,fsize,sizeof(char),f);
  str[fsize]='\0';
  // close file
  fclose(f);
  // return string
  return (str);
}

/* -------------------------------------------------------- */
