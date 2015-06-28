CC            = g++
LINKER        = g++
FLAGS         = -Wall -Wextra
CFLAGS        = $(FLAGS) -std=c++0x -c -Iinclude -DGLM_FORCE_RADIANS
LFLAGS        = $(FLAGS) -Llib -lpng -lGLEW -lglfw -lseal -Wl,-rpath,lib
SRCDIR        = src/
BINDIR        = bin/
OUTPUTS       = $(DODGEROUTPUT)
DODGEROUTPUT  = $(BINDIR)dodger
DODGEROBJECTS = $(addprefix $(SRCDIR),\
                  audio_renderer.o dodger.o error.o game.o image.o light.o\
                  material.o openal_renderer.o opengl_renderer.o primitive.o\
                  renderer.o scene_node.o shader_program.o sky.o sky_renderer.o\
                  spirit.o spirit_renderer.o terrain.o terrain_renderer.o\
                  texture.o vertex_array.o)

ifeq ($(DEBUG),1)
  FLAGS += -g
else
  FLAGS += -O3
  CFLAGS += -DNDEBUG
endif

ifeq ($(shell uname),Darwin)
	LFLAGS += -framework OpenGL
else
	LFLAGS += -lGL
endif

.PHONY: clean

all: $(OUTPUTS)

$(DODGEROUTPUT): $(DODGEROBJECTS) bin/
	$(LINKER) $(DODGEROBJECTS) -o $@ $(LFLAGS)

bin/:
	mkdir -p bin

-include $(DODGEROBJECTS:.o=.d)

%.o: %.cc
	$(CC) -c -MMD $(CFLAGS) $< -o $@

clean:
	find . -name '*.o' -o -name '*.d' | xargs rm -f
