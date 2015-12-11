#include "sprite.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include <stdlib.h>
#include <string.h>

/* internal globals. DO NOT EXTERN OUTSIDE OF THIS FILE!*/
Uint32		_max_sprites = 0;  /*this is going to be configurable*/
Sprite		*_sprite_list = NULL;
Uint32		_num_sprites = 0;
int			_sprite_init = 0;

GraphicsView _graphics_view;

/* internal private functions */
void sprite_set_gl_coordinates(Sprite *sprite);
void sprite_resync_graphics_view(GraphicsView info);
void sprite_close();


int sprite_initialized()
{
	return _sprite_init;
}

void sprite_init()
{
  _max_sprites = 255;
	
	slog("sprites: initializing\n");
  
  _num_sprites = 0;
  _sprite_list = (Sprite *)malloc(sizeof(Sprite)*_max_sprites);
  memset(_sprite_list,0,sizeof(Sprite)*_max_sprites);
  graphics_register_resize(sprite_resync_graphics_view);
  atexit(sprite_close);
  _sprite_init = 1;
  slog("sprites: initialized\n");
}

Sprite *sprite_get_loaded_by_filename(Line filename)
{
	int i;
	if ((_sprite_list == NULL)||(!_sprite_init))
  {
  	slog("sprite:sprite system initialized\n");
  	return NULL;
  }
	for (i = 0;i < _max_sprites;i++)
  {
  	if (_sprite_list[i]._refCount)
    {
    	if (strcmp(_sprite_list[i].filename,filename)==0)
    		return &_sprite_list[i];
    }
  }
  return NULL;
}

Sprite *sprite_new()
{
  int i;
  /*makesure we have the room for a new sprite*/
  if(_num_sprites >= _max_sprites)
  {
    slog("sprite: Maximum Sprites Reached.\n");
    return NULL;
  }
  for(i = 0;i < _max_sprites;i++)
  {
    if(_sprite_list[i]._refCount == 0)break;
  }
  if (i == _max_sprites)
  {
    slog("sprite count error!");
  	return NULL;
  }
  memset(&_sprite_list[i],0,sizeof(Sprite));
  _sprite_list[i]._refCount = 1;
  _num_sprites++;
  return &_sprite_list[i];
}

Sprite *sprite_load(
    Line filename,
    Uint32 frameWidth,
    Uint32 frameHeight
  )
{
  int k,j;
  SDL_Surface *temp;
  SDL_Surface *temp1;
  int w, h;
  Uint8 r,g,b,a;

  /*first see if the sprite is already in memory*/
  Sprite *newSprite = NULL;
  if ((!filename) || (strlen(filename) <= 0))
  {
    slog("sprite:cannot load a sprite of empty filename.\n");
    return NULL;
  }
  newSprite = sprite_get_loaded_by_filename(filename);
  if (newSprite != NULL)
  {
  	newSprite->_refCount++;
  	return newSprite;
  }

  /*now get a new blank sprite*/
  newSprite = sprite_new();
  if (newSprite == NULL)
  {
    slog("sprite: out of sprite slots.!\n");
  	return NULL;
  }

  /*begin new sprite loading and construction*/
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
    slog("unable to load a sprite\n");
    return NULL;
  }
  temp1 = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGBA4444,0);
  strncpy(newSprite->filename,filename,80);
  
  newSprite->framesPerLine = 16;
  /*if width is not specified, assume whole image is a single frame*/
  if(frameWidth != -1)
  {
    newSprite->frameWidth = frameWidth;
    newSprite->frameHeight = frameHeight;
  }
  else
  {
    newSprite->frameWidth = temp1->w;
    newSprite->frameHeight = temp1->h;
  }
  /*makes sure the width and height of the entire surface is opengl compatable*/
  newSprite->imageWidth = w = temp1->w;
  newSprite->imageHeight = h = temp1->h;

  newSprite->_sdlSurface = SDL_CreateRGBSurface(
    0,
    w,
    h,
    _graphics_view.depth,
    _graphics_view.rMask,
    _graphics_view.gMask,
    _graphics_view.bMask,
    _graphics_view.aMask
  );

  /*Sets the clear color on the surface*/
/*  clearColor = SDL_MapRGBA(newSprite->_sdlSurface->format, 0, 0, 0,255);*/
  /* 
   * This goes through the pixel data copying it and checking each pixel for the
   * clear color, when found sets the alpha to 0
   */
  for(k=0;k < newSprite->_sdlSurface->h; k++)
  {
    for(j=0;j < newSprite->_sdlSurface->w; j++)
    {
      SDL_GetRGBA(graphics_get_pixel(temp1, j, k), temp1->format, &r, &g, &b, &a);
      graphics_put_pixel(newSprite->_sdlSurface, j, k, SDL_MapRGBA(newSprite->_sdlSurface->format, r, g, b, a));
      
    }
  }
  if (_graphics_view.openGLOn)
  {
    newSprite->frameTextureWidth = (float)newSprite->frameWidth/(float)w;
    newSprite->frameTextureHeight= (float)newSprite->frameHeight/(float)h;

    sprite_set_gl_coordinates(newSprite);

    glGenTextures(1, &newSprite->_glImage);
    glBindTexture(GL_TEXTURE_2D,newSprite->_glImage);

    if(_graphics_view.mipmapping == 1)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE,
          newSprite->_sdlSurface->pixels);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
          newSprite->_sdlSurface->pixels);
    }

    SDL_FreeSurface(newSprite->_sdlSurface);
    newSprite->_sdlSurface = NULL;
  }
  SDL_FreeSurface(temp1);
  temp1 = NULL;
  SDL_FreeSurface(temp);
  temp = NULL;
  return newSprite;
}

void sprite_resync_graphics_view(GraphicsView info)
{
  int i;
	memcpy(&_graphics_view,&info,sizeof(GraphicsView));
  if (_sprite_list == NULL)
  {
  	return;
  }
  for (i = 0; i < _max_sprites; i++)
  {
    if (_sprite_list[i]._refCount <= 0)
      continue;
    sprite_set_gl_coordinates(&_sprite_list[i]);
  }
}

void sprite_draw_3d(
    Sprite *sprite,
    Uint32 frame,
    Vec3D position,
    Vec3D scale,
    float alpha
  )
{
  sprite_draw_transformed_3d(
    sprite,
    frame,
    position,
    scale,
    vec3d(0,0,0),
    vec3d(1,1,1),
    alpha
  );
}

void sprite_draw_transformed_3d(
    Sprite *sprite,
    Uint32 frame,
    Vec3D position,
    Vec3D scale,
    Vec3D rotation,
    Vec3D color,
    float alpha
  )
{
  RectFloat src;
  float left,right,top,bottom;
  if(sprite == NULL)
  {
    slog("passed in a NULL sprite to draw.\n");
    return;
  }
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  
  glDepthFunc(GL_LEQUAL);
  
  glColor4f(color.x,color.y,color.z,alpha);
  
  src.x = frame%sprite->framesPerLine * sprite->frameTextureWidth;
  src.y = frame/sprite->framesPerLine * sprite->frameTextureHeight;
  src.w = sprite->frameTextureWidth + src.x;
  src.h = sprite->frameTextureHeight + src.y;

  left = src.x;
  right = src.w;
  top = src.y;
  bottom = src.h;
  
  glBindTexture(GL_TEXTURE_2D,sprite->_glImage);
  glPushMatrix();
  glTranslatef(position.x,position.y,position.z);
  glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(scale.x,scale.y,scale.z);
  glTranslatef(sprite->x3D *  0.5f,sprite->y3D *  0.5f,0.0f);
  glTranslatef(sprite->x3D * -0.5f,sprite->y3D * -0.5f,0.0f);
  glBegin( GL_QUADS );
  
  glTexCoord2f(left,top);
  glVertex3f(-sprite->x3D/2,-sprite->y3D/2,0.0f);
  
  glTexCoord2f(left,bottom);
  glVertex3f(-sprite->x3D/2,sprite->y3D/2,0.0f);
  
  glTexCoord2f(right,bottom);
  glVertex3f(sprite->x3D/2,sprite->y3D/2,0.0f);
  
  glTexCoord2f(right,top);
  glVertex3f(sprite->x3D/2,-sprite->y3D/2,0.0f);
  glEnd( );
  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glDisable(GL_NORMALIZE);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);  
}

void sprite_draw(
  Sprite *sprite,
  Uint32    frame,
  int     x,
  int     y
)
{
  sprite_draw_transformed(
    sprite,
    frame,
    x,
    y,
    1,
    1,
    0,
    0,
    0,
    vec3d(1,1,1),
    1
  );
}

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
  )
{
  RectFloat src;
  Vec3D pos;
  float left,right,top,bottom;
  GLdouble scrx, scry, glx, gly, glz;

  if(sprite == NULL)
  {
    slog("passed in a NULL sprite to draw.\n");
    return;
  }

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  
  src.x = frame%sprite->framesPerLine * sprite->frameTextureWidth;
  src.y = frame/sprite->framesPerLine * sprite->frameTextureHeight;
  src.w = sprite->frameTextureWidth + src.x;
  src.h = sprite->frameTextureHeight + src.y;
  
  scrx = (GLdouble)x;
  scry = _graphics_view.viewPort[3] - (GLdouble)y;
  glx = (GLdouble)pos.x;
  gly = (GLdouble)pos.y;
  glz = (GLdouble)pos.z;
  gluUnProject(scrx,scry,0.99f,
		_graphics_view.modelView,
        _graphics_view.projection,
        _graphics_view.viewPort,
        &glx,&gly,&glz
  );
  
  glBindTexture(GL_TEXTURE_2D,sprite->_glImage);
  if (!hFlip)
  {
    left = src.x;
    right = src.w;
  }
  else
  {
    left = src.w;
    right = src.x;
  }
  if (!vFlip)
  {
    top = src.y;
    bottom = src.h;
  }
  else
  {
    top = src.h;
    bottom = src.y;
  }
  glPushMatrix();
  glTranslatef(pos.x,pos.y,pos.z);
  glRotatef(rotation, 0.0f, 0.0f, 1.0f);
  glScalef(scaleX,scaleY,1);
  glTranslatef(sprite->x3D *  0.5f,sprite->y3D *  0.5f,0.0f);
  glBegin( GL_QUADS );
  
  glTexCoord2f(left,top);
  glVertex3f(-sprite->x3D/2,-sprite->y3D/2,0.0f);
  
  glTexCoord2f(left,bottom);
  glVertex3f(-sprite->x3D/2,sprite->y3D/2,0.0f);
  
  glTexCoord2f(right,bottom);
  glVertex3f(sprite->x3D/2,sprite->y3D/2,0.0f);
  
  glTexCoord2f(right,top);
  glVertex3f(sprite->x3D/2,-sprite->y3D/2,0.0f);
  
  glEnd( );
  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
}

void sprite_free(Sprite **sprite)
{
  if (!sprite_initialized())return;
  if (!sprite)return;
  if (!*sprite)return;
  if ((*sprite)->_refCount > 0)
  {
    (*sprite)->_refCount--;
  }
  *sprite = NULL;
}

void sprite_clear_all()
{
  int i;
  for(i = 0;i < _max_sprites;i++)
  {
    if(_sprite_list[i]._sdlSurface != NULL)
    {
      SDL_FreeSurface(_sprite_list[i]._sdlSurface);
    }
    if(glIsTexture(_sprite_list[i]._glImage) == GL_TRUE)
    {
      glDeleteTextures(
        1,
        &_sprite_list[i]._glImage
        );
    }
  }
  memset(_sprite_list,0,sizeof(Sprite) * _max_sprites);
}

void sprite_close()
{
  slog("sprite: closing\n");
  sprite_clear_all();
  free(_sprite_list);
  _sprite_list = NULL;
  _num_sprites = 0;
  _sprite_init = 0;
  slog("sprite: closed\n");
}


void sprite_set_gl_coordinates(Sprite *sprite)
{
  GLdouble scrx, scry, glx, gly, glz;
  
  scrx = (GLdouble)((_graphics_view.xRes>>1) + sprite->frameWidth);
  scry = _graphics_view.viewPort[3] - (GLdouble)((_graphics_view.yRes>>1) + sprite->frameHeight);
  glx = (GLdouble)sprite->x3D;
  gly = (GLdouble)sprite->y3D;
  glz = (GLdouble)sprite->z3D;
  gluUnProject(scrx,scry,0.99f,
		_graphics_view.modelView,
        _graphics_view.projection,
        _graphics_view.viewPort,
        &glx,&gly,&glz
  );
}


























/*#define MaxSprites    1024

static Sprite SpriteList[MaxSprites];


void InitSpriteList()
{
  int x;
  memset(SpriteList,0,sizeof(Sprite) * MaxSprites);
  for(x = 0;x < MaxSprites;x++)SpriteList[x].image = NULL;
  atexit(CloseSprites);
}


Sprite *SpriteNew()
{
    int i;
    for (i = 0;i < MaxSprites;i++)
    {
        if (!SpriteList[i].used)
        {
            memset(&SpriteList[i],0,sizeof(Sprite));
            SpriteList[i].used = 1;
            return &SpriteList[i];
        }
    }
    return NULL;
}

Sprite *SpriteGetByFilename(char *filename)
{
    int i;
    for (i = 0;i < MaxSprites;i++)
    {
        if  ((SpriteList[i].used > 0)
            &&(strcmp(SpriteList[i].filename,filename) == 0))
        {
            return &SpriteList[i];
        }
    }
    return NULL;
}

Sprite *LoadSprite(char *filename,int fw, int fh)
{
    Sprite *sprite;
    SDL_Surface *image;
    int Mode = GL_RGB;
    
    sprite = SpriteGetByFilename(filename);
    if (sprite)return sprite;
    
    sprite = SpriteNew();
    if (!sprite)return NULL;
    
    image = IMG_Load(filename);
    if (!image)
    {
        slog("failed to load sprite image file: %s, re: %s",filename, SDL_GetError());
        return NULL;
    }
    sprite->image = image;
    if (fw == -1)
    {
        sprite->w = sprite->image->w;
    }
    if (fh == -1)
    {
        sprite->h = sprite->image->h;
    }
    /*set the rest of the data/
    
    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    glGenTextures(1, &sprite->texture);
    glBindTexture(GL_TEXTURE_2D, sprite->texture);
    
    
    if(sprite->image->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, sprite->image->w, sprite->image->h, 0, Mode, GL_UNSIGNED_BYTE, sprite->image->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    return sprite;
}

void DeleteSprite(Sprite *sprite)
{
    if (!sprite)return;
    if (sprite->image != NULL)
    {
        SDL_FreeSurface(sprite->image);
    }
    memset(sprite,0,sizeof(Sprite));
}

void FreeSprite(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used./
  if (!sprite)return;
  sprite->used--;
  if(sprite->used <= 0)
  {
      DeleteSprite(sprite);
  }
}

void CloseSprites()
{
  int i;
  for(i = 0;i < MaxSprites;i++)
  {
      DeleteSprite(&SpriteList[i]);
  }
}
*/