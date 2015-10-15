#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "collisions.h"
#include "body.h"

typedef struct Entity_S
{
    Obj *objModel;
    Sprite *texture;    /**<object texture*/
    Body body;
	void (*think) (struct ENTITY_S *self);	/**<think function*/
	int inuse;
    int uid;		/**<unique id of this entity*/
    char name[128];
    Vec3D acceleration;
    Vec3D rotation;
    Vec3D scale;
    Vec4D color;
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

void clear_collisions(Body *self);
void check_collisions(Body *self, Cube a, Cube b);

Entity *make_player(Vec3D position);
void player_think(Entity *self);

/*Keyboard Input Stuff*/
void InitKeyboard();
void ClearKeyboard();
void UpdateKeyboard();
int isKeyPressed(int key);
int isKeyReleased(int key);
int isKeyHeld(int key);

#endif
