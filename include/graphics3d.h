#ifndef __GRAPHICS3D_H__
#define __GRAPHICS3D_H__

#include "types.h"
#include <SDL.h>
#include <glew.h>

#define Red         0xDD0000
#define Green       0x00DD00
#define Blue        0x0000DD
#define Yellow      0xDDDD00
#define Orange      0xEE5522
#define Violet      0xDD00DD
#define Brown       0x663311
#define Grey        0x888888
#define DarkRed     0x880000
#define DarkGreen   0x008800
#define DarkBlue    0x000088
#define DarkYellow  0x888800
#define DarkOrange  0x774411
#define DarkViolet  0x660066
#define DarkBrown   0x442205
#define DarkGrey    0x444444
#define LightRed    0xFF2222
#define LightGreen  0x33FF33
#define LightBlue   0x3333FF
#define LightYellow 0xFFFF11
#define LightOrange 0xFFDD11
#define LightViolet 0xFF55FF
#define LightBrown  0x886622
#define LightGrey   0xBBBBBB
#define Black       0x000000
#define White       0xFEFEFE
#define Tan         0xFFDD22
#define Gold        0xBB9900
#define Silver      0xAAAABB
#define YellowGreen 0x44EE01
#define Cyan        0x00FFFF
#define Magenta     0xFF00FF

/*color constants:*/
enum colors {
  E_Red         = 1,
  E_Green       = 2,
  E_Blue        = 3,
  E_Yellow      = 4,
  E_Orange      = 5,
  E_Violet      = 6,
  E_Brown       = 7,
  E_Grey        = 8,
  E_DarkRed     = 9,
  E_DarkGreen   = 10,
  E_DarkBlue    = 11,
  E_DarkYellow  = 12,
  E_DarkOrange  = 13,
  E_DarkViolet  = 14,
  E_DarkBrown   = 15,
  E_DarkGrey    = 16,
  E_LightRed    = 17,
  E_LightGreen  = 18,
  E_LightBlue   = 19,
  E_LightYellow = 20,
  E_LightOrange = 21,
  E_LightViolet = 22,
  E_LightBrown  = 23,
  E_LightGrey   = 24,
  E_Black       = 25,
  E_White       = 26,
  E_Tan         = 27,
  E_Gold        = 28,
  E_Silver      = 29,
  E_YellowGreen = 30,
  E_Cyan        = 31,
  E_Magenta     = 32
};

typedef struct
{
  Uint32		xRes;           /**<horizontal screen resolition*/
  Uint32		yRes;           /**<vertical screen resolution*/
  Uint32		depth;          /**<screen bit depth*/
  Uint32		aMask;          /**<bit mask for alpha*/
  Uint32		rMask;          /**<bit mask for red*/
  Uint32		gMask;          /**<bit mask for green*/
  Uint32		bMask;          /**<bit mask for blue*/
  int				openGLOn;       /**<1 if rendering with openGL,0 uses SDL*/
  int				mipmapping;     /**<true if the engine is using mipmapping*/
  GLint				viewPort[4];    /**<GL view port matrix*/
  GLdouble			modelView[16];  /**<GL model view matrix*/
  GLdouble			projection[16]; /**<GL projection matrix*/
  GLdouble			projectionX[16];
  Vec3D			rotation;
}GraphicsView;


/**
* @brief Initializes the graphics system.
* Will load up the default config file in system/graphics.cfg and set up the
* rending system based on the settings in the config file.
*/
void graphics_init();

/**
 * @breif loads up graphics config information from file or sets defaults
 */
void graphics_config();

/**
* @brief Query the graphics ready state.
* @return 1 if ready, 0 otherwise.
*/
int graphics_initialized();

GLuint get_shader_program();

/**
* @brief relaunches the game screen in the new screen resolution. Invoked an
* registered callbacks.
*
* @param w the new width for the screen
* @param h the new height for the screen
* @param fs set true for fullscreen mode or false for window mode.
*/
void resize_graphics(Uint32 w, Uint32 h, int fs);

/**
* @brief Sets up a function that will be called after any change to the internal
* graphics settings is made.  It will call the function once to set up initial
* states.
*
* @param callback the function to be invoked when the graphics are resized.
*/
void graphics_register_resize(void (*callback)(GraphicsView info));

/*
* @brief takes a screen shot and saves it to filename specififed
*
* @param file the name of the file to save over / create for the screen shot.
*/
void graphics_save_screen_shot(const char *file);

/**
* @brief Given an index, it returns a hex code that corresponds to that color
* @param color the color from colors
* @return the hex defined useful in direct graphics calls.
*/
Uint32 index_color(Uint32 color);

/**
 * @brief blanks the drawing buffer to the preset background color.  Should be
 * called befor starting any frame buffer drawing.
 */
void graphics_frame_begin();

/**
* @brief blit the contents of the draw buffer to the screen and toggle the draw
* buffers.  It will prepare a freshly cleared drawing buffer for the next frame
*/
void graphics_frame_end();

/**
 * @brief returns the hax color value of the pixel at the given position in the
 * provided sdl surface.  Note that the surface will need to be locked while
 * this operation is being performed.
 *
 * @param surface the sdl surface that holds the pixel data
 * @param x the x position to check
 * @param y the y position to check
 *
 * @return the hex color value of the color found.
 */
Uint32 graphics_get_pixel(SDL_Surface *surface, Uint32 x, Uint32 y);

/**
 * @brief change a specific pixel in an SDL surface.
 *
 * @param surface a valid SDL_Surface that will be written to
 * @param x the x position in the image
 * @param y the y position in the image
 * @param pixel the hex value of the color to be drawn.
 *
 */
void graphics_put_pixel(
    SDL_Surface *surface,
    Uint32 x,
    Uint32 y,
    Uint32 pixel
  );

/**
 * @brief retrieve the viewports active pixel format
 *
 * @return a pointer to the advice pixel format data.  DO NOT WRITE TO IT.
 */
SDL_PixelFormat *graphics_get_pixel_format();

/**
 * @brief retrieve the current calculated FPS
 *
 * @return the calculated frame per second
 */
float graphics_get_FPS();

/**
 * @brief retrieves the current graphics view state
 *
 * @param view output.  Current view is loaded into the structure passed.
 */
void graphics_get_view(GraphicsView *view);

void graphics_set_proj(GLdouble proj[16]);

/**
 * @brief retrieves the current graphics frame time index
 */
Uint32 graphics_get_now();

/**
 * @brief retrieves the current screen resolution
 * @param w the output width of the view screen
 * @param h the output height of the view screen
 */
void graphics_get_size(Uint32 * w, Uint32 * h);

/**
 * @brief calculate the 3D position for the given 2D position at the depth
 * provided
 *
 * @param x the 2D space x position
 * @param y the 2D space y position
 * @param z the depth into 3D space to calculate the projection
 * @param model the current gl model
 * @param proj the current gl projection
 * @param view the current gl view
 * @param glx the output projected x position in 3D
 * @param gly the output projected y position in 3D
 * @param glz the output projected z position in 3D
 */
void opengl_get_gl_coordinate(
  GLint    x,
  GLint    y,
  GLdouble z,
  const GLdouble * model,
  const GLdouble * proj,
  const GLint    * view,
  GLfloat * glx,
  GLfloat * gly,
  GLfloat * glz
  );

/**
 * @brief calculate the 2D screen coordinates for the given 3D position
 * @param glx x position in 3D
 * @param gly y position in 3D
 * @param glz z position in 3D
 * @param model the current gl model
 * @param proj the current gl projection
 * @param view the current gl view
 * @param x output the 2D space x position
 * @param y output he 2D space y position
 */
void opengl_get_screen_coordinate(
  GLdouble glx,
  GLdouble gly,
  GLdouble glz,
  const GLdouble * model,
  const GLdouble * proj,
  const GLint    * view,
  GLint    *x,
  GLint    *y
);


#endif