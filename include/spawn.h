#ifndef __SPAWN_H__
#define __SPAWN_H__

#include "vector.h"

typedef struct
{
    char *classname;		/**<name of the entity to spawn*/
	Vec3D position;			/**<position to spawn at*/
	int a;
	int b;
	Vec3D position2;
}Spawn;

void spawn_init(int max_spawn);
Spawn spawn_load(char *filename);
void spawn_save(Spawn *spawn_list, char *filename);


#endif