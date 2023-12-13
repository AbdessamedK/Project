incSDL2 = /opt/homebrew/include
libSDL2 = /opt/homebrew/lib

all : bin/survivor_hero_txt bin/test bin/SDLJeu

doc : doc/image.doxy
	doxygen -g doc/image.doxy && doxygen doc/image.doxy

bin/SDLJeu : obj/mainSDL.o obj/NBComplex.o obj/PouvoirBasique.o  obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Terrain.o obj/Niveau.o obj/Monde.o obj/SDLMonde.o 
	g++ -std=c++11 -I$(incSDL2) -L$(libSDL2) obj/mainSDL.o obj/NBComplex.o obj/PouvoirBasique.o  obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Niveau.o obj/Terrain.o obj/Monde.o obj/SDLMonde.o -o bin/SDLJeu -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

bin/survivor_hero_txt : obj/main_txt.o obj/NBComplex.o obj/PouvoirBasique.o  obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Terrain.o obj/Niveau.o obj/Monde.o obj/txtMonde.o obj/winTxt.o 
	g++ -std=c++11 -I$(incSDL2) -L$(libSDL2) obj/main_txt.o obj/NBComplex.o obj/PouvoirBasique.o  obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Niveau.o obj/Terrain.o obj/Monde.o obj/txtMonde.o obj/winTxt.o -o bin/survivor_hero_txt -lSDL2 -lSDL2_ttf -lSDL2_image

bin/test: obj/mainTestRegression.o obj/NBComplex.o obj/PouvoirBasique.o  obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Niveau.o obj/Terrain.o obj/Monde.o
	g++ -std=c++11 -I$(incSDL2) -L$(libSDL2) obj/mainTestRegression.o obj/NBComplex.o obj/PouvoirBasique.o obj/Corps.o obj/Personnage.o obj/Bouclier.o obj/Aura.o obj/Monstre.o obj/Niveau.o obj/Terrain.o obj/Monde.o -o bin/test -lSDL2 -lSDL2_ttf -lSDL2_image

obj/mainSDL.o : src/sdl/SDLMonde.h src/sdl/mainSDL.cpp
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/sdl/mainSDL.cpp -o obj/mainSDL.o 

obj/SDLMonde.o : src/core/Monde.h src/sdl/SDLMonde.cpp src/sdl/SDLMonde.h
	g++ -std=c++11 -I$(incSDL2) -c src/sdl/SDLMonde.cpp -o obj/SDLMonde.o

obj/txtMonde.o : src/core/Monde.h src/txt/txtMonde.cpp src/txt/txtMonde.h 
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/txt/txtMonde.cpp -o obj/txtMonde.o

obj/winTxt.o : src/txt/winTxt.cpp src/txt/winTxt.h 
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/txt/winTxt.cpp -o obj/winTxt.o 

obj/main_txt.o : src/txt/winTxt.h src/txt/txtMonde.h src/txt/main_txt.cpp
	g++ -std=c++11 -I$(incSDL2) -Wall -c -g src/txt/main_txt.cpp -o obj/main_txt.o

obj/Monde.o : src/core/Monde.cpp src/core/Monde.h src/core/Personnage.h src/core/Niveau.h src/core/Terrain.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/Monde.cpp -o obj/Monde.o

obj/Corps.o : src/core/Corps.h src/core/Corps.cpp src/core/NBComplex.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/Corps.cpp -o obj/Corps.o

obj/PouvoirBasique.o : src/core/PouvoirBasique.cpp src/core/PouvoirBasique.h src/core/NBComplex.h src/core/Pouvoir.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/PouvoirBasique.cpp -o obj/PouvoirBasique.o 

obj/Bouclier.o : src/core/Bouclier.cpp src/core/Bouclier.h src/core/NBComplex.h src/core/Pouvoir.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/Bouclier.cpp -o obj/Bouclier.o 

obj/Niveau.o : src/core/Niveau.cpp src/core/Niveau.h src/core/Monstre.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/Niveau.cpp -o obj/Niveau.o 

obj/Monstre.o : src/core/Monstre.cpp src/core/Monstre.h 
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/Monstre.cpp -o obj/Monstre.o 

obj/Aura.o : src/core/Aura.cpp src/core/Aura.h src/core/NBComplex.h src/core/Pouvoir.h
	g++ -std=c++11 -I$(incSDL2)  -Wall -c src/core/Aura.cpp -o obj/Aura.o 

obj/mainTestRegression.o : src/core/mainTestRegression.cpp src/core/NBComplex.h src/core/Personnage.h src/core/Corps.h src/core/Terrain.h src/core/Aura.h src/core/Bouclier.h
	g++ -std=c++11 -I$(incSDL2) -Wall -c src/core/mainTestRegression.cpp -o obj/mainTestRegression.o

obj/NBComplex.o : src/core/NBComplex.cpp src/core/NBComplex.h 
	g++ -std=c++11 -Wall -c src/core/NBComplex.cpp -o obj/NBComplex.o 

obj/Personnage.o : src/core/Personnage.h src/core/Personnage.cpp src/core/Corps.h src/core/NBComplex.h
	g++ -std=c++11 -I$(incSDL2)  -Wall -c src/core/Personnage.cpp -o obj/Personnage.o

obj/Terrain.o : src/core/Terrain.h src/core/Terrain.cpp
	g++ -std=c++11 -I$(incSDL2)  -Wall -c src/core/Terrain.cpp -o obj/Terrain.o

clean : 
	rm -rf obj/*.o bin/*