C_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations  \
-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wconversion 						\
-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 			\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 			\
-Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SFML_STUFF = -l sfml-graphics -l sfml-window -l sfml-system

CPP = g++
SRC_NAME  = main
EXEC_NAME = mandelbrot

start: $(SRC_NAME).o common.o
	$(CPP) $(SRC_NAME).o common.o -o $(EXEC_NAME) $(SFML_STUFF) $(C_FLAGS)

go:
	./$(EXEC_NAME)

$(SRC_NAME).o: ./$(SRC_NAME).cpp
	$(CPP) ./$(SRC_NAME).cpp -c -o $(SRC_NAME).o

common.o: ./common/.*
	$(CPP) ./common/common.cpp -c -o common.o

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o
