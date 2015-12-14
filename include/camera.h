#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "types.h"
#include <SDL.h>

enum CameraTypes {
  CameraEditor  = 0, /**<follows editor controls*/
  Camera2D      = 1, /**<limits to 2D bounds of play area*/
  CameraThird   = 2, /**<over the shoulder third person view of target entity*/
  CameraFirst   = 3, /**<first person of the target entity*/
  CameraCustom  = 4  /**<custom camera type.*/
};

/**
* @brief function pointer prototype for getting the target's orientation
*/
typedef Orientation (*CameraGetTargetOri)(void *target);

typedef struct Camera_S
{
  Uint32     cameratype;  /**<see CameraTypes*/
  Vec2D    bounds;      /**<width height of screen*/
  Vec3D    position;    /**<position of the cameratype*/
  Vec3D    rotation;    /**<rotation of the camera*/
  Vec3D	   followPos;
  float    followDist;  /**<the following distance to the target*/
  float    followHeight;/**<how high up the camera should be when following a target*/
  float    moveStep;    /**<how fast the camera moves with mouse.*/
  void * target;      /**<target entity to watch*/
  CameraGetTargetOri getOri; /**<for getting the target orientation*/
  void (*update)(struct Camera_S *cam,void *data);  /**<what update function to call for camera*/
  void      * customData;  /**<for custom camera type*/
}Camera;

/**
 * @brief function pointer prototype for setting a custom
 */
typedef void (*CameraUpdate)(Camera *cam, void *data);

/**
 * @brief loads camera configuration from file or sets up defaults.
 */
void camera_config();

/**
 * @brief sets up camera perspective based on graphics view.
 */
void camera_init();

/**
 * @brief updates the camera based on type.  Should be called each frame before setup.
 */
void camera_update();

/**
 * @brief sets up the camera for this frame within the current context
 */
void camera_setup();

/**
* @brief Tell the camera to follow an entity
* @param target which entity to follow
*/
void camera_set_target(void *target);

void camera_set_height(float dist);
void camera_set_near_clip(float clip);
void camera_set_far_clip(float clip);
void camera_set_aspect_ratio(float aspect);
void camera_set_view_angle(float clip);
void camera_set_target_orientation(CameraGetTargetOri targetFunc);
void camera_set_position_2d(Vec2D position);
void camera_set_position(Vec3D position);
void camera_set_rotation(Vec3D rotation);
void camera_set_roll(float roll);
void camera_set_pitch(float pitch);
void camera_set_yaw(float yaw);
void camera_set_dist(float dist);
float camera_get_dist();
void camera_set_follow(Vec3D pos);
void camera_get_follow(Vec3D *pos);

/**
 * @brief get the camera position in 3D space
 * @param position output position of the camera
 */
void camera_get_position(Vec3D *position);

/**
 * @brief get the camera rotation
 * @param rotation output position of the camera
 */
void camera_get_rotation(Vec3D *rotation);

int camera_is_rect_in_view(Vec3D point, RectFloat bounds);
int camera_is_circle_in_view(Vec3D point, float radius);

Uint32 camera_get_type_by_string(char *camtype);


#endif