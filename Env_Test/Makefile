INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

FLAGS = -Wall
CC = g++                                  # change to gcc if using C
CFLAGS = $(FLAGS) $(INCLUDE)
LIBS =  -lglut -lGL -lGLU -lGLEW -lm

All: your_app                             # change your_app.

your_app: your_app.o
    $(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBS) # The initial white space is a tab
