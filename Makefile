ALLEGRO_VERSION = 5.0.10
MINGW_VERSION = 4.7.0
FOLDER = ./

FOLDER_NAME = \allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO = $(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO = \lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO = \include

all: outro.o

TPAllegro.o: TPAllegro.c
	gcc TPAllegro.c -I$(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -o $@

clean:
	rm TPAllegro.o

