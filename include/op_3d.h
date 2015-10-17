#ifndef __OP_3D__
#define __OP_3D__

#include "vector.h"

/*edge operations*/

/**
 * @brief test if a ray intersects an edge in 2d space
 * @param start the starting point of the ray
 * @param v the vecctor of the ray
 * @param e1 one end point of the edge
 * @param e2 one end point of the edge
 * @param contact output optional.  if provided this will be set with the point
 *                of intersection
 *                if ray and edge are not parallel, this will still be set to
 *                the intersection point if the lines were infinite
 * @return eolTrue if the ray intersects the edge, eolFalse otherwise
 * 
 */
int ray_cross_edge(
  Vec2D start,
  Vec2D v,
  Vec2D e1,
  Vec2D e2,
  Vec3D *contact);

float edge_length_3d(Vec3D e1,Vec3D e2);

/*ray casting*/

/**
 * @brief get the normal described by the triangle defined by the 3 points passed
 * @param t1 one triangle vertex
 * @param t2 one triangle vertex
 * @param t3 one triangle vertex
 * @param normal output normal vector for the triangle
 */
void get_triangle_normal(
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D *normal);

/**
 * @brief given a triangle's 3 points, calculate the normal and distance from the origin
 * @param t1 one triangle vertex
 * @param t2 one triangle vertex
 * @param t3 one triangle vertex
 * @param normal output normal vector for the triangle
 * @param D output distance of the plane from origin
 */
void get_triangle_plane(
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D *normal,
  float *D);

/**
 * @brief calculate the contact point of the ray with the provided plane
 * @param start starting point of the ray
 * @param dir ray vector
 * @param normal the normal describing the plane
 * @param D the distance of the plane from origin
 * @param contact output optional the point where the ray would intersect the plane
 * @return 0 ray is perpendicular to the plane, it will never intersect
 *         < 0 ray is pointing away from the plane  Contact point is still set where the ray would have passed through
 *         > 0 to 1 ray intersects plane contact point is the intersection point
 *         > 1 ray does not intersect, but it would if it were longer by this factor
 */
float ray_in_plane(
  Vec3D start,
  Vec3D dir,
  Vec3D normal,
  float D,
  Vec3D *contact);

/**
 * @brief given a point on the same plane of the triangle, determine if it is
 * inside the triangle or outside
 * @param point the point to test
 * @param t1 one triangle vertex
 * @param t2 one triangle vertex
 * @param t3 one triangle vertex
 * @param normal optional if this is provided, normal calculation can be skipped
 */
int point_in_triangle(
  Vec3D point,
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D *normal);

/**
 * @brief given a quad and a ray, determine if they intersect
 * @param start starting point of ray
 * @param dir vector of the ray
 * @param t1 one quad vertex
 * @param t2 one quad vertex
 * @param t3 one quad vertex
 * @param t4 one quad vertex
 * @param contact output optional the point where the ray would intersect the quad
 * @return eolTrue if the ray intersects the quad, eolFalse otherwise.
 *         contact is not set if there is not intersection
 */
int ray_in_quad(
  Vec3D start,
  Vec3D dir,
  Vec3D t1,
  Vec3D t2,
  Vec3D t3,
  Vec3D t4,
  Vec3D *contact);

#endif