#ifndef __BODY_H__
#define __BODY_H__

#include "vector.h"
#include "collisions.h"
#include "mgl_callback.h"

typedef struct
{
    Vec3D position;
    Vec3D velocity;
    Cube bounds;
    MglCallback touch;
    int  _needsBackoff;
    int  _done;
    Vec3D _stepOffVector;
}Body;

/**
 * @brief resets a body pre-physics calculation
 * @param body the body to reset
 */
void body_reset(Body *body);

/**
 * @brief applies any post physics processing needed for the body
 * @param body the body to process
 */
void body_process(Body *body);

#endif
