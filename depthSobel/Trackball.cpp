// ------------------------------------------------------
//
// A simple trackball implementation
//
// ------------------------------------------------------
// Sylvain Lefebvre - 2006-11-29
// ------------------------------------------------------

#include "Trackball.h"

#ifdef _WIN32
#include <GL/glut.h>  
#else
//#include <GLUT/glut.h>
#include <GL/glut.h>
#endif

static quatf s_Rotation;
static v3f   s_Translation;
static v3f   s_Center;
static uint  s_Width  =0;
static uint  s_Height =0;
static uint  s_PrevX  =0;
static uint  s_PrevY  =0;
static float s_Elapsed=0;
static uint  s_Status =0;
static float s_Radius =1.0f;

void trackballSetRadius(float r)
{
  s_Radius=r;
}

void trackballSetCenter(const v3f& center)
{
  s_Center=center;
}

quatf& trackballRotation()
{
  return (s_Rotation);
}

v3f& trackballTranslation()
{
  return (s_Translation);
}

const m4x4f trackballMatrix()
{
  return (
    translationMatrix(s_Translation)
    *translationMatrix(s_Center)
    *s_Rotation.toMatrix()
    *translationMatrix(-s_Center)
    );
}

void  trackballStopMotion()
{
  s_Status=0;
}

static quatf trackballDeltaRotation(float dx,float dy)
{
  //quatf qx=quatf(V3F(1,0,0),dy);
  //quatf qy=quatf(qx.inverse()*V3F(0,1,0),dx);
 
  if (dx*dx+dy*dy > 1e-10) {
    quatf qd=quatf(V3F(dy,dx,0),sqrt(dx*dx+dy*dy));
    return (qd);
  } else {
    return quatf();
  }
}

static void trackballInitRotation(uint x,uint y)
{
  s_PrevX=x;
  s_PrevY=y;
}

static void trackballInitTranslation(uint x,uint y)
{
  s_PrevX=x;
  s_PrevY=y;
}

static void trackballInitZoom(uint x,uint y)
{
  s_PrevX=x;
  s_PrevY=y;
}

static void trackballUpdateRotation(uint x,uint y)
{
  float speed = 3.0f; //*s_Elapsed;
  float dx    = (float(x)-float(s_PrevX))*speed/float(s_Width);
  float dy    = (float(y)-float(s_PrevY))*speed/float(s_Height);
  s_PrevX     = x;
  s_PrevY     = y;
  quatf dr    = trackballDeltaRotation(dx,dy);
  s_Rotation  = dr*s_Rotation;
}

static void trackballUpdateTranslation(uint x,uint y)
{
  float speed   = 1.5f*0.25f/* *s_Elapsed */ *(1.0f+abs(s_Translation[2]));
  float dx      = (float(x)-float(s_PrevX))*speed/float(s_Width);
  float dy      = -(float(y)-float(s_PrevY))*speed/float(s_Height);
  s_PrevX       = x;
  s_PrevY       = y;
  v3f   tx      = dx*V3F(1,0,0);
  v3f   ty      = dy*V3F(0,1,0);
  s_Translation = s_Translation + tx + ty;
}

static void trackballUpdateZoom(uint x,uint y)
{
  float speed   = 3.0f*0.5f* /* s_Elapsed* */(1.0f+abs(s_Translation[2]));
  float d       = -(float(y)-float(s_PrevY))*speed/float(s_Width);
  s_PrevX       = x;
  s_PrevY       = y;
  v3f   tz      = d*V3F(0,0,1);
  s_Translation = s_Translation + tz;
}

void trackballInit(uint w,uint h)
{
  s_Width       = w;
  s_Height      = h;
  s_Translation = V3F(0,0,-s_Radius);
  s_Center      = V3F(0,0,0);
}

void trackballButtonPressed(uint button,uint x,uint y)
{
  if (button == GLUT_LEFT_BUTTON) {   
    s_Status=s_Status|ROTATING;
    trackballInitRotation(x,y);
  } else if (button == GLUT_MIDDLE_BUTTON) {
    s_Status=s_Status|ZOOMING;
    trackballInitZoom(x,y);
  } else if (button == GLUT_RIGHT_BUTTON) { 
    s_Status=s_Status|TRANSLATING;
    trackballInitTranslation(x,y);
  }
}

void trackballButtonReleased(uint button)
{
  if (button == GLUT_LEFT_BUTTON) {   
    s_Status=s_Status&(~uint(ROTATING));
  } else if (button == GLUT_MIDDLE_BUTTON) {
    s_Status=s_Status&(~uint(ZOOMING));
  } else if (button == GLUT_RIGHT_BUTTON) { 
    s_Status=s_Status&(~uint(TRANSLATING));
  }
}

void trackballUpdate(uint x,uint y)
{
  if (s_Status & TRANSLATING)  trackballUpdateTranslation(x,y);
  if (s_Status & ROTATING   )  trackballUpdateRotation(x,y);
  if (s_Status & ZOOMING    )  trackballUpdateZoom(x,y);
}