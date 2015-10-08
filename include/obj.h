#ifndef __OBJ_H__
#define __OBJ_H__
/**
 * obj
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "model.h"
#include "vector.h"

/**
 * @purpse provide an interface to load and parse object files into the model data type
 */

typedef struct TriPoint_S
{
    GLuint v;   /**<vertex index*/
    GLuint n;   /**<normal index*/
    GLuint t;   /**<texel index*/
}TriPoint;

typedef struct
{
    TriPoint p[3];
}ObjTriangle;

typedef struct
{
    char   filename[512];
    
    Uint32 num_tris;
    Uint32 num_vertices;
    Uint32 num_texels;
    Uint32 num_normals;
    
    float *vertex_array;
    float *texel_array;
    float *normal_array;
    ObjTriangle *triangle_array;
    
    Uint8 used;             /**<refcount*/
}Obj;

/**
 * @brief initialize the object subsystem
 */
void obj_init();

/**
 * @brief load an object file into a model
 * @param filename the object file to parse
 * @return NULL on failure or a model pointer otherwise;
 */
Model *obj_load_model(char *filename);

/**
 * @brief load an object file into an obj
 * @param filename the object file to parse
 * @return NULL on failure or an Obj pointer otherwise;
 */
Obj *obj_load(char *filename);

/**
 * @brief free an object from memory if no longer in use
 * @param obj the object to free
 */
void obj_free(Obj *obj);

/**
 * @brief draw an object primitive
 * @param obj the obj to draw
 * @param position the position to draw it at
 * @param rotation the yaw, pitch and roll of the obj to draw
 * @param scale the x,y,z stretch of the obj.  1,1,1 is no deformation
 * @param color the r,g,b,a value for the model as a whole
 * @param texture [optional] if provided, render with texture
 */
void obj_draw(
    Obj *obj,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec4D color,
    Sprite *texture
);


#endif