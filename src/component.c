#include "component.h"
#include "component_button.h"
#include "font.h"
#include "sprite.h"
#include "mouse.h"
#include "draw.h"
#include "simple_logger.h"
#include <glib.h>

/*local types*/
typedef struct
{
  GString  * buffer;
  Uint32    justify;
  int    wordWrap;
  Uint32    fontSize;
  Vec3D   color;
  float   alpha;
  Font  * font;    /**<if defined, it will use the custom font to draw text*/
}ComponentLabel;

typedef struct
{
  Sprite *image;
  Uint32    frame;
  int    scaleToBounds;
}ComponentImage;

typedef struct
{
  Uint32    sliderType;
  Sprite *slider;
  Sprite *sliderHigh;
  Sprite *bar;
  Sprite *cap1;
  Sprite *cap2;
  Vec3D   barColor;
  Vec3D   sliderColor;
  int    vertical;
  float   position;
  float   oldPosition;
  Uint32    sliderState;
}ComponentSlider;

/*local global variables*/
Sprite	 *__component_slider[4] = {NULL,NULL,NULL,NULL};
Line     __component_slider_files[4];
Vec3D    __component_label_color = {1,1,1};
float    __component_label_alpha = 1;
Uint32     __component_label_font_size = 1;

/*local function prototypes*/
void component_label_new(Component *component);
void component_label_free(Component *component);
void component_label_move(Component *component,RectFloat newbounds);
void component_slider_new(Component *component);

int component_has_changed(Component *component);
int component_get_state(Component *component);

void component_textinput_new(Component *component);
void component_textinput_get_text(Component *component,char *text);


void component_get_rect_from_bounds(RectFloat *rect,RectFloat canvas, RectFloat bounds);

void component_label_free(Component *component);
void component_slider_free(Component *component);
void component_image_free(Component *component);

void component_make_label(
    Component * component,
    char         * text,
    Uint32        justify,
    int         fontSize,
    int        wordWrap,
    char         * fontName,
    Vec3D       color,
    float       alpha
  );


Component * component_new()
{
  Component *component = NULL;
  component = (Component *)malloc(sizeof(Component));
  if (component == NULL)
  {
    slog("component: failed to allocate new component!");
    return NULL;
  }
  memset(component,0,sizeof(Component));
  return component;
}

void component_free(Component **component)
{
  if ((!component)||(!*component))return;
  if ((*component)->data_free != NULL)
  {
    (*component)->data_free(*component);
  }
  free(*component);
  *component = NULL;
}

void component_move(Component *component,RectFloat newbounds)
{
  if (!component)return;
  if (component->data_move != NULL)
  {
    component->data_move(component,newbounds);
  }
  else
  {
    component_get_rect_from_bounds(&component->bounds,newbounds,component->rect);
  }
}

int component_update(Component *component)
{
  if (!component)return 0;
  if (component->data_update != NULL)
  {
    return component->data_update(component);
  }
  return 0;
}

int component_changed(Component *component)
{
  if (!component)return 0;
  if (component->data_changed == NULL)return 0;
  return component->data_changed(component);
}

void component_set_focus(Component *component,int focus)
{
  if (!component)return;
  if (!component->canHasFocus)return;
  component->hasFocus = focus;
}

ComponentLabel *component_get_label_data(Component *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != LabelComponent))
  {
    return NULL;
  }
  return (ComponentLabel*)component->componentData;
}

ComponentImage *component_get_image_data(Component *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != ImageComponent))
  {
    return NULL;
  }
  return (ComponentImage*)component->componentData;
}

ComponentSlider *component_get_slider_data(Component *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != SliderComponent))
  {
    return NULL;
  }
  return (ComponentSlider*)component->componentData;
}

/*
 **** Free ****
*/

void component_label_free(Component *component)
{
  ComponentLabel *label = component_get_label_data(component);
  if (label == NULL)return;
  if (label->buffer != NULL)g_string_free(label->buffer,0);
  if (label->font != NULL)font_free(&label->font);
  free(label);
  label = NULL;
  component->componentData = NULL;
}

void component_slider_free(Component *component)
{
  ComponentSlider *slider = component_get_slider_data(component);
  if (slider == NULL)return;
  sprite_free(&slider->slider);
  sprite_free(&slider->sliderHigh);
  sprite_free(&slider->bar);
  sprite_free(&slider->cap1);
  sprite_free(&slider->cap2);
  free(slider);
  component->componentData = NULL;
}

void component_image_free(Component *component)
{
  ComponentImage * image = component_get_image_data(component);
  if (image == NULL)return;
  sprite_free(&image->image);
  free(image);
  component->componentData = NULL;
}

/*
**** DRAW ****
*/


void component_slider_draw(Component *component)
{
  RectFloat r;
  Vec2D drawPosition;
  ComponentSlider *slider = component_get_slider_data(component);
  if (slider == NULL)return;
  rect_copy(&r,component->bounds);

  if (slider->vertical)
  {
    r.w = component->bounds.w * 0.5;
    r.x += r.w * 0.5;
    drawPosition.x = r.x + (r.w * 0.5);
    drawPosition.y = r.y + (r.h * slider->position);
  }
  else
  {
    r.h = component->bounds.h * 0.5;
    r.y += r.h * 0.5;
    drawPosition.y = r.y + (r.h * 0.5);
    drawPosition.x = r.x + (r.w * slider->position);
  }
  
  //draw_solid_rect(r,slider->barColor,1);
  //draw_rect(r,vec3d(1,1,1),1);
  if (slider->sliderState == SliderIdle)
  {
    if (slider->slider != NULL)
    {
      drawPosition.x -= slider->slider->frameWidth / 2;
      drawPosition.y -= slider->slider->frameHeight / 2;
      sprite_draw(slider->slider,0,drawPosition.x,drawPosition.y);
    }
    else
    {
      /*TODO: draw a slider*/
    }
  }
  else
  {
    if (slider->sliderHigh != NULL)
    {
      drawPosition.x -= slider->sliderHigh->frameWidth / 2;
      drawPosition.y -= slider->sliderHigh->frameHeight / 2;
      sprite_draw(slider->sliderHigh,0,drawPosition.x,drawPosition.y);
    }
    else
    {
      /*TODO: draw a slider*/
    }
  }
}


void component_label_draw(Component *component)
{
  ComponentLabel *label = component_get_label_data(component);
  if (label == NULL)return;
  if (label->buffer == NULL)return;
  if (label->font == NULL)
  {
    if (label->wordWrap)
    {
      font_draw_text_block(
      label->buffer->str,
      component->bounds.x,
      component->bounds.y,
      component->bounds.w,
      0,
      label->color,
      label->alpha,
      label->fontSize
      );
    }
    else
    {
      font_draw_text_justify(
        label->buffer->str,
        component->bounds.x,
        component->bounds.y,
        label->color,
        label->alpha,
        label->fontSize,
        label->justify
      );
    }
  }
  else
  {
    if (label->wordWrap)
    {
      font_draw_text_block_custom(
        label->buffer->str,
        component->bounds,
        label->color,
        label->alpha,
        label->font
      );
    }
    else
    {
      font_draw_text_justify_custom(
        label->buffer->str,
        component->bounds.x,
        component->bounds.y,
        label->color,
        label->alpha,
        label->font,
        label->justify
      );
    }
  }
}

void component_draw(Component *component)
{
  if (!component)return;
  if (component->data_draw == NULL)
  {
    printf("component %s has no draw function!\n",component->name);
    return;
  }
  if (!component->hidden)
  {
    component->data_draw(component);
  }
}

/*
  ***** Update *****
  
 */


int component_slider_update(Component *component)
{
  ComponentSlider *slider;
  RectFloat sliderRect;
  Vec2D drawPosition;
  int x = 0,y = 0;
  if (!component)return 0;
  slider = component_get_slider_data(component);
  if (slider == NULL)return 0;

  mouse_get_position(&x,&y);
  
  if (slider->vertical)
  {
    drawPosition.x = component->bounds.x + (component->bounds.w * 0.5);
    drawPosition.y = component->bounds.y + (component->bounds.h * slider->position);
  }
  else
  {
    drawPosition.y = component->bounds.y + (component->bounds.h * 0.5);
    drawPosition.x = component->bounds.x + (component->bounds.w * slider->position);
  }
  
  if (slider->slider != NULL)
  {
    sliderRect.x = drawPosition.x - (slider->slider->frameWidth /2);
    sliderRect.y = drawPosition.y - (slider->slider->frameHeight /2);
    sliderRect.w = slider->slider->frameWidth;
    sliderRect.h = slider->slider->frameHeight;
  }
  else
  {
    /*TODO: support drawn sliders*/
  }
  if ((slider->sliderState == SliderHeld) && (mouse_input_released(MouseLeft)))
  {
    return 1;
  }
  if ((slider->sliderState == SliderHeld) && (mouse_input_held(MouseLeft)))
  {
    slider->oldPosition = slider->position;
    if (slider->vertical)
    {
      if (y < component->bounds.y)y = component->bounds.y;
      if (y > component->bounds.y + component->bounds.h)y = component->bounds.y + component->bounds.h;
      y -= component->bounds.y;
      if (component->bounds.h != 0)slider->position = (float)y / component->bounds.h;
    }
    else
    {
      if (x < component->bounds.x)x = component->bounds.x;
      if (x > (component->bounds.x + component->bounds.w))x = component->bounds.x + component->bounds.w;
      x -= component->bounds.x;
      if (component->bounds.w != 0)slider->position = (float)x / component->bounds.w;
    }
    return 0;
  }
  if (mouse_in_rect(sliderRect))
  {
    slider->sliderState = SliderHigh;
    if (mouse_input_pressed(MouseLeft))
    {
      slider->sliderState = SliderHeld;
    }
  }
  else
  {
    slider->sliderState = SliderIdle;
  }
  return 0;
}

/*
  ****** Makers ******
*/

void component_make_slider(
    Component * component,
    int        vertical,
    Line        sliderFile,
    Line        sliderHigh,
    Line        bar,
    Line        cap1,
    Line        cap2,
    Vec3D       barColor,
    Vec3D       sliderColor,
    float       startPosition,
    Uint32        sliderType
  )
{
  ComponentSlider * slider = NULL;
  if (!component)return;
  component_slider_new(component);
  slider = component_get_slider_data(component);
  if (slider == NULL)
  {
    return;
  }
  
  slider->vertical = vertical;
  switch(sliderType)
  {
    case SliderStock:
    case SliderCommon:
      if (vertical)
      {
        slider->slider = __component_slider[0];
        slider->sliderHigh = __component_slider[1];
      }
      else
      {
        slider->slider = __component_slider[2];
        slider->sliderHigh = __component_slider[3];
      }
      break;
    case SliderCustom:
      slider->slider = sprite_load(sliderFile,-1,-1);
      slider->sliderHigh = sprite_load(sliderHigh,-1,-1);
      slider->bar = sprite_load(bar,-1,-1);
      slider->cap1 = sprite_load(cap1,-1,-1);
      slider->cap2 = sprite_load(cap2,-1,-1);
      break;
  }
  slider->sliderType = sliderType;
  vec3d_cpy(slider->barColor,barColor);
  vec3d_cpy(slider->sliderColor,sliderColor);
  slider->position = slider->oldPosition = startPosition;
  
  component->data_free = component_slider_free;
  component->data_draw = component_slider_draw;
  component->data_update = component_slider_update;
}

void component_make_label(
    Component * component,
    char         * text,
    Uint32        justify,
    int         fontSize,
    int        wordWrap,
    char         * fontName,
    Vec3D       color,
    float       alpha
  )
{
  RectFloat r;
  ComponentLabel * label = NULL;
  if (!component)return;
  component_label_new(component);
  
  label = component_get_label_data(component);
  if (label == NULL)
  {
    return;
  }

  label->buffer = g_string_new(text);
  if (label->buffer == NULL)
  {
    slog("component:Failed to duplicate string for new label");
    component_label_free(component);
    return;
  }
  
  if ((fontName != NULL)&&(strlen(fontName) > 0))
  {
    label->font = font_load(fontName,fontSize);
    r = font_get_bounds_custom(text,label->font);
  }
  else
  {
    r = font_get_bounds(text,fontSize);
  }
  if (r.w > component->bounds.w)component->bounds.w = r.w;
  if (r.h > component->bounds.h)component->bounds.h = r.h;
  label->justify = justify;
  if (label->fontSize == -1)
  {
    /*default*/
    label->fontSize = __component_label_font_size;
  }
  else
  {
    label->fontSize = fontSize;
  }
  label->alpha = alpha;
  label->wordWrap = wordWrap;
  vec3d_cpy(label->color,color);
  component->data_free = component_label_free;
  component->data_draw = component_label_draw;
  component->data_move = component_label_move;
}

void component_slider_new(Component *component)
{
  if (component->componentData != NULL)
  {
    slog("component_slider_new:tried to make a label out of an existing component");
    return;
  }
  component->componentData = malloc(sizeof(ComponentSlider));
  if (component->componentData == NULL)
  {
    slog("component_slider_new: failed to allocate data for new slider");
    return;
  }
  memset(component->componentData,0,sizeof(ComponentSlider));
  component->type = SliderComponent;
}

void component_label_new(Component *component)
{
  if (component->componentData != NULL)
  {
    slog("component_label_new:tried to make a label out of an existing component");
    return;
  }
  component->componentData = malloc(sizeof(ComponentLabel));
  if (component->componentData == NULL)
  {
    slog("component_label_new: failed to allocate data for new label");
    return;
  }
  memset(component->componentData,0,sizeof(ComponentLabel));
  component->type = LabelComponent;
}

void label_get_text_size(Component *comp,Uint32 *w,Uint32 *h)
{
  RectFloat fontRect = {0,0,0,0};
  ComponentLabel * label = NULL;
  label = component_get_label_data(comp);
  if (label == NULL)return;
  if (!label->buffer)return;
  if (label->font)
  {
    fontRect = font_get_bounds_custom(
      label->buffer->str,
      label->font
    );
  }
  else
  {
    fontRect = font_get_bounds(
      label->buffer->str,
      label->fontSize
    );
  }
  if (w)
  {
    *w = fontRect.w;
  }
  if (h)
  {
    *h = fontRect.h;
  }
}

void label_get_text(Component *comp,Line text)
{
  ComponentLabel * label = NULL;
  label = component_get_label_data(comp);
  if (label == NULL)return;
  line_cpy(text,label->buffer->str);
}

void label_set_text(Component *comp,char *text)
{
  ComponentLabel * label = NULL;
  if (!text)return;
  label = component_get_label_data(comp);
  if (label == NULL)return;
  label->buffer = g_string_assign(label->buffer,text);
}

float slider_get_position(Component *comp)
{
  ComponentSlider * slider = NULL;
  slider = component_get_slider_data(comp);
  if (!slider)return 0;
  return slider->position;
}

void slider_set_position(Component *comp, float newPos)
{
  ComponentSlider * slider = NULL;
  slider = component_get_slider_data(comp);
  if (!slider)return;
  if (newPos < 0)newPos = 0;
  if (newPos > 1)newPos = 1;
  slider->position = newPos;
}

/*
  ******* new components *******
 */

Component *slider_common_new(
    Uint32        id,
    Line        name,
    RectFloat   rect,
    RectFloat        bounds,
    int        vertical,
    Vec3D       barColor,
    float       startPosition
  )
{
  if (vertical)
  {
    return slider_new(id,
                          name,
                          rect,
                          bounds,
                          vertical,
                          "",
                          "",
                          "",
                          "",
                          "",
                          barColor,
                          vec3d(1,1,1),
                          startPosition,
                          SliderCommon
                          );
  }
  else
  {
    
    return slider_new(id,
                          name,
                          rect,
                          bounds,
                          vertical,
                          "",
                          "",
                          "",
                          "",
                          "",
                          barColor,
                          vec3d(1,1,1),
                          startPosition,
                          SliderCommon
                          );
  }
}

Component *slider_new(
    Uint32       id,
    Line       name,
    RectFloat  rect,
    RectFloat       bounds,
    int       vertical,
    Line       slider,
    Line       sliderHigh,
    Line       bar,
    Line       cap1,
    Line       cap2,
    Vec3D      barColor,
    Vec3D      sliderColor,
    float      startPosition,
    Uint32       sliderType
  )
{
  Component *component = NULL;
  component = component_new();
  if (!component)return NULL;
  component_make_slider(component,
                            vertical,
                            slider,
                            sliderHigh,
                            bar,
                            cap1,
                            cap2,
                            barColor,
                            sliderColor,
                            startPosition,
                            sliderType);
  if (component->componentData == NULL)
  {
    component_free(&component);
    return NULL;
  }
  component->id = id;
  line_cpy(component->name,name);
  component->type = SliderComponent;
  rect_copy(&component->rect,rect);
  component_get_rect_from_bounds(&component->bounds,bounds, rect);
  if ((rect.w <= 1) && (rect.w >= 0))
    component->bounds.w = bounds.w * rect.w;
  else component->bounds.w = rect.w;
  if ((rect.h <= 1)  && (rect.h >= 0))
    component->bounds.h = bounds.h * rect.h;
  else component->bounds.h = rect.h;
  return component;
}

Component *label_new_default(
  Uint32        id,
  Line        name,
  RectFloat   rect,
  RectFloat        bounds,
  char         * text,
  int        wordWrap
)
{
  return label_new(
    id,
    name,
    rect,
    bounds,
    0,
    text,
    JustifyLeft,
    wordWrap,
    __component_label_font_size,
    NULL,
    __component_label_color,
    __component_label_alpha
  );
}

Component *label_new(
    Uint32        id,
    Line        name,
    RectFloat   rect,
    RectFloat        bounds,
    int        canHasFocus,
    char         * text,
    Uint32        justify,
    int        wordWrap,
    int         fontSize,
    char         * fontName,
    Vec3D       color,
    float       alpha
  )
{
  Component *component = NULL;
  if (!text)return NULL;
  component = component_new();
  if (!component)return NULL;
  component_get_rect_from_bounds(&component->bounds,bounds, rect);
  component_make_label(
    component,
    text,
    justify,
    fontSize,
    wordWrap,
    fontName,
    color,
    alpha
  );
  if (component->componentData == NULL)
  {
    component_free(&component);
    return NULL;
  }
  component->id = id;
  strncpy(component->name,name,WORDLEN);
  rect_copy(&component->rect,rect);
  component->type = LabelComponent;
  return component;
}

void component_label_move(Component *component,RectFloat newbounds)
{
  if (!component)return;
  rect_copy(&component->canvas,newbounds);
  component_get_rect_from_bounds(&component->bounds,newbounds,component->rect);
}

/*utility functions*/
void component_get_rect_from_bounds(RectFloat *rect,RectFloat canvas, RectFloat bounds)
{
  RectFloat rbounds;
  if (!rect)return;

  if (bounds.x < -1)
  {
    /*raw number from right edge*/
    rbounds.x = (canvas.w + bounds.x) / (float)canvas.w;
  }
  else if (bounds.x < 0)
  {
    /*percent of canvas from right*/
    rbounds.x = 1 + bounds.x;
  }
  else if (bounds.x <= 1)
  {
    /*percent of canvas from left*/
    rbounds.x = bounds.x;
  }
  else
  {
    rbounds.x = bounds.x / (float)canvas.w;
  }

  if (bounds.y < -1)
  {
    /*raw number from right edge*/
    rbounds.y = (canvas.h + bounds.y) / (float)canvas.h;
  }
  else if (bounds.y < 0)
  {
    /*percent of canvas from right*/
    rbounds.y = 1 + bounds.y;
  }
  else if (bounds.y <= 1)
  {
    /*percent of canvas from left*/
    rbounds.y = bounds.y;
  }
  else
  {
    rbounds.y = bounds.y / (float)canvas.h;
  }


  if (bounds.w <= 0)
  {
    /*fill to end*/
    rbounds.w = 1 - rbounds.x;
  }
  else if (bounds.w > 1)
  {
    /*exact size*/
    rbounds.w = bounds.w / (float)canvas.w;
  }
  else
  {
    /*% of canvas*/
    rbounds.w = bounds.w;
  }

  if (bounds.h <= 0)
  {
    /*fill to end*/
    rbounds.h = 1 - rbounds.y;
  }
  else if (bounds.h > 1)
  {
    /*exact size*/
    rbounds.h = bounds.h / (float)canvas.h;
  }
  else
  {
    /*% of canvas*/
    rbounds.h = bounds.h;
  }
  
  rect->x = canvas.x + (rbounds.x * canvas.w);
  rect->y = canvas.y + (rbounds.y * canvas.h);
  rect->w = canvas.w * rbounds.w;
  rect->h = canvas.h * rbounds.h;
}