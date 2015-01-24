// ------------------------------------------------------
//
// A simple trackball implementation
//
// ------------------------------------------------------
// Sylvain Lefebvre - 2006-11-29
// ------------------------------------------------------

#pragma once

#define TRANSLATING 1
#define ROTATING    2
#define ZOOMING     4

#include "Vertex.h"
#include "Quaternion.h"

void            trackballSetRadius(float r);
void            trackballSetCenter(const v3f& center);
quatf&          trackballRotation();
v3f&            trackballTranslation();
const m4x4f     trackballMatrix();
void            trackballStopMotion();

void trackballInit(uint w,uint h);
void trackballButtonPressed(uint button,uint x,uint y);
void trackballButtonReleased(uint button);
void trackballUpdate(uint x,uint y);

// ------------------------------------------------------
