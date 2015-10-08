#include "graphics3d.h"
#include "simple_logger.h"
#include "shader.h"
#include <glu.h>


static SDL_GLContext __graphics3d_gl_context;
static SDL_Window  * __graphics3d_window = NULL;
static GLuint        __graphics3d_shader_program;
static Uint32        __graphics3d_frame_delay = 33;

void graphics3d_close();

GLuint graphics3d_get_shader_program()
{
    return __graphics3d_shader_program;
}

void graphics3d_setup_default_light();

int graphics3d_init(int sw,int sh,int fullscreen,const char *project,Uint32 frameDelay)
{
    const unsigned char *version;
    GLenum glew_status;
        
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        slog("failed to initialize SDL!");
        return -1;
    }
    atexit(SDL_Quit);
    __graphics3d_frame_delay = frameDelay;
    
    __graphics3d_window = SDL_CreateWindow(project?project:"gametest3d",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              sw, sh,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    
    __graphics3d_gl_context = SDL_GL_CreateContext(__graphics3d_window);
    if (__graphics3d_gl_context == NULL)
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    version = glGetString(GL_VERSION);
    if (version == NULL) 
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    SDL_GL_MakeCurrent(__graphics3d_window, __graphics3d_gl_context);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    //MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    glew_status = glewInit();
    if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        return -1;
    }
    
    
    __graphics3d_shader_program = BuildShaderProgram("shaders/vs1.glsl", "shaders/fs1.glsl");
    if (__graphics3d_shader_program == -1)
    {
        return -1;
    }
    
    
    glViewport(0,0,sw, sh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*view angle, aspect ratio, near clip distance, far clip distance*/
    /*TODO: put near/far clip in graphics view config*/
    gluPerspective( 40, (float)sw / (float)sh, .01, 2000.0f);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    glClear( 1 );
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /*Enables alpha testing*/
/*    glAlphaFunc(GL_GREATER,0);
    glEnable(GL_ALPHA_TEST);*/
    
    graphics3d_setup_default_light();
    atexit(graphics3d_close);
    return 0;
}

void graphics3d_close()
{
    SDL_GL_DeleteContext(__graphics3d_gl_context);
}

void graphics3d_frame_begin()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glPushMatrix();
}

void graphics3d_next_frame()
{
    static Uint32 then = 0;
    Uint32 now;
    glPopMatrix();
    SDL_GL_SwapWindow(__graphics3d_window);
    now = SDL_GetTicks();
    if ((now - then) < __graphics3d_frame_delay)
    {
        SDL_Delay(__graphics3d_frame_delay - (now - then));        
    }
    //slog("ticks passed this frame: %i",(now - then));
    then = now;
}

void graphics3d_setup_default_light()
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


/*eol@eof*/
