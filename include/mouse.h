#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "types.h"
#include "op_3d.h"
#include <SDL.h>

enum MouseStates {
  MouseLeft       = 0,
  MouseCenter     = 1,
  MouseRight      = 2,
  MouseWheelUp    = 3,
  MouseWheelDown  = 4
};

/**
* @brief Initializes internal mouse data based on loaded config.
*/
void mouse_init();

/**
* @brief draws the mouse to the screen based on current states
*/
void mouse_draw();

/**
* @brief Stops drawing the mouse
*/
void mouse_hide();

/**
* @brief Starts drawing the mouse
*/
void mouse_show();

/**
  * @brief resests the show/hiding of a mouse.  Sets it to hiden.
  * Useful when moving into a new context
  */
void mouse_reset();

/**
 * @brief updates the internal mouse information.  Relies on global input update.
 */
void mouse_update();

/**
 * @brief returns the state of the mouse button specified
 *
 * @param button which mouse button to check
 * @return true if the button is in the pressed state.
*/
int mouse_input_state(Uint32 button);

/**
* @brief checks if the mouse button is pressed this frame
*
* @param button which mouse button to check
* @return true if the button was pressed this frame
*/
int mouse_input_pressed(Uint32 button);

/**
* @brief checks if the mouse button is released this frame
*
* @param button which mouse button to check
* @return true if the button was released
*/
int mouse_input_released(Uint32 button);

/**
* @brief checks if the mouse button is held
*
* @param button which mouse button to check
* @return true if the button is held
*/
int mouse_input_held(Uint32 button);

/**
* @brief checks if the mouse button was double-tapped
*
* @param button which mouse button to check
* @return true if the button was double tapped
*/
int mouse_input_double_tapped(Uint32 button);

/**
 * @brief gets the current mouse position
 *
 * @param x putput pointer to where the X position can be written to.
 * @param y putput pointer to where the Y position can be written to.
 */
void mouse_get_position(int *x, int *y);

/**
 * @brief gets the current mouse position as a 2d vector
 *
 * @param pos putput pointer to where the X & Y position can be written to.
 */
void mouse_get_position_vec2d(Vec2D *pos);

/**
* @brief gets the change in mouse position since the last input frame
*
* @param x putput pointer to where the change in X position can be written to.
* @param y putput pointer to where the change in Y position can be written to.
*/
void mouse_get_motion(int *dx, int *dy);

/**
* @brief checks if the mouse is within the rect specified.
*
* @param rect the bounding rect to check for.
* @return 1 if the mouse is in the rect, 0 otherwise.
*/
int mouse_in_rect(RectFloat rect);

/**
 * @brief this returns the mouse position in 3D space and the vector that
 * points into the screen.  This can be used to have mouse selection of objects in 3D space
 * @param position output optional if passed, this will return the mouse's position in 3D space
 * @param vecctor output optional if passed, this will return a vector pointing from the mouse into the screen
 */
void mouse_get_3d_ray(Vec3D *position,Vec3D *vector);

/**
 * @brief draw the line under the mouse in 3D space.
 * NOTE: it may only be visible as a red dot with a green border.
 * If the ray intersects something, only the red dot.
 */
void mouse_draw_3d_ray();

/**
 * @brief get the point where the mouse intersects the provided quad.
 * @param contact output optional this will be the point the mouse is over on the quad
 * @param quad the quad to test
 */
int mouse_get_quad3d_intersect(Vec3D *contact,Quad3D quad);


#endif