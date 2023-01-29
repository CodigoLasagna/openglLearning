CXX := gcc
CFLAGS := -g -ansi -pedantic-errors
LDFLAGS= -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lGL
TARGET := main

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	rm -rf $(TARGET) *.o
cleanO:
	rm -rf main.o
	
.PHONY: all clean
