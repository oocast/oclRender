CC = g++
CFLAGS = -g -Wall -std=c++11

BINS = test_render
HDRS = color.h ellipse.h geometry.h image.h oclrender.h poly.h scene.h shape.h
SRCS = color.cpp ellipse.cpp geometry.cpp image.cpp poly.cpp scene.cpp shape.cpp test_render.cpp
OBJS = $(SRCS:.cpp=.o)

all: test_render

test_render: $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	-rm -r -f $(OBJS) *.o *~ *core* $(BINS)
