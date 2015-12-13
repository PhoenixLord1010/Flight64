#include "component_percent_bar.h"
#include "simple_logger.h"
#include "draw.h"

/*local function prototypes*/
void component_percent_bar_new(Component *component);
void component_percent_bar_draw(Component *component);
int component_percent_bar_update(Component *component);
void component_percent_bar_free(Component *component);


/*function definitions*/

ComponentPercentBar *component_get_percent_bar_data(Component *component)
{
  if ((!component)||
    (!component->componentData)||
    (component->type != PercentBarComponent))
  {
    return NULL;
  }
  return (ComponentPercentBar*)component->componentData;
}


void component_make_percent_bar(
    Component  * component,
    int         vertical,
    int         ascending,
    float        percent,
    Vec3D        statusColor,
    Vec3D        backgroundColor,
    float        statusAlpha,
    float        backgroundAlpha
  )
{
  ComponentPercentBar * bar = NULL;
  if (!component)return;
  component_percent_bar_new(component);
  bar = component_get_percent_bar_data(component);
  if (bar == NULL)
  {
    return;
  }
  bar->statusAlpha = statusAlpha;
  bar->backgroundAlpha = backgroundAlpha;
  bar->vertical = vertical;
  bar->percent = percent;
  bar->ascending = ascending;
  vec3d_cpy(bar->statusColor,statusColor);
  vec3d_cpy(bar->backgroundColor,backgroundColor);
  component->data_free = component_percent_bar_free;
  component->data_draw = component_percent_bar_draw;
  component->data_update = component_percent_bar_update;
}

Component *percent_bar_new(
    Uint32       id,
    Word       name,
    RectFloat  rect,
    RectFloat      bounds,
    int       vertical,
    int       ascending,
    float      percent,
    Vec3D      statusColor,
    Vec3D      backgroundColor,
    float      statusAlpha,
    float      backgroundAlpha
    )
{
  Component *component = NULL;
  ComponentPercentBar *bar = NULL;
  component = component_new();
  if (!component)return NULL;
  rect_copy(&component->rect,rect);
  component_get_rect_from_bounds(&component->bounds,bounds, rect);

  component_make_percent_bar(
    component,
    vertical,
    ascending,
    percent,
    statusColor,
    backgroundColor,
    statusAlpha,
    backgroundAlpha
  );

  bar = component_get_percent_bar_data(component);
  if (!bar)
  {
    component_free(&component);
    return NULL;
  }
  component->id = id;
  line_cpy(component->name,name);

  return component;
}

void component_percent_bar_free(Component *component)
{
  ComponentPercentBar * bar = component_get_percent_bar_data(component);
  if (bar == NULL)return;
  free(bar);
  component->componentData = NULL;
}

int component_percent_bar_update(Component *component)
{
  ComponentPercentBar *bar = component_get_percent_bar_data(component);
  if (bar == NULL)return 0;
  if ((bar->max != NULL) && (bar->value != NULL) && (*bar->max != 0))
  {
    bar->percent = *bar->value / *bar->max;
  }
  if (bar->percent > 1)bar->percent = 1;
  if (bar->percent < 0)bar->percent = 0;
  return 0;
}

void component_percent_bar_draw(Component *component)
{
  RectFloat r;
  ComponentPercentBar *bar = component_get_percent_bar_data(component);
  if (bar == NULL)return;
  rect_copy(&r,component->bounds);
  draw_solid_rect(component->bounds,bar->backgroundColor,bar->backgroundAlpha);
  if (bar->vertical)
  {
    r.h = component->bounds.h * bar->percent;
    if (bar->ascending)
    {
      r.y = component->bounds.y + (component->bounds.h - r.h);
    }
  }
  else
  {
    r.w = component->bounds.w * bar->percent;
    if (bar->ascending)
    {
      r.x = component->bounds.x + (component->bounds.w - r.w);
    }
  }
  draw_solid_rect(r,bar->statusColor,bar->statusAlpha);
}

void component_percent_bar_new(Component *component)
{
  if (component->componentData != NULL)
  {
    slog("component_percent_bar:tried to make a percent bar out of an existing component\n");
    return;
  }
  component->componentData = malloc(sizeof(ComponentPercentBar));
  if (component->componentData == NULL)
  {
    slog("component_percent_bar: failed to allocate data for new percent bar\n");
    return;
  }
  memset(component->componentData,0,sizeof(ComponentPercentBar));
  component->type = PercentBarComponent;
}

void component_percent_bar_assign_values(Component *component,float *max,float *value)
{
  ComponentPercentBar *bar = component_get_percent_bar_data(component);
  if (bar == NULL)return;
  bar->max = max;
  bar->value = value;
}

void component_percent_bar_set(Component *component,float percent)
{
  ComponentPercentBar *bar = component_get_percent_bar_data(component);
  if (bar == NULL)return;
  bar->percent = percent;
}
