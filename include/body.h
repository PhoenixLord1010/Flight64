#ifndef __BODY_H__
#define __BODY_H__

#include "vector.h"
#include "collisions.h"
#include "mgl_callback.h"

enum TYPES {T_PLAYER,T_ENEMY,T_OBJECT,T_WARP,T_NULL};

typedef struct
{
    Vec3D position;
    Vec3D velocity;
    Cube bounds;
    MglCallback touch;
    int  _needsBackoff;
    int  _done;
    Vec3D _stepOffVector;
	int uCheck, uCheck2, dCheck, lCheck, rCheck, fCheck, bCheck;		/**<collision checks*/
	Cube collision;		/**<collision axis points*/
	Vec3D collvec;		/**<collision vector*/
	int tang;			/**<is this tangible?*/
	int hit;			/**<is this a hitbox?*/
	int hurt;			/**<collided with hitbox?*/
	Vec3D hitvec;		/**<hitbox collision location*/
	int type;			/**<player, enemy, object?*/
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

/**
 * @brief clears collision check variables for the next frame
 * @param self the entity to clear from
 */
void clear_collisions(Body *self);

/**
 * @brief finds out the type of collision
 * @param self the first entity
 * @param other the second entity
 * @a the first entity's position
 * @b the second entity's position
 */
void check_collisions(Body *self, Body *other, Cube a, Cube b);

#endif
