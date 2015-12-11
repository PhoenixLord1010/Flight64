#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "obj.h"
#include "types.h"
#include "sprite.h"
#include "space.h"

enum STATES {ST_IDLE,ST_WALK,ST_JUMP1,ST_JUMP2,ST_DASH,ST_POUND,ST_DEAD};

typedef struct Entity_S
{
    Obj *objModel;
    Sprite *texture;    /**<object texture*/
    Body body;
	void (*think) (struct ENTITY_S *self);	/**<think function*/
	int inuse;
	int shown;
    int uid;			/**<unique id of this entity*/
    Line name;
	int state;
    Vec3D acceleration;
    Vec3D rotation;
    Vec3D scale;
    Vec4D color;
	float accel;
	int shadow;
	Body *owner;

	int busy;
	int delay;
	int ck1,ck2;

	int health;
	int healthmax;
	int invuln;
}Entity;

/**
 * @brief initialize the entity sub system
 * @param maxEntity the maximum number of simultaneously supported entities.
 */
void entity_init(int maxEntity);

/**
 * @brief get a pointer to a new entity
 * @return NULL on no more entities or error,  a valid entity pointer otherwise
 */
Entity *entity_new();

/**
 * @brief draws all active entities
 */
void entity_draw_all();

/**
 * @brief draws an entity
 * @param ent the entity to draw
 */
void entity_draw(Entity *ent);

/**
 * @brief frees an entity
 */
void entity_free(Entity *ent);

/**
 * @brief checks if this is an entity
 * @param data the data in question
 */
int entity_is_entity(void *data);

/**
 * @brief updates think functions
 */
void update_entities();

/**
 * @brief clears all entities
 */
void clear_entities();

/**
 * @brief clears all but one entity
 * @param ent entity not to clear
 */
void clear_entities_except(Entity *ent);


/**
 * @brief creates the player entity
 * @param position the starting position
 */
Entity *make_player(Vec3D position);

/**
 * @brief player think function
 * @param self the player entity
 */
void player_think(Entity *self);

/**
 * @brief creates the spear entity
 */
void make_spear();

/**
 * @brief spear think function
 * @param self the spear entity
 */
void spear_think(Entity *self);

/**
 * @brief create shadow entity
 * @param owner the entity this shadow belongs to
 */
void make_shadow(Body *owner);

/**
 * @brief shadow think function
 * @param self the shadow entity
 */
void shadow_think(Entity *self);

/**
 * @brief create snake enemy entity
 * @param position the starting position
 * @param ck1 the direction it circles in
 */
Entity *spawn_snake(Vec3D position, int ck1);

/**
 * @brief snake enemy think function
 * @param self this snake entity
 */
void snake_think(Entity *self);

/**
 * @brief create eye enemy entity
 * @param position the starting position
 * @param ck1 which direction will it start going
 */
Entity *spawn_eye(Vec3D position, int ck1);

/**
 * @brief eye enemy think function
 * @param self this eye entity
 */
void eye_think(Entity *self);

/**
 * @brief create eye enemy spawn point entity
 * @param position the spawn position
 * @param ck1 which direction will they be going
 * @param ck2 spawn rate
 */
Entity *eye_spawner(Vec3D position, int ck1, int ck2);

/**
 * @brief eye spawner think function
 * @param self this eye spawner entity
 */
void eye_spawner_think(Entity *self);

/**
 * @brief create frog enemy entity
 * @param position the starting position
 */
Entity *spawn_frog(Vec3D position);

/**
 * @brief frog enemy think function
 * @param self this frog entity
 */
void frog_think(Entity *self);

/**
 * @brief create cube entity
 * @param position the starting position
 */
Entity *build_cube(Vec3D position);

/**
 * @brief create ground entity
 * @param position the starting position
 */
Entity *build_ground(Vec3D position);

/**
 * @brief create multiple ground entities in a row
 * @param position the starting position
 * @param n the amount to create
 */
void *build_road(Vec3D position, int n);

/**
 * @brief create wall entity
 * @param position the starting position
 */
Entity *build_wall(Vec3D position);

/**
 * @brief create spike entity
 * @param position the starting position
 * @param i the speed of the spikes
 * @param j the offset
 */
Entity *build_spikes(Vec3D position, int i, int j);

/**
 * @brief spike entity think function
 * @param self this spike entity
 */
void spike_think(Entity *self);

/**
 * @brief create spike base entity
 * @param position the starting position
 */
Entity *build_spike_base(Vec3D position);

/**
 * @brief create moving platform entity
 * @param position1 the first position
 * @param position2 the second position
 */
Entity *build_platform(Vec3D position1, Vec3D position2);

/**
 * @brief moving platform think function
 * @param self this moving platform entity
 */
void platform_think(Entity *self);

/**
 * @brief basic object think function
 * @param self this entity
 */
void object_think(Entity *self);

/**
 * @brief create warp pad entity
 * @param position the starting position
 */
Entity *build_warp(Vec3D position);

/**
 * @brief warp pad entity think function
 * @param self this warp pad entity
 */
void warp_think(Entity *self);

/*Keyboard Input Stuff*/
void InitKeyboard();
void ClearKeyboard();
void UpdateKeyboard();
int isKeyPressed(int key);
int isKeyReleased(int key);
int isKeyHeld(int key);

#endif
