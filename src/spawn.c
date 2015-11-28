#include "spawn.h"
#include "simple_logger.h"
#include <glib.h>
#include <gstdio.h>
#include <stdlib.h>
#include <string.h>

Spawn *spawn_list = NULL;
int spawn_max = 0;
int spawn_initialized = 0;
int spawn_ct = 0;

static void spawn_deinitialize();

void spawn_init(int max_spawn)
{
    if (spawn_initialized)
    {
        slog("already initialized");
        return;
    }
    spawn_list = (Spawn *)malloc(sizeof(Spawn)*max_spawn);
    memset(spawn_list,0,sizeof(Spawn)*max_spawn);
    spawn_max = max_spawn;
    spawn_initialized = 1;
    slog("initialized %i entities",max_spawn);
    atexit(spawn_deinitialize);
}

static void spawn_deinitialize()
{
    free(spawn_list);
    spawn_max = 0;
    spawn_initialized = 0;
}

Spawn spawn_load(char *filename)
{
	gchar *contents = NULL;
	gsize length = NULL;
	GError *error = NULL;
	char *pch;
	int i = 0;

	g_file_get_contents(filename,&contents,&length,&error);
	
	pch = strtok(contents,",");
	while(pch != NULL)
	{
		spawn_list[i].classname = pch;
		pch = strtok(NULL,",");
		spawn_list[i].position.x = atof(pch);
		pch = strtok(NULL,",");
		spawn_list[i].position.y = atof(pch);
		pch = strtok(NULL,",");
		spawn_list[i].position.z = atof(pch);
		pch = strtok(NULL,",");
		spawn_list[i].a = (int)(pch[0] - '0');
		pch = strtok(NULL,",");
		spawn_list[i].b = (int)(pch[0] - '0');
		pch = strtok(NULL,",");
		spawn_list[i].position2.x = atof(pch);
		pch = strtok(NULL,",");
		spawn_list[i].position2.y = atof(pch);
		pch = strtok(NULL,",");
		spawn_list[i].position2.z = atof(pch);
		pch = strtok(NULL,",");
		i++;
	}
}
