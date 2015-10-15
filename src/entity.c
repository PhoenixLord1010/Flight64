#include "entity.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <math.h>
#include <glut.h>
#include <glew.h>

static Entity *__entity_list = NULL;
static int __entity_max = 0;
static int __entity_initialized = 0;
Entity *Player = NULL;

int KeyCount;
Uint8 *oldkeys; /*last frame's key presses*/
Uint8 *keys;    /*current frame's key presses*/

static void entity_deinitialize();

void entity_init(int maxEntity)
{
    if (__entity_initialized)
    {
        slog("already initialized");
        return;
    }
    __entity_list = (Entity *)malloc(sizeof(Entity)*maxEntity);
    memset(__entity_list,0,sizeof(Entity)*maxEntity);
    __entity_max = maxEntity;
    __entity_initialized = 1;
    slog("initialized %i entities",maxEntity);
    atexit(entity_deinitialize);
}

static void entity_deinitialize()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_free(&__entity_list[i]);
        }
    }
    free(__entity_list);
    __entity_max = 0;
    __entity_initialized = 0;
}

void entity_free(Entity *ent)
{
    if (!ent)
    {
        slog("passed a null entity");
        return;
    }
    ent[0].inuse = 0;
    obj_free(ent->objModel);
    FreeSprite(ent->texture);
}

Entity *entity_new()
{
    int i;
    for (i = 0; i < __entity_max;i++)
    {
        if (!__entity_list[i].inuse)
        {
            memset(&__entity_list[i],0,sizeof(Entity));
            __entity_list[i].inuse = 1;
            vec3d_set(__entity_list[i].scale,1,1,1);
            vec4d_set(__entity_list[i].color,1,1,1,1);
            return &__entity_list[i];
        }
    }
    return NULL;
}

void entity_draw_all()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_draw(&__entity_list[i]);
        }
    }
}

void entity_draw(Entity *ent)
{
    if (!ent)
    {
        return;
    }
    obj_draw(
        ent->objModel,
        ent->body.position,
        ent->rotation,
        ent->scale,
        ent->color,
        ent->texture
    );
}

int entity_is_entity(void *data)
{
    if (!data)return 0;
    if (!__entity_initialized)return 0;
    if ((Entity *)data < __entity_list)return 0;
    if ((Entity *)data >= (__entity_list + __entity_max))return 0;
    return 1;
}

void update_entities()
{
	int i;

	for(i = 0; i < __entity_max; i++)
	{
		if(__entity_list[i].inuse)
		{
			if(__entity_list[i].think != NULL)
			{
				__entity_list[i].think(&__entity_list[i]);
			}
		}
	}
}

void clear_collisions(Body *self)
{
	self->uCheck = 0;
	self->dCheck = 0;
	self->lCheck = 0;
	self->rCheck = 0;
	self->fCheck = 0;
	self->bCheck = 0;
	vec3d_set(self->collision,0,0,0);
	self->collision.w = 0;
	self->collision.h = 0;
	self->collision.d = 0;
}

void check_collisions(Body *self, Cube a, Cube b)
{
	if((abs((b.y+b.h)-a.y) <= abs((a.x+a.w)-b.x)) && (abs((b.y+b.h)-a.y) <= abs((b.x+b.w)-a.x)) && (abs((b.y+b.h)-a.y) <= abs((a.z+a.d)-b.z)) && (abs((b.y+b.h)-a.y) <= abs((b.z+b.d)-a.z)) && (abs((b.y+b.h)-a.y) <= abs((a.y+a.h)-b.y)))
	{
		self->uCheck = 1;
		self->collision.h = b.y+b.h;
	}
	else if((abs((a.x+a.w)-b.x) <= abs((b.x+b.w)-a.x)) && (abs((a.x+a.w)-b.x) <= abs((a.z+a.d)-b.z)) && (abs((a.x+a.w)-b.x) <= abs((b.z+b.d)-a.z)) && (abs((a.x+a.w)-b.x) <= abs((a.y+a.h)-b.y)))
	{
		self->lCheck = 1;
		self->collision.x = b.x;
	}
	else if((abs((b.x+b.w)-a.x) <= abs((a.z+a.d)-b.z)) && (abs((b.x+b.w)-a.x) <= abs((b.z+b.d)-a.z)) && (abs((b.x+b.w)-a.x) <= abs((a.y+a.h)-b.y)))
	{
		self->rCheck = 1;
		self->collision.w = b.x+b.w;
	}
	else if((abs((a.z+a.d)-b.z) <= abs((b.z+b.d)-a.z)) && (abs((a.z+a.d)-b.z) <= abs((a.y+a.h)-b.y)))
	{
		self->fCheck = 1;
		self->collision.z = b.z;
	}
	else if((abs((b.z+b.d)-a.z) <= abs((a.y+a.h)-b.y)))
	{
		self->bCheck = 1;
		self->collision.d = b.z+b.d;
	}
	else
	{
		self->dCheck = 1;
		self->collision.y = b.y;
	}
}


Entity *make_player(Vec3D position)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
    ent->think = player_think;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
	Player = ent;
	return ent;
}

void player_think(Entity *self)
{
	SDL_Event e;
	
	if(self->body.uCheck)
	{
		self->body.velocity.y = 0;
		self->body.position.y = self->body.collision.h + self->body.bounds.h/2;
	}
	else
	{
		if(self->body.velocity.y > -2)self->body.velocity.y -= 0.04;
	}
	if(self->body.lCheck)
	{
		self->body.velocity.x = 0;
		self->body.position.x = self->body.collision.x - self->body.bounds.w/2;
	}
	if(self->body.rCheck)
	{
		self->body.velocity.x = 0;
		self->body.position.x = self->body.collision.w + self->body.bounds.w/2;
	}

	if(isKeyHeld(SDL_SCANCODE_SPACE) && self->body.uCheck)
	{
		self->body.velocity.y += 0.5;
		self->body.uCheck = 0;
	}
	if(isKeyHeld(SDL_SCANCODE_A) || isKeyHeld(SDL_SCANCODE_D))
	{
		if(isKeyHeld(SDL_SCANCODE_D))
		{
			if(self->body.velocity.x < 1)self->body.velocity.x += 0.01;
		}
		if(isKeyHeld(SDL_SCANCODE_A))
		{
			if(self->body.velocity.x > -1)self->body.velocity.x -= 0.01;
		}
	}
	else
	{
		self->body.velocity.x = 0;
	}
}

void InitKeyboard()
{
  keys = SDL_GetKeyboardState(&KeyCount);
  oldkeys = (Uint8 *)malloc(sizeof(Uint8)*KeyCount);
  if(oldkeys == NULL)
  {
    slog("problem");
  }
}

void ClearKeyboard()
{
  if(oldkeys == NULL)return;
  memset(oldkeys,0,sizeof(Uint8)*KeyCount);
}

void UpdateKeyboard()
{
  int i;
  if((oldkeys == NULL)||(keys == NULL))
  {
    return;
  }
  for(i = 0; i < KeyCount;i++)
  {
    oldkeys[i] = keys[i];
  }
  keys = SDL_GetKeyboardState(NULL);
}

int isKeyPressed(int key)
{
  if((oldkeys == NULL)||(keys == NULL))
  {
    return 0;
  }
  if((keys[key]) && (!oldkeys[key]))
  {
    return 1;
  }
  return 0;
}

int isKeyReleased(int key)
{
  if((oldkeys == NULL)||(keys == NULL))
  {
    return 0;
  }
  if((!keys[key]) && (oldkeys[key]))
  {
    return 1;
  }
  return 0;
}

int isKeyHeld(int key)
{
  if((oldkeys == NULL)||(keys == NULL))
  {
    return 0;
  }
  if((keys[key]) && (oldkeys[key]))
  {
    return 1;
  }
  return 0;
}
