#include "input.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Input * __input_list = NULL;
Uint32    __input_count = 0;
int    __input_initialized = 0;
Uint32    __input_held_threshold = 0;
Uint32    __input_double_tap_threshold = 0;
Uint32    _this_frame_keyboard_mod = 0;
Uint32    _last_frame_keyboard_mod = 0;
Uint8    * _this_frame_keyboard = NULL;
Uint8    * _last_frame_keyboard = NULL;
int     __input_keyboard_numkeys = 0;

/* local funtions */
void input_keyboard_init();
void input_keyboard_close();
void updateKeyboard();
void input_close();

void input_init()
{
  slog("input: initializing\n");
  //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
  input_keyboard_init();
  atexit(input_close);
  __input_initialized = 1;
  slog("input: initialized\n");
}

void input_close()
{
  slog("input: closing\n");
  input_keyboard_close();
  if (__input_list != NULL)
  {
    free(__input_list);
    __input_list = NULL;
  }
  __input_initialized = 0;
  slog("input: closed\n");
}

int input_is_initialized()
{
  return __input_initialized;
}

void input_update()
{
  int i;
  SDL_Keymod mod;
  SDL_Event event;
  while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT));
  updateKeyboard();
  SDL_PumpEvents();
  mod = SDL_GetModState();
  for(i = 0;i < __input_count;i++)
  {
    __input_list[i].oldvalue = __input_list[i].value;
    if(strcmp(__input_list[i].type,"key") == 0)
    {
      __input_list[i].value = _this_frame_keyboard[__input_list[i].id];
    }
    else if(strcmp(__input_list[i].type,"mod") == 0)
    {
      if(mod & __input_list[i].id)
      {
        __input_list[i].value = 1;
      }
      else __input_list[i].value = 0;
    }
    /*TODO:mouse section*/
    /*TODO:joystick section*/
    if(__input_list[i].value == 1)
    {
      __input_list[i].lastPress = __input_list[i].timePress;
      __input_list[i].timePress = SDL_GetTicks();
    }
  }
}

int input_parse(char *type, char *input)
{
  int i;
  if((type == NULL)||(input == NULL))return -1;
  if(strcmp(type,"key")==0)
  {
    if(input[0] == 'F')
    {
      sscanf(input,"F%i",&i);
      i--;
      return SDLK_F1 + i;
    }
    if((input[0] >= 'a')&&(input[0] <= 'z'))
    {
      return input[0]-'a' + SDLK_a;
    }
    if((input[0] >= '0')&&(input[0] <= '9'))
    {
      return input[0]-'0' + SDLK_0;
    }
    if(input[0] == '/')return SDLK_SLASH;
    if(input[0] == '\\')return SDLK_BACKSLASH;
    if(input[0] == '[')return SDLK_LEFTBRACKET;
    if(input[0] == ']')return SDLK_RIGHTBRACKET;
    if(input[0] == '-')return SDLK_MINUS;
    if(input[0] == '=')return SDLK_EQUALS;
    if(input[0] == '`')return SDLK_CARET;
    if(input[0] == ',')return SDLK_COMMA;
    if(input[0] == ';')return SDLK_SEMICOLON;
    if(input[0] == ':')return SDLK_COLON;
    if(input[0] == '.')return SDLK_PERIOD;
    if(strcmp(input,"TAB")==0)return SDLK_TAB;
    if(strcmp(input,"ESC")==0)return SDLK_ESCAPE;
    if(strcmp(input,"ENTER")==0)return SDLK_RETURN;
    if(strcmp(input,"SPACE")==0)return SDLK_SPACE;
    if(strcmp(input,"UPARROW")==0)return SDLK_UP;
    if(strcmp(input,"DOWNARROW")==0)return SDLK_DOWN;
    if(strcmp(input,"LEFTARROW")==0)return SDLK_LEFT;
    if(strcmp(input,"RIGHTARROW")==0)return SDLK_RIGHT;
    if(strcmp(input,"BACKSPACE")==0)return SDLK_BACKSPACE;
  }
  if(strcmp(type,"mod")==0)
  {
    if(strcmp(input,"LShift")==0)
    {
      return KMOD_LSHIFT;
    }
    if(strcmp(input,"RShift")==0)
    {
      return KMOD_RSHIFT;
    }
    if(strcmp(input,"LCtrl")==0)
    {
      return KMOD_LCTRL;
    }
    if(strcmp(input,"RCtrl")==0)
    {
      return KMOD_RCTRL;
    }
    if(strcmp(input,"LAlt")==0)
    {
      return KMOD_LALT;
    }
    if(strcmp(input,"RAlt")==0)
    {
      return KMOD_RALT;
    }
  }
  if(strcmp(type,"button")==0)
  {
    sscanf(input,"%i",&i);
    return i;
  }
  if(strcmp(type,"+axis")==0)
  {
    sscanf(input,"%i",&i);
    return i;
  }
  if(strcmp(type,"-axis")==0)
  {
    sscanf(input,"%i",&i);
    return i;
  }
  return -1;
}

void input_keyboard_close()
{
  if (_last_frame_keyboard != NULL)free(_last_frame_keyboard);
}

void input_keyboard_init()
{
  _this_frame_keyboard = SDL_GetKeyboardState(&__input_keyboard_numkeys);
  _last_frame_keyboard = (Uint8 *)malloc(sizeof(Uint8)*__input_keyboard_numkeys);
  memcpy(
      _last_frame_keyboard,
      _this_frame_keyboard,
      sizeof(Uint8)*__input_keyboard_numkeys
      );
      if(_last_frame_keyboard == NULL)
  {
    slog("input: unable to initialize keyboard structure!\n");
  }
}

void updateKeyboard()
{
  int i;
  if((!__input_initialized) ||
     (_last_frame_keyboard == NULL) ||
     (_this_frame_keyboard == NULL))return;
  for (i = 0; i < __input_keyboard_numkeys;i++)
  {
    _last_frame_keyboard[i] = _this_frame_keyboard[i];
  }
  _last_frame_keyboard_mod = _this_frame_keyboard_mod;
  _this_frame_keyboard_mod = SDL_GetModState();
}

void input_clear_keyboard()
{
  if((!__input_initialized) ||
     (_last_frame_keyboard == NULL))return;
  memset(_last_frame_keyboard,0,sizeof(Uint8)*__input_keyboard_numkeys);
  _last_frame_keyboard_mod = 0;
  _this_frame_keyboard_mod = 0;
}

int input_is_key_pressed(int key)
{
  if((!__input_initialized) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if ((key < 0) || (key >= __input_keyboard_numkeys))
  {
    slog("input: key out of range\n");
    return 0;
  }
  if ((_last_frame_keyboard[key] == 0) &&
      (_this_frame_keyboard[key] == 1))
    return 1;
  return 0;
}

int input_is_key_released(int key)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.\n");
    return 0;
  }
  if ((key < 0) || (key >= __input_keyboard_numkeys))
  {
    slog("input: key out of range\n");
    return 0;
  }
  if ((_last_frame_keyboard[key] == 1) &&
    (_this_frame_keyboard[key] == 0))
    return 1;
  return 0;
}

int input_is_key_held(int key)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if ((key < 0) || (key >= __input_keyboard_numkeys))
  {
    slog("input: key out of range\n");
    return 0;
  }
  if ((_last_frame_keyboard[key] == 1) &&
    (_this_frame_keyboard[key] == 1))
  {
    return 1;
  }
  return 0;
}

int input_is_mod_held(Uint32 mod)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if ((_this_frame_keyboard_mod & mod) &&
    (_last_frame_keyboard_mod & mod))
  {
    return 1;
  }
  return 0;
}

int input_is_mod_down(Uint32 mod)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if (_this_frame_keyboard_mod & mod)
  {
    return 1;
  }
  return 0;
}

int input_is_mod_pressed(Uint32 mod)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if ((_this_frame_keyboard_mod & mod) &&
    (!(_last_frame_keyboard_mod & mod)))
  {
    return 1;
  }
  return 0;
}

int input_is_mod_released(Uint32 mod)
{
  if((__input_initialized == 0) ||
    (_last_frame_keyboard == NULL) ||
    (_this_frame_keyboard == NULL))
  {
    slog("input: uninitialized.");
    return 0;
  }
  if ((_last_frame_keyboard_mod & mod) &&
    (!(_this_frame_keyboard_mod & mod)))
  {
    return 1;
  }
  return 0;
}

int input_quit_check()
{
  SDL_Event event;
  while(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT))
  {
    if(event.type == SDL_QUIT)
    {
      return 1;
    }
  }
  return 0;
}

Input *input_get_by_name(char *input)
{
  int i;
  for (i = 0;i < __input_count;i++)
  {
    if (strncmp(input,__input_list[i].name,LINELEN) == 0)
    {
      return &__input_list[i];
    }
  }
  slog("input:failed to find input %s\n",input);
  return NULL;
}

float input_is_input_down(char *inputName)
{
  Input *input = NULL;
  input = input_get_by_name(inputName);
  if (input == NULL)return 0;
  if (input->value != 0)return input->value;
  return 0;
}

int input_is_input_pressed(char *inputName)
{
  Input *input = NULL;
  input = input_get_by_name(inputName);
  if (input == NULL)return 0;
  if ((input->value == 0) && (input->oldvalue != 0))return 1;
  return 0;
}

int input_is_input_released(char *inputName)
{
  Input *input = NULL;
  input = input_get_by_name(inputName);
  if (input == NULL)return 0;
  if ((input->value != 0) && (input->oldvalue == 0))return 1;
  return 0;
}

float input_is_input_held(char *inputName)
{
  Input *input = NULL;
  input = input_get_by_name(inputName);
  if (input == NULL)return 0;
  if ((input->value != 0) && (input->oldvalue != 0))return input->value;
  return 0;
}
