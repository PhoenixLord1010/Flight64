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
int title_screen();
void run_game();
void level_edit();

int main(int argc, char *argv[])
{
    int m;
    
    init_all();
	m = title_screen();

	if(m == 1)level_edit();
	if(m == 0)run_game();
	
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

int title_screen()
{
	int done = 0;
	int a = 0;
	int keyn;
	Uint8 *keys;
	GLint iResolution;
	GLint iGlobalTime;

	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");

	glUseProgram(get_shader_program());

	while(!done)
	{
		SDL_PumpEvents();
		UpdateKeyboard();
		
		keys = SDL_GetKeyboardState(&keyn);
		if(keys[SDL_SCANCODE_RETURN])return a;
		if(keys[SDL_SCANCODE_ESCAPE])exit(1);

		if(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])a = 0;
		if(keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])a = 1;


		graphics_frame_begin();
			glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
			glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
			font_draw_text("Flight 64",400,300,vec3d(1,1,1),1,4);
			font_draw_text("Start Game",460,400,vec3d(1,1,1),1,2);
			font_draw_text("Edit Level",460,430,vec3d(1,1,1),1,2);
			font_draw_text("-->",440,400+(a*30),vec3d(1,1,1),1,2);
		graphics_frame_end();
	}
}

void run_game()
{
	int i;
    char bGameLoopRunning = 1;
    Vec3D cameraPos;
	Vec3D cameraCent = {0,0,0};
    Vec3D cameraRot = {0,0,0};
	float cameraDist = 16;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
	Map *map1;
	Component *prcnt;
	RectFloat prcnt1,prcnt2;

	rect_set(&prcnt1,0,0,-1,-1);
	rect_set(&prcnt2,10,10,400,40);

	map1 = map_load("spawn.txt");
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
    space = space_new();
    space_set_steps(space,50);
    
	glUseProgram(get_shader_program());

	/*Initialise stuff*/
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


		graphics_frame_begin();
		
        glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
		glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
		
		glPushMatrix();
			camera_setup();
			entity_draw_all(); 
			component_draw(prcnt);
			mouse_draw_dot();
        glPopMatrix();
		
		graphics_frame_end();	

    } 
}

void level_edit()
{
	int i,b;
    char bEditLoopRunning = 1;
    Vec3D cameraPos;
	Vec3D cameraCent = {0,0,0};
    Vec3D cameraRot = {-30,45,0};
	float cameraDist = 24;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
	Map *map1;
	int ent = 0;
	Component *ent_L,*ent_R,*label;
	RectFloat comp,ent_L1,ent_R1,label1;

	rect_set(&comp,0,0,-1,-1);
	rect_set(&ent_L1,800,100,100,50);
	rect_set(&ent_R1,910,100,100,50);
	rect_set(&label1,800,65,210,25);

	map1 = map_load("spawn.txt");

	map1->spawnList[0];
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
	glUseProgram(get_shader_program());

	/*Initialise stuff*/
	ent_L = component_new();
	ent_L = button_new(0,"buttonL",comp,ent_L1,"   <--Ent",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	ent_R = component_new();
	ent_R = button_new(1,"buttonR",comp,ent_R1,"   Ent-->",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	label = component_new();
	label = button_new(2,"label",comp,label1,"player",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	mouse_show();
	map_draw(map1);
	camera_set_position(vec3d(0,5,15));
	camera_set_pitch(cameraRot.x);

    while (bEditLoopRunning)
    {
		keys = SDL_GetKeyboardState(&keyn);
		if(keys[SDL_SCANCODE_ESCAPE])bEditLoopRunning = 0;

		SDL_PumpEvents();

		mouse_update();
		UpdateKeyboard();
		update_highlight(ent,&b);

		/*Camera Stuff*/
		if(keys[SDL_SCANCODE_W])
		{
			cameraCent.x += -sin(cameraRot.y * DEGTORAD);
			cameraCent.z += -cos(cameraRot.y * DEGTORAD);
		}
		if(keys[SDL_SCANCODE_S])
		{
			cameraCent.x -= -sin(cameraRot.y * DEGTORAD);
			cameraCent.z -= -cos(cameraRot.y * DEGTORAD);
		}
		if(keys[SDL_SCANCODE_A])
		{
			cameraCent.x += -cos(-cameraRot.y * DEGTORAD);
			cameraCent.z += -sin(-cameraRot.y * DEGTORAD);
		}
		if(keys[SDL_SCANCODE_D])
		{
			cameraCent.x -= -cos(-cameraRot.y * DEGTORAD);
			cameraCent.z -= -sin(-cameraRot.y * DEGTORAD);
		}
		if(keys[SDL_SCANCODE_Q])cameraCent.y--;
		if(keys[SDL_SCANCODE_E])cameraCent.y++;
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
		camera_set_follow(cameraCent);
       
		/*Rotation Fixes*/
		if(cameraRot.x > 180)cameraRot.x -= 360;
		if(cameraRot.x <= -180)cameraRot.x += 360;
		if(cameraRot.y > 180)cameraRot.y -= 360;
		if(cameraRot.y <= -180)cameraRot.y += 360;


		graphics_frame_begin();
		
        glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
		glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
		
		glPushMatrix();
			camera_setup();
			entity_draw_all(); 
			component_draw(ent_L);
			component_draw(ent_R);
			component_draw(label);
			mouse_draw_dot();
        glPopMatrix();
		
		graphics_frame_end();	

		if(mouse_in_rect(ent_L1))
		{
			if(mouse_input_pressed(MouseLeft))
			{
				b = 0;
				ent_L->state = ButtonPressed;
				do
				{
					if(!b)ent--;
					if(ent < 0)ent = max_entities();
					update_highlight(ent,&b);
				}
				while(!b);
				button_set_text(label,get_name(ent));
			}
			else ent_L->state = ButtonHighlight;
		}
		else ent_L->state = ButtonIdle;

		if(mouse_in_rect(ent_R1))
		{
			if(mouse_input_pressed(MouseLeft))
			{
				b = 0;
				ent_R->state = ButtonPressed;
				do
				{
					if(!b)ent++;
					if(ent > max_entities())ent = 0;
					update_highlight(ent,&b);
				}
				while(!b);
				button_set_text(label,get_name(ent));
			}
			else ent_R->state = ButtonHighlight;
		}
		else ent_R->state = ButtonIdle;

    } 
}