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
#include "component_button.h"
#include "level.h"
#include "math.h"
#include <stdio.h>
#include <glut.h>
#include <string.h>


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

extern Entity *Player;
Space *space;

void init_all();


int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    char bGameLoopRunning = 1;
    Vec3D cameraPos;
	Vec3D cameraCent = {0,0,0};
    Vec3D cameraRot = {0,0,0};
	float cameraDist = 16;
    SDL_Event e;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
	Map *map1;
	Component *btn,*prcnt;
	RectFloat btn1,btn2,prcnt1,prcnt2;
	rect_set(&btn1,0,0,-1,-1);
	rect_set(&btn2,0,0,1024,768);
	rect_set(&prcnt1,0,0,-1,-1);
	rect_set(&prcnt2,-7500,-5500,4000,500);
    
    init_all();

	map1 = map_load("spawn.txt");
	//spawn_load("spawn.txt");
	//spawn_save("spawn2.txt");
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
    space = space_new();
    space_set_steps(space,50);
    
	glUseProgram(get_shader_program());

	/*Initialise stuff*/
	btn = component_new();
	btn = button_new(0,"button",btn1,btn2,"push",3,ButtonRect,0,0,1,1,NULL,NULL,NULL,vec3d(255,0,0),0.7,vec3d(0,255,0),vec3d(0,0,255));
	prcnt = component_new();
	prcnt = percent_bar_new(1,"percent",prcnt1,prcnt2,0,0,0.1,vec3d(0,255,0),vec3d(255,0,0),0.9,0.9);
	mouse_show();
	map_draw(map1);
	camera_set_position(vec3d(0,5,15));
	camera_set_pitch(cameraRot.x);

    while (bGameLoopRunning)
    {
		keys = SDL_GetKeyboardState(&keyn);
		if(keys[SDL_SCANCODE_ESCAPE])bGameLoopRunning = 0;

		SDL_PumpEvents();

		input_update();
		update_entities();
		mouse_update();
		UpdateKeyboard();
		for (i = 0; i < 50; i++)
        {
            space_do_step(space);
        }

		/*Camera Stuff*/
		if(keys[SDL_SCANCODE_KP_2])cameraRot.x++;
		if(keys[SDL_SCANCODE_KP_8])cameraRot.x--;
		if(keys[SDL_SCANCODE_KP_4])cameraRot.y-=2;
		if(keys[SDL_SCANCODE_KP_6])cameraRot.y+=2;
		if(keys[SDL_SCANCODE_KP_PLUS])cameraDist+=0.2;
		if(keys[SDL_SCANCODE_KP_MINUS])cameraDist-=0.2;
		camera_set_rotation(cameraRot);
		cameraPos.y = cameraCent.y - (cameraDist * sin(cameraRot.x * DEGTORAD));
		cameraPos.x = cameraCent.x + ((cameraDist * cos(cameraRot.x * DEGTORAD)) * sin(cameraRot.y * DEGTORAD));
		cameraPos.z = cameraCent.z + ((cameraDist * cos(cameraRot.x * DEGTORAD)) * cos(cameraRot.y * DEGTORAD));
		camera_set_position(cameraPos);


		graphics_frame_begin();
		
        glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
		glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
		
		glPushMatrix();
			camera_setup();
			entity_draw_all(); 
			component_draw(btn);
			component_draw(prcnt);
			mouse_draw();
        glPopMatrix();
		
		graphics_frame_end();

		for(i = 0; i < 10; i++)
		{
			if(Player->body.position.x < (cameraCent.x - 2))cameraCent.x-=0.1;
			if(Player->body.position.x > (cameraCent.x + 2))cameraCent.x+=0.1;
			
			if(Player->body.position.z < (cameraCent.z - 2))cameraCent.z-=0.1;
			if(Player->body.position.z > (cameraCent.z + 2))cameraCent.z+=0.1;

			if(Player->body.position.y < (cameraCent.y - 2))cameraCent.y-=0.03;
			if(Player->body.position.y > (cameraCent.y + 2))cameraCent.y+=0.035;
		}
		camera_set_follow(cameraCent);
       
		/*Rotation Fixes*/
		if(cameraRot.x > 180)cameraRot.x -= 360;
		if(cameraRot.x <= -180)cameraRot.x += 360;
		if(cameraRot.y > 180)cameraRot.y -= 360;
		if(cameraRot.y <= -180)cameraRot.y += 360;
    } 
    return 0;
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
	button_configure();
	InitKeyboard();
}
