CC                = g++
LINKER            = g++
FLAGS             = -Wall -Wextra
CFLAGS            = $(FLAGS) -std=c++0x -c -Iinclude
LFLAGS            = $(FLAGS) -Llib -lGLEW -lglfw -lGL -Wl,-rpath,lib
SRCDIR            = src/
BINDIR            = bin/
OUTPUTS           = $(DODGEROUTPUT)
DODGEROUTPUT      = $(BINDIR)dodger
DODGEROBJECTS     = $(addprefix $(SRCDIR), dodger.o renderer.o)

ifeq ($(DEBUG),1)
  FLAGS += -g
else
  FLAGS += -O3
  CFLAGS += -DNDEBUG
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
	rm -rf {bin,lib}
