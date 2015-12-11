#ifndef __INPUT_H__
#define __INPUT_H__

#include "types.h"
#include <SDL.h>

/**
 * Definition of terms for input:
 *
 *  - Down - state of the key when the input is active.
 *  - Up - state of the key when the input is inactive.
 *  - Pressed - state of a key or input when the last frame was not "down" and this
 * frame it is "down"
 *  - Released - state of an input wen the last frame was "down" and this frame is
 * "up"
 *  - Held - state of an input when this frame and the previous few frames have
 *  been "down".  Note that the threshold for this is configurable
 *  - Double Tap - state of an input when the key is rapidly tapped twice in
 *  succession.  Threshold is configurable.
 *
 */

/**
 * @brief structure for defining a control type.
 */
typedef struct
{
  Line  name;  /*name of the input*/
  Word  type;  /*input type: key, button, axis*/
  Uint32  id;      /*which key or axis or button*/
  float value;    /*current state of the input*/
  float oldvalue; /*last state of the input*/
  Uint32  timePress; /*this time index*/
  Uint32  lastPress; /*time index of the last time this input was pressed*/
}Input;

/**
 * @brief converts input config into sdl_key info
 *
 * @param type type of input: "mod" "key" "button", "+axis" "-axis"
 */
int input_parse(char *type, char *input);


/**
 * @brief initializes internal components of input
 */
void input_init();

/**
 * @brief returns the initialization status of input
*
 * @return 1 if input has been initialized, 0 otherwise.
 */
int input_is_initialized();

/**
 * @brief determines if the keyboard key was pressed this frame
 *
 * @param key the SDLKey value of the key of interest.  example: SDLK_a for the
 * a key.
 *
 * @return 1 if pressed, 0 otherwise
 */
int input_is_key_pressed(int key);

/**
 * @brief determines if the keyboard key was released this frame
 *
 * @param key the SDLKey value of the key of interest.  example: SDLK_a for the
 * a key.
 *
 * @return 1 if released, 0 otherwise
 */
int input_is_key_released(int key);

/**
 * @brief determines if the keyboard key is held during this frame
 *
 * @param key the SDLKey value of the key of interest.  example: SDLK_a for the
 * a key.
 *
 * @return 1 if held, 0 otherwise
 */
int input_is_key_held(int key);

/**
 * @brief determines if the keyboard modifier key is held during this frame
 * Uses SDLMod enumerations, ex: KMOD_LSHIFT, KMOD_ALT, etc.
 * @param key the SDLMod value of the mod of interest.  
 *
 * @return 1 if held, 0 otherwise
 */
int input_is_mod_held(Uint32 mod);

/**
 * @brief determines if the keyboard modifier key is down during this frame
 * Uses SDLMod enumerations, ex: KMOD_LSHIFT, KMOD_ALT, etc.
 * @param key the SDLMod value of the mod of interest.
 *
 * @return 1 if held, 0 otherwise
 */
int input_is_mod_down(Uint32 mod);

/**
 * @brief determines if the keyboard modifier key is pressed during this frame
 * Uses SDLMod enumerations, ex: KMOD_LSHIFT, KMOD_ALT, etc.
 * @param key the SDLMod value of the mod of interest.  
 *
 * @return 1 if held, 0 otherwise
 */
int input_is_mod_pressed(Uint32 mod);

/**
* @brief determines if the keyboard modifier key is released during this frame
* Uses SDLMod enumerations, ex: KMOD_LSHIFT, KMOD_ALT, etc.
* @param key the SDLMod value of the mod of interest.  
*
* @return 1 if held, 0 otherwise
*/
int input_is_mod_released(Uint32 mod);

Input *input_get_nth_input(char *name,int n);

/**
 * @brief determines the if the named input is in a down state at the point of
 * this call.
 *
 * @param input the name of the input in question.  eg: "jump"
 *
 * @return the value of the input (if analog a float, if not 1 or 0) 
 */
float input_is_input_down(char *input);

/**
 * @brief determines the if the named input has been down continuously for set
 * period of time.  the held threshold is configureable.
 *
 * @param input the name of the input in question.  eg: "jump"
 *
 * @return the value of the input (if analog a float, if not 1) if it is held, 0 otherwise
 */
float input_is_input_held(char *input);

/**
 * @brief determines the if the named input was just pressed this frame
 *
 * @param input the name of the input in question.  eg: "jump"
 *
 * @return true if it is pressed
 */
int input_is_input_pressed(char *input);

/**
 * @brief determines the if the named input was just released this frame
 *
 * @param input the name of the input in question.  eg: "jump"
 *
 * @return true if it is pressed
 */
int input_is_input_released(char *input);

/**
 * @brief determines the if the named input was tapped twice in rapid
 * succession.  The threshold for "double tap" is configurable
 *
 * @param input the name of the input in question.  eg: "jump"
 *
 * @return true if it is pressed
 */
int input_is_input_double_tapped(char *input);

/**
 * @brief updated internal input states.  This need to be called every frame or
 * as often as you need your inputs updated.
 */
void input_update();

/**
 * @brief return the value of the input specified.  This is useful for analog
 * control types such as a joystick's X axis.
 */
float get_input_value(char *input);

/**
 * @brief checks for any quit condition from the user.
 *
 * @return 1 if a quit condition has been found.
 */
int input_quit_check();

#endif