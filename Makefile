TARGET	:= pacman

SOURCES := src
IDIR 	:= include

ODIR	:= build

LIBS	= -lglut -lGLU -lGL -lm -ljpeg

_DEPS = game-object.h \
		cell.h \
		map.h \
		util.h \
		map-generator.h \
		game-manager.h \
		player.h \
		phantom.h \
		observer.h \
		texture.h \
		texture-manager.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o \
		cell.o \
		map.o \
		util.o \
		map-generator.o \
		game-manager.o \
		player.o \
		phantom.o \
		observer.o \
		texture.o \
		texture-manager.o
		
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: clean all

all: directories $(TARGET)

$(ODIR)/%.o: $(SOURCES)/%.cpp $(DEPS)
	g++ -c -o $@ $< -Iinclude

directories: 
	@mkdir -p build

$(TARGET): $(OBJ)
	g++ -o $@ $^ $(LIBS) 

clean:
	@rm -f $(TARGET)

dist:
	zip guillem-orellana-pacman.zip src/* include/* Makefile

test: dist
	@mkdir -p test/
	@unzip -o guillem-orellana-pacman.zip -d test/
	@make -C test/
	@-timeout 5 test/pacman
	@rm -rf test