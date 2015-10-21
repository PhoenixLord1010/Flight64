#include "level.h"
#include "entity.h"

extern Entity *Player;

int current_level = 0;
int section = 0;
int loaded = 0;
Vec3D pnt;

void render_level(int level, Space *space)
{
	if(level == 0)
	{
		if(section == 0)
		{
			make_player(vec3d(8,0,0),space);
			
			spawn_frog(vec3d(8,0,-16),space);

			build_cube(vec3d(1,-2,0),space);
			build_cube(vec3d(15,-2,0),space);
			build_cube(vec3d(1,-2,-10),space);
			build_cube(vec3d(15,-2,-10),space);
			build_cube(vec3d(1,-2,-20),space);
			build_cube(vec3d(15,-2,-20),space);
			build_road(vec3d(8,-4,6),space,8);

			build_warp(vec3d(8,-2.5,-24),space);

			section++;
		}
			
		if(section == 2)
		{
			build_cube(vec3d(1,-2,-52),space);
			build_cube(vec3d(15,-2,-52),space);
			build_cube(vec3d(1,-2,-62),space);
			build_cube(vec3d(15,-2,-62),space);
			build_cube(vec3d(1,-2,-72),space);
			build_cube(vec3d(15,-2,-72),space);
			spawn_snake(vec3d(4,-2,-62),space,1);
			spawn_snake(vec3d(12,-2,-68),space,0);
			build_road(vec3d(8,-4,-44),space,8);

			build_warp(vec3d(8,-2.5,-74), space);
			
			section++;
		}

		if(section == 4)
		{
			spawn_eye(vec3d(-4,3,-108),space,1);
			spawn_eye(vec3d(20,3,-112),space,0);
			
			build_road(vec3d(8,-4,-94),space,8);

			build_warp(vec3d(8,-2.5,-124), space);

			section++;
		}

		if(section == 6)
		{
			build_road(vec3d(8,-4,-144),space,2);
			build_spikes(vec3d(8,-4,-152),space,60,0);
			build_road(vec3d(8,-4,-156),space,2);
			build_spikes(vec3d(8,-4,-160),space,60,0);
			build_road(vec3d(8,-4,-164),space,2);

			build_warp(vec3d(8,-2.5,-174), space);

			section++;
		}

			
			//build_spikes(vec3d(8,-4,-36),space,60,0);
			//build_platform(vec3d(0,-1,-4),vec3d(16,-1,-4),space);
			//loaded++;
	}
}

void update_level(Space *space)
{
	if(Player != NULL && Player->ck2 && section == 1)
	{
		section++;
		vec3d_set(pnt,8,0,-50);
		vec3d_cpy(Player->body.position,pnt); 
		Player->ck2 = 0;
	}
	if(Player != NULL && Player->ck2 && section == 3)
	{
		section++;
		vec3d_set(pnt,8,0,-100);
		vec3d_cpy(Player->body.position,pnt); 
		Player->ck2 = 0;
	}
	if(Player != NULL && Player->ck2 && section == 5)
	{
		section++;
		vec3d_set(pnt,8,0,-150);
		vec3d_cpy(Player->body.position,pnt); 
		Player->ck2 = 0;
	}
	if(Player != NULL && Player->ck2 && section == 7)
	{
		section++;
		vec3d_set(pnt,8,0,-200);
		vec3d_cpy(Player->body.position,pnt); 
		Player->ck2 = 0;
	}

	render_level(current_level, space);
}