#include "op_3d.h"
#include <stdlib.h>


float edge_length_3d(Vec3D e1,Vec3D e2)
{
  return vec3d_magnitude(vec3d(e1.x-e2.x,e1.y-e2.y,e1.z-e2.z));
}

int ray_cross_edge(
  Vec2D start,
  Vec2D v,
  Vec2D e1,
  Vec2D e2,
  Vec3D *contact)
{
  float testx, testy;
  float endx, endy;
  float Ua,Ub,Uden;
  
  endx = start.x + v.x;
  endy = start.y + v.y;
  
  Uden = ((e2.y - e1.y)*(endx - start.x)) - ((e2.x - e1.x)*(endy - start.y));
  if(Uden == 0)
  {
    return 0;/*parallel, can't hit*/
  }
    
  Ua = (((e2.x - e1.x)*(start.y - e1.y))-((e2.y - e1.y)*(start.x - e1.x))) / Uden;
  Ub = (((endx - start.x)*(start.y - e1.y))-((endy - start.y)*(start.x - e1.x))) / Uden;
  
  testx = start.x + (Ua * (endx - start.x));
  testy = start.y + (Ua * (endy - start.y));
  
  if(contact != NULL)
  {
    contact->x = testx;
    contact->y = testy;
  }
  
  if((Ua >= 0) && (Ua <= 1) && (Ub >= 0) && ( Ub <= 1))
  {
    return 1;
  }
  return 0;  
}

void get_triangle_normal(Vec3D t1, Vec3D t2, Vec3D t3, Vec3D *normal)
{
  if (normal)
  {
    normal->x = (t1.y * (t2.z - t3.z)) + (t2.y * (t3.z - t1.z)) + (t3.y * (t1.z - t2.z));
    normal->y = (t1.z * (t2.x - t3.x)) + (t2.z * (t3.x - t1.x)) + (t3.z * (t1.x - t2.x));
    normal->z = (t1.x * (t2.y - t3.y)) + (t2.x * (t3.y - t1.y)) + (t3.x * (t1.y - t2.y));
    vec3d_normalize(normal);
  }
}

void get_triangle_plane(Vec3D t1, Vec3D t2, Vec3D t3, Vec3D *normal, float *D)
{
  get_triangle_normal(t1, t2, t3, normal);
  if (D)
  {
    *D = -((t1.x *(t2.y * t3.z - t3.y * t2.z)) +
          (t2.x *(t3.y * t1.z - t1.y * t3.z)) +
          (t3.x *(t1.y * t2.z - t2.y * t1.z)));
  }
}

float ray_in_plane(
  Vec3D start,
  Vec3D dir,
  Vec3D normal,
  float D,
  Vec3D *contact)
{
  float denom,t;
  denom = ((normal.x * dir.x) + (normal.y * dir.y) + (normal.z * dir.z));
  if(denom == 0)return 0;
  t = - (((normal.x * start.x) + (normal.y * start.y) + (normal.z * start.z) + D) / denom);
  if((t > 0)&&(t <= 1))
  {
    if (contact)
    {
      contact->x = start.x + (dir.x * t);
      contact->y = start.y + (dir.y * t);
      contact->z = start.z + (dir.z * t);
    }
    return 1;
  }
  if (contact)
  {
    contact->x = start.x + (dir.x * t);
    contact->y = start.y + (dir.y * t);
    contact->z = start.z + (dir.z * t);
  }
  return -1;
}

int point_in_triangle(
  Vec3D point,
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D *normal)
{
  Vec3D n;
  Vec2D rayTest = {0,0};
  int intersectCount = 0;
  if (!normal)
  {
    get_triangle_normal(t1, t2, t3, &n);
    normal = &n;
  }

  rayTest.y = edge_length_3d(t1,t2) + edge_length_3d(t2,t3) + edge_length_3d(t3,t1);
  
  if (normal->x > (MAX(normal->y,normal->z)))
  {
    /*project triangle to yz plane*/
    if (ray_cross_edge(
      vec2d(point.y,point.z),
      rayTest,
      vec2d(t1.y,t1.z),
      vec2d(t2.y,t2.z),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.y,point.z),
      rayTest,
      vec2d(t2.y,t2.z),
      vec2d(t3.y,t3.z),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.y,point.z),
      rayTest,
      vec2d(t3.y,t3.z),
      vec2d(t1.y,t1.z),
      NULL))
    {
      intersectCount++;
    }    
  }
  else if (normal->y > (MAX(normal->x,normal->z)))
  {
    /*project triangle to xz plane*/
    if (ray_cross_edge(
      vec2d(point.x,point.z),
      rayTest,
      vec2d(t1.x,t1.z),
      vec2d(t2.x,t2.z),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.x,point.z),
      rayTest,
      vec2d(t2.x,t2.z),
      vec2d(t3.x,t3.z),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.x,point.z),
      rayTest,
      vec2d(t3.x,t3.z),
      vec2d(t1.x,t1.z),
      NULL))
    {
      intersectCount++;
    }
  }
  else
  {
    /*project triangle to xy plane*/
    if (ray_cross_edge(
      vec2d(point.x,point.y),
      rayTest,
      vec2d(t1.x,t1.y),
      vec2d(t2.x,t2.y),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.x,point.y),
      rayTest,
      vec2d(t2.x,t2.y),
      vec2d(t3.x,t3.y),
      NULL))
    {
      intersectCount++;
    }
    if (ray_cross_edge(
      vec2d(point.x,point.y),
      rayTest,
      vec2d(t3.x,t3.y),
      vec2d(t1.x,t1.y),
      NULL))
    {
      intersectCount++;
    }
  }
  if (intersectCount % 2)
  {
    return 1;
  }
  return 0;
}

int ray_in_quad(
  Vec3D start,
  Vec3D dir,
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D t4,
  Vec3D *contact)
{
  float t;
  Vec3D normal;
  float D;
  Vec3D intersectPoint;

  get_triangle_plane(t1,t2,t3,&normal,&D);
  
  t = ray_in_plane(
    start,
    dir,
    normal,
    D,
    &intersectPoint);
  
  if ((t <= 0)|| (t > 1))
  {
    return 0;
  }
  if (point_in_triangle(
    intersectPoint,
    t1,
    t2,
    t3,
    &normal))
  {
    if (contact)
    {
      vec3d_cpy((*contact),intersectPoint);
    }
    return 1;
  }
  if (point_in_triangle(
    intersectPoint,
    t1,
    t3,
    t4,
    &normal))
  {
    if (contact)
    {
      vec3d_cpy((*contact),intersectPoint);
    }
    return 1;
  }
  return 0;
}
