#ifndef NP_AREA2D_H
#define NP_AREA2D_H

#include "neophysics/math.h"
#include "neophysics/core2d.h"

/*
 functions to get the total area of an object 
*/

/*
 get/calculate an area of polygon2d object
*/
NP_COLD
NP_INLINE NPReal __NPPolygon2DArea(const NPReal2 *vertices, const NPUint8 len, const NPReal scale[2]) {
 NPReal accumulated_area = (NPReal)0.0;
 NPUint8 index = 0;
 
 while(index < len) {
  const NPUint8 next_index = (index + 1) % len;
  accumulated_area += __NPCross2(vertices[index], vertices[next_index]);
  //accumulated_area += vertices[index].x * vertices[next_index].y - vertices[index].y * vertices[next_index].x;
  index++;
 }
 return __NPAbs(accumulated_area) * (NPReal)0.5;
}

#endif

