#ifndef __COMPONENT_BUTTON_H__
#define __COMPONENT_BUTTON_H__

#include "component.h"
#include "types.h"
#include "sprite.h"
#include <glist.h>

typedef struct
{
  Uint32		input;			/**<if defined, the input will operate as a hotkey*/
  Uint32		hotkeymod;
  int				justify;
  int				centered;
  Uint32		fontSize;
  float				alpha;
  int				activeOnPress;
  int				activeOnHold;
  int				activeOnRelease;
  Line				buttonText;				/**<text to display over button...should it be a label component?*/
  Uint32		buttonType;						/**<if its an image, or raw text or both*/
  Sprite			*button[ButtonStateMax];		/**<if defined, it will use these over stock button images*/
  Vec3D				textColor[ButtonStateMax];
  Vec3D				backgroundColor;
  float				backgroundAlpha;
  Vec3D				highlightColor;
  Vec3D				pressColor;
}ComponentButton;

void button_configure();

#endif