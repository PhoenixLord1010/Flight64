#include "mouse.h"
#include "sprite.h"
#include "graphics3d.h"
#include "draw.h"
#include "camera.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <string.h>


typedef struct
{
  int      x,y;
  int      ox,oy;
  Uint32     buttons;
  Uint32     oldButtons;
  Uint32     lastButtons;
  Uint32     timePress; /*this time index*/
  Uint32     lastPress; /*time index of the last time this input was pressed*/
  Sprite * sprite;
  Uint32     frame;
  int      showHide;
}Mouse;

Mouse __mouse;
Line  __mouse_file;
int   __mouse_frame_width = 1600;
int   __mouse_frame_height = 1600;

void mouse_close()
{
  sprite_free(&__mouse.sprite);
  memset(&__mouse,0,sizeof(Mouse));
}

void mouse_init()
{
	strncpy(__mouse_file,"images/mouse_icon.png",128);
	memset(&__mouse,0,sizeof(Mouse));
	__mouse.sprite = sprite_load(__mouse_file,
                                      __mouse_frame_width,
                                      __mouse_frame_height);
	SDL_ShowCursor(SDL_DISABLE);
	atexit(mouse_close);
}

void mouse_draw()
{
  if (__mouse.showHide <= 0)return;
  sprite_draw(__mouse.sprite,
                  __mouse.frame,
                  __mouse.x,
                  __mouse.y);
}

void mouse_update()
{
  __mouse.ox = __mouse.x;
  __mouse.oy = __mouse.y;
  __mouse.oldButtons = __mouse.buttons;
  __mouse.buttons = SDL_GetMouseState(&__mouse.x,&__mouse.y);
  if (__mouse.buttons != __mouse.oldButtons)
  {
    if (__mouse.oldButtons)
    {
      __mouse.lastButtons = __mouse.oldButtons;
      __mouse.lastPress = __mouse.timePress;
      __mouse.timePress = graphics_get_now();
    }
  }
  //slog("x=%d, y=%d",__mouse.x,__mouse.y);
}

int mouse_old_state(Uint32 button)
{
  Uint32 mask = 1;
  mask = mask << button;
  return (__mouse.oldButtons & mask);
}

int mouse_last_state(Uint32 button)
{
  Uint32 mask = 1;
  mask = mask << button;
  return (__mouse.lastButtons & mask);
}

int mouse_input_state(Uint32 button)
{
  Uint32 mask = 1;
  mask = mask << button;
  return (__mouse.buttons & mask);
}

void mouse_get_position_vec2d(Vec2D *pos)
{
  if (!pos)return;
  pos->x = (float)__mouse.x;
  pos->y = (float)__mouse.y;
}

void mouse_get_position(int *x, int *y)
{
  if (x)*x = __mouse.x;
  if (y)*y = __mouse.y;
}

void mouse_get_motion(int *dx, int *dy)
{
  if (dx)*dx = __mouse.x - __mouse.ox;
  if (dy)*dy = __mouse.y - __mouse.oy;
}

int mouse_in_rect(RectFloat rect)
{
  Vec2D v;
  int x,y;
  mouse_get_position(&x, &y);
  v.x = x;
  v.y = y;
  return vec_in_rect(v,rect);
}

int mouse_input_pressed(Uint32 button)
{
  return ((mouse_input_state(button)) &&
          (!mouse_old_state(button)));
}

int mouse_input_released(Uint32 button)
{
  return ((!mouse_input_state(button)) &&
          (mouse_old_state(button)));
}

int mouse_input_held(Uint32 button)
{
  return ((mouse_input_state(button)) &&
          (mouse_old_state(button)));
}

int mouse_input_double_tapped(Uint32 button)
{
  return ((mouse_input_state(button)) &&
          (!mouse_old_state(button)) &&
          (mouse_last_state(button)) &&
          ((__mouse.lastPress - __mouse.lastPress) > 15));
          /*TODO the 15 needs to be brought in fromthe input config*/
}

void mouse_hide()
{
  __mouse.showHide--;
}

void mouse_show()
{
  __mouse.showHide++;
}

void mouse_reset()
{
  __mouse.showHide = 0;
}

void mouse_draw_dot()
{
	Vec3D position,cam;
	GraphicsView view;

	graphics_get_view(&view);
	camera_get_position(&cam);

	gluUnProject(__mouse.x, 768-__mouse.y, 0.99f, view.modelView, view.projection, view.viewPort, &position.x, &position.y, &position.z);
	
	position.x += cam.x;
	position.y += cam.y;
	position.z += cam.z;
	
	draw_dot_3D(position,
                  9,
                  vec3d(0,0,0),
                  1);
	draw_dot_3D(position,
                  6,
                  vec3d(1,1,1),
                  1);
}

void mouse_draw_3d_ray()
{
  Vec3D position,vector, position2;
  mouse_get_3d_ray(&position,&vector);
  
  vec3d_scale(vector,vector,100);

  vec3d_add(position2,position,vector);

  draw_dot_3D(position,
                  4,
                  vec3d(1,0,0),
                  1);
  draw_line_3D(position,
                   position2,
                   2,
                   vec3d(0,1,0),
                   1);
  draw_dot_3D(position2,
                  7,
                  vec3d(0,0,1),
                  1);
}

int mouse_in_3d_quad(Vec3D t1,Vec3D t2,Vec3D t3,Vec3D t4)
{
  Vec3D position,vector;
  mouse_get_3d_ray(&position,&vector);
  return ray_in_quad(
    position,
    vector,
    t1,
    t2,
    t3,
    t4,
    NULL);
}

int mouse_in_quad3d(Quad3D quad)
{
  Vec3D position,vector;
  mouse_get_3d_ray(&position,&vector);
  return ray_in_quad3d(
    position,
    vector,
    quad,
    NULL);
}

int mouse_get_quad3d_intersect(Vec3D *contact,Quad3D quad)
{
  Text text;
  Vec3D position,vector;
  mouse_get_3d_ray(&position,&vector);
  vec3d_scale(vector,vector,10000);
  print_quad3d(text,quad);
  return ray_in_quad3d(
    position,
    vector,
    quad,
    contact);
}


int mouse_in_3d_triangle(Vec3D t1,Vec3D t2,Vec3D t3)
{
  Vec3D position,vector;
  mouse_get_3d_ray(&position,&vector);
  return ray_in_triangle(
    position,
    vector,
    t1,
    t2,
    t3,
    NULL);
}

int mouse_in_triangle3d(Triangle3D tri)
{
  Vec3D position,vector;
  mouse_get_3d_ray(&position,&vector);
  return ray_in_triangle3d(
    position,
    vector,
    tri,
    NULL);
}


void mouse_get_3d_ray(Vec3D *position,Vec3D *vector)
{
  Vec3D cam,pos1,pos2,rotation;
  GraphicsView view;
  
  graphics_get_view(&view);
  camera_get_position(&cam);
  
  gluUnProject(__mouse.x, __mouse.y, 0.99f, view.modelView, view.projectionX, view.viewPort, &pos1.x, &pos1.y, &pos1.z);
  gluUnProject(__mouse.x, __mouse.y, 1, view.modelView, view.projectionX, view.viewPort, &pos2.x, &pos2.y, &pos2.z);

  pos2.y *= -1;
  pos1.y *= -1;
  vec3d_add(pos1,cam,pos1);
  vec3d_add(pos2,cam,pos2);
  
  if (position)
  {
    vec3d_cpy((*position),pos1);
  }
  if (vector)
  {
    vec3d_sub((*vector),pos2,pos1);
    vec3d_normalize(vector);
    camera_get_rotation(&rotation);
    vec3d_rotate_about_x(vector,rotation.x);
    vec3d_rotate_about_y(vector,rotation.y);
    vec3d_rotate_about_z(vector,rotation.z);
  }
}
