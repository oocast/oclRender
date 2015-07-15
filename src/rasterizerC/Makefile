CC = g++
CFLAGS = -g -Wall
CLFLAG = -std=c++11 -l OpenCL
VAFLAG = -D HAVE_VA_X11

BINS = test_camera
HDRV = va_display.h sysdeps.h
HDRS = color.h ellipse.h geometry.h image.h oclrender.h poly.h scene.h shape.h cl_helper.h pv_helper.h csg.h interface.h
SRCV = va_display.c va_display_x11.c
SRCS = color.cpp ellipse.cpp geometry.cpp image.cpp poly.cpp scene.cpp shape.cpp cl_helper.cpp pv_helper.cpp csg.cpp interface.cpp test_camera.cpp 
DOBJ = libcl.so libva.so.1.3700.0 libva-x11.so.1.3700.0 libX11.so.6.3.0
OBJV = $(SRCV:.c=.o)
#Oaaa = va_display.o
#OBJV = va_display.o va_display_x11.o
OBJS = $(SRCS:.cpp=.o) 

all: test_camera

test_camera: $(OBJV) $(OBJS) $(HDRS) 
	$(CC) $(CFLAGS) -o $@ $(OBJV) $(OBJS) $(DOBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CLFLAG) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(VAFLAG)

clean:
	-rm -r -f $(OBJV) $(OBJS) *.o *~ *core* $(BINS)
