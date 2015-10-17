#include "vector.h"
#include "math.h"


Vec2D vec2d(float x,float y)
{
    Vec2D v = {x,y};
    return v;
}

Vec3D vec3d(float x,float y, float z)
{
    Vec3D v = {x,y,z};
    return v;
}

Vec4D vec4d(float x,float y, float z, float w)
{
    Vec4D v = {x,y,z,w};
    return v;
}

float vec3d_magnitude (Vec3D V)
{
  return sqrt(V.x * V.x + V.y * V.y + V.z * V.z);
}

void vec3d_normalize (Vec3D *V)
{
  float M;
  if (!V)return;
  M= vec3d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
}