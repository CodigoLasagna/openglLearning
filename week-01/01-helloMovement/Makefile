CXX := gcc
CFLAGS := -g -ansi -pedantic-errors
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lGL -lm 
SRCE := ./src/
TARGET := main

SRCS := $(wildcard  *.c $(SRCE)*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	rm -rf $(TARGET) *.o $(SRCE)*.o
	
.PHONY: all clean
