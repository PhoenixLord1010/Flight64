#include "camera.h"
#include "mouse.h"
#include "graphics3d.h"
#include "types.h"
#include "simple_logger.h"
#include <math.h>

/*local global variables*/
Camera __camera;

/*local function prototypes*/
void camera_editor_update(Camera *cam, void *data);
void camera_2d_update(Camera *cam, void *data);
void camera_third_update(Camera *cam, void *data);
void camera_first_update(Camera *cam, void *data);

/*function definitions*/

void camera_config()
{
  memset(&__camera,0,sizeof(Camera));
  __camera.cameratype = Camera2D;
  __camera.followDist = 16;
  __camera.moveStep = 1;
}

void camera_init()
{
  GraphicsView view;
  slog("camera:initializing\n");
  graphics_get_view(&view);
  __camera.bounds.x = view.xRes;
  __camera.bounds.y = view.yRes;
  switch(__camera.cameratype)
  {
    case CameraEditor:
      __camera.update = camera_editor_update;
      __camera.position.y = __camera.followHeight;
      break;
    case Camera2D:
      __camera.update = camera_2d_update;
      __camera.position.y = __camera.followHeight;
      break;
    case CameraThird:
      __camera.update = camera_third_update;
      break;
    case CameraFirst:
      __camera.update = camera_first_update;
      break;
  }
  slog("camera:initialized\n");
}

void camera_editor_update(Camera *cam, void *data)
{
  int x,y;
  /*check input*/
  if (!cam)return;
  if (mouse_input_held(MouseCenter))
  {
    mouse_get_motion(&x, &y);
    cam->position.x += (-x * __camera.moveStep);
    cam->position.z += (y * __camera.moveStep);
  }
  if (mouse_input_held(MouseLeft) && mouse_input_held(MouseRight))
  {
    mouse_get_motion(&x, &y);
    cam->position.y += (-y * __camera.moveStep);
  }
}

void camera_2d_update(Camera *cam, void *data)
{
  Orientation ori;
  if (__camera.getOri == NULL)return;
  ori = __camera.getOri(__camera.target);
  __camera.position.x = (ori.position.x + __camera.position.x) * 0.5;
  __camera.position.y = (ori.position.y + __camera.position.y) * 0.5;
}

void camera_third_update(Camera *cam, void *data)
{
}

void camera_first_update(Camera *cam, void *data)
{
}

void camera_update()
{
  if (__camera.update == NULL)return;
  __camera.update(&__camera,NULL);
}

void camera_setup()
{
  glRotatef(-__camera.rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(-__camera.rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(-__camera.rotation.z, 0.0f, 0.0f, 1.0f);
  glTranslatef(-__camera.position.x,
               -__camera.position.y,
               -__camera.position.z);
}

void camera_set_height(float dist)
{
  __camera.position.y = __camera.followHeight = dist;
}

void camera_set_target_orientation(CameraGetTargetOri targetFunc)
{
  __camera.getOri = targetFunc;
}

void camera_set_target(void *target)
{
  __camera.target = target;
}

void camera_set_yaw(float yaw)
{
  __camera.rotation.y = yaw;
}

void camera_set_roll(float roll)
{
  __camera.rotation.z = roll;
}

void camera_set_pitch(float pitch)
{
  __camera.rotation.x = pitch;
}

void camera_set_dist(float dist)
{
	__camera.followDist = dist;
}

float camera_get_dist()
{
	return __camera.followDist;
}

void camera_set_follow(Vec3D pos)
{
	vec3d_cpy(__camera.followPos,pos);
}

void camera_get_follow(Vec3D *pos)
{
	if (!pos)return;
	vec3d_cpy((*pos),__camera.followPos); 
}

void camera_set_rotation(Vec3D rotation)
{
  int i;
  GraphicsView view;
  GLdouble mx[16],rt[16];
  graphics_get_view(&view);

  vec3d_cpy(__camera.rotation,rotation);

  rt[0] = cos((__camera.rotation.y) * DEGTORAD) * cos((__camera.rotation.z) * DEGTORAD);
  rt[1] = (sin((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.y) * DEGTORAD) * cos((__camera.rotation.z) * DEGTORAD)) - (cos((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.z) * DEGTORAD));
  rt[2] = (cos((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.y) * DEGTORAD) * cos((__camera.rotation.z) * DEGTORAD)) + (sin((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.z) * DEGTORAD));
  rt[3] = 0;
  rt[4] = cos((__camera.rotation.y) * DEGTORAD) * sin((__camera.rotation.z) * DEGTORAD);
  rt[5] = (sin((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.y) * DEGTORAD) * sin((__camera.rotation.z) * DEGTORAD)) + (cos((__camera.rotation.x) * DEGTORAD) * cos((__camera.rotation.z) * DEGTORAD));
  rt[6] = (cos((__camera.rotation.x) * DEGTORAD) * sin((__camera.rotation.y) * DEGTORAD) * sin((__camera.rotation.z) * DEGTORAD)) - (sin((__camera.rotation.x) * DEGTORAD) * cos((__camera.rotation.z) * DEGTORAD));
  rt[7] = 0;
  rt[8] = -sin((__camera.rotation.y) * DEGTORAD);
  rt[9] = sin((__camera.rotation.x) * DEGTORAD) * cos((__camera.rotation.y) * DEGTORAD);
  rt[10] = cos((__camera.rotation.x) * DEGTORAD) * cos((__camera.rotation.y) * DEGTORAD);
  rt[11] = 0;
  rt[12] = 0;
  rt[13] = 0;
  rt[14] = 0;
  rt[15] = 1;

  mx[0] = (rt[0] * view.projectionX[0]) + (rt[1] * view.projectionX[4]) + (rt[2] * view.projectionX[8]) + (rt[3] * view.projectionX[12]);
  mx[1] = (rt[0] * view.projectionX[1]) + (rt[1] * view.projectionX[5]) + (rt[2] * view.projectionX[9]) + (rt[3] * view.projectionX[13]);
  mx[2] = (rt[0] * view.projectionX[2]) + (rt[1] * view.projectionX[6]) + (rt[2] * view.projectionX[10]) + (rt[3] * view.projectionX[14]);
  mx[3] = (rt[0] * view.projectionX[3]) + (rt[1] * view.projectionX[7]) + (rt[2] * view.projectionX[11]) + (rt[3] * view.projectionX[15]);
  mx[4] = (rt[4] * view.projectionX[0]) + (rt[5] * view.projectionX[4]) + (rt[6] * view.projectionX[8]) + (rt[7] * view.projectionX[12]);
  mx[5] = (rt[4] * view.projectionX[1]) + (rt[5] * view.projectionX[5]) + (rt[6] * view.projectionX[9]) + (rt[7] * view.projectionX[13]);
  mx[6] = (rt[4] * view.projectionX[2]) + (rt[5] * view.projectionX[6]) + (rt[6] * view.projectionX[10]) + (rt[7] * view.projectionX[14]);
  mx[7] = (rt[4] * view.projectionX[3]) + (rt[5] * view.projectionX[7]) + (rt[6] * view.projectionX[11]) + (rt[7] * view.projectionX[15]);
  mx[8] = (rt[8] * view.projectionX[0]) + (rt[9] * view.projectionX[4]) + (rt[10] * view.projectionX[8]) + (rt[11] * view.projectionX[12]);
  mx[9] = (rt[8] * view.projectionX[1]) + (rt[9] * view.projectionX[5]) + (rt[10] * view.projectionX[9]) + (rt[11] * view.projectionX[13]);
  mx[10] = (rt[8] * view.projectionX[2]) + (rt[9] * view.projectionX[6]) + (rt[10] * view.projectionX[10]) + (rt[11] * view.projectionX[14]);
  mx[11] = (rt[8] * view.projectionX[3]) + (rt[9] * view.projectionX[7]) + (rt[10] * view.projectionX[11]) + (rt[11] * view.projectionX[15]);
  mx[12] = (rt[12] * view.projectionX[0]) + (rt[13] * view.projectionX[4]) + (rt[14] * view.projectionX[8]) + (rt[15] * view.projectionX[12]);
  mx[13] = (rt[12] * view.projectionX[1]) + (rt[13] * view.projectionX[5]) + (rt[14] * view.projectionX[9]) + (rt[15] * view.projectionX[13]);
  mx[14] = (rt[12] * view.projectionX[2]) + (rt[13] * view.projectionX[6]) + (rt[14] * view.projectionX[10]) + (rt[15] * view.projectionX[14]);
  mx[15] = (rt[12] * view.projectionX[3]) + (rt[13] * view.projectionX[7]) + (rt[14] * view.projectionX[11]) + (rt[15] * view.projectionX[15]);

  graphics_set_proj(mx);

  //slog("camera.z = %d, proj.z = %d",__camera.rotation.z,view.rotation.z);
}

void camera_get_rotation(Vec3D *rotation)
{
  if (!rotation)return;
  vec3d_cpy((*rotation),__camera.rotation);
}

void camera_get_position(Vec3D *position)
{
  if (!position)return;
  vec3d_cpy((*position),__camera.position);
}

void camera_set_position_2d(Vec2D position)
{
  vec2d_cpy(__camera.position,position);
}

void camera_set_position(Vec3D position)
{
  vec3d_cpy(__camera.position,position);
}

Uint32 camera_get_type_by_string(char *camtype)
{
  if (!camtype)return 0;
  if (strcmp(camtype,"EDITOR") == 0)
    return CameraEditor;
  if (strcmp(camtype,"2D") == 0)
    return Camera2D;
  if (strcmp(camtype,"THIRD") == 0)
    return CameraThird;
  if (strcmp(camtype,"FIRST") == 0)
    return CameraFirst;
  if (strcmp(camtype,"CUSTOM") == 0)
    return CameraCustom;
  return 0;
}
