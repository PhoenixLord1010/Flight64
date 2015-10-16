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
			make_player(vec3d(8,3,0),space);
			build_cube(vec3d(1,0,0),space);
			build_cube(vec3d(15,0,0),space);
			build_ground(vec3d(8,-2,6),space);
			build_ground(vec3d(8,-2,4),space);
			build_ground(vec3d(8,-2,2),space);
			build_ground(vec3d(8,-2,0),space);
			build_ground(vec3d(8,-2,-2),space);
			build_ground(vec3d(8,-2,-4),space);
			build_ground(vec3d(8,-2,-6),space);
			build_ground(vec3d(8,-2,-8),space);
			build_ground(vec3d(8,-2,-10),space);
			build_ground(vec3d(8,-2,-12),space);
			build_ground(vec3d(8,-2,-14),space);
			build_ground(vec3d(8,-2,-16),space);

			loaded++;
		}
	}
}

void update_level(Space *space)
{
	render_level(current_level, space);
}