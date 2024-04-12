C_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations  \
-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wconversion 						\
-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 			\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 			\
-Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SFML_STUFF = -l sfml-graphics -l sfml-window -l sfml-system

AVX_STUFF  = -mavx2

CPP = g++
OPTIMIZE = -O1
SRC_NAME  = main
EXEC_NAME = mandelbrot.out

start: $(SRC_NAME).o common.o mandelbrot.o tests.o
	$(CPP) $(SRC_NAME).o common.o mandelbrot.o tester.o -o $(EXEC_NAME) $(SFML_STUFF) $(C_FLAGS) $(OPTIMIZE)

run:
	./$(EXEC_NAME)

test:
	./$(EXEC_NAME) --test-performance

test-light:
	./$(EXEC_NAME) -tp tests/testresults/res1.csv tests/testcases/cases_light.txt

analyze:
	./$(EXEC_NAME) --analyze-results

$(SRC_NAME).o: ./$(SRC_NAME).cpp
	$(CPP) ./$(SRC_NAME).cpp -c -o $(SRC_NAME).o

common.o: ./common/*.*
	$(CPP) ./common/common.cpp -c -o common.o

mandelbrot.o: ./mandelbrot/*.*
	$(CPP) ./mandelbrot/mandelbrot.cpp -c -o mandelbrot.o $(AVX_STUFF)

tests.o: ./tests/*.*
	$(CPP) ./tests/tester.cpp -c -o tester.o

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o
