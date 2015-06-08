CC = g++
CFLAGS = -g -Wall -std=c++11 -l OpenCL

BINS = test_render
HDRS = color.h ellipse.h geometry.h image.h oclrender.h poly.h scene.h shape.h cl_helper.h
SRCS = color.cpp ellipse.cpp geometry.cpp image.cpp poly.cpp scene.cpp shape.cpp cl_helper.cpp test_render.cpp
ABCC = libcl.so
OBJS = $(SRCS:.cpp=.o) 

all: test_render

test_render: $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(ABCC)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	-rm -r -f $(OBJS) *.o *~ *core* $(BINS)
