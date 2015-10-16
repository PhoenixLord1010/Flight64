#ifndef __LEVEL__
#define __LEVEL__

/*
Functions for displaying and updating the levels
*/

#include "graphics3d.h"
#include "space.h"

void render_level(int current_level, Space *space);
void update_level(Space *space);

#endif