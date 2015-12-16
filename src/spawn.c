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

Spawn *spawn_load(char *filename, int *ct)
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
		pch = strtok(NULL,"\n");
		pch = strtok(NULL,",");
		i++;
		spawn_ct++;
	}

	*ct = spawn_ct;
	return spawn_list;
}

void spawn_save(char *filename)
{
	int i,j,n;
	int ct = 0;
	char contents[5012] = "";
	char fch[10];
	GError *error = NULL;

	for(i = 0; i < spawn_ct; i++)
	{
		for(j = 0; j < strlen(spawn_list[i].classname); j++)
		{
			contents[ct] = spawn_list[i].classname[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;
		
		n = sprintf(fch,"%.2f",spawn_list[i].position.x);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;

		n = sprintf(fch,"%.2f",spawn_list[i].position.y);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;

		n = sprintf(fch,"%.2f",spawn_list[i].position.z);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;
		
		n = sprintf(fch,"%d",spawn_list[i].a);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;
		
		n = sprintf(fch,"%d",spawn_list[i].b);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;
		
		n = sprintf(fch,"%.2f",spawn_list[i].position2.x);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;

		n = sprintf(fch,"%.2f",spawn_list[i].position2.y);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;

		n = sprintf(fch,"%.2f",spawn_list[i].position2.z);
		for(j = 0; j < n; j++)
		{
			contents[ct] = fch[j];
			ct++;
		}
		contents[ct] = ',';
		ct++;
	}

	g_file_set_contents(filename,contents,-1,&error);
}

char * get_name(int id)
{
	int i;

	for(i = 0; i < spawn_ct; i++)
	{
		if(spawn_list[i].uid == id)return spawn_list[i].classname;
	}
}