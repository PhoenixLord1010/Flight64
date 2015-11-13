#include "body.h"
#include "entity.h"

extern Entity *Player;


void body_reset(Body *body)
{
    if (!body)return;
    body->_needsBackoff = 0;
    body->_done = 0;    
}

void body_process(Body *body)
{
    if (!body)return;
    if (body->_needsBackoff)
    {
        vec3d_add(body->position,body->position,body->_stepOffVector);
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
	vec3d_set(self->collvec,0,0,0);
}

void check_collisions(Body *self, Body *other, Cube a, Cube b)
{
	if(self->type != T_OBJECT && self->tang && other->tang)
	{
		if((abs((b.y+b.h)-a.y) < 0.1) && (abs((b.y+b.h)-a.y) <= abs((a.x+a.w)-b.x)) && (abs((b.y+b.h)-a.y) <= abs((b.x+b.w)-a.x)) && (abs((b.y+b.h)-a.y) <= abs((a.z+a.d)-b.z)) && (abs((b.y+b.h)-a.y) <= abs((b.z+b.d)-a.z)) && (abs((b.y+b.h)-a.y) <= abs((a.y+a.h)-b.y)) && (self->velocity.y <= other->velocity.y) && other->uTang)
		{
			self->uCheck = 1;
			self->collision.h = b.y+b.h;
			vec3d_cpy(self->collvec,other->velocity);
		}
		if((abs((a.x+a.w)-b.x) < 0.1) && (abs((a.x+a.w)-b.x) < abs((b.y+b.h)-a.y)) && (abs((a.x+a.w)-b.x) <= abs((b.x+b.w)-a.x)) && (abs((a.x+a.w)-b.x) <= abs((a.z+a.d)-b.z)) && (abs((a.x+a.w)-b.x) <= abs((b.z+b.d)-a.z)) && (abs((a.x+a.w)-b.x) <= abs((a.y+a.h)-b.y)))
		{
			self->lCheck = 1;
			self->collision.x = b.x;
		}
		if((abs((b.x+b.w)-a.x) < 0.1) && (abs((b.x+b.w)-a.x) < abs((b.y+b.h)-a.y)) && (abs((b.x+b.w)-a.x) < abs((a.x+a.w)-b.x)) && (abs((b.x+b.w)-a.x) <= abs((a.z+a.d)-b.z)) && (abs((b.x+b.w)-a.x) <= abs((b.z+b.d)-a.z)) && (abs((b.x+b.w)-a.x) <= abs((a.y+a.h)-b.y)))
		{
			self->rCheck = 1;
			self->collision.w = b.x+b.w;
		}
		if((abs((a.z+a.d)-b.z) < 0.1) && (abs((a.z+a.d)-b.z) < abs((b.y+b.h)-a.y)) && (abs((a.z+a.d)-b.z) < abs((a.x+a.w)-b.x)) && (abs((a.z+a.d)-b.z) < abs((b.x+b.w)-a.x)) && (abs((a.z+a.d)-b.z) <= abs((b.z+b.d)-a.z)) && (abs((a.z+a.d)-b.z) <= abs((a.y+a.h)-b.y)))
		{
			self->fCheck = 1;
			self->collision.z = b.z;
		}
		if((abs((b.z+b.d)-a.z) < 0.1) && (abs((b.z+b.d)-a.z) < abs((b.y+b.h)-a.y)) && (abs((b.z+b.d)-a.z) < abs((a.x+a.w)-b.x)) && (abs((b.z+b.d)-a.z) < abs((b.x+b.w)-a.x)) && (abs((b.z+b.d)-a.z) < abs((a.z+a.d)-b.z)) && (abs((b.z+b.d)-a.z) <= abs((a.y+a.h)-b.y)))
		{
			self->bCheck = 1;
			self->collision.d = b.z+b.d;
		}
		if((abs((a.y+a.h)-b.y) < 0.1) && (abs((a.y+a.h)-b.y) < abs((b.y+b.h)-a.y)) && (abs((a.y+a.h)-b.y) < abs((a.x+a.w)-b.x)) && (abs((a.y+a.h)-b.y) < abs((b.x+b.w)-a.x)) && (abs((a.y+a.h)-b.y) < abs((a.z+a.d)-b.z)) && (abs((a.y+a.h)-b.y) < abs((b.z+b.d)-a.z)))
		{
			self->dCheck = 1;
			self->collision.y = b.y;
		}
	}

	if(self->type != T_OBJECT && other->hit && (self->type != other->type))
	{
		self->hurt = 1;
		vec3d_cpy(self->hitvec,other->position);
	}

	if(self->type == T_PLAYER && self->uCheck && other->type == T_WARP && other->dCheck)Player->ck2 = 1;
}
