#include "map.h"
#include <glib.h>

Map *map = NULL;

static void map_deinitialize();

Map *map_load(char *filename)
{
	Spawn *spawn_list;
	int spawn_ct;
	
	if(map != NULL)return map;

	spawn_list = spawn_load(filename,&spawn_ct);
	map = (Map *)malloc(sizeof(Map));
	memset(map,0,sizeof(Map));
	map->mapname = "Test";
	map->spawnList = spawn_list;
	map->spawnCount = spawn_ct;

	return map;
}

void map_deinitialize()
{
	free(map);
}

void map_draw(Map *map)
{
	int i;
	Spawn ent;

	for(i = 0; i < map->spawnCount; i++)
	{
		ent = map->spawnList[i];
		if(ent.uid != -1)
		{
			if(strcmp(ent.classname,"player") == 0)
			{
				make_player(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"snake") == 0)
			{
				spawn_snake(ent.position,ent.a);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"eye") == 0)
			{
				spawn_eye(ent.position,ent.a);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"frog") == 0)
			{
				spawn_frog(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"cube") == 0)
			{
				build_cube(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"ground") == 0)
			{
				build_ground(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"wall") == 0)
			{
				build_wall(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
			if(strcmp(ent.classname,"warp") == 0)
			{
				build_warp(ent.position);
				map->spawnList[i].uid = max_entities()-1;
			}
		}
	}
}

void map_close(Map *map)
{

}
