#ifndef __COMPONENT_PERCENT_BAR_H__
#define __COMPONENT_PERCENT_BAR_H__

#include "component.h"
#include "types.h"
#include <glist.h>

typedef struct
{
  int     vertical;
  int     ascending;
  float  * max;
  float  * value;
  float    percent;
  Vec3D    statusColor;
  Vec3D    backgroundColor;
  float    statusAlpha;
  float    backgroundAlpha;
}ComponentPercentBar;


#endif