############################################################################
#
# The Linux-GCC Makefile
#
##############################################################################

#
# Object files.
#

OBJ = $(patsubst %.c,%.o,$(wildcard *.c))

#
# Compiler stuff -- adjust to your system.
#

# Linux
PROJECT = gametest3d
CC      = gcc
#CC	= clang
MGL_LIBS = 
MGL_STATIC_LIBS = 
MGL_LIB_PATH = ../../libs
MGL_LDFLAGS = -L$(MGL_LIB_PATH) $(foreach d, $(MGL_STATIC_LIBS),$(MGL_LIB_PATH)/$d)

MGL_INC_PATHS = ../include
MGL_CFLAGS = $(foreach d, $(MGL_INC_PATHS), -I$d)

GLIB_CFLAGS = `pkg-config --cflags glib-2.0`
GLIB_LDFLAGS = `pkg-config --libs glib-2.0`

SDL_CFLAGS = `sdl2-config --cflags`
SDL_LDFLAGS = `sdl2-config --libs` -lm -lSDL2_image -lpng -ljpeg -lz -lGL -lGLEW -lGLU

LFLAGS = -g  -o ../$(PROJECT)
CFLAGS = -g $(MGL_CFLAGS) -Wall -pedantic -std=gnu99 -fgnu89-inline -Wno-unknown-pragmas -Wno-variadic-macros
# -ffast-math for relase version

DOXYGEN = doxygen

#
# Targets
#

$(PROJECT): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) $(MGL_LDFLAGS) $(SDL_LDFLAGS) $(GLIB_LDFLAGS)
docs:
	$(DOXYGEN) doxygen.cfg

makefile.dep: depend

depend:
	@touch makefile.dep
	@-rm makefile.dep
	@echo Creating dependencies.
	@for i in *.c; do $(CC) $(INC) $(MGL_CFLAGS) -MM $$i; done > makefile.dep
	@echo Done.

clean:
	rm *.o ../$(PROJECT)

count:
	wc *.c *.h makefile

#
# Dependencies.
#

include makefile.dep

#
# The default rule.
#

.c.o:
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(GLIB_CFLAGS) -c $<

