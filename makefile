EXE := test

source := $(wildcard *.cpp)

CC := g++ -std=c++11 -O3
RM := rm -rf
LIB := -lpthread

${EXE} : main.cpp
	${CC} -o $@ $< ${LIB}

.PHONY: clean
clean: 
	${RM} ${EXE} 
