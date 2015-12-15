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
	run_game();
	
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
	Vec3D pl[4];
	float cameraDist = 16;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
	Map *map1;
	Component *btn,*prcnt;
	RectFloat btn1,btn2,prcnt1,prcnt2;

	rect_set(&btn1,0,0,-1,-1);
	rect_set(&btn2,500,200,133,100);
	rect_set(&prcnt1,0,0,-1,-1);
	rect_set(&prcnt2,10,10,400,40);

	map1 = map_load("spawn.txt");
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
    space = space_new();
    space_set_steps(space,50);
    
	glUseProgram(get_shader_program());

	/*Initialise stuff*/
	btn = component_new();
	btn = button_new(0,"button",btn1,btn2,"push",4,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
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

		pl[0] = vec3d(Player->body.position.x+Player->body.bounds.x,Player->body.position.y+Player->body.bounds.y,Player->body.position.z+Player->body.bounds.z+Player->body.bounds.d);
		pl[1] = vec3d(Player->body.position.x+Player->body.bounds.x,Player->body.position.y+Player->body.bounds.y+Player->body.bounds.h,Player->body.position.z+Player->body.bounds.z+Player->body.bounds.d);
		pl[2] = vec3d(Player->body.position.x+Player->body.bounds.x+Player->body.bounds.w,Player->body.position.y+Player->body.bounds.y+Player->body.bounds.h,Player->body.position.z+Player->body.bounds.z+Player->body.bounds.d);
		pl[3] = vec3d(Player->body.position.x+Player->body.bounds.x+Player->body.bounds.w,Player->body.position.y+Player->body.bounds.y,Player->body.position.z+Player->body.bounds.z+Player->body.bounds.d);
		//if(mouse_in_rect(btn2))btn->state = ButtonHighlight;
		//if(ray_in_quad(pl[0],pl[1],pl[2],pl[3]))btn->state = ButtonHighlight;
		//else btn->state = ButtonIdle;

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
			component_draw(btn);
			component_draw(prcnt);
			mouse_draw_dot();
        glPopMatrix();
		
		graphics_frame_end();		
    } 
}

void level_edit()
{

}