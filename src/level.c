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
			make_player(vec3d(8,0,1));
			
			build_road(vec3d(8,-4,6),16);
			build_wall(vec3d(4,0,-4));
			build_wall(vec3d(4,8,-4));
			build_wall(vec3d(12,0,-4));
			build_wall(vec3d(12,8,-4));

			build_wall(vec3d(8,-7,7));

			section++;
		}
	}
	
	if(level == 1)
	{
		if(section == 0)
		{
			make_player(vec3d(8,0,1));
			
			spawn_frog(vec3d(8,0,-32));
			spawn_frog(vec3d(4,0,-40));
			spawn_frog(vec3d(12,0,-48));

			build_cube(vec3d(1,-2,0));
			build_cube(vec3d(15,-2,0));
			build_cube(vec3d(1,-2,-15));
			build_cube(vec3d(15,-2,-15));
			build_cube(vec3d(1,-2,-30));
			build_cube(vec3d(15,-2,-30));
			build_cube(vec3d(1,-2,-45));
			build_cube(vec3d(15,-2,-45));
			build_road(vec3d(8,-4,6),16);

			build_warp(vec3d(8,-2.5,-46));

			section++;
		}
			
		if(section == 2)
		{
			spawn_snake(vec3d(4,-2,-122),1);
			spawn_snake(vec3d(12,-2,-138),0);
			
			build_cube(vec3d(1,-2,-100));
			build_cube(vec3d(15,-2,-100));
			build_cube(vec3d(1,-2,-115));
			build_cube(vec3d(15,-2,-115));
			build_cube(vec3d(1,-2,-130));
			build_cube(vec3d(15,-2,-130));
			build_cube(vec3d(1,-2,-145));
			build_cube(vec3d(15,-2,-145));
			build_road(vec3d(8,-4,-94),16);

			build_warp(vec3d(8,-2.5,-146), space);
			
			section++;
		}

		if(section == 4)
		{
			spawn_eye(vec3d(8,3,-212),0);
			spawn_eye(vec3d(-4,3,-218),1);
			spawn_eye(vec3d(20,3,-222),0);
			spawn_eye(vec3d(8,3,-228),1);
			
			build_road(vec3d(8,-4,-194),7);
			build_road(vec3d(8,-4,-230),7);

			build_warp(vec3d(8,-2.5,-246));

			section++;
		}

		if(section == 6)
		{
			spawn_snake(vec3d(3,-2,-340),1);
			spawn_snake(vec3d(13,-2,-340),0);

			build_road(vec3d(8,-4,-294),4);
			build_spikes(vec3d(8,-4,-308),75,0);
			build_road(vec3d(8,-4,-310),3);
			build_spikes(vec3d(8,-4,-322),75,0);
			build_road(vec3d(8,-4,-324),8);

			build_warp(vec3d(8,-2.5,-346));

			section++;
		}

		if(section == 8)
		{
			build_road(vec3d(8,-4,-394),3);
			build_platform(vec3d(0,-1,-404),vec3d(16,-1,-404));
			build_platform(vec3d(16,-1,-414),vec3d(0,-1,-414));
			build_platform(vec3d(0,-1,-424),vec3d(16,-1,-424));
			build_platform(vec3d(16,-1,-434),vec3d(0,-1,-434));
			build_road(vec3d(8,-4,-442),3);

			build_warp(vec3d(8,-2.5,-446));
			
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
		}

		section++;
		Player->ck2 = 0;
		Player->rotation.y = 0;
		Player->accel = 0;
		vec3d_set(Player->body.velocity,0,0,0);
	}

	render_level(current_level, space);
}