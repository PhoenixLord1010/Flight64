#ifndef __SPRITE__
#define __SPRITE__

#include "graphics3d.h"
#include <SDL.h>

#define MAXSPRITEFILENAME 512

typedef struct Sprite_T
{
    SDL_Surface *image;               /**<pointer to the actual image in memory*/
    GLuint texture;
    char  filename[MAXSPRITEFILENAME]; /**<the name of the file, keep them short*/
    int   w, h;                         /**<the width and height of the frames of the sprites, not the file itself*/
    int   framesperline;                /**<default is 16*/
    int   color1,color2,color3;         /**<if the file was palette swapped these are the colors that were changed*/
    Uint8 used;                         /**<used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;


/**
 * @brief initializes internal sprite system
 */
void InitSpriteList();

/**
 * @brief Decriments the reference count for the sprite passed.
 * frees the sprite when no longer used
 * 
 * @param img the sprite to free
 */
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
 */
void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame);

/**
 * @brief clears all loaded sprites in system.  Called before program exit
 */
void CloseSprites();



#endif
