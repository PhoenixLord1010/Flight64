#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "sprite.h"
#include "shader.h"
#include "obj.h"
#include "types.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"
#include "map.h"
#include "camera.h"
#include "font.h"
#include "input.h"
#include "mouse.h"
#include "level.h"
#include "math.h"
#include <stdio.h>
#include <glut.h>


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#include <string.h>

extern Entity *Player;
Space *space;

void set_camera(Vec3D position, Vec3D rotation);
void init_all();


int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {8,5,15};
    Vec3D cameraRotation = {-20,0,0};
    SDL_Event e;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
	Map *map1;
	int isDrawn = 0;
    
    init_all();

	map1 = map_load("spawn.txt");
	//spawn_load("spawn.txt");
	//spawn_save("spawn2.txt");
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
    space = space_new();
    space_set_steps(space,50);
    
	glUseProgram(get_shader_program());

    while (bGameLoopRunning)
    {
		keys = SDL_GetKeyboardState(&keyn);
		if(keys[SDL_SCANCODE_ESCAPE])bGameLoopRunning = 0;

		SDL_PumpEvents();

		update_entities();
		for (i = 0; i < 50; i++)
        {
            space_do_step(space);
        }

        /*while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    cameraPosition.z++;
                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                    cameraPosition.z--;
                }
                else if (e.key.keysym.sym == SDLK_w)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            sin(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_d)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    cameraRotation.z += 1;
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraRotation.z -= 1;
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    cameraRotation.x += 1;
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    cameraRotation.x -= 1;
                }
            }
        }*/
		
		
        graphics_frame_begin();
		
        glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
		glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
        
		if(!isDrawn)
		{
			map_draw(map1);
			isDrawn++;
		}
		
		glPushMatrix();
			set_camera(
				cameraPosition,
				cameraRotation);
			entity_draw_all();  
        glPopMatrix();

		
		//update_level(space);
		UpdateKeyboard();

		for(i = 0; i < 10; i++)
		{
			if(Player->body.position.z < (cameraPosition.z - 17))cameraPosition.z-=0.1;
			if(Player->body.position.z > (cameraPosition.z - 13))cameraPosition.z+=0.1;

			if(Player->body.position.y < (cameraPosition.y - 6))cameraPosition.y-=0.03;
			if(Player->body.position.y > (cameraPosition.y - 2))cameraPosition.y+=0.035;
		}

        graphics_frame_end();
    } 
    return 0;
}

void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

void init_all()
{
	init_logger("flight64.log");
    graphics_config();
	graphics_init();
	sprite_init();
	camera_config();
	camera_init();
    model_init();
    obj_init();
    entity_init(255);
	spawn_init(255);
	font_init();
	input_init();
	mouse_init();
	InitKeyboard();
}
