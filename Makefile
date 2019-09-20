TARGET	:= pacman

SOURCES := src
IDIR 	:= include

ODIR	:= build

LIBS	= -lglut -lGLU -lGL -lm

_DEPS = game-object.h cell.h map.h util.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cell.o map.o util.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SOURCES)/%.cpp $(DEPS)
	g++ -c -o $@ $< -Iinclude

$(TARGET): $(OBJ)
	g++ -o $@ $^ $(LIBS) 

.PHONY: clean

clean:
	@rm -f $(TARGET)