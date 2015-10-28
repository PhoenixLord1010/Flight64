#ifndef __LEVEL__
#define __LEVEL__

/*
Functions for displaying and updating the levels
*/

#include "graphics3d.h"
#include "space.h"

/**
 * @brief calls entity functions for each level and section
 * @param current_level the current level
 * @param space the space to add bodies to
 */
void render_level(int current_level, Space *space);

/**
 * @brief updates current level and section numbers
 * @param space the space to add bodies to
 */
void update_level(Space *space);

#endif