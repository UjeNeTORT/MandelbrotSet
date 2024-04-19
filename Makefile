C_FLAGS =

SFML_STUFF = -l sfml-graphics -l sfml-window -l sfml-system

AVX_STUFF  = -mavx2

CPP = g++
SRC_NAME  = main
EXEC_NAME = mandelbrot.out
OPTIMIZE  = -O1
DEBUG_INFO = -g

start: $(SRC_NAME).o common.o mandelbrot.o tests.o
	$(CPP) $(SRC_NAME).o common.o mandelbrot.o tester.o -o $(EXEC_NAME) $(SFML_STUFF) $(C_FLAGS)

run:
	./$(EXEC_NAME)

test:
	./$(EXEC_NAME) --test-performance

test-fast:
	./$(EXEC_NAME) -tp tests/testresults/res1.csv tests/testcases/cases_light.txt

analyze:
	./$(EXEC_NAME) --analyze-results

$(SRC_NAME).o: ./$(SRC_NAME).cpp
	$(CPP) $(OPTIMIZE) $(DEBUG_INFO) ./$(SRC_NAME).cpp -c -o $(SRC_NAME).o

common.o: ./common/*.*
	$(CPP) $(OPTIMIZE) $(DEBUG_INFO) ./common/common.cpp -c -o common.o

mandelbrot.o: ./mandelbrot/*.*
	$(CPP) $(OPTIMIZE) $(DEBUG_INFO) ./mandelbrot/mandelbrot.cpp -c -o mandelbrot.o $(AVX_STUFF)

tests.o: ./tests/*.*
	$(CPP) $(OPTIMIZE) $(DEBUG_INFO) ./tests/tester.cpp -c -o tester.o

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o
