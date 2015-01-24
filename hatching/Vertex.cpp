// ------------------------------------------------------

#include "Vertex.h"

// ------------------------------------------------------
#ifndef _WIN32
//#include "Math.h"
#include <math.h>
// for Matrix GD hack
double cotan(double i) { return(1 / tan(i)); }
#endif

v2f V2F(float x,float y)           
{v2f v; v[0]=x; v[1]=y; return (v);}

v2d V2D(double x,double y)
{v2d v; v[0]=x; v[1]=y; return (v);}

v3f V3F(float x,float y,float z)
{v3f v; v[0]=x; v[1]=y; v[2]=z; return (v);}

v3d V3D(double x,double y,double z)
{v3d v; v[0]=x; v[1]=y; v[2]=z; return (v);}

v4f V4F(float x,float y,float z,float w)
{v4f v; v[0]=x; v[1]=y; v[2]=z; v[3]=w; return (v);}

v4d V4D(double x,double y,double z,double w)
{v4d v; v[0]=x; v[1]=y; v[2]=z; v[3]=w; return (v);}

// ------------------------------------------------------
