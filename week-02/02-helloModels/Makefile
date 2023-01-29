CXX := gcc
CFLAGS := -g -std=gnu89 -pedantic-errors
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lGL -lm -lassimp
SRCE := ./source/engine/
TARGET := main

SRCS := $(wildcard  *.c $(SRCE)*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf $(TARGET) *.o $(SRCE)*.o
	
.PHONY: all clean
