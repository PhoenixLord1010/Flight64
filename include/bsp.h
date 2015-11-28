#ifndef __BSP_H__
#define __BSP_H__

#include "vector.h"
#include "obj.h"

#define BSP_L 0
#define BSP_R 1

typedef struct BSP_S
{
    Vec3D triangle[3];/**<triangle*/
    Vec3D normal;   /**<plane normal*/
    float d;        /**<plane distance from origin*/
    struct BSP_S *children[2];
}BSP;


/**
 * @brief converts obj data into a BSP
 * @param object the object to convert
 * @return NULL on error or an allocated and built binary space partition
 */
BSP *bsp_convert_from_obj(Obj *object);

/**
 * @brief saves BSP data to file
 * @param bsp the bsp to save to file
 * @param filename the file to save to
 */
void bsp_save_to_file(BSP *bsp, char * filename);

/**
 * @brief create and load a BSP from file
 * @param filename the bsp file to load
 * @return NULL on error or a valid BSP otherwise
 */
BSP *bsp_load_from_file(char * filename);

/**
 * @brief checks to see if the cube provided intersects with a polygon in the bsp
 * @param bsp the bsp to check against
 * @param cube the cube to check
 * @return false on no collision or error, true if there were a collision
 */
int bsp_cube_collision(BSP *bsp,Cube *cube );

/**
 * @brief checks for a ray intersection with the bsp
 * @param bsp the bsp to check
 * @param start the starting point of the ray to check
 * @param end the ending point of the ray to check
 * @return 1 on no collision, less than 1 collides
 */
float bsp_ray_collision(BSP *bsp,Vec3D start, Vec3D end);

#endif