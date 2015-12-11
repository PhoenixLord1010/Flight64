#ifndef __DRAW_H__
#define __DRAW_H__

#include "types.h"
#include <SDL.h>

/**
 * @brief gets the 3d space coordinates based on 2d screen coordinates
 * @param out output if NULL, this is a no-op
 * @param in the input coordinates
 */
void draw_get_3d_position(Vec3D *out,Vec2D in);

/**
 * @brief gets the 2d screen coordinates based on 3d space coordinates
 * @param out output if NULL, this is a no-op
 * @param in the input coordinates
 */
void draw_get_2d_position(Vec2D *out,Vec3D in);

void draw_dot_3D(Vec3D point,
                     float radius,
                     Vec3D color,
                     float alpha);

void draw_circle_3D(Vec3D point,
                        float radius,
                        Uint32  segments,
                        Vec3D color,
                        float alpha);

void draw_circle_2D(Vec2D point,
                        float radius,
                        Uint32  segments,
                        Vec3D color,
                        float alpha);

void draw_line_3D(Vec3D p1,
                      Vec3D p2,
                      float radius,
                      Vec3D color,
                      float alpha);

void draw_line_2D(Vec2D p1,
                      Vec2D p2,
                      float radius,
                      Vec3D color,
                      float alpha);

void draw_tri_3D(Vec3D p1,
                     Vec3D p2,
                     Vec3D p3,
                     Vec3D color,
                     float alpha);

/**
 * @brief draw a rectangle in 3D space described by the 2D float rect
 * @param rect the rect to draw
 * @param radius the thickness of the lines to draw
 * @param ori the orientation to transform the rect by
 */
void draw_rect_3D(RectFloat rect, Uint32 radius, Orientation ori);

void draw_rect_3D_solid(RectFloat rect, Uint32 radius, Orientation ori);

/**
 * @brief draws a rectangle on the screen in 2D space
 * @param rect the defined rectangle to draw
 * @param color the color of the drawn rect
 * @param alpha the translucency of the drawn rect 
 */
void draw_solid_rect(RectFloat rect,Vec3D color,float alpha);

void draw_rect(RectFloat rect,Vec3D color,float alpha);

#endif