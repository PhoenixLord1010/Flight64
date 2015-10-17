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




Entity *make_player(Vec3D position, Space *space)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/player.obj");
    ent->texture = LoadSprite("models/player_text.png",1024,1024);
    ent->think = player_think;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1.5,-1,2,3,2);
	space_add_body(space,&ent->body);
	Player = ent;
	return ent;
}

void player_think(Entity *self)
{
	SDL_Event e;
	float speed = 1;
	float accel = 0.2;
	float decel = 0.01;
	float rot = 4;
	Vec3D down = {0,-1,0};
	
	/*Collisions*/
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

	/*Player Inputs*/
	if(isKeyHeld(SDL_SCANCODE_W) || isKeyHeld(SDL_SCANCODE_S))		/*Move Forward/Back*/
	{
		if(isKeyHeld(SDL_SCANCODE_W))
		{
			if(self->body.velocity.z > -speed)
			{
				vec3d_add(
						self->body.position,
						self->body.position,
                        vec3d(
                            -sin(self->rotation.y * DEGTORAD) * accel,
                            0,
                            -cos(self->rotation.y * DEGTORAD) * accel
                        ));
			}
			//if(self->body.velocity.z < -speed)self->body.velocity.z = -speed;
		}
		if(isKeyHeld(SDL_SCANCODE_S))
		{
			if(self->body.velocity.z < speed)
			{
				vec3d_add(
						self->body.position,
						self->body.position,
                        vec3d(
                            sin(self->rotation.y * DEGTORAD) * accel,
                            0,
                            cos(self->rotation.y * DEGTORAD) * accel
                        ));
			}
			//if(self->body.velocity.z > speed)self->body.velocity.z = speed;
		}
	}
	else
	{
		if(abs(self->body.velocity.z) > decel)
		{
			if(self->body.velocity.z >= decel)self->body.velocity.z -= decel;
			if(self->body.velocity.z <= -decel)self->body.velocity.z += decel;
		}
		else self->body.velocity.z = 0;
	}

	if(isKeyHeld(SDL_SCANCODE_A) || isKeyHeld(SDL_SCANCODE_D))		/*Turn Left/Right*/
	{
		if(isKeyHeld(SDL_SCANCODE_A))	/*Left*/
		{
			self->rotation.y += rot;
		}
		if(isKeyHeld(SDL_SCANCODE_D))	/*Right*/
		{
			self->rotation.y -= rot;
		}
	}
	
	if(isKeyHeld(SDL_SCANCODE_SPACE) && self->body.uCheck)		/*Jump*/
	{
		self->body.velocity.y += 0.5;
	}
}

Entity *build_cube(Vec3D position, Space *space)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
	space_add_body(space,&ent->body);
    return ent;
}

Entity *build_ground(Vec3D position, Space *space)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/ground.obj");
    ent->texture = LoadSprite("models/ground_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-8,-1,-1,16,2,2);
	space_add_body(space,&ent->body);
    return ent;
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
