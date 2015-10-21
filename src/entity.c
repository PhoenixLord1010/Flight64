#include "entity.h"
#include "op_3d.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <math.h>
#include <glut.h>
#include <glew.h>

static Entity *__entity_list = NULL;
static int __entity_max = 0;
static int __entity_initialized = 0;
static int __uid = 0;

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
			__entity_list[i].shown = 1;
            vec3d_set(__entity_list[i].scale,1,1,1);
            vec4d_set(__entity_list[i].color,1,1,1,1);
			__entity_list[i].uid = __uid;
			__uid++;
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
	if(ent->shown)
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

void clear_entities()
{
	int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_free(&__entity_list[i]);
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
	ent->state = ST_IDLE;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1.5,-1,2,3,2);
	ent->body.tang = 1;
	ent->body.hit = 0;
	ent->body.hurt = 0;
	ent->body.type = ST_PLAYER;
	ent->accel = 0;
	ent->shadow = 0;
	ent->busy = 0;
	ent->healthmax = 5;
	ent->health = ent->healthmax;
	ent->invuln = 0;
	space_add_body(space,&ent->body);
	Player = ent;
	make_spear(space);
	return ent;
}

void player_think(Entity *self)
{
	float speed = 0.2;
	float accel = 0.02;
	float decel = 0.01;
	float rot = 4;
	float grav = -2;
	float weight = 0.04;
	float dash = 0.4;
	
	if(self->body.hurt && !self->invuln)
	{
		self->health--;
		self->body.hurt = 0;
		self->invuln = 30;
		self->rotation.y = atan2(self->body.position.x-self->body.hitvec.x,self->body.position.z-self->body.hitvec.z) * RADTODEG;
		self->body.velocity.y = 0.5;
		self->body.uCheck = 0;
		self->accel = -0.2;
	}

	/*Do while alive*/
	if(self->health > 0)
	{
		/*Collisions*/
		if(self->body.uCheck)
		{
			self->body.velocity.y = 0;
			self->body.position.y = self->body.collision.h + self->body.bounds.h*0.5;
		}
		if(self->body.dCheck)
		{
			self->body.velocity.y = -2;
			self->body.position.y = self->body.collision.y - self->body.bounds.h*0.5;
		}
		if(self->body.lCheck)
		{
			self->body.velocity.x = 0;
			self->body.position.x = self->body.collision.x - self->body.bounds.w*0.5;
		}
		if(self->body.rCheck)
		{
			self->body.velocity.x = 0;
			self->body.position.x = self->body.collision.w + self->body.bounds.w*0.5;
		}
		if(self->body.fCheck)
		{
			self->body.velocity.z = 0;
			self->body.position.z = self->body.collision.z - self->body.bounds.d*0.5;
		}
		if(self->body.bCheck)
		{
			self->body.velocity.z = 0;
			self->body.position.z = self->body.collision.d + self->body.bounds.d*0.5;
		}
		
		if(self->body.uCheck && !self->busy)
		{
			if(vec3d_and(self->body.velocity,vec3d(0,0,0)))
				self->state = ST_IDLE;
			else if(self->state != ST_DASH)
				self->state = ST_WALK;
		}

		if(self->body.uCheck2 && !self->body.uCheck)		/*Walking off Edges*/
		{
			if(self->state != ST_DASH)
			{
				self->state = ST_JUMP1;
			}
		}

		/*Player Inputs*/
		if(self->invuln <= 15)
		{
			if((self->state != ST_DASH && self->state != ST_POUND) || !self->busy)
			{
				if(isKeyHeld(SDL_SCANCODE_W) || isKeyHeld(SDL_SCANCODE_S))		/*Move Forward/Back*/
				{
					if(pow(self->body.velocity.x,2)+pow(self->body.velocity.z,2) < pow(speed,2))
					{
						if(isKeyHeld(SDL_SCANCODE_W))		/*Forward*/
						{
							self->accel += accel;
						}
						if(isKeyHeld(SDL_SCANCODE_S))		/*Back*/
						{
							self->accel -= accel;
						}
					}
					else
					{
						if(self->accel > 0)self->accel = speed;
						else self->accel = -speed;
					}
				}
				else
				{
					if((pow(self->body.velocity.x,2)+pow(self->body.velocity.z,2)) > pow(decel,2))
					{
						if(self->accel > 0)self->accel -= decel;
						else self->accel += decel;
					}
					else
					{
						self->accel = 0;
					}
				}
			}

			if((isKeyHeld(SDL_SCANCODE_A) || isKeyHeld(SDL_SCANCODE_D)) && self->state != ST_POUND)		/*Turn Left/Right*/
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
	
			if(isKeyHeld(SDL_SCANCODE_SPACE) && self->body.velocity.y <= 0 && self->state != ST_JUMP2 && self->state != ST_POUND)	/*Jump*/
			{
				if(self->state == ST_JUMP1)
				{
					self->state = ST_JUMP2;
					self->body.velocity.y = 0.6;
				}
				else 
				{
					self->state = ST_JUMP1;
					self->body.velocity.y = 0.5;
				}
				if(self->shadow == 0)
				{
					make_shadow(&self->body);
					self->shadow++;
				}
			}

			if(isKeyHeld(SDL_SCANCODE_SPACE) && self->body.velocity.y < 0 && self->state != ST_POUND)		/*Float*/
			{
				weight = 0.001;
			}

			if(isKeyHeld(SDL_SCANCODE_I) && !self->busy)	
			{
				if(self->body.uCheck)		/*Dash*/
				{
					self->state = ST_DASH;
					self->busy = 15;
				}
				else						/*Pound*/
				{
					self->state = ST_POUND;
					self->busy = 40;
				}
			}
			if(self->state == ST_DASH && self->busy)
			{
				self->accel = dash;
			}
			if(self->state == ST_POUND && self->busy)
			{
				self->accel = 0;
				if(!self->body.uCheck)self->body.velocity.y = -0.8;
			}
		}

		if(!self->body.uCheck)		/*In the Air*/
		{
			if(self->body.velocity.y > grav)self->body.velocity.y -= weight;
			if(self->shadow == 0)
			{
				make_shadow(&self->body);
				self->shadow++;
			}
		}

		/*Movement*/
		self->body.velocity.x = -sin(self->rotation.y * DEGTORAD) * self->accel;
		self->body.velocity.z = -cos(self->rotation.y * DEGTORAD) * self->accel;
		if(self->body.lCheck && self->body.velocity.x > 0)self->body.velocity.x = 0;
		if(self->body.rCheck && self->body.velocity.x < 0)self->body.velocity.x = 0;
		if(self->body.fCheck && self->body.velocity.z > 0)self->body.velocity.z = 0;
		if(self->body.bCheck && self->body.velocity.z < 0)self->body.velocity.z = 0;

		if(self->busy > 0)self->busy--;		/*Attack Counter*/
		if(self->invuln > 0)self->invuln--;		/*Invulnerability Counter*/
		self->body.uCheck2 = self->body.uCheck;		/*Last frame's uCheck*/

		if(self->rotation.y > 180)self->rotation.y -= 360;	/*Rotation Fix*/
		if(self->rotation.y <= -180)self->rotation.y += 360;
		self->body.hurt = 0;
	}
	else		/*Do While Dead*/
	{
		if(self->state != ST_DEAD)
		{
			self->body.velocity.y = 1;
			self->state = ST_DEAD;
			self->body.tang = 0;
		}
		vec3d_set(self->rotation,180,self->rotation.y,0);
		self->body.velocity.x = 0;
		self->body.velocity.y -= 0.1;
		self->body.velocity.z = 0;
	}

	if(self->body.position.y < -20)		/*"Respawn"*/
	{
		self->health = 5;
		self->state = ST_IDLE;
		self->body.tang = 1;
		self->body.hurt = 0;
		self->accel = 0;
		vec3d_set(self->body.position,8,0,0);
		vec3d_set(self->rotation,0,0,0);
		vec3d_set(self->body.velocity,0,0,0);
	}
}

void make_spear(Space *space)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return;

    ent->objModel1 = obj_load("models/spear.obj");
	ent->objModel2 = obj_load("models/spear2.obj");
	ent->objModel = ent->objModel1;
    ent->texture = LoadSprite("models/spear_text.png",1024,1024);
    ent->think = spear_think;
	ent->shown = 0;
	vec3d_cpy(ent->body.position,
			vec3d((Player->body.position.x+(-sin(ent->rotation.y * DEGTORAD))),
			Player->body.position.y,
			(Player->body.position.z+(-cos(ent->rotation.y * DEGTORAD)))));
    cube_set(ent->body.bounds,-0.375,-0.375,-0.375,0.75,0.75,0.75);
	ent->body.tang = 0;
	ent->body.hit = 0;
	ent->body.type = ST_PLAYER;
	ent->accel = 0;
	ent->busy = 0;
	ent->invuln = 0;
	space_add_body(space,&ent->body);
	return;
}

void spear_think(Entity *self)
{
	float dist;
	Obj *s;
	
	if(Player == NULL)entity_free(self);
	else
	{
		/*Move with Player*/
		if(self->busy <= 0)
		{
			vec3d_cpy(self->body.position,
					vec3d((Player->body.position.x+(-sin(self->rotation.y * DEGTORAD))),
					Player->body.position.y,
					(Player->body.position.z+(-cos(self->rotation.y * DEGTORAD)))));
			vec3d_cpy(self->rotation,Player->rotation);
			self->objModel = self->objModel1;
		}

		if(Player->health > 0)
		{
			if(isKeyHeld(SDL_SCANCODE_O) && self->busy <= 0 && self->busy > -11)
			{
				self->busy--;
			}
			else
			{
				if(self->busy < -10)
				{
					self->shown = 1;
					self->body.hit = 1;
					self->busy = 30;
					self->shadow = 21;
				}
				if(self->busy < 0)
				{
					self->shown = 1;
					self->body.hit = 1;
					self->busy = 20;
					self->shadow = 16;
				}
			}
			if(isKeyHeld(SDL_SCANCODE_I) && !self->busy)
			{
				if(Player->body.uCheck)
				{
					self->shown = 1;
					self->body.hit = 1;
					self->busy = 40;
					self->state = ST_DASH;
				}
				else
				{
					/*self->objModel = self->objModel2;
					self->shown = 1;
					self->body.hit = 1;
					self->busy = 40;
					self->state = ST_POUND;*/
				}
			}

			if(Player->state != ST_DASH && self->state == ST_DASH)
			{
				self->busy = 0;
				self->state = ST_IDLE;
			}

			if(self->busy > 12)
			{
				if(Player->state != ST_DASH && Player->state != ST_POUND)
				{
					vec3d_cpy(self->rotation,Player->rotation);
					dist = sqrt(pow(self->body.position.x - Player->body.position.x,2) + pow(self->body.position.z - Player->body.position.z,2));
					self->body.position.x = (-sin(self->rotation.y * DEGTORAD) * dist) + Player->body.position.x;
					self->body.position.y = Player->body.position.y;
					self->body.position.z = (-cos(self->rotation.y * DEGTORAD) * dist) + Player->body.position.z;
					self->body.velocity.x = -sin(self->rotation.y * DEGTORAD) * (self->busy-self->shadow) * 0.05 + Player->body.velocity.x;
					self->body.velocity.z = -cos(self->rotation.y * DEGTORAD) * (self->busy-self->shadow) * 0.05 + Player->body.velocity.z;
				}
				else if(Player->state == ST_DASH)
				{
					vec3d_cpy(self->rotation,Player->rotation);
					vec3d_cpy(self->body.position,
							vec3d((Player->body.position.x + (-sin(self->rotation.y * DEGTORAD) * 2)),
							Player->body.position.y,
							(Player->body.position.z + (-cos(self->rotation.y * DEGTORAD) * 2))));
				}
				else
				{
					vec3d_cpy(self->rotation,Player->rotation);
					vec3d_cpy(self->body.position,
							vec3d((Player->body.position.x + (-sin(self->rotation.y * DEGTORAD) * 2)),
							Player->body.position.y - 0.6,
							(Player->body.position.z + (-cos(self->rotation.y * DEGTORAD) * 2))));
				}
			}
			else		/*Cooldown*/
			{
				self->shown = 0;
				self->body.hit = 0;
			}

			if(Player->invuln)
			{
				self->shown = 0;
				self->body.hit = 0;
			}

			if(self->busy > 0)self->busy--;
		}
	}
}

void make_shadow(Body *owner)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return;

    ent->objModel = obj_load("models/shadow.obj");
    ent->texture = LoadSprite("models/shadow_text.png",1024,1024);
    ent->think = shadow_think;
	vec3d_cpy(ent->body.position,owner->position);
	cube_set(ent->body.bounds,-1,-0.05,-1,2,0.1,2);
	ent->owner = owner;
	return;
}

void shadow_think(Entity *self)
{
	int i;
	float y;
	Entity ent;
	Vec3D start,dir,t1,t2,t3,t4,contact;

	vec3d_set(start,self->owner->position.x,self->owner->position.y + self->owner->bounds.y,self->owner->position.z);
	vec3d_set(dir,0,-100,0);
	y = self->owner->position.y - 100;

	
	for(i = 0; i < __entity_max; i++)
	{
		ent = __entity_list[i];
		if(ent.inuse && (ent.uid != self->uid))
		{
			vec3d_set(t1, ent.body.bounds.x, ent.body.bounds.y+ent.body.bounds.h, ent.body.bounds.z);
			vec3d_set(t2, ent.body.bounds.x, ent.body.bounds.y+ent.body.bounds.h, ent.body.bounds.z+ent.body.bounds.d);
			vec3d_set(t3, ent.body.bounds.x+ent.body.bounds.w, ent.body.bounds.y+ent.body.bounds.h, ent.body.bounds.z+ent.body.bounds.d);
			vec3d_set(t4, ent.body.bounds.x+ent.body.bounds.w, ent.body.bounds.y+ent.body.bounds.h, ent.body.bounds.z);
			vec3d_add(t1,t1,ent.body.position);
			vec3d_add(t2,t2,ent.body.position);
			vec3d_add(t3,t3,ent.body.position);
			vec3d_add(t4,t4,ent.body.position);
			if(ray_in_quad(start,dir,t1,t2,t3,t4,&contact))
			{
				if(t1.y > y)y = t1.y;
			}
		}
	}

	self->body.position.x = self->owner->position.x;
	self->body.position.y = y + 0.2;
	self->body.position.z = self->owner->position.z;
}

Entity *spawn_snake(Vec3D position, Space *space, int ck1)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/snake.obj");
    ent->texture = LoadSprite("models/snake_text.png",1024,1024);
    ent->think = snake_think;
	ent->state = ST_WALK;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-0.5,-1,-0.5,1,2,1);
	ent->body.tang = 1;
	ent->body.hit = 1;
	ent->body.hurt = 0;
	ent->body.type = ST_ENEMY;
	space_add_body(space,&ent->body);
	ent->ck1 = ck1;		/*Rotation Direction*/
	ent->health = 1;
	return ent;
}

void snake_think(Entity *self)
{
	float speed = 0.1;
	float accel = 0.02;
	float dist = 0;
	float deg = 0;
	
	if(self->body.hurt)
	{
		self->health--;
	}
	
	if(self->health > 0)
	{
		if(Player)
		{
			dist = sqrt(pow(Player->body.position.x-self->body.position.x,2)+pow(Player->body.position.y-self->body.position.y,2)+pow(Player->body.position.z-self->body.position.z,2));
			deg = atan2(self->body.position.x-Player->body.position.x,self->body.position.z-Player->body.position.z) * RADTODEG;
		}

		if(dist < 10 && self->state == ST_WALK)
		{
			self->state = ST_JUMP1;
			self->body.uCheck = 0;
			self->body.velocity.y += 0.5;
		}

		/*Movement*/
		if(self->state == ST_WALK)
		{
			if(self->ck1)self->rotation.y -= 3;
			else self->rotation.y += 3;
			self->body.velocity.x = -sin(self->rotation.y * DEGTORAD) * speed;
			self->body.velocity.z = -cos(self->rotation.y * DEGTORAD) * speed;
		}

		if(self->state == ST_JUMP1)
		{
			if(abs(deg - self->rotation.y) < 5)self->rotation.y = deg;
			if(((deg - self->rotation.y < 0) && (deg - self->rotation.y > -180)) || (deg - self->rotation.y >= 180))self->rotation.y -= 4;
			else self->rotation.y += 4;
			if(!self->body.uCheck)
			{
				self->body.velocity.x = 0;
				self->body.velocity.z = 0;
			}
			else
			{
				self->body.velocity.x = -sin(self->rotation.y * DEGTORAD) * speed;
				self->body.velocity.z = -cos(self->rotation.y * DEGTORAD) * speed;
			}
		}
		
		/*Collisions*/
		if(self->body.uCheck)
		{
			self->body.velocity.y = 0;
			self->body.position.y = self->body.collision.h + self->body.bounds.h*0.5;
		}
		else
		{
			if(self->body.velocity.y > -2)self->body.velocity.y -= 0.05;
		}

		if(self->body.lCheck)
		{
			self->body.velocity.x = 0;
			self->body.position.x = self->body.collision.x - self->body.bounds.w*0.5;
		}
		if(self->body.rCheck)
		{
			self->body.velocity.x = 0;
			self->body.position.x = self->body.collision.w + self->body.bounds.w*0.5;
		}
		if(self->body.fCheck)
		{
			self->body.velocity.z = 0;
			self->body.position.z = self->body.collision.z - self->body.bounds.d*0.5;
		}
		if(self->body.bCheck)
		{
			self->body.velocity.z = 0;
			self->body.position.z = self->body.collision.d + self->body.bounds.d*0.5;
		}
		if(self->rotation.y > 180)self->rotation.y -= 360;	/*Rotation Fix*/
		if(self->rotation.y <= -180)self->rotation.y += 360;
	}
	else
	{
		if(self->state != ST_DEAD)
		{
			self->body.velocity.y = 1;
			self->state = ST_DEAD;
			self->body.tang = 0;
			self->body.hit = 0;
		}
		vec3d_set(self->rotation,180,self->rotation.y,0);
		self->body.velocity.x = 0;
		self->body.velocity.y -= 0.1;
		self->body.velocity.z = 0;
	}

	if(self->body.position.y < -20)entity_free(self);
}

Entity *spawn_eye(Vec3D position, Space *space, int ck1)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/eye.obj");
    ent->texture = LoadSprite("models/eye_text.png",1024,1024);
    ent->think = eye_think;
	ent->state = ST_IDLE;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
	ent->body.hit = 1;
	ent->body.hurt = 0;
	ent->body.type = ST_ENEMY;
	space_add_body(space,&ent->body);
	ent->ck1 = ck1;
	ent->health = 1;
	make_shadow(&ent->body);
	
	return ent;
}

void eye_think(Entity *self)
{
	if(self->body.hurt)
	{
		self->health--;
	}

	if(self->health > 0)
	{
		if(self->body.position.x < -8)self->ck1 = 1;
		if(self->body.position.x > 23.5)self->ck1 = 0;
		if(self->ck1)
		{
			self->rotation.y = -90;
			self->body.velocity.x = 0.1;
		}
		else
		{
			self->rotation.y = 90;
			self->body.velocity.x = -0.1;
		}
		self->body.velocity.y = sin(self->body.position.x) * 0.2;
		self->body.velocity.z = 0;
	}
	else
	{
		if(self->state != ST_DEAD)
		{
			self->body.velocity.y = 1;
			self->state = ST_DEAD;
			self->body.tang = 0;
			self->body.hit = 0;
		}
		vec3d_set(self->rotation,180,self->rotation.y,0);
		self->body.velocity.x = 0;
		self->body.velocity.y -= 0.1;
		self->body.velocity.z = 0;
	}

	if(self->body.position.y < -20 || self->body.position.x < -20 || self->body.position.x > 30)entity_free(self);
}

Entity *eye_spawner(Vec3D position, Space *space, int ck1, int ck2)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->think = eye_spawner_think;
	ent->shown = 0;
	vec3d_cpy(ent->body.position,position);
    ent->space = space;
	ent->ck1 = ck1;
	ent->ck2 = ck2;
	ent->busy = 0;
	return ent;
}

void eye_spawner_think(Entity *self)
{
	if(!self->busy)
	{
		spawn_eye(self->body.position,self->space,self->ck1);
		self->busy = self->ck2;
	}
	else self->busy--;
}

Entity *build_cube(Vec3D position, Space *space)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
	ent->body.tang = 1;
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
	ent->body.tang = 1;
	space_add_body(space,&ent->body);
    return ent;
}

void *build_road(Vec3D position, Space *space, int n)
{	
	int i;
	
	for(i = 0; i < n; i++)
	{
		vec3d_add(position,position,vec3d(0,0,-2));
		build_ground(position, space);
	}
	return;
}

Entity *build_spikes(Vec3D position, Space *space, int i, int j)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->objModel = obj_load("models/spikes.obj");
    ent->texture = LoadSprite("models/spikes_text.png",1024,1024);
	ent->think = spike_think;
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-8,-1,-1,16,2,2);
	ent->body.tang = 0;
	ent->body.type = ST_OBJECT;
	ent->busy = 0;
	ent->ck1 = i;
	ent->delay = j;
	space_add_body(space,&ent->body);
	build_spike_base(position, space);
    return ent;
}

void spike_think(Entity *self)
{
	if(!self->delay)
	{
		if(self->busy < ((self->ck1 * 3) / 5))
		{
			self->body.hit = 0;
			self->body.velocity.y = 0;
		}
		if((self->busy >= ((self->ck1 * 3) / 5)) && (self->busy < ((self->ck1 * 4) / 5)))
		{
			self->body.hit = 1;
			self->body.velocity.y = (float)2 / (self->ck1 / 4);
		}
		if(self->busy >= ((self->ck1 * 4) / 5))
		{
			self->body.velocity.y = (float)-2 / (self->ck1 / 4);
		}

		self->busy++;
		if(self->busy >= self->ck1)self->busy = 0;
	}
	else self->delay--;
}

Entity *build_spike_base(Vec3D position, Space *space)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/spike_base.obj");
    ent->texture = LoadSprite("models/spike_base_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-8,-1,-1,16,2,2);
	ent->body.tang = 1;
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
