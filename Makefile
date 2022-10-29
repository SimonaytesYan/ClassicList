all: logging
	g++ main.cpp Obj/Logging.o -o Exe\Start.exe 

create_dirs:
	mkdir GraphicDumps
	mkdir Exe
	mkdir Obj

logging:
	g++ -c Libs/Logging/Logging.cpp -o Obj/Logging.o
