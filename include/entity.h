#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "obj.h"
#include "vector.h"
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
    int uid;		/**<unique id of this entity*/
    char name[128];
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

int entity_is_entity(void *data);

void update_entities();

void clear_entities();

Entity *make_player(Vec3D position, Space *space);
void player_think(Entity *self);
void make_spear(Space *space);
void spear_think(Entity *self);
void make_shadow(Body *owner);
void shadow_think(Entity *self);
Entity *spawn_snake(Vec3D position, Space *space, int ck1);
void snake_think(Entity *self);
Entity *spawn_eye(Vec3D position, Space *space, int ck1);
void eye_think(Entity *self);
Entity *eye_spawner(Vec3D position, Space *space, int ck1, int ck2);
void eye_spawner_think(Entity *self);
Entity *spawn_frog(Vec3D position, Space *space);
void frog_think(Entity *self);
Entity *build_cube(Vec3D position, Space *space);
Entity *build_ground(Vec3D position, Space *space);
void *build_road(Vec3D position, Space *space, int n);
Entity *build_spikes(Vec3D position, Space *space, int i, int j);
void spike_think(Entity *self);
Entity *build_spike_base(Vec3D position, Space *space);
Entity *build_platform(Vec3D position1, Vec3D position2, Space *space);
void platform_think(Entity *self);

/*Keyboard Input Stuff*/
void InitKeyboard();
void ClearKeyboard();
void UpdateKeyboard();
int isKeyPressed(int key);
int isKeyReleased(int key);
int isKeyHeld(int key);

#endif
