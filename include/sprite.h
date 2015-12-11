#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "graphics3d.h"
#include "types.h"
#include <SDL.h>
#include <SDL_image.h>

/**
  data structure to handle sprite data
*/
typedef struct
{
  Uint32       _refCount;         /**<internal ref count.  DO NOT ACCESS*/
  GLuint        _glImage;          /**<opengl texture handle*/
  char          __buf;
  SDL_Surface * _sdlSurface;       /**<SDL_surface loaded.*/
  Line       filename;          /**<name with path of the file loaded*/
  Uint32       framesPerLine;     /**<How many frames are on each line of the
                                       sprite sheet*/
  Uint32       numFrames;         /**<How Many frames are in the sprite*/
  Uint32       imageWidth;        /**<Width of the image in pixels*/
  Uint32       imageHeight;       /**<Height of the image in pixels*/
  float      texWidth;          /**<Width of the image as a percentage of the
                                       texture*/
  float      texHeight;         /**<Height of the image as a percentate of
                                       the texture*/
  double     x3D;   /**<width of the texture relative to gl
                                       coordinate space*/ 
  double     y3D;  /**<height of the texture relative to gl
                                       coordinate space*/ 
  double     z3D;  /**<depth of the texture relative to gl
                                       coordinate space*/ 
  Uint32       frameWidth;        /**<width of an individual frame in pixels*/
  Uint32       frameHeight;       /**<Height of an individual frame in pixels*/
  float      frameTextureWidth; /**<width of an individual frame as a
                                       pecentage of the texture*/
  float      frameTextureHeight;/**<height of an individual frame as a
                                       percentage of the texture*/
}Sprite;


/**
 * @brief set up the sprite system for use.  Must be called before any sprite
 * functions are used.  graphics_init() must have been previously called.
 */
void sprite_init();

/**
 * @brief Load the sprite from file, or if it has already been loaded, get a
 * pointer to the one in memory
 *
 * @param filename the sprite file to load
 * @param frameWidth the width of an individual frame in pixels
 * @param frameHeight the height of an individual frame in pixels
 */
Sprite *sprite_load(
    Line filename,
    Uint32 frameWidth,
    Uint32 frameHeight
  );

/**
 * @brief free the given sprite
 *
 * @param sprite a pointer to the sprite pointer.  the sprite will be freed and the
 * pointer to the sprite will be set to NULL;
 */
void sprite_free(Sprite **sprite);

/**
 * @brief draw a given sprite to the screen buffer with the given transformation
 * information
 *
 *  @param sprite the sprite to be drawn
 *  @param frame the frame to draw
 *  @param x the x position to draw the sprite
 *  @patam y the y position to draw the sprite
 *  @param scaleX the x scaling factor to be aplied
 *  @param scaleY the y scaling factor to be aplied
 *  @param rotation the rotation, in degrees to draw the sprite
 *  @param hFlip if true, the sprite will be flipped horizontally
 *  @param vFlip if true, the sprite will be flipped vertically
 *  @param color a vector of (R,G,B) color shift. (1,1,1) is no shift
 *  @param alpha the transparency of the draw.  0 is clear, 1 is solid
 */
void sprite_draw_transformed(
    Sprite *sprite,
    Uint32 frame,
    int x,
    int y,
    float scaleX,
    float scaleY,
    float rotation,
    int hFlip,
    int vFlip,
    Vec3D color,
    float alpha
  );

/**
 * @brief draw a given sprite to the screen buffer
 *
 *  @param sprite the sprite to be drawn
 *  @param frame the frame to draw
 *  @param x the x position to draw the sprite
 *  @patam y the y position to draw the sprite
 */
void sprite_draw(
  Sprite *sprite,
  Uint32    frame,
  int     x,
  int     y
);

/**
 * @brief draw a sprite in 3D space
 *
 * @param sprite the sprite to draw
 * @param frame the frame to draw
 * @param position the position in space to draw the sprite
 * @param scale the scale to apply to the sprite.  1,1,1 is no scale
 * @param rotation the orientation to apply to the sprite
 * @param color a vector of (R,G,B) color shift. (1,1,1) is no shift
 * @param alpha the transparency of the draw.  0 is clear, 1 is solid
 */
void sprite_draw_transformed_3d(
    Sprite *sprite,
    Uint32 frame,
    Vec3D position,
    Vec3D scale,
    Vec3D rotation,
    Vec3D color,
    float alpha
  );

/**
 * @brief draw a sprite in 3D space oriented towards the camera in "billboard"
 * style
 *
 * @param sprite the sprite to draw
 * @param frame the frame to draw
 * @param position the position in space to draw the sprite
 * @param scale the scale to apply to the sprite.  1,1,1 is no scale
 * @param alpha the transparency of the draw.  0 is clear, 1 is solid
 */
void sprite_draw_3d(
    Sprite *sprite,
    Uint32 frame,
    Vec3D position,
    Vec3D scale,
    float alpha
  );

/**
 * @brief creates a blank sprite and returns it.  surface and gl texture data
 * are not set.  This could use be used in a custome sprite set up such as
 * storing the test rendered from the font system.
 */
Sprite *sprite_new();


















/*#define MAXSPRITEFILENAME 512

typedef struct Sprite_T
{
    SDL_Surface *image;               /**<pointer to the actual image in memory/
    GLuint texture;
    char  filename[MAXSPRITEFILENAME]; /**<the name of the file, keep them short/
    int   w, h;                         /**<the width and height of the frames of the sprites, not the file itself/
    int   framesperline;                /**<default is 16/
    int   color1,color2,color3;         /**<if the file was palette swapped these are the colors that were changed/
    Uint8 used;                         /**<used by the maintanenc functions to keep track of how many times a single sprite is being used/
}Sprite;


/**
 * @brief initializes internal sprite system
 /
void InitSpriteList();

/**
 * @brief Decriments the reference count for the sprite passed.
 * frees the sprite when no longer used
 * 
 * @param img the sprite to free
 /
void FreeSprite(Sprite *img);


Sprite *LoadSprite(char *filename,int sizex, int sizey);

/**
 * @brief draws a specifif sprite to a surface
 * 
 * @param sprite a pointer to the sprite to draw
 * @param surface a pointer to the SDL Surface to draw to
 * @param sx the x position on the surface to draw the sprite at
 * @param sy the y position on the surface to draw the sprite at
 * @param frame the sprite frame to draw
 /
void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame);

/**
 * @brief clears all loaded sprites in system.  Called before program exit
 /
void CloseSprites();*/



#endif