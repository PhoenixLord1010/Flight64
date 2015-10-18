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
			build_cube(vec3d(1,-2,0),space);
			build_cube(vec3d(15,-2,0),space);
			build_ground(vec3d(8,-4,6),space);
			build_ground(vec3d(8,-4,4),space);
			build_ground(vec3d(8,-4,2),space);
			build_ground(vec3d(8,-4,0),space);
			build_ground(vec3d(8,-4,-2),space);
			build_ground(vec3d(8,-4,-4),space);
			build_ground(vec3d(8,-4,-6),space);
			build_ground(vec3d(8,-4,-8),space);
			build_ground(vec3d(8,-4,-10),space);
			build_ground(vec3d(8,-4,-12),space);
			build_ground(vec3d(8,-4,-14),space);
			build_ground(vec3d(8,-4,-16),space);
			build_ground(vec3d(8,-4,-18),space);
			build_ground(vec3d(8,-4,-20),space);
			build_ground(vec3d(8,-4,-22),space);
			build_ground(vec3d(8,-4,-24),space);
			build_ground(vec3d(8,-4,-26),space);
			build_ground(vec3d(8,-4,-28),space);
			build_ground(vec3d(8,-4,-30),space);
			build_ground(vec3d(8,-4,-32),space);

			loaded++;
		}
	}
}

void update_level(Space *space)
{
	render_level(current_level, space);
}