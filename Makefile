TARGET	:= pacman

SOURCES := src
IDIR 	:= include

ODIR	:= build

LIBS	= -lglut -lGLU -lGL -lm

_DEPS = game-object.h cell.h map.h util.h map-generator.h game-manager.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cell.o map.o util.o map-generator.o game-manager.o
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