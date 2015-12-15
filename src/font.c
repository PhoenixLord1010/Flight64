#include "font.h"
#include "graphics3d.h"
#include "simple_logger.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*local global variables*/
int   __font_initialized = 0;
Line   __default_font;
Font * __font_list = NULL;
Uint32   __font_max = 0;
Uint32   __font_count = 0;
Font * __font_size[FontSizeMax];

/*local function prototypes*/
void font_delete(Font *font);
void font_cleanup();
void font_clear();

void font_init()
{
  __font_max = 10;
  strncpy(__default_font,"fonts/default.ttf",LINELEN);
  slog("font: initializing\n");
  if (TTF_Init() != 0)
  {
    slog("font:unable to setup SDL_TTF");
    return;
  }
  atexit(TTF_Quit);
  atexit(font_close);
  __font_list = (Font *)malloc(sizeof(Font)*__font_max);
  memset(__font_list,0,sizeof(Font)*__font_max);
  __font_initialized = 1;
  __font_size[FontTiny] = font_load(__default_font,10);
  __font_size[FontSmall] = font_load(__default_font,12);
  __font_size[FontMedium] = font_load(__default_font,14);
  __font_size[FontLarge] = font_load(__default_font,18);
  __font_size[FontHuge] = font_load(__default_font,52);
  slog("font: initialized\n");
}

int font_initialized()
{
  return __font_initialized;
}

void font_close()
{
  int i;
  for ( i = 0; i < __font_max;i++)
  {
    font_delete(&__font_list[i]); 
  }
  free(__font_list);
  __font_list = NULL;
  __font_count = 0;
  __font_initialized = 0;
}

Font * font_new()
{
  int i;
  if (!font_initialized())
  {
    return NULL;
  }
  for (i = 0;i < __font_max;i++)
  {
    if (__font_list[i]._refCount == 0)
    {
      font_delete(&__font_list[i]);
      __font_list[i]._refCount = 1;
      __font_list[i].index = i;
      return &__font_list[i];
    }
  }
  return NULL;
}

Font * font_get_by_name_size(char *filename,int pointSize)
{
  int i;
  if ((!filename) ||
       (strlen(filename) == 0))
  {
    return NULL;
  }
  for (i = 0; i < __font_max; i++)
  {
    if ((strncmp(filename,__font_list[i].filename,128) == 0) &&
        (__font_list[i].point == pointSize))
    {
      return &__font_list[i];
    }
  }
  return NULL;
}

Font * font_load(
    char   *filename,
    int  pointSize
  )
{
  Font *font = NULL;
  font = font_get_by_name_size(filename,pointSize);
  if (font != NULL)
  {
    font->_refCount++;
    return font;
  }
  font = font_new();
  if (font == NULL)return NULL;
  font->_font = TTF_OpenFont(filename,pointSize);
  if(font->_font == NULL)
  {
    slog("Couldn't initialize Font: %s\n",SDL_GetError());
    font_delete(font);
    return NULL;
  }
  font->point = pointSize;
  strncpy(font->filename,filename,LINELEN);
  return font;
}

void font_free(
    Font **font
  )
{
  if ((font == NULL) ||
      (*font == NULL) ||
      ((*font)->_refCount == 0))
  {
    return;
  }
  (*font)->_refCount--;
  *font = NULL;
}

void font_delete(Font *font)
{
  if (font == NULL)return;
  if (font->_font != NULL)
  {
    TTF_CloseFont(font->_font);
  }
  memset(font,0,sizeof(Font));
}

void font_draw_text_justify_custom(
    char    * text,
    int    x,
    int    y,
    Vec3D  color,
    float  alpha,
    Font * font,
    Uint32   justify
  )
{
  RectFloat r;
  int dx;
  if (!font_initialized())return;
  switch (justify)
  {
    case JustifyLeft:
      dx = x;
      break;
    case JustifyCenter:
      dx = (x - (r.w/2));
      break;
    case JustifyRight:
      dx = (x - r.w);
      break;
  }
  r = font_get_bounds_custom(text,font);
  font_draw_text_custom(
    text,
    dx,
    y,
    color,
    alpha,
    font
  );
}

void font_draw_text_justify(
    char   * text,
    int   x,
    int   y,
    Vec3D color,
    float alpha,
    Uint32  size,
    Uint32  justify
  )
{
  RectFloat r;
  int dx;
  if (!font_initialized())return;
  r = font_get_bounds(text,size);
  switch (justify)
  {
    case JustifyLeft:
      dx = x;
      break;
    case JustifyCenter:
      dx = (x - (r.w/2));
      break;
    case JustifyRight:
      dx = (x - r.w);
      break;
  }
  font_draw_text(
    text,
    dx,
    y,
    color,
    alpha,
    size
  );
}

void font_draw_text(
    char   * text,
    int   x,
    int   y,
    Vec3D  color,
    float alpha,
    Uint32   size
  )
{
  if (!font_initialized())return;
  if (size >= FontSizeMax)
  {
    slog("font: passed a font size outside of available range");
    return;
  }
  font_draw_text_custom(
    text,
    x,
    y,
    color,
    alpha,
    __font_size[size]
  );
}

/*output of this function needs to be freed*/
char * font_clean_control_characters(char *in)
{
  char *out;
  int outIndex = 0;
  int inIndex = 0;
  out = malloc(strlen(in)*2);
  if (!out)return NULL;
  for (inIndex = 0;inIndex < strlen(in);inIndex++,outIndex++)
  {
    if (in[inIndex] == '\t')
    {
      out[outIndex++] = ' ';
      out[outIndex] = ' ';
      continue;
    }
    if (in[inIndex] == '\r')
    {
      out[outIndex] = ' ';
      continue;
    }
    out[outIndex] = in[inIndex];
  }
  out[outIndex] = '\0';
  return out;
}

void font_draw_text_custom(
    char    *text,
    int   x,
    int   y,
    Vec3D color,
    float alpha,
    Font *font
  )
{
  Vec3D pos,pos2,pos3,pos4,cam;
  int w,h;
  char *renderText = NULL;
  GLuint image;
  SDL_Surface *temp1 = NULL;
  SDL_Surface *temp = NULL;
  SDL_Surface *fontpic = NULL;
  SDL_Color colortype;
  GraphicsView view;

  if (!font_initialized())return;

  if((text == NULL) || 
      (strlen(text) <= 0))
  {
    slog("font:passed empty string for text draw.");
    return;
  }
  if (font == NULL)
  {
    slog("font: no font provided for draw.");
    return;
  }
  if (font->_font == NULL)
  {
    slog("font: bad Font provided for draw.");
    return;
  }
  camera_get_position(&cam);
  graphics_get_view(&view);
  colortype.r = 255 * color.x;
  colortype.g = 255 * color.y;
  colortype.b = 255 * color.z;
  colortype.a = SDL_ALPHA_OPAQUE;
  renderText = font_clean_control_characters(text);
  if (!renderText)
  {
    temp = TTF_RenderText_Blended(font->_font, text,colortype);
  }
  else
  {
    temp = TTF_RenderText_Blended(font->_font, renderText,colortype);
    free(renderText);
  }
  if (temp == NULL)
  {
    slog("font: unable to render text from SDL_ttf\n");
      return;
  }
  w = temp->w;
  h = temp->h;
  /*Creates an opengl compatable RGBA surface*/
  fontpic = SDL_CreateRGBSurface(0,w, h,view.depth,view.rMask,view.gMask,view.bMask,view.aMask);	
  /*Copies pixel data from the image to surface*/
  SDL_BlitSurface(temp, NULL, fontpic, NULL);
  SDL_FreeSurface(temp);

  glGenTextures(1, &image);
  glBindTexture(GL_TEXTURE_2D,image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, fontpic->pixels); 
  SDL_FreeSurface(fontpic);
  glEnable(GL_TEXTURE_2D);
	
  x=x+w;
  y=y+h;


  opengl_get_gl_coordinate(
    x,
    y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos.x,
    &pos.y,
    &pos.z
  );

  opengl_get_gl_coordinate(
    x,
    y-h,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos2.x,
    &pos2.y,
    &pos2.z
  );

  opengl_get_gl_coordinate(
    x-w,
    y-h,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos3.x,
    &pos3.y,
    &pos3.z
  );

  opengl_get_gl_coordinate(
    x-w,
    y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos4.x,
    &pos4.y,
    &pos4.z
  );


  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBegin( GL_QUADS );

  glTexCoord2f(0.0,0.0);
  glVertex3f(pos3.x + cam.x,pos3.y + cam.y,pos3.z + cam.z);

  glTexCoord2f(0.0,1.0);
  glVertex3f( pos4.x + cam.x,pos4.y + cam.y,pos4.z + cam.z);
	
  glTexCoord2f(1.0,1.0);
  glVertex3f( pos.x + cam.x,pos.y + cam.y,pos.z + cam.z);
	
  glTexCoord2f(1.0,0.0);
  glVertex3f( pos2.x + cam.x,pos2.y + cam.y,pos2.z + cam.z);
	
  glEnd( );
  glEnable(GL_DEPTH_TEST);
  
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glDeleteTextures(1, &image);
}

void font_crop_string(char *text,int length,int strl)
{
  int i;
  for(i = 0;i < strl - length;i++)
  {
    text[i] = text[i + length];
  }
  text[i - 1] = '\0';/*null terminate in case its overwritten*/
}


void font_draw_text_block(
    char   * text,
    int   x,
    int   y,
    int   w,
    int   h,
    Vec3D color,
    float alpha,
    int   size
  )
{
  RectFloat rect;
	
  if (!font_initialized())return;
  if (size >= FontSizeMax)
  {
    slog("font: passed a font size outside of available range");
    return;
  }

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  font_draw_text_block_custom(
    text,
    rect,
    color,
    alpha,
    __font_size[size]
  );
}

void font_draw_text_block_custom(
    char   * thetext,
    RectFloat  block,
    Vec3D color,
    float alpha,
    Font *font
  )
{
  Text textline;
  Text temptextline;
  Text text;
  Line word;
  int whitespace;
  int drawheight = block.y;
  int w,h;
  int i;
  int space;
  int lindex = 0;
  if (!font_initialized())return;
  if ((thetext == NULL)||(thetext[0] == '\0'))
  {
    slog("font: no text provided for draw.\n");
    return;
  }
  if (font == NULL)
  {
    slog("font: no font provided for draw.\n");
    return;
  }
  if (font->_font == NULL)
  {
    slog("font: bad Font provided for draw.\n");
    return;
  }
  strncpy(text,thetext,TEXTLEN);
  temptextline[0] = '\0';
  do
  {
    space = 0;
    i = 0;
    do
    {
      whitespace = 0;
      if(sscanf(&text[i],"%c",&word[0]) == EOF)break;
      if(word[0] == ' ')
      {
        space++;
        whitespace = 1;
      }
      if(word[0] == '\t')
      {
        space+=2;
        whitespace = 1;
      }
      i++;
    }while (whitespace);
    if (sscanf(text,"%s",word) == EOF)
    {
      block.y=drawheight+h;
      font_draw_text_custom(temptextline,block.x,block.y, color, alpha, font);
      return;
    }

    font_crop_string(text,strlen(word) + space,TEXTLEN);
    strncpy(textline,temptextline,TEXTLEN);/*keep the last line that worked*/
    for (i = 0;i < (space - 1);i++)
    {
      sprintf(temptextline,"%s%c",temptextline,' '); /*add spaces*/
    }
    sprintf(temptextline,"%s %s",temptextline,word); /*add a word*/
    TTF_SizeText(font->_font, temptextline, &w, &h); /*see how big it is now*/
    lindex += strlen(word);
    if(w > block.w)         /*see if we have gone over*/
    {
      block.y=drawheight+h;
      font_draw_text_custom(textline,block.x,block.y, color, alpha, font);

      /*draw the line and get ready for the next line*/
      drawheight += h;
      if (block.h != 0)
      {
        if ((drawheight + h) > block.h)
        {
          break;
        }
      }
      sprintf(temptextline,"%s",word); /*add a word*/
    }
  }while(1);
}

RectFloat font_get_bounds(
    char * text,
    int size
  )
{
  RectFloat r = {0,0,0,0};
  if (!font_initialized())return r;
  if (size >= FontSizeMax)
  {
    slog("font: passed a font size outside of available range");
    return r;
  }
  return font_get_bounds_custom(
    text,
    __font_size[size]
  );
}

RectFloat font_get_bounds_custom(
    char    * text,
    Font * font
  )
{
  RectFloat r = {0,0,0,0};
  if (!font_initialized())return r;
  if ((text == NULL)||(text[0] == '\0'))
  {
    slog("font: no text provided for draw.\n");
    return r;
  }
  if (font == NULL)
  {
    slog("font: no font provided for draw.\n");
    return r;
  }
  TTF_SizeText(font->_font,text, (int *)&r.w, (int *)&r.h);
  return r;
}

RectFloat font_get_block_bounds(
    char * text,
    int size,
    Uint32 w,
    Uint32 h
  )
{
  RectFloat r = {0,0,0,0};
  if (!font_initialized())return r;
  if (size >= FontSizeMax)
  {
    slog("font: passed a font size outside of available range");
    return r;
  }
  return font_get_block_bounds_custom(
    text,
    __font_size[size],
    w,
    h
  );
}
  


RectFloat font_get_block_bounds_custom(
    char    * thetext,
    Font * font,
    Uint32   w,
    Uint32   h
  )
{
  RectFloat r = {0,0,0,0};
  Text textline;
  Text temptextline;
  Text text;
  Line word;
  int whitespace;
  int tw = 0, th = 0;
  int drawheight = 0;
  int i = 0;
  int space = 0;
  int lindex = 0;
  if (!font_initialized())return r;
  if((thetext == NULL)||(thetext[0] == '\0'))
  {
    return r;
  }
  if (font == NULL)
  {
    slog("font: no font provided for draw.\n");
    return r;
  }

  strncpy(text,thetext,TEXTLEN);
  temptextline[0] = '\0';
  do
  {
    space = 0;
    i = 0;
    whitespace = 0;
    do
    {
      if(sscanf(&text[i],"%c",&word[0]) == EOF)break;
      if(word[0] == ' ')
      {
        space++;
        whitespace = 1;
      }
      if(word[0] == '\t')
      {
        space+=2;
        whitespace = 1;
      }
      i++;
    }while(whitespace);
    
    if(sscanf(text,"%s",word) == EOF)
    {
      break;
    }
    font_crop_string(text,strlen(word) + 1,TEXTLEN);
    strncpy(textline,temptextline,TEXTLEN);/*keep the last line that worked*/
    for(i = 0;i < (space - 1);i++)
    {
      sprintf(temptextline,"%s%c",temptextline,' '); /*add spaces*/
    }
    sprintf(temptextline,"%s %s",temptextline,word); /*add a word*/
    TTF_SizeText(font->_font, temptextline, &tw, &th); /*see how big it is now*/
    lindex += strlen(word);
    if(tw > w)         /*see if we have gone over*/
    {
      drawheight += th;
      if (h != 0)
      {
        if ((drawheight + th) > h)
        {
          break;
        }
      }
      sprintf(temptextline,"%s",word); /*add a word*/
    }
    else if (tw > r.w)
    {
      r.w = tw;
    }
  }while(1);
  r.h = drawheight + th;
  return r;
}

Uint32 font_get_text_height_average(Uint32 fontsize)
{
  RectFloat r;
  r = font_get_bounds(
    "gL",
    fontsize
  );
  return r.h;
}

Uint32 font_get_text_height_average_custom(Font *font)
{
  RectFloat r;
  r = font_get_bounds_custom(
    "gL",
    font
  );
  return r.h;
}

Uint32 font_justify_from_string(Line justify)
{
  if ((strncmp(justify,"LEFT",128) == 0) ||
      (strncmp(justify,"left",128) == 0) ||
      (strncmp(justify,"Left",128) == 0))
    return JustifyLeft;
  if ((strncmp(justify,"CENTER",128) == 0) ||
    (strncmp(justify,"center",128) == 0) ||
    (strncmp(justify,"Center",128) == 0))
    return JustifyCenter;
  if ((strncmp(justify,"RIGHT",128) == 0) ||
    (strncmp(justify,"right",128) == 0) ||
    (strncmp(justify,"Right",128) == 0))
    return JustifyRight;
  return JustifyNone;
}
