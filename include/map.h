#ifndef __MAP_H__
#define __MAP_H__

#include "collisions.h"
#include "bsp.h"
#include "spawn.h"
#include "entity.h"

typedef struct
{
    char *mapname;
    Cube bounds;		/**<absolute world bounds*/
    BSP *bsp;			/**<static world geometry for collision*/
    Obj *obj;			/**<object to draw*/
    int spawnCount;		/**<how many spawn items*/
    Spawn *spawnList;	/**<the list of spawn items*/
}Map;

/**
 * @brief closes current map if one is open, loads the new map and puts the player
 * into it at the position specified by the spawn item (class,id)
 * @param filename the map file to load
 * @param id which of the spawn items to use as the player spawn point
 */
Map *map_load(char *filename);

void map_draw(Map *map);

void map_close(Map *map);

#endif