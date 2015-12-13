#include "graphics3d.h"
#include "simple_logger.h"
#include "shader.h"
#include <glu.h>
#include <glib.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  void (*callback)(GraphicsView info);
}GraphicsCallback;

typedef struct
{
  int				trackFPS;     /**if 1, outputs frame data to terminal*/
  int				fullScreen;   /**1 for full screen, 0 for window mode*/
  double			scaleFactor;  /**all rendering will be scaled by this
                                    factor.  default to 1*/
  Uint32			frameDelay;   /**minimum number of milliseconds to wait for
                                    drawing the next frame*/
  GraphicsView		graphicsView; /**graphics view data*/
}GraphicsConfig;

/* internal globals. DO NOT EXTERN OUTSIDE OF THIS FILE!*/
int					__graphics_initialized = 0;
GraphicsConfig		__GraphicsConfig;
SDL_GLContext		__gl_context;
SDL_Window			*__window = NULL;
GLuint				__shader_program;
GList				*__resize_callbacks = NULL;
float				__graphics_FPS = 0;   /*calculated Frames per second*/
Uint32				__graphics_NOW = 0;


/*local function prototypes*/
void graphics_setup_resize_callbacks();
void graphics_exit(void);
void resize_graphics(Uint32 w, Uint32 h, int fs);
int  register_resize_callback(void (*callback)(GraphicsView view));
void save_screen_shot(const char *file);
void setup_default_light();


/*function definitions*/
void graphics_config()
{
  __GraphicsConfig.graphicsView.depth = 32;
  __GraphicsConfig.trackFPS = 1;
  __GraphicsConfig.fullScreen = 0; 
  __GraphicsConfig.graphicsView.openGLOn = 1;
  __GraphicsConfig.scaleFactor = 1;
  __GraphicsConfig.frameDelay = 16;
  __GraphicsConfig.graphicsView.xRes = 1024;
  __GraphicsConfig.graphicsView.yRes = 768;
  __GraphicsConfig.graphicsView.mipmapping = 0;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  __GraphicsConfig.graphicsView.rMask = 0xff000000;
  __GraphicsConfig.graphicsView.gMask = 0x00ff0000;
  __GraphicsConfig.graphicsView.bMask = 0x0000ff00;
  __GraphicsConfig.graphicsView.aMask = 0x000000ff;
  #else
  __GraphicsConfig.graphicsView.rMask = 0x000000ff;
  __GraphicsConfig.graphicsView.gMask = 0x0000ff00;
  __GraphicsConfig.graphicsView.bMask = 0x00ff0000;
  __GraphicsConfig.graphicsView.aMask = 0xff000000;
  #endif
  
  return;
}

void setup_default_fog()
{

float fogcolor[] = {0.75,0.50,4,1};
glEnable(GL_FOG);
glFogfv(GL_FOG_COLOR,fogcolor);
glFogi(GL_FOG_MODE, GL_EXP);
glFogf(GL_FOG_DENSITY, 0.001f);
glFogf(GL_FOG_START, 1.0f);
glFogf(GL_FOG_END, 200.0f);
}


void graphics_init()
{
	const unsigned char *version;
	GLenum glew_status;
	int x,y;

	x = __GraphicsConfig.graphicsView.xRes;
	y = __GraphicsConfig.graphicsView.yRes;
  
	if (SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		exit(1);
	}
	atexit(SDL_Quit);
  
	__window = SDL_CreateWindow("Flight64",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		__GraphicsConfig.graphicsView.xRes,
		__GraphicsConfig.graphicsView.yRes,
		SDL_WINDOW_OPENGL);
	if ( __window == NULL )
	{
		exit(1);
	}

	__gl_context = SDL_GL_CreateContext(__window);
	if (__gl_context == NULL)
    {
		slog("There was an error creating the OpenGL context!\n");
        exit(1);
    }

	version = glGetString(GL_VERSION);
	if(version == NULL)
	{
		slog("There was an error creating the OpenGL context!\n");
		exit(1);
	}

	SDL_GL_MakeCurrent(__window, __gl_context);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    glew_status = glewInit();
    if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        exit(1);
    }
    
    __shader_program = BuildShaderProgram("shaders/vs1.glsl", "shaders/fs1.glsl");
    if (__shader_program == -1)
    {
        exit(1);
    }

	/*sets the viewing port to the size of the screen*/
	glViewport(0,0,__GraphicsConfig.graphicsView.xRes, __GraphicsConfig.graphicsView.yRes);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/*view angle, aspect ratio, near clip distance, far clip distance*/
	gluPerspective( 40, (double)x / (double)y, .01, 2000.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	/*Clear color for the buffer*/
	glClearColor(0,0,0,0);
	glClear(1);
	
	/*Enables drawing pixels according to thier depth*/
	glDepthFunc(GL_LESS /*GL_LEQUAL*/);
	glEnable(GL_DEPTH_TEST);
	
	/*Enables alpha testing*/
	glAlphaFunc(GL_GREATER,0);
	glEnable(GL_ALPHA_TEST);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
	/*now record the matrix view data*/
	glGetIntegerv(GL_VIEWPORT,__GraphicsConfig.graphicsView.viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, __GraphicsConfig.graphicsView.modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, __GraphicsConfig.graphicsView.projection);


	setup_default_light();
	__graphics_initialized = 1;
	atexit(graphics_exit);
	setup_default_fog();
}

int graphics_initialized()
{
	return __graphics_initialized;
}

GLuint get_shader_program()
{
	return __shader_program;
}

void frame_delay(Uint32 delay)
{
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}

void graphics_frame_begin()
{
	glClearColor(0,0,0,0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glPushMatrix();
}

void graphics_frame_end()
{
  Uint32 Then;
  Uint32 delay;

  glPopMatrix();
  SDL_GL_SwapWindow(__window); 
  frame_delay(__GraphicsConfig.frameDelay);
  Then = __graphics_NOW;
  __graphics_NOW = SDL_GetTicks();
  if(__GraphicsConfig.trackFPS)
  {
    delay = __graphics_NOW - Then;
    if (delay != 0)
    {
        __graphics_FPS = (__graphics_FPS + (1000.0 / delay))*0.5;
    }
  }
}

Uint32 graphics_get_now()
{
  return __graphics_NOW;
}

Uint32 graphics_get_pixel(SDL_Surface *surface, Uint32 x, Uint32 y)
{
  /* Here p is the address to the pixel we want to retrieve*/
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
  if((x < 0)||(x >= surface->w)||(y < 0)||(y >= surface->h))return -1;
  switch(surface->format->BytesPerPixel)
  {
    case 1:
      return *p;

    case 2:
      return *(Uint16 *)p;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
      return *(Uint32 *)p;

  }
  return 0;       /*shouldn't happen, but avoids warnings*/
}

void graphics_put_pixel(SDL_Surface *surface, Uint32 x, Uint32 y, Uint32 pixel)
{
  SDL_Rect point = {0,0,1,1};
  point.x = x;
  point.y = y;
  SDL_FillRect(surface,&point,pixel);
}

/**
 * @brief destructor for the graphics component.  called at exit.
 * */
void graphics_exit(void)
{
  __graphics_initialized = 0;
  g_list_foreach(__resize_callbacks, (GFunc)free, NULL);
  g_list_free(__resize_callbacks);
}

void graphics_register_resize(void (*callback)(GraphicsView info))
{
  GraphicsCallback *newCallback = NULL;
  if ((callback == NULL) ||
    (__graphics_initialized == 0))
  {
    return;
  }
  newCallback = (GraphicsCallback *)malloc(sizeof(GraphicsCallback));
    newCallback->callback = callback;
  __resize_callbacks = g_list_append(__resize_callbacks,(void *)newCallback);
  callback(__GraphicsConfig.graphicsView);
}


void graphics_get_view(GraphicsView *view)
{
  if (!graphics_initialized())
  {
    return;
  }
  if (view == NULL)
  {
    return;
  }
  /*TODO: make this thread safe*/
  memcpy(view,&__GraphicsConfig.graphicsView,sizeof(GraphicsView));
}

float graphics_get_FPS()
{
  return __graphics_FPS;
}

void graphics_get_size(Uint32 * w, Uint32 * h)
{
  if (w != NULL)*w = __GraphicsConfig.graphicsView.xRes;
  if (h != NULL)*h = __GraphicsConfig.graphicsView.yRes;
}

void setup_default_light()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { -10.0, -10.0, 10.0, 0.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat light1_ambient[] = { 1.2, 1.2, 1.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { -20.0, 2.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
    
    GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    
    //glShadeModel (GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.9);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.7);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
}

void opengl_get_screen_coordinate(
  GLdouble glx,
  GLdouble gly,
  GLdouble glz,
  const GLdouble * model,
  const GLdouble * proj,
  const GLint    * view,
  GLint    *x,
  GLint    *y
)
{
  GLdouble tx,ty,tz;
  gluProject(glx,gly,glz,model,proj,view,&tx,&ty,&tz);
  if (x)*x = (GLint)x;
  if (y)*y = (GLint)y;
}

void opengl_get_gl_coordinate(
  GLint    x,
  GLint    y,
  GLdouble z,
  const GLdouble * model,
  const GLdouble * proj,
  const GLint    * view,
  GLfloat * glx,
  GLfloat * gly,
  GLfloat * glz
  )
{  
  GLdouble scrx, scry, gx, gy, gz;
  scrx = (GLdouble)x;
  scry = view[3] - (GLdouble)y;
  
  gluUnProject(scrx,scry,z,model,proj,view,&gx,&gy,&gz);

  *glx = gx;
  *gly = gy;
  *glz = gz;
}
