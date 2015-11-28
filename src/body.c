#include "body.h"
#include "entity.h"
#include "math.h"

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
	float what = 25;
	
	if(self->type != T_OBJECT && self->tang && other->tang)
	{
		if((fabs((b.y+b.h)-a.y) < 0.75) && (fabs((b.y+b.h)-a.y) < fabs((a.x+a.w)-b.x)) && (fabs((b.y+b.h)-a.y) < fabs((b.x+b.w)-a.x)) && (fabs((b.y+b.h)-a.y) < fabs((a.z+a.d)-b.z)) && (fabs((b.y+b.h)-a.y) < fabs((b.z+b.d)-a.z)) && (fabs((b.y+b.h)-a.y) < fabs((a.y+a.h)-b.y)) && (self->velocity.y <= other->velocity.y) && other->uTang)
		{
			self->uCheck = 1;
			self->collision.h = b.y+b.h;
			vec3d_cpy(self->collvec,other->velocity);
		}
		if((fabs((a.x+a.w)-b.x) < 0.75) && (fabs((a.x+a.w)-b.x) < fabs((b.y+b.h)-a.y)) && (fabs((a.x+a.w)-b.x) <= fabs((b.x+b.w)-a.x)) && (fabs((a.x+a.w)-b.x) < fabs((a.z+a.d)-b.z)) && (fabs((a.x+a.w)-b.x) < fabs((b.z+b.d)-a.z)) && (fabs((a.x+a.w)-b.x) < fabs((a.y+a.h)-b.y)) && other->lTang)
		{
			self->lCheck = 1;
			self->collision.x = b.x;
		}
		if((fabs((b.x+b.w)-a.x) < 0.75) && (fabs((b.x+b.w)-a.x) < fabs((b.y+b.h)-a.y)) && (fabs((b.x+b.w)-a.x) < fabs((a.x+a.w)-b.x)) && (fabs((b.x+b.w)-a.x) < fabs((a.z+a.d)-b.z)) && (fabs((b.x+b.w)-a.x) < fabs((b.z+b.d)-a.z)) && (fabs((b.x+b.w)-a.x) < fabs((a.y+a.h)-b.y)) && other->rTang)
		{
			self->rCheck = 1;
			self->collision.w = b.x+b.w;
		}
		if((fabs((a.z+a.d)-b.z) < 0.75) && (fabs((a.z+a.d)-b.z) < fabs((b.y+b.h)-a.y)) && (fabs((a.z+a.d)-b.z) < fabs((a.x+a.w)-b.x)) && (fabs((a.z+a.d)-b.z) < fabs((b.x+b.w)-a.x)) && (fabs((a.z+a.d)-b.z) < fabs((b.z+b.d)-a.z)) && (fabs((a.z+a.d)-b.z) < fabs((a.y+a.h)-b.y)) && other->fTang)
		{
			self->fCheck = 1;
			self->collision.z = b.z;
		}
		if((fabs((b.z+b.d)-a.z) < 0.75) && (fabs((b.z+b.d)-a.z) < fabs((b.y+b.h)-a.y)) && (fabs((b.z+b.d)-a.z) < fabs((a.x+a.w)-b.x)) && (fabs((b.z+b.d)-a.z) < fabs((b.x+b.w)-a.x)) && (fabs((b.z+b.d)-a.z) < fabs((a.z+a.d)-b.z)) && (fabs((b.z+b.d)-a.z) < fabs((a.y+a.h)-b.y)) && other->bTang)
		{
			self->bCheck = 1;
			self->collision.d = b.z+b.d;
		}
		if((fabs((a.y+a.h)-b.y) < 0.75) && (fabs((a.y+a.h)-b.y) < fabs((b.y+b.h)-a.y)) && (fabs((a.y+a.h)-b.y) < fabs((a.x+a.w)-b.x)) && (fabs((a.y+a.h)-b.y) < fabs((b.x+b.w)-a.x)) && (fabs((a.y+a.h)-b.y) < fabs((a.z+a.d)-b.z)) && (fabs((a.y+a.h)-b.y) < fabs((b.z+b.d)-a.z)) && other->dTang)
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
