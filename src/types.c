#include "types.h"
#include "simple_logger.h"
#include <math.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void vec3d_print(Text out, Vec3D v)
{
  sprintf(out,"(%f,%f,%f)",v.x,v.y,v.z);
}

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

void vec3d_set_angle_by_radians(Vec3D *out,float radians)
{
  if(!out)return;
  out->x = cos(radians);
  out->y = sin(radians);
}

void vec4d_set_angle_by_radians(Vec4D *out,float radians)
{
  if(!out)return;
  out->x = cos(radians);
  out->y = sin(radians);
}

float vector_angle(float x,float y)
{
  float angle = atan2(y, x) + M_PI;
  float fraction = angle * 0.5 / M_PI;
  if (fraction >= 1.0)
  {
    fraction -= 1.0;
  }
  return (fraction * 360);
}

float vec2d_angle(Vec2D v)
{
  return vector_angle(v.x,v.y);
}

float angle_between_degrees(float a, float b)
{
  float angle;
  angle = fabs(a - b);
  while (angle >= 360)angle -= 360;
  while (angle < 0)angle += 360;
  if (angle > 180)angle -= 180;
  return angle;
}

void angle_clamp_degrees(float *a)
{
  if (!a)return;
  while (*a >= 360)*a -= 360;
  while (*a < 0)*a += 360;
}

RectFloat rectf(
  float  x,
  float  y,
  float  w,
  float  h
)
{
  RectFloat r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return r;
}

void rect_set(
    RectFloat * d,
    float       x,
    float       y,
    float       w,
    float       h
)
{
  if (!d)return;
  d->x = x;
  d->y = y;
  d->w = w;
  d->h = h;
}

void rect_copy(
    RectFloat * d,
    RectFloat   s
  )
{
  if (!d)return;
  d->x = s.x;
  d->y = s.y;
  d->w = s.w;
  d->h = s.h;
}

void rect3d_clear(Rect3D *r)
{
  if (!r)return;
  memset(r,0,sizeof(Rect3D));
}

Rect3D rect3d_from_vec3d(Vec3D corner,Vec3D size)
{
  Rect3D r;
  r.x = corner.x;
  r.y = corner.y;
  r.z = corner.z;
  r.w = size.x;
  r.h = size.y;
  r.d = size.z;
  return r;
}

Rect3D rect3d(float x,float y,float z,float w,float h,float d)
{
  Rect3D r;
  r.x = x;
  r.y = y;
  r.z = z;
  r.w = w;
  r.h = h;
  r.d = d;
  return r;
}

int rect3d_overlap(Rect3D a, Rect3D b)
{
  if ((a.x > (b.x + b.w)) || (a.y > (b.y + b.h))
    || (b.x > (a.x + a.w)) || (b.y > (a.y + a.h))
    || (b.z > (a.z + a.d)) || (b.z > (a.z + a.d)))
    return 0;
  return 1;
}

void orientation_zero(Orientation *out)
{
  if (!out)return;
  memset(out,0,sizeof(Orientation));
}

void orientation_copy(Orientation *dst,Orientation src)
{
  if (!dst)return;
  vec3d_cpy(dst->position,src.position);
  vec3d_cpy(dst->rotation,src.rotation);
  vec3d_cpy(dst->scale,src.scale);
  vec3d_cpy(dst->color,src.color);
  dst->alpha = src.alpha;
}

void orientation_clear(Orientation *out)
{
  if (!out)return;
  vec3d_set(out->position,0,0,0);
  vec3d_set(out->rotation,0,0,0);
  vec3d_set(out->scale,1,1,1);
  vec3d_set(out->color,1,1,1);
  out->alpha = 1;
}

void orientation_add(Orientation * out,
                         Orientation   in1,
                         Orientation   in2)
{
  if (!out)return;
  vec3d_add(out->position,in1.position,in2.position);
  vec3d_add(out->rotation,in1.rotation,in2.rotation);
  vec3d_add(out->scale,in1.scale,in2.scale);
  vec3d_add(out->color,in1.color,in2.color);
  out->alpha = in1.alpha + in2.alpha;

  if (out->alpha < 0)out->alpha = 0;
  else if (out->alpha > 1)out->alpha = 1;
  
  if (out->color.x < 0)out->color.x = 0;
  else if (out->color.x > 1)out->color.x = 1;
  
  if (out->color.y < 0)out->color.y = 0;
  else if (out->color.y > 1)out->color.y = 1;
  
  if (out->color.z < 0)out->color.z = 0;
  else if (out->color.z > 1)out->color.z = 1;
}

void orientation_mix(Orientation * out,
                         Orientation   in1,
                         Orientation   in2)
{
  if (!out)return;
  vec3d_add(in1.position,in2.position,out->position);
  vec3d_add(in1.rotation,in2.rotation,out->rotation);
  vec3d_add(in1.scale,in2.scale,out->scale);

  out->scale.x = in1.scale.x * in2.scale.x;
  out->scale.y = in1.scale.y * in2.scale.y;
  out->scale.z = in1.scale.z * in2.scale.z;
  
  out->color.x = in1.color.x * in2.color.x;
  out->color.y = in1.color.y * in2.color.y;
  out->color.z = in1.color.z * in2.color.z;
  
  out->alpha = in1.alpha * in2.alpha;

  if (out->alpha < 0)out->alpha = 0;
  else if (out->alpha > 1)out->alpha = 1;

  if (out->color.x < 0)out->color.x = 0;
  else if (out->color.x > 1)out->color.x = 1;

  if (out->color.y < 0)out->color.y = 0;
  else if (out->color.y > 1)out->color.y = 1;

  if (out->color.z < 0)out->color.z = 0;
  else if (out->color.z > 1)out->color.z = 1;
}

void vec3d_cross_product(Vec3D *out, Vec3D v1, Vec3D v2)
{
  out->x = v1.y*v2.z - v1.z*v2.y;
  out->y = v1.z*v2.x - v1.x*v2.z;
  out->z = v1.x*v2.y - v1.y*v2.x;
}

Vec2D *vec2d_new()
{
  Vec2D *vec = NULL;
  vec = (Vec2D *)malloc(sizeof(Vec2D));
  if (vec == NULL)
  {
    slog("types:failed to allocate a new vector!\n");
    return NULL;
  }
  vec2d_clear((*vec));
  return vec;
}

Vec3D *vec3d_new()
{
  Vec3D *vec = NULL;
  vec = (Vec3D *)malloc(sizeof(Vec3D));
  if (vec == NULL)
  {
    slog("types:failed to allocate a new vector!\n");
    return NULL;
  }
  vec3d_clear((*vec));
  return vec;
}

Vec4D *vec4d_new()
{
  Vec4D *vec = NULL;
  vec = (Vec4D *)malloc(sizeof(Vec4D));
  if (vec == NULL)
  {
    slog("types:failed to allocate a new vector!\n");
    return NULL;
  }
  vec4d_clear((*vec));
  return vec;
}

Vec2D *vec2d_dup(Vec2D old)
{
  Vec2D *duped = NULL;
  duped = vec2d_new();
  if (!duped)return NULL;
  vec2d_cpy((*duped),old);
  return duped;
}

Vec3D *vec3d_dup(Vec3D old)
{
  Vec3D *duped = NULL;
  duped = vec3d_new();
  if (!duped)return NULL;
  vec3d_cpy((*duped),old);
  return duped;
}

Vec4D *vec4d_dup(Vec4D old)
{
  Vec4D *duped = NULL;
  duped = vec4d_new();
  if (!duped)return NULL;
  vec4d_cpy((*duped),old);
  return duped;
}


void vec2d_normalize (Vec2D *V)
{
  float M;
  if (!V)return;
  M = vec2d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
}

void vec3d_normalize(Vec3D *V)
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

void vec4d_normalize (Vec4D *V)
{
  float M;
  if (!V)return;
  M = vec4d_magnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
  V->w *= M;
}

float vec2d_magnitude (Vec2D V)
{
  return sqrt (V.x * V.x + V.y * V.y);
}

float vec3d_magnitude(Vec3D V)
{
  return sqrt(V.x * V.x + V.y * V.y + V.z * V.z);
}

float vec4d_magnitude (Vec4D V)
{
  return sqrt (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

float vec2d_magnitude_squared(Vec2D V)
{
  return (V.x * V.x + V.y * V.y);
}

float vec3d_magnitude_squared(Vec3D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z);
}

float vec4d_magnitude_squared(Vec4D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

int vec3d_magnitude_less_than(Vec3D V,float size)
{
  return (vec3d_magnitude_squared(V) < (size * size));
}


int distance_between_less_than3d(Vec3D p1,Vec3D p2,float size)
{
  return vec3d_magnitude_less_than(vec3d(p1.x - p2.x,
											p1.y - p2.y,
											p1.z - p2.z),
									size);
}


void vec3d_project_to_plane(Vec3D *dst, Vec3D p, Vec3D normal)
{
  float d;
  Vec3D n;
  float inv_denom;
  if (!dst)return;
  inv_denom = 1.0F / vec3d_dot_product( normal, normal );
  
  d = vec3d_dot_product( normal, p ) * inv_denom;
  
  n.x = normal.x * inv_denom;
  n.y = normal.y * inv_denom;
  n.z = normal.z * inv_denom;
  
  dst->x = p.z - d * n.x;
  dst->y = p.y - d * n.y;
  dst->z = p.x - d * n.z;
}

void vec3d_perpendicular(Vec3D *dst, Vec3D src)
{
  int pos;
  float minelem = 1.0F;
  Vec3D tempvec;
  if (!dst)return;
  /*
   * * find the smallest magnitude axially aligned vector
   */
  pos=0;
  
  if ( fabs( src.x ) < minelem )
  {
    pos=0;
    minelem = fabs( src.x );
  }
  if ( fabs( src.y ) < minelem )
  {
    pos=1;
    minelem = fabs( src.y );
  }
  if ( fabs( src.y ) < minelem )
  {
    pos=2;
    minelem = fabs( src.z );
  }
  
  tempvec.x=0;
  tempvec.y=0;
  tempvec.z=0;
  
  switch(pos)
  {
    case 0:
      tempvec.x=1;
      break;
    case 1:
      tempvec.y=1;
      break;
    case 2:
      tempvec.z=1;
      break;
  }
  
  /*
   * * project the point onto the plane defined by src
   */
  vec3d_project_to_plane( dst, tempvec, src );
  
  /*
   * * normalize the result
   */
  vec3d_normalize( dst );
}

void rotation_concacenate(float in1[3][3], float in2[3][3], float out[3][3])
{
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
  in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
  in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
  in1[0][2] * in2[2][2];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
  in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
  in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
  in1[1][2] * in2[2][2];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
  in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
  in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
  in1[2][2] * in2[2][2];
}

void vec3d_rotate_about_vector(Vec3D *dst, Vec3D dir, Vec3D point, float degrees)
{
  float m[3][3];
  float im[3][3];
  float zrot[3][3];
  float tmpmat[3][3];
  float rot[3][3];
  Vec3D vr, vup, vf;
  
  if (!dst)return;
  
  vf.x = dir.x;
  vf.y = dir.y;
  vf.z = dir.z;
  
  vec3d_perpendicular( &vr, dir );
  vec3d_cross_product(&vup, vr, vf);

  m[0][0] = vr.x;
  m[1][0] = vr.y;
  m[2][0] = vr.z;
  
  m[0][1] = vup.x;
  m[1][1] = vup.y;
  m[2][1] = vup.z;
  
  m[0][2] = vf.x;
  m[1][2] = vf.y;
  m[2][2] = vf.z;
  
  memcpy( im, m, sizeof( im ) );
  
  im[0][1] = m[1][0];
  im[0][2] = m[2][0];
  im[1][0] = m[0][1];
  im[1][2] = m[2][1];
  im[2][0] = m[0][2];
  im[2][1] = m[1][2];
  
  memset( zrot, 0, sizeof( zrot ) );
  zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;
  
  zrot[0][0] = cos( ( degrees*DEGTORAD ) );
  zrot[0][1] = sin( ( degrees*DEGTORAD ) );
  zrot[1][0] = -sin( ( degrees*DEGTORAD ) );
  zrot[1][1] = cos( ( degrees*DEGTORAD ) );
  
  rotation_concacenate( m, zrot, tmpmat );
  rotation_concacenate( tmpmat, im, rot );
  
  dst->x = rot[0][0] * point.x + rot[0][1] * point.y + rot[0][2] * point.z;
  dst->y = rot[1][0] * point.x + rot[1][1] * point.y + rot[1][2] * point.z;
  dst->z = rot[2][0] * point.x + rot[2][1] * point.y + rot[2][2] * point.z;
}

void vec3d_rotate_about_x(Vec3D *vect, float angle)
{
  Vec3D temp;
  if (!vect)return;
  
  angle=angle*DEGTORAD;
  
  temp.x=vect->x;
  temp.y=(vect->y*cos(angle))-(vect->z*sin(angle));
  temp.z=(vect->y*sin(angle))+(vect->z*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}

void vec3d_rotate_about_y(Vec3D *vect, float angle)
{
  Vec3D temp;
  if (!vect)return;
  
  angle=angle*DEGTORAD;
  
  temp.y=vect->y;
  temp.x=(vect->x*cos(angle))+(vect->z*sin(angle));
  temp.z=(vect->x*sin(angle)*(-1))+(vect->z*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}

void vec3d_rotate_about_z(Vec3D *vect, float angle)
{
  Vec3D temp;
  if (!vect)return;
  
  angle=angle*DEGTORAD;
  
  temp.z=vect->z;
  temp.x=(vect->x*cos(angle))-(vect->y*sin(angle));
  temp.y=(vect->x*sin(angle))+(vect->y*cos(angle));
  
  vect->x=temp.x;
  vect->y=temp.y;
  vect->z=temp.z;
}


int vec3d_and(Vec3D V, Vec3D W)
{
	if((V.x == W.x) && (V.y == W.y) && (V.z == W.z))return 1;
	else return 0;
}
