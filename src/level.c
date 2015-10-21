#include "level.h"
#include "entity.h"

extern Entity *Player;

int current_level = 0;
int loaded = 0;

void render_level(int level, Space *space)
{
	if(level == 0)
	{
		if(!loaded)
		{
			make_player(vec3d(8,0,0),space);
			
			//spawn_snake(vec3d(4,-2,-12),space,1);
			//spawn_snake(vec3d(12,-2,-18),space,0);
			//spawn_eye(vec3d(-4,3,-8),space,1);
			//spawn_eye(vec3d(20,3,-12),space,0);
			spawn_frog(vec3d(8,0,-16),space);

			build_cube(vec3d(1,-2,0),space);
			build_cube(vec3d(15,-2,0),space);
			build_road(vec3d(8,-4,6),space,20);
			build_spikes(vec3d(8,-4,-36),space,60,0);

			build_platform(vec3d(0,-1,-4),vec3d(16,-1,-4),space);
			loaded++;
		} 
	}
}

void update_level(Space *space)
{
	render_level(current_level, space);
}