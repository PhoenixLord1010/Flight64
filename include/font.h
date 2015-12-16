#ifndef __FONT_H__
#define __FONT_H__

#include "types.h"
#include <SDL_ttf.h>

/**
 * @brief these enumerations will be used as a shorthand access to the default
 *        loaded fonts at different point sizes
 */
enum FontSizes {
  FontTiny   = 0,  /**<10 point loaded font*/
  FontSmall  = 1,  /**<12 point loaded font*/
  FontMedium = 2,  /**<14 point loaded font*/
  FontLarge  = 3,  /**<18 point loaded font*/
  FontXLarge = 4,  /**<32 point loaded font*/
  FontHuge   = 5   /**<52 point loaded font*/
};
/*below should be one larger than largest font*/
#define FontSizeMax 6

enum TextJustify {
  JustifyNone      = -1,
  JustifyLeft      = 0,
  JustifyCenter    = 1,
  JustifyRight     = 2
};

typedef struct
{
  Uint32    _refCount;
  Line    filename;
  int     point;
  int     index;
  TTF_Font * _font;
}Font;

void font_init();
int font_initialized();
void font_close();

/**
 * @brief loads a specific ttf font file from disk.  Returns a pointer to the
 *        loaded font or NULL on failure.
 * @param filename the file of the font to load.
 * @param pointSize the pointsize to load the font at
 */
Font * font_load(
    char   *filename,
    int  pointSize
  );

/**
 * @brief frees a font resource and sets the provided pointer to NULL
 * 
 * @param a pointer to a font pointer.
 */
void font_free(
    Font **font
  );

/**
 * @brief Renders text to screen using the default font of the specified size
 *
 * @param text the text to render.  does nothing if NULL or string length is zero
 * @param x the x position (in pixels) to draw from (left).
 * @param y the y position (in pixels) to draw from (top). 
 * @param color the (r,g,b) color to render the font in
 * @param alpha the translucency to render the font it
 * @param size which of the enumerated sizes to draw the font with
 */
void font_draw_text(
    char   * text,
    int   x,
    int   y,
    Vec3D color,
    float alpha,
    Uint32  size
  );
  
/**
 * @brief same as font_draw_text, except it will be right justified.
 * @param x the x position (in pixels) to draw from (right).
 * @param y the y position (in pixels) to draw from (top). 
 * @param color the (r,g,b) color to render the font in
 * @param alpha the translucency to render the font it
 * @param size which of the enumerated sizes to draw the font with
 * @param justify see enumerations for how it will be justified
 */
void font_draw_text_justify(
    char   * text,
    int   x,
    int   y,
    Vec3D color,
    float alpha,
    Uint32  size,
    Uint32  justify
  );

void font_draw_text_justify_custom(
    char    * text,
    int    x,
    int    y,
    Vec3D  color,
    float  alpha,
    Font * font,
    Uint32   justify
  );

/**
 * @brief Renders text to screen using the default font of the specified size
 *
 * @param text the text to render.  does nothing if NULL or string length is zero
 * @param x the x position (in pixels) to draw from (left).
 * @param y the y position (in pixels) to draw from (top). 
 * @param color the (r,g,b) color to render the font in
 * @param alpha the translucency to render the font it
 * @param font a pointer to a specific font to draw with
 */    
void font_draw_text_custom(
    char    *text,
    int   x,
    int   y,
    Vec3D color,
    float alpha,
    Font *font
  );

/**
 * @brief Renders text to screen using the default font of the specified size
 *
 * @param text the text to render.  does nothing if NULL or string length is zero
 * @param x the x position (in pixels) to draw from (left).
 * @param y the y position (in pixels) to draw from (top). 
 * @param w the width (in pixels) to limit writing to
 * @param h the height (in pixels) to limit writing to.  if 0, no limit applied
 * @param color the (r,g,b) color to render the font in
 * @param alpha the translucency to render the font it
 * @param size which of the enumerated sizes to draw the font with
 */
void font_draw_text_block(
    char   * text,
    int   x,
    int   y,
    int   w,
    int   h,
    Vec3D color,
    float alpha,
    int   size
  );

/**
 * @brief Renders text to screen using the default font of the specified size
 *
 * @param text the text to render.  does nothing if NULL or string length is zero
 * @param block the bounding rectangle to limit drawing by.  
 *        X and Y components are used for position
          if the H component is 0, no bottom limit is applied.
 * @param color the (r,g,b) color to render the font in
 * @param alpha the translucency to render the font it
 * @param font a pointer to the specific font to draw with
 */
void font_draw_text_block_custom(
    char   * text,
    RectFloat  block,
    Vec3D color,
    float alpha,
    Font *font
  );

/**
 * @brief returns the bounding rect ( in screen pixels ) of the text.
 * 
 * @param text the text to use in determining the size
 * @param size which default font to use for determining the size
 *
 * @return a rectangle that contains the bounding rectangle of the text.
 *         The x and y components will be empty.
 */
RectFloat font_get_bounds(
    char * text,
    int size
  );

/**
 * @brief returns the bounding rect ( in screen pixels ) of the text.
 * 
 * @param text the text to use in determining the size
 * @param font a pointer to the font to use in determining size
 *
 * @return a rectangle that contains the bounding rectangle of the text.
 *         The x and y components will be empty.
 */
RectFloat font_get_bounds_custom(
    char    * text,
    Font * font
  );

/**
 * @brief returns the bounding rect ( in screen pixels ) of the text.
 * 
 * @param text the text to use in determining the size
 * @param size which default font to use for determining the size
 * @param w the width limit to apply to the block.
 * @param h the height limit to apply to the block.
 * 
 * @return a rectangle that contains the bounding rectangle of the text.
 *         The x and y components will be empty.  Note that the w and h 
 *         provided may not be the actual bounds produced.
 */
RectFloat font_get_block_bounds(
    char * text,
    int size,
    Uint32 w,
    Uint32 h
  );

/**
 * @brief returns the bounding rect ( in screen pixels ) of the text.
 * 
 * @param text the text to use in determining the size
 * @param font pointer of the font to use in determining the size
 * @param w the width limit to apply to the block.
 * @param h the height limit to apply to the block.
 * 
 * @return a rectangle that contains the bounding rectangle of the text.
 *         The x and y components will be empty.  Note that the w and h 
 *         provided may not be the actual bounds produced.
 */
RectFloat font_get_block_bounds_custom(
    char    * text,
    Font * font,
    Uint32   w,
    Uint32   h
  );


void font_draw_text_3D(
    char *text,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec3D color,
    float alpha,
    int   size
  );

void font_draw_text_3D_custom(
    char *text,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec3D color,
    float alpha,
    Font *font
  );

void font_draw_text_3D_block(
    char *text,
    RectFloat  block,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec3D color,
    float alpha,
    int   size
  );

void font_draw_text_3D_block_custom(
    char *text,
    RectFloat  block,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec3D color,
    float alpha,
    Font *font
  );

Uint32 font_get_text_height_average(Uint32 fontsize);

Uint32 font_get_text_height_average_custom(Font *font);

Uint32 font_justify_from_string(Line justify);

#endif