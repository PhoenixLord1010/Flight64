#include "level.h"
#include "entity.h"
#include "simple_logger.h"

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
			make_player(vec3d(8,0,1),space);
			
			build_road(vec3d(8,-4,6),space,16);
			build_wall(vec3d(4,0,-4),space,0);
			build_wall(vec3d(4,8,-4),space,1);
			build_wall(vec3d(12,0,-4),space,0);
			build_wall(vec3d(12,8,-4),space,1);

			build_cube(vec3d(8,-2,2),space);

			section++;
		}
	}
	
	if(level == 1)
	{
		if(section == 0)
		{
			make_player(vec3d(8,0,1),space);
			
			spawn_frog(vec3d(8,0,-32),space);
			spawn_frog(vec3d(4,0,-40),space);
			spawn_frog(vec3d(12,0,-48),space);

			build_cube(vec3d(1,-2,0),space);
			build_cube(vec3d(15,-2,0),space);
			build_cube(vec3d(1,-2,-15),space);
			build_cube(vec3d(15,-2,-15),space);
			build_cube(vec3d(1,-2,-30),space);
			build_cube(vec3d(15,-2,-30),space);
			build_cube(vec3d(1,-2,-45),space);
			build_cube(vec3d(15,-2,-45),space);
			build_road(vec3d(8,-4,6),space,16);

			build_warp(vec3d(8,-2.5,-46),space);

			section++;
		}
			
		if(section == 2)
		{
			spawn_snake(vec3d(4,-2,-122),space,1);
			spawn_snake(vec3d(12,-2,-138),space,0);
			
			build_cube(vec3d(1,-2,-100),space);
			build_cube(vec3d(15,-2,-100),space);
			build_cube(vec3d(1,-2,-115),space);
			build_cube(vec3d(15,-2,-115),space);
			build_cube(vec3d(1,-2,-130),space);
			build_cube(vec3d(15,-2,-130),space);
			build_cube(vec3d(1,-2,-145),space);
			build_cube(vec3d(15,-2,-145),space);
			build_road(vec3d(8,-4,-94),space,16);

			build_warp(vec3d(8,-2.5,-146), space);
			
			section++;
		}

		if(section == 4)
		{
			spawn_eye(vec3d(8,3,-212),space,0);
			spawn_eye(vec3d(-4,3,-218),space,1);
			spawn_eye(vec3d(20,3,-222),space,0);
			spawn_eye(vec3d(8,3,-228),space,1);
			
			build_road(vec3d(8,-4,-194),space,7);
			build_road(vec3d(8,-4,-230),space,7);

			build_warp(vec3d(8,-2.5,-246), space);

			section++;
		}

		if(section == 6)
		{
			spawn_snake(vec3d(3,-2,-340),space,1);
			spawn_snake(vec3d(13,-2,-340),space,0);

			build_road(vec3d(8,-4,-294),space,4);
			build_spikes(vec3d(8,-4,-308),space,75,0);
			build_road(vec3d(8,-4,-310),space,3);
			build_spikes(vec3d(8,-4,-322),space,75,0);
			build_road(vec3d(8,-4,-324),space,8);

			build_warp(vec3d(8,-2.5,-346),space);

			section++;
		}

		if(section == 8)
		{
			build_road(vec3d(8,-4,-394),space,3);
			build_platform(vec3d(0,-1,-404),vec3d(16,-1,-404),space);
			build_platform(vec3d(16,-1,-414),vec3d(0,-1,-414),space);
			build_platform(vec3d(0,-1,-424),vec3d(16,-1,-424),space);
			build_platform(vec3d(16,-1,-434),vec3d(0,-1,-434),space);
			build_road(vec3d(8,-4,-442),space,3);

			build_warp(vec3d(8,-2.5,-446),space);
			
			section++;
		}
	}
}

void update_level(Space *space)
{
	if(section == 0)vec3d_set(pnt,8,0,0);
	
	if(Player != NULL && Player->ck2)
	{
		if(section == 1)
		{
			vec3d_set(pnt,8,0,-100);
			vec3d_cpy(Player->body.position,pnt); 
		}
		if(section == 3)
		{
			vec3d_set(pnt,8,0,-200);
			vec3d_cpy(Player->body.position,pnt); 
		}
		if(section == 5)
		{
			vec3d_set(pnt,8,0,-300);
			vec3d_cpy(Player->body.position,pnt); 
		}
		if(section == 7)
		{
			vec3d_set(pnt,8,0,-400);
			vec3d_cpy(Player->body.position,pnt);
		}
		if(section == 9)
		{
			exit(1);
			slog("win");
		}

		section++;
		Player->ck2 = 0;
		Player->rotation.y = 0;
		Player->accel = 0;
		vec3d_set(Player->body.velocity,0,0,0);
	}

	render_level(current_level, space);
}