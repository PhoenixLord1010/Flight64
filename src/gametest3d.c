/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"
#include "math.h"
#include <stdio.h>
#include <glut.h>


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#include <string.h>

void set_camera(Vec3D position, Vec3D rotation);
void init_all();

/*void touch_callback(void *data, void *context)
{
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        //slog("%s is ",other->name);
    }
    //slog("touching me.... touching youuuuuuuu");
}*/

Entity *newCube(Vec3D position,const char *name)
{
    Entity * ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
    return ent;
}

Entity *newGround(Vec3D position)
{
    Entity * ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/ground.obj");
    ent->texture = LoadSprite("models/ground_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-8,-1,-1,16,2,2);
    return ent;
}

int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    Space *space;
    Entity *cube1,*cube2,*ground, *player;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {8,3,20};
    Vec3D cameraRotation = {-5,0,0};
    SDL_Event e;
    //Obj *bgobj;
    //Sprite *bgtext;
	GLint iResolution;
	GLint iGlobalTime;
	int keyn;
	Uint8 *keys;
    
    init_all();
	
	iResolution = glGetUniformLocation(graphics3d_get_shader_program(),"iResolution");
	iGlobalTime = glGetUniformLocation(graphics3d_get_shader_program(),"iGlobalTime");
    //bgobj = obj_load("models/mountainvillage.obj");
    //bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
    cube1 = newCube(vec3d(1,0,0),"Cubert");
    cube2 = newCube(vec3d(15,0,0),"Hobbes");
	ground = newGround(vec3d(8,-2,0));
	player = make_player(vec3d(1,5,0));
    
    space = space_new();
    space_set_steps(space,100);
    
    space_add_body(space,&cube1->body);
    space_add_body(space,&cube2->body);
	space_add_body(space,&ground->body);
	space_add_body(space,&player->body);
	glUseProgram(graphics3d_get_shader_program());

    while (bGameLoopRunning)
    {
		update_entities();
		for (i = 0; i < 100;i++)
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
		
		keys = SDL_GetKeyboardState(&keyn);
		if(keys[SDL_SCANCODE_ESCAPE])bGameLoopRunning = 0;

		SDL_PumpEvents();

        graphics3d_frame_begin();
		
        glUniform3f(iResolution, SCREEN_WIDTH, SCREEN_HEIGHT, 100.0f);
		glUniform1f(iGlobalTime, SDL_GetTicks() / 1000.0f);
        
		glPushMatrix();
			set_camera(
				cameraPosition,
				cameraRotation);
			entity_draw_all();  
        glPopMatrix();

		UpdateKeyboard();
        graphics3d_next_frame();
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
    if (graphics3d_init(1024,768,1,"flight64",33) != 0)
    {
        return -1;
    }

    model_init();
    obj_init();
    entity_init(255);
	InitKeyboard();
}
