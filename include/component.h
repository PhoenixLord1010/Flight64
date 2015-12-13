#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "types.h"
#include <glist.h>
#include <SDL.h>

enum ComponentTypes {
  NullComponent,		/**<this type is set by default*/
  LabelComponent,		/**<For text string labels*/
  ButtonComponent,		/**<for buttons*/
  EntryComponent,		/**<for text input entries*/
  SliderComponent,		/**<for slider controls and scroll bars*/
  ImageComponent,		/**<for static images and icons*/
  ListComponent,		/**<for lists of stuff*/
  PercentBarComponent,	/**<for a percent bar*/
  CheckComponent		/**<for a check box button*/
};

enum CheckStates {
  CheckBadState  = -1,	/**<state for a check button that is in a bad state*/
  CheckChecked   = 0,	/**<state for a check button that is CHECKED*/
  CheckUnchecked = 1	/**<state for a check button that is UNCHECKED*/
};

enum SliderTypes {
  SliderCommon,	/**<stock slider image, drawn bar*/
  SliderDraw,	/**<drawn slider, drawn bar*/
  SliderStock,	/**<stock slider, stock bar*/
  SliderCustom	/**<custom images for slider and bar*/
};

enum SliderStates {
  SliderIdle,		/**<Not touched*/
  SliderHigh,		/**<Mouse is over it, or the slider has focus*/
  SliderHeld		/**<Mouse is interacting with it*/
};

enum ButtonTypes {
  ButtonStock,		/**<use the common stock button graphics*/
  ButtonText,		/**<doesnt use button graphics, but highlights text*/
  ButtonRect,		/**<does not use a sprite, is drawn functionally*/
  ButtonHidden,		/**<does not draw the button, but it can still get hotkey input*/
  ButtonCustom		/**<same as stock, but uses custom graphics*/
};

enum ButtonStates {
  ButtonIdle,		/**<no activity*/
  ButtonHighlight,	/**<mouse is over the button or the button has focus*/
  ButtonPressed,	/**<mouse or hotkey is pressed on the button*/
  ButtonSleep		/**<button is marked inactive*/
};
#define ButtonStateMax 4

enum ListTypes {
  ListText,		/**<items are text and drawn from top to bottom*/
  ListLines,	/**<items are drawn from top to bottom fit within bounding
                         rect*/
  ListBlock,	/**<items are drawn left to right, then top to bottom fit
                         within bounding rect*/
  ListDock		/**<items are drawn left to right.  fit within bounding rect*/
};

/**
 * @note the relative float rect for all components follows this convention:
 * x or y should be from 0-1, 0 being at the far left/top, 1 meaning far right/bottom.
 * if x or y is negative it is taken in relative postiton from the right/bottom.
 * w and h are widths relative to the size of the bounding canvas.  So 0.5 means the
 * width should take up half of the space.
 * 0 or negative w or h means SNAP to edge.  Just like a 0 would mean for x or y.
 * @example rect {0.25,-0.25,-1,0.2} means that the top left corner will be at
 * (0.25,0.75) and the bottom right corner will be at (1,0.95)
*/

/**
  @brief this structure serves as a header for all components
 */
typedef struct Component_S
{
  Uint32		id;
  Line			name;
  RectFloat		rect;			/**<relative position to draw*/
  RectFloat		bounds;			/**<calculated position to draw*/
  RectFloat		canvas;			/**<area allowed to draw in*/
  int			canHasFocus;
  int			hasFocus;
  int			state;
  int			oldState;
  Uint32		type;
  int			hidden;			/**<if 1, it is not drawn*/
  void			*componentData;
  void			(*data_free)(struct Component_S *component);
  void			(*data_draw)(struct Component_S *component);
  void			(*data_move)(struct Component_S *component,RectFloat bounds);
  int			(*data_update)(struct Component_S *component);
  int			(*data_get_state)(struct Component_S *component);
  int			(*data_changed)(struct Component_S *component);
}Component;

/*default*/
void component_config();

/*Generic Component functions*/
int component_changed(Component *component);
Component *component_new();
void component_free(Component **component);
int component_update(Component *component);
void component_set_focus(Component *component,int focus);
int component_has_changed(Component *component);
int component_get_state(Component *component);
void component_draw(Component *component);
void component_move(Component *component,RectFloat newbounds);

/*Slider functions*/
Component *slider_new(
    Uint32			id,
    Line			name,
    RectFloat		rect,
    RectFloat		bounds,
    int				vertical,
    Line			slider,
    Line			sliderHigh,
    Line			bar,
    Line			cap1,
    Line			cap2,
    Vec3D			barColor,
    Vec3D			sliderColor,
    float			startPosition,
    Uint32			sliderType
);
Component *percent_bar_create_from_config(RectFloat parentRect);
Component *slider_common_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat        bounds,
    int				vertical,
    Vec3D			barColor,
    float			startPosition
);
Component *slider_create_from_config(RectFloat parentRect);
float slider_get_position(Component *comp);
void slider_set_position(Component *comp, float newPos);

/*Label functions*/
Component *label_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat        bounds,
    int				canHasFocus,
    char			*text,
    Uint32    justify,
    int				wordWrap,
    int				fontSize,
    char			*fontName,
    Vec3D			color,
    float			alpha
  );
Component *label_new_default(
	Uint32			id,
	char			name,
	RectFloat		rect,
	RectFloat        bounds,
	char			*text,
	int				wordWrap
);
void label_set_text(Component *comp,char *text);
void label_get_text(Component *comp,Line text);
void label_get_text_size(Component *comp,Uint32 *w,Uint32 *h);
Component *component_create_label_from_config(RectFloat parentRect);

/*Button functions*/
Component *button_new(
    Uint32			id,
    Line			name,
    RectFloat		rect,
    RectFloat        bounds,
    char			*buttonText,
    Uint32    fontSize,
    Uint32    buttonType,
    int				buttonHotkey,
    Uint32    buttonHotkeymod,
    int				center,
    int				justify,
    char			*buttonFileUp,
    char			*buttonFileHigh,
    char			*buttonFileDown,
    Vec3D			backgroundColor,
    float			backgroundAlpha,
    Vec3D			highlightColor,
    Vec3D			pressColor
  );
Component *button_stock_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat        bounds,
    char			*buttonText,
    int				justify,
    int				buttonHotkey,
    Uint32    buttonHotkeymod,
    int				center
  );
Component *button_text_new(
	Uint32	id,
    Line			name,
	RectFloat		rect,
    RectFloat        bounds,
    char			*buttonText,
    int				justify,
    Uint32    fontSize,
    int				buttonHotkey,
    Uint32    buttonHotkeymod,
    int				center
  );
void button_set_text(Component *button,Line newText);
void button_get_text(Line outText,Component *button);

void button_set_inactive(Component *button);
void button_set_active(Component *button);

Uint32 button_get_state(Component *button);
void button_get_size(Component *button,Uint32 *w, Uint32 *h);
void button_get_stock_size(Uint32 *w, Uint32 *h);

/*Entry functions*/
Component *entry_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat		bounds,
    char			*output,
    int				outputLimit,
    Uint32    justify,
    int				wordWrap,
    Uint32    fontSize,
	Line			fontName,
    int				number,
    Vec3D			color,
    float			alpha,
    Vec3D			bgcolor
);
void entry_assign_callback(Component *component,void *data, void (*callback)(void *data,Line output));
void entry_assign_output(Component *component);
Component *entry_create_from_config(RectFloat parentRect);
Component *line_entry_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat		bounds,
    Line			output
);
void entry_get_line(Component *component,Line output);

/*List functions*/
Component *list_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat		bounds,
    Uint32    listType,
    Vec2D			itemDimensions,
    Vec2D			itemPadding,
    int				showVSlider,
    int				showHSlider,
    Uint32    fontSize
);
Component *list_create_from_config(RectFloat winRect);
GList *list_get_updates(Component *listComp);
Component *component_list_get_item_by_name(Component *list,Line name);
void list_add_text_item(Component *list,Uint32 itemId,Line text);
void component_list_clear(Component *component);
void component_list_deselect_all(Component *component);
int component_list_get_count(Uint32 *count,Component *component);
void component_list_select_item_n(Component *component,Uint32 n);
int component_list_get_selected_id(Uint32 *id,Component *list);
int component_list_get_selected_item(Component **itemOut,Component *list);
int component_list_delete_selected_item(Component *list);
int component_list_get_nth_item(Component **itemOut,Component *list,Uint32 n);

void component_get_rect_from_bounds(RectFloat *rect,RectFloat canvas, RectFloat bounds);

/*Percent Bar functions*/
Component *percent_bar_new(
    Uint32	id,
    Line			name,
    RectFloat		rect,
    RectFloat		bounds,
    int				vertical,
    int				ascending,
    float			percent,
    Vec3D			statusColor,
    Vec3D			backgroundColor,
    float			statusAlpha,
    float			backgroundAlpha
);
void component_percent_bar_set(Component *component,float percent);
void component_percent_bar_assign_values(Component *component,float *max,float *value);

/*Check Box functions*/
Component *check_create_from_config(RectFloat parentRect);
void component_check_set_state(Component *component,int state);
int component_check_get_state(Component *component);

#endif