#include "component_button.h"
#include "font.h"
#include "draw.h"
#include "mouse.h"
#include "input.h"
#include "simple_logger.h";
#include <string.h>

/*local function prototypes*/
ComponentButton *component_get_button_data(Component *component);
void component_button_free(Component *component);
void component_button_new(Component *component);
void component_button_set_activation(Component* comp,
                                         int release,
                                         int press,
                                         int hold);

/*local global variables*/
Sprite	*__component_stock_button[ButtonStateMax] = {NULL,NULL,NULL,NULL};
Vec3D	__component_button_color[ButtonStateMax];
int		__component_button_offset_x = 0;
int		__component_button_offset_y = 0;
int     __component_button_text_outline = 1;
float	__component_button_alpha = 1;
int     __component_button_press_active = 0;
int     __component_button_hold_active = 0;
int     __component_button_release_active = 1;
int		__component_button_font_size = 3;

/*function definitions*/
void button_configure()
{
  Line buf;
  Line buttonfile,buttonhitfile,buttonhighfile,buttonsleepfile;

  __component_button_offset_x = 0;
  __component_button_offset_y = 0;
  __component_button_press_active = 0;
  __component_button_hold_active = 0;
  __component_button_release_active = 1;
  __component_button_font_size = 3;
  vec3d_set(__component_button_color[0],0.8,0.8,0.8);
  vec3d_set(__component_button_color[1],1,1,0);
  vec3d_set(__component_button_color[2],0.6,0.6,0.6);
  vec3d_set(__component_button_color[3],0.3,0.2,0.3);
  strcpy(buttonfile,"images/UI/btn.png");
  strcpy(buttonhitfile,"images/UI/btn_hit.png");
  strcpy(buttonhighfile,"images/UI/btn_high.png");
  strcpy(buttonsleepfile,"images/UI/btn_sleep.png");
  
  __component_stock_button[0] = sprite_load(buttonfile,-1,-1);
  __component_stock_button[1] = sprite_load(buttonhighfile,-1,-1);
  __component_stock_button[2] = sprite_load(buttonhitfile,-1,-1);
  __component_stock_button[3] = sprite_load(buttonsleepfile,-1,-1);
}

Uint32 button_get_state(Component *button)
{
  if (!component_get_button_data(button))return 0;
  return button->state;
}

void button_set_inactive(Component *button)
{
  if (!component_get_button_data(button))return;
  button->state = ButtonSleep;
}

void button_set_active(Component *button)
{
  if (!component_get_button_data(button))return;
  if (button->state == ButtonSleep)
    button->state = ButtonIdle;
}

void button_set_text(Component *button,Line newText)
{
  ComponentButton *buttonData = NULL;
  buttonData = component_get_button_data(button);
  if (!buttonData)return;
  strcpy(buttonData->buttonText,newText);
}

void button_get_text(Line outText,Component *button)
{
  ComponentButton *buttonData = NULL;
  buttonData = component_get_button_data(button);
  if (!buttonData)return;
  strcpy(outText,buttonData->buttonText);
}

void button_get_size(Component *button,Uint32 *w, Uint32 *h)
{
  RectFloat fontRect = {0,0,0,0};
  Uint32 imageW = 0, imageH = 0;
  ComponentButton *buttonData = NULL;
  buttonData = component_get_button_data(button);
  if (!buttonData)return;
  if (buttonData->button[0] != NULL)
  {
    imageW = buttonData->button[0]->frameWidth;
    imageH = buttonData->button[0]->frameHeight;
  }
  if (strlen(buttonData->buttonText)> 0)
  {
    fontRect = font_get_bounds(
      buttonData->buttonText,
      buttonData->fontSize
    );    
  }
  if (w)
  {
    *w = MAX(imageW,fontRect.w);
  }
  if (h)
  {
    *h = MAX(imageH,fontRect.h);
  }
}

void button_get_stock_size(Uint32 *w, Uint32 *h)
{
  if (__component_stock_button[0] == NULL)return;
  if (w)*w = __component_stock_button[0]->frameWidth;
  if (h)*h = __component_stock_button[0]->frameHeight;
}

ComponentButton *component_get_button_data(Component *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != ButtonComponent))
  {
    return NULL;
  }
  return (ComponentButton*)component->componentData;
}

void component_button_free(Component *component)
{
  ComponentButton *button;
  int i;
  button = component_get_button_data(component);
  if (button)return;
  for (i = 0; i < ButtonStateMax; i++)
  {
    if (button->button[i] != NULL)
    {
      sprite_free(&button->button[i]);
    }
  }
  free(button);
  button = NULL;
  component->componentData = NULL;
}

int component_button_update(Component *component)
{
  Vec2D v;
  int x,y;
  int mod = 1;
  ComponentButton *button = NULL;
  if (!component)return 0;
  button = component_get_button_data(component);
  if (!button)return 0;
  if (component->state == ButtonSleep)return 0;

  component->oldState = component->state;
  mouse_get_position(&x,&y);
  v.x = x;
  v.y = y;
  if (button->hotkeymod)
  {
    mod = input_is_mod_held(button->hotkeymod);
  }
  if ((button->input > 0) && (mod))
  {
    if (input_is_key_pressed(button->input))
    {
      component->state = ButtonPressed;
      return button->activeOnPress;
    }
    if (input_is_key_held(button->input))
    {
      component->state = ButtonPressed;
      return button->activeOnHold;
    }
    if (input_is_key_released(button->input))
    {
      component->state = ButtonIdle;
      return button->activeOnRelease;
    }
  }
  if (vec_in_rect(v,component->bounds))
  {
    component->state = ButtonHighlight;
    if (mouse_input_held(MouseLeft))
    {
      component->state = ButtonPressed;
      return button->activeOnHold;
    }
    if (mouse_input_pressed(MouseLeft))
    {
      component->state = ButtonPressed;
      return button->activeOnPress;
    }
    else if (mouse_input_released(MouseLeft))
    {
      component->state = ButtonIdle;
      return button->activeOnRelease;
    }
    return 0;
  }
  component->state = ButtonIdle;
  return 0;
}

void component_button_draw_rect(Component *component)
{
  RectFloat r;
  ComponentButton *button = NULL;
  button = component_get_button_data(component);
  if (!button)return;
  draw_solid_rect(component->bounds,button->backgroundColor,button->backgroundAlpha);
  switch (component->state)
  {
    case ButtonIdle:
      break;
    case ButtonHighlight:
      draw_solid_rect(component->bounds,button->highlightColor,0.3);
      break;
    case ButtonPressed:
      draw_solid_rect(component->bounds,button->pressColor,0.3);
      break;
  }
  
  font_draw_text_justify(
    button->buttonText,
    component->bounds.x + 2,
    component->bounds.y + 2,
    button->textColor[component->state],
    button->alpha,
    button->fontSize,
    button->justify
  );

  rect_copy(&r,component->bounds);
  r.x++;
  r.y++;
  r.h-=2;
  r.w-=2;
  draw_rect(component->bounds,vec3d(1,1,1),1);
  draw_rect(r,vec3d(1,1,1),1);
}

void component_button_draw(Component *component)
{
  RectFloat r;
  ComponentButton *button = NULL;
  Sprite *img = NULL;
  int x,y;
  Uint32 ofx = 0, ofy = 0;
  button = component_get_button_data(component);
  if (button == NULL)return;
  x = component->bounds.x;
  y = component->bounds.y;
  r = font_get_bounds(
    button->buttonText,
    button->fontSize
  );
  if ((component->state >= 0) &&
    (component->state < ButtonStateMax))
  {
    img = button->button[component->state];

    if (img != NULL)
    {
      sprite_draw(img,
                      0,
                      x,
                      y);
      x = x + (img->frameWidth/2);
      y = y + (img->frameHeight/2) - (r.h*0.5);
      ofx = __component_button_offset_x;
      ofy = __component_button_offset_y;
    }
    r.x = x + ofx;
    r.y = y + ofy;
    font_draw_text_justify(
      button->buttonText,
      r.x,
      r.y,
      button->textColor[component->state],
      button->alpha,
      button->fontSize,
      button->justify
    );
    if ((button->buttonType == ButtonText) &&
        (component->state == ButtonHighlight) &&
        (__component_button_text_outline))
    {
      draw_rect(component->bounds,__component_button_color[component->state],button->alpha);
    }
  }
}

void component_make_button(
    Component		*component,
    char			*buttonText,
    Uint32	justify,
    Uint32	buttonType,
    int				buttonHotkey,
    Uint32	buttonHotkeymod,
    Line			buttonUpFile,
    Line			buttonHighFile,
    Line			buttonDownFile,
    Vec3D			backgroundColor,
    float			backgroundAlpha,
    Vec3D			highlightColor,
    Vec3D			pressColor,
    Uint32	fontSize
  )
{
  ComponentButton * button = NULL;
  if (!component)return;
  component_button_new(component);

  button = component_get_button_data(component);
  if (button == NULL)
  {
    return;
  }
  button->alpha = 1;
  button->fontSize = fontSize;
  button->input = buttonHotkey;
  button->hotkeymod = buttonHotkeymod;

  strncpy(button->buttonText,buttonText,LINELEN);
  button->buttonType = buttonType;
  component->data_update = component_button_update;
  component->data_free = component_button_free;
  component->data_draw = component_button_draw;
  button->justify = justify;
  switch(buttonType)
  {
    case ButtonCustom:
      button->button[ButtonIdle] = sprite_load(buttonUpFile,-1,-1);
      button->button[ButtonHighlight] = sprite_load(buttonHighFile,-1,-1);
      button->button[ButtonPressed] = sprite_load(buttonDownFile,-1,-1);
      if (button->justify == JustifyNone)
      {
        button->justify = JustifyCenter;
      }
      break;
    case ButtonText:
      button->button[ButtonIdle] = NULL;
      button->button[ButtonHighlight] = NULL;
      button->button[ButtonPressed] = NULL;
      if (button->justify == JustifyNone)
      {
        button->justify = JustifyLeft;
      }
      break;
    case ButtonStock:
      button->button[ButtonIdle] = __component_stock_button[ButtonIdle];
      button->button[ButtonHighlight] = __component_stock_button[ButtonHighlight];
      button->button[ButtonPressed] = __component_stock_button[ButtonPressed];
      if (button->justify == JustifyNone)
      {
        button->justify = JustifyCenter;
      }
      break;
    case ButtonRect:
      component->data_draw = component_button_draw_rect;
      vec3d_cpy(button->backgroundColor,backgroundColor);
      button->backgroundAlpha = backgroundAlpha;
      if (button->justify == JustifyNone)
      {
        button->justify = JustifyLeft;
      }
      vec3d_cpy(button->highlightColor,highlightColor);
      vec3d_cpy(button->pressColor,pressColor);
      break;
  }
}

void component_button_new(Component *component)
{
  if (component->componentData != NULL)
  {
    slog("component:tried to make a button out of an existing component\n");
    return;
  }
  component->componentData = malloc(sizeof(ComponentButton));
  if (component->componentData == NULL)
  {
    slog("actor: failed to allocate data for new button\n");
    return;
  }
  memset(component->componentData,0,sizeof(ComponentButton));
  component->type = ButtonComponent;
}

Component *button_stock_new(
    Uint32        id,
    Word        name,
    RectFloat   rect,
    RectFloat        bounds,
    char         * buttonText,
    int         justify,
    int         buttonHotkey,
    Uint32        buttonHotkeymod,
    int        center
  )
{
  return button_new(
    id,
    name,
    rect,
    bounds,
    buttonText,
    3,
    ButtonStock,
    buttonHotkey,
    buttonHotkeymod,
    center,
    justify,
    NULL,
    NULL,
    NULL,
    vec3d(0,0,0),
    0,
    vec3d(0,0,0),
    vec3d(0,0,0)
  );
}

Component *button_text_new(
    Uint32        id,
    Word        name,
    RectFloat   rect,
    RectFloat        bounds,
    char         * buttonText,
    int         justify,
    Uint32        fontSize,
    int         buttonHotkey,
    Uint32        buttonHotkeymod,
    int        center
  )
{
  return button_new(
    id,
    name,
    rect,
    bounds,
    buttonText,
    fontSize,
    ButtonText,
    buttonHotkey,
    buttonHotkeymod,
    center,
    justify,
    NULL,
    NULL,
    NULL,
    vec3d(0,0,0),
    0,
    vec3d(0,0,0),
    vec3d(0,0,0)
  );
}

void button_move(Component *component,RectFloat newbounds)
{
  RectFloat r;
  ComponentButton *button = NULL;
  button = component_get_button_data(component);
  if (!button)return;
  component_get_rect_from_bounds(&component->bounds,newbounds,component->rect);
  switch (button->buttonType)
  {
    case ButtonCustom:
      if (button->button[ButtonIdle] != NULL)
      {
        component->bounds.w = button->button[ButtonIdle]->frameWidth;
        component->bounds.h = button->button[ButtonIdle]->frameHeight;
      }
      break;
    case ButtonText:
      r = font_get_bounds(button->buttonText,button->fontSize);
      component->bounds.w = r.w;
      component->bounds.h = r.h;
      break;
    case ButtonStock:
      if (button->button[ButtonIdle] != NULL)
      {
        component->bounds.w = button->button[ButtonIdle]->frameWidth;
        component->bounds.h = button->button[ButtonIdle]->frameHeight;
      }
      break;
    case ButtonRect:
      break;
  }
  if (button->centered)
  {
    component->bounds.y -= component->bounds.h/2;
  }
  if (button->justify == JustifyCenter)
  {
    component->bounds.x -= component->bounds.w/2;
  }
}

Component *button_new(
    Uint32        id,
    Word        name,
    RectFloat   rect,
    RectFloat        bounds,
    char         * buttonText,
    Uint32        fontSize,
    Uint32        buttonType,
    int         buttonHotkey,
    Uint32        buttonHotkeymod,
    int        center,
    int         justify,
    char         * buttonFileUp,
    char         * buttonFileHigh,
    char         * buttonFileDown,
    Vec3D       backgroundColor,
    float       backgroundAlpha,
    Vec3D       highlightColor,
    Vec3D       pressColor
  )
{
  int i;
  ComponentButton *button = NULL;
  Component *component = NULL;
  component = component_new();
  if (!component)return NULL;
  component_make_button(
    component,
    buttonText,
    justify,
    buttonType,
    buttonHotkey,
    buttonHotkeymod,
    buttonFileUp,
    buttonFileHigh,
    buttonFileDown,
    backgroundColor,
    backgroundAlpha,
    highlightColor,
    pressColor,
    fontSize
  );
  rect_copy(&component->rect,rect);
  button = component_get_button_data(component);
  if (button == NULL)
  {
    component_free(&component);
    return NULL;
  }
  button->centered = center;
  component->id = id;
  strncpy(component->name,name,WORDLEN);
  component->type = ButtonComponent;
  component->data_move = button_move;
  for (i = 0; i < ButtonStateMax;i++)
  {
    vec3d_cpy(button->textColor[i],__component_button_color[i]);
  }
  button_move(component,bounds);
  component_button_set_activation(component,
                                      __component_button_release_active,
                                      __component_button_press_active,
                                      __component_button_hold_active);
  return component;
}

void component_button_set_activation(Component* comp,
                                         int release,
                                         int press,
                                         int hold)
{
  ComponentButton *button = NULL;
  button = component_get_button_data(comp);
  if (button == NULL)return;
  button->activeOnRelease = release;
  button->activeOnPress = press;
  button->activeOnHold = hold;
}

Component *component_button_load(RectFloat winrect)
{
  Component *comp = NULL;
  Line buttonType;
  Uint32 id;
  Uint32 fontSize = __component_button_font_size;
  RectFloat rect;
  Line justify = "NONE";
  Line buttonText;
  Line name;
  Line hotkey;
  Line hotmod;
  int hotkeybutton = 0;
  int hotkeymod = 0;
  Line buttonFile;
  Line buttonHighFile;
  Line buttonHitFile;
  int active_on_hold;
  int active_on_press;
  int active_on_release;
  Vec3D backgroundColor;
  float backgroundAlpha;
  Vec3D highlightColor;
  Vec3D pressColor;
  int  center = 0;

  active_on_hold = __component_button_hold_active;
  active_on_press = __component_button_press_active;
  active_on_release = __component_button_release_active;

  if (line_cmp(justify,"CENTER") == 0)center = 1;
  if (line_cmp(buttonType,"STOCK") == 0)
  {
    comp = button_stock_new(
      id,
      name,
      rect,
      winrect,
      buttonText,
      font_justify_from_string(justify),
      hotkeybutton,
      hotkeymod,
      0
    );
  }
  else if (line_cmp(buttonType,"TEXT") == 0)
  {
    comp = button_text_new(
      id,
      name,
      rect,
      winrect,
      buttonText,
      font_justify_from_string(justify),
      fontSize,
      hotkeybutton,
      hotkeymod,
      0
    );
  }
  else if (line_cmp(buttonType,"RECT") == 0)
  {
    comp = button_new(
      id,
      name,
      rect,
      winrect,
      buttonText,
      fontSize,
      ButtonRect,
      hotkeybutton,
      hotkeymod,
      center,
      font_justify_from_string(justify),
      buttonFile,
      buttonHighFile,
      buttonHitFile,
      backgroundColor,
      backgroundAlpha,
      highlightColor,
      pressColor
    );
  }
  else if (line_cmp(buttonType,"CUSTOM") == 0)
  {
    comp = button_new(
      id,
      name,
      rect,
      winrect,
      buttonText,
      fontSize,
      ButtonCustom,
      hotkeybutton,
      hotkeymod,
      center,
      font_justify_from_string(justify),
      buttonFile,
      buttonHighFile,
      buttonHitFile,
      backgroundColor,
      backgroundAlpha,
      highlightColor,
      pressColor
    );
  }
  component_button_set_activation(comp,active_on_release,active_on_press,active_on_hold);
  return comp;
}