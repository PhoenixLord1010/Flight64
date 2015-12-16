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
int m;

void init_all();
int title_screen();
void run_game();
void level_edit();

int main(int argc, char *argv[])
{   
    init_all();
	m = title_screen();

	if(m == 1)level_edit();
	m=0;
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
			font_draw_text("Flight 64",400,300,vec3d(1,1,1),1,5);
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
    Vec3D cameraRot = {-20,0,0};
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
	prcnt = percent_bar_new(1,"percent",prcnt1,prcnt2,0,0,1,vec3d(0,255,0),vec3d(255,0,0),0.9,0.9);
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
		
		component_percent_bar_set(prcnt,((float)Player->health-0.6)/((float)Player->healthmax-0.6));

		glPushMatrix();
			camera_setup();
			entity_draw_all(); 
			component_draw(prcnt);
        glPopMatrix();
		
		graphics_frame_end();	

    } 
}

void level_edit()
{
	int i,b,n;
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
	char fch[10];
	Component *ent_L,*ent_R,*label,*x_L,*x_R,*x_T,*y_L,*y_R,*y_T,*z_L,*z_R,*z_T;
	Component *a_M,*a_T,*del,*save;
	RectFloat comp,ent_L1,ent_R1,label1,x_L1,x_R1,x_T1,y_L1,y_R1,y_T1,z_L1,z_R1,z_T1;
	RectFloat a_M1,a_T1,del1,save1;

	rect_set(&comp,0,0,-1,-1);
	rect_set(&ent_L1,800,100,100,50);
	rect_set(&ent_R1,910,100,100,50);
	rect_set(&label1,800,65,210,25);
	rect_set(&x_L1,800,220,60,30);
	rect_set(&x_R1,950,220,60,30);
	rect_set(&x_T1,870,222.5,70,25);
	rect_set(&y_L1,800,260,60,30);
	rect_set(&y_R1,950,260,60,30);
	rect_set(&y_T1,870,262.5,70,25);
	rect_set(&z_L1,800,300,60,30);
	rect_set(&z_R1,950,300,60,30);
	rect_set(&z_T1,870,302.5,70,25);
	rect_set(&a_M1,870,370,30,30);
	rect_set(&a_T1,910,372.5,25,25);
	rect_set(&del1,825,430,160,50);
	rect_set(&save1,800,500,210,50);

	map1 = map_load("spawn.txt");

	map1->spawnList[0];
	
	iResolution = glGetUniformLocation(get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(get_shader_program(),"iGlobalTime");
    
	glUseProgram(get_shader_program());

	/*Initialise stuff*/
	ent_L = component_new();
	ent_L = button_new(0,"entL",comp,ent_L1,"   <--Ent",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	ent_R = component_new();
	ent_R = button_new(1,"entR",comp,ent_R1,"   Ent-->",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	label = component_new();
	label = button_new(2,"label",comp,label1,"player",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	x_L = component_new();
	x_L = button_new(3,"xL",comp,x_L1,"   --X",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	x_R = component_new();
	x_R = button_new(4,"xR",comp,x_R1,"   X++",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	x_T = component_new();
	x_T = button_new(5,"xT",comp,x_T1,"0",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	y_L = component_new();
	y_L = button_new(6,"yL",comp,y_L1,"   --Y",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	y_R = component_new();
	y_R = button_new(7,"yR",comp,y_R1,"   Y++",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	y_T = component_new();
	y_T = button_new(8,"yT",comp,y_T1,"0",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	z_L = component_new();
	z_L = button_new(9,"zL",comp,z_L1,"   --Z",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	z_R = component_new();
	z_R = button_new(10,"zR",comp,z_R1,"   Z++",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	z_T = component_new();
	z_T = button_new(11,"zT",comp,z_T1,"0",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	a_M = component_new();
	a_M = button_new(12,"aM",comp,a_M1," a",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,0,0),0.9,vec3d(0,1,0),vec3d(0,0,1));
	a_T = component_new();
	a_T = button_new(13,"aT",comp,a_T1,"0",3,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(1,1,1),0.9,vec3d(1,1,1),vec3d(1,1,1));
	del = component_new();
	del = button_new(14,"delete",comp,del1,"  DELETE",4,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(0.3,0.3,0.3),0.9,vec3d(0.1,0.1,0.1),vec3d(1,1,1));
	save = component_new();
	save = button_new(15,"save",comp,save1,"  SAVE LEVEL",4,ButtonRect,0,0,0,0,NULL,NULL,NULL,vec3d(0,1,0),0.9,vec3d(0,0,1),vec3d(1,0,0));


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
		update_highlight(map1->spawnList[ent].uid,&b);

		/*Camera Stuff*/
		if(keys[SDL_SCANCODE_W])
		{
			cameraCent.x += -sin(cameraRot.y * DEGTORAD) * 0.5;
			cameraCent.z += -cos(cameraRot.y * DEGTORAD) * 0.5;
		}
		if(keys[SDL_SCANCODE_S])
		{
			cameraCent.x -= -sin(cameraRot.y * DEGTORAD) * 0.5;
			cameraCent.z -= -cos(cameraRot.y * DEGTORAD) * 0.5;
		}
		if(keys[SDL_SCANCODE_A])
		{
			cameraCent.x += -cos(-cameraRot.y * DEGTORAD) * 0.5;
			cameraCent.z += -sin(-cameraRot.y * DEGTORAD) * 0.5;
		}
		if(keys[SDL_SCANCODE_D])
		{
			cameraCent.x -= -cos(-cameraRot.y * DEGTORAD) * 0.5;
			cameraCent.z -= -sin(-cameraRot.y * DEGTORAD) * 0.5;
		}
		if(keys[SDL_SCANCODE_Q])cameraCent.y--;
		if(keys[SDL_SCANCODE_E])cameraCent.y++;
		if(keys[SDL_SCANCODE_KP_2])cameraRot.x++;
		if(keys[SDL_SCANCODE_KP_8])cameraRot.x--;
		if(keys[SDL_SCANCODE_KP_4])cameraRot.y-=2;
		if(keys[SDL_SCANCODE_KP_6])cameraRot.y+=2;
		if(keys[SDL_SCANCODE_KP_PLUS])cameraDist+=0.2;
		if(keys[SDL_SCANCODE_KP_MINUS])cameraDist-=0.2;

		cameraPos.y = cameraCent.y - (cameraDist * sin(cameraRot.x * DEGTORAD));
		cameraPos.x = cameraCent.x + ((cameraDist * cos(cameraRot.x * DEGTORAD)) * sin(cameraRot.y * DEGTORAD));
		cameraPos.z = cameraCent.z + ((cameraDist * cos(cameraRot.x * DEGTORAD)) * cos(cameraRot.y * DEGTORAD));
		camera_set_rotation(cameraRot);
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
			component_draw(x_L);
			component_draw(x_R);
			component_draw(x_T);
			component_draw(y_L);
			component_draw(y_R);
			component_draw(y_T);
			component_draw(z_L);
			component_draw(z_R);
			component_draw(z_T);
			component_draw(a_M);
			component_draw(a_T);
			component_draw(del);
			component_draw(save);
			mouse_draw_dot();
        glPopMatrix();
		
		graphics_frame_end();	


		if(mouse_in_rect(ent_L1))		/*Previous Entity button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				b = 0;
				ent_L->state = ButtonPressed;
				do
				{
					if(!b)ent--;
					if(ent < 0)ent = spawn_get_ct();
					update_highlight(map1->spawnList[ent].uid,&b);
				}
				while(!b);
			}
			else ent_L->state = ButtonHighlight;
		}
		else ent_L->state = ButtonIdle;

		if(mouse_in_rect(ent_R1))		/*Next Entity button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				b = 0;
				ent_R->state = ButtonPressed;
				do
				{
					if(!b)ent++;
					if(ent >= spawn_get_ct())ent = 0;
					update_highlight(map1->spawnList[ent].uid,&b);
				}
				while(!b);
			}
			else ent_R->state = ButtonHighlight;
		}
		else ent_R->state = ButtonIdle;


		if(mouse_in_rect(x_L1))			/*X Minus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				x_L->state = ButtonPressed;
				map1->spawnList[ent].position.x -= 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else x_L->state = ButtonHighlight;
		}
		else x_L->state = ButtonIdle;

		if(mouse_in_rect(x_R1))			/*X Plus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				x_R->state = ButtonPressed;
				map1->spawnList[ent].position.x += 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else x_R->state = ButtonHighlight;
		}
		else x_R->state = ButtonIdle;

		if(mouse_in_rect(y_L1))			/*Y Minus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				y_L->state = ButtonPressed;
				map1->spawnList[ent].position.y -= 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else y_L->state = ButtonHighlight;
		}
		else y_L->state = ButtonIdle;

		if(mouse_in_rect(y_R1))			/*Y Plus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				y_R->state = ButtonPressed;
				map1->spawnList[ent].position.y += 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else y_R->state = ButtonHighlight;
		}
		else y_R->state = ButtonIdle;

		if(mouse_in_rect(z_L1))			/*Z Minus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				z_L->state = ButtonPressed;
				map1->spawnList[ent].position.z -= 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else z_L->state = ButtonHighlight;
		}
		else z_L->state = ButtonIdle;

		if(mouse_in_rect(z_R1))			/*Z Plus button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				z_R->state = ButtonPressed;
				map1->spawnList[ent].position.z += 0.5;
				update_position(map1->spawnList[ent].uid,map1->spawnList[ent].position);
			}
			else z_R->state = ButtonHighlight;
		}
		else z_R->state = ButtonIdle;

		if(mouse_in_rect(a_M1))			/*A button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				a_M->state = ButtonPressed;
				map1->spawnList[ent].a++;
				map1->spawnList[ent].a %= 2;
			}
			else a_M->state = ButtonHighlight;
		}
		else a_M->state = ButtonIdle;


		if(mouse_in_rect(del1))		/*Delete button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				b=0;
				del->state = ButtonPressed;
				update_position(map1->spawnList[ent].uid,vec3d(-10000,-10000,10000));
				spawn_free(&map1->spawnList[ent]);

				do
				{
					if(!b)ent++;
					if(ent >= spawn_get_ct())ent = 0;
					update_highlight(map1->spawnList[ent].uid,&b);
				}
				while(!b);
			}
			else del->state = ButtonHighlight;
		}
		else del->state = ButtonIdle;

		if(mouse_in_rect(save1))		/*Save button*/
		{
			if(mouse_input_pressed(MouseLeft))
			{
				save->state = ButtonPressed;
				spawn_save("spawn.txt");
				bEditLoopRunning = 0;
			}
			else save->state = ButtonHighlight;
		}
		else save->state = ButtonIdle;


		for(i = 0; i < spawn_get_ct(); i++)
		{
			if(map1->spawnList[i].uid == map1->spawnList[ent].uid)
			{
				button_set_text(label,map1->spawnList[i].classname);
				sprintf(fch,"%.2f",map1->spawnList[i].position.x);
				button_set_text(x_T,fch);
				sprintf(fch,"%.2f",map1->spawnList[i].position.y);
				button_set_text(y_T,fch);
				sprintf(fch,"%.2f",map1->spawnList[i].position.z);
				button_set_text(z_T,fch);
				sprintf(fch,"%d",map1->spawnList[i].a);
				button_set_text(a_T,fch);
			}
		}

    } 
	clear_entities();
}