OFFICE_HOME?=/usr/lib/libreoffice
OO_SDK_HOME?=$(OFFICE_HOME)/sdk
OFFICE_PROGRAM_PATH?=$(OFFICE_HOME)/program


CXX = g++
CXXFLAGS = -std=c++26 -fpic -fvisibility=hidden -I/usr/local/include -I/home/trevor/Development/libreoffice26.2_sdk/inc \
	-I$(OO_SDK_HOME)/include -I./include -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3
LINKFLAGS = -Wl,--allow-shlib-undefined -Wl,-export-dynamic -Wl,-z,defs -Wl,--no-whole-archive
LINKDIRS = -L/usr/local/lib -L$(OO_SDK_HOME)/lib -L$(OFFICE_HOME)/program
LINKLIBS = -lcpr -lcurl -luno_cppuhelpergcc3 -luno_cppu -luno_salhelpergcc3 -luno_sal -lmpdec++ -lmpdec 

SRC_DIRS = .
INC_DIRS = ./include
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
HEADERS = $(shell find ${INC_DIRS} -name '*.hpp')
OBJ_FILES := $(patsubst %.cpp,./obj/%.o,$(SRCS))

DOLLAR = $$

./bin/helper: ./bin/Main
	@echo "#!/bin/bash" > "./bin/helper"
	@echo 'LD_LIBRARY_PATH=${OO_SDK_HOME}/lib:${OFFICE_PROGRAM_PATH}:.:$${LD_LIBRARY_PATH}' >> "./bin/helper"
	@echo 'PATH=${OO_SDK_HOME}/bin:${OFFICE_PROGRAM_PATH}:$${PATH}' >> "./bin/helper"
	@echo 'UNO_PATH=${OFFICE_PROGRAM_PATH}' >> "./bin/helper"
	@echo export LD_LIBRARY_PATH >> "./bin/helper"
	@echo export PATH >> "./bin/helper"
	@echo export UNO_PATH >> "./bin/helper"
	@echo "\"/home/trevor/MEGA/Finances/Code/Transactions/bin/Main\"" >> "./bin/helper"
	@chmod +x "./bin/helper"

./bin/Main: $(OBJ_FILES)
	$(CXX) $(LINKFLAGS) -o $@ $(OBJ_FILES) $(LINKDIRS) $(LINKLIBS)

./obj/Main.o: Main.cpp ./include/Plaid.hpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

./obj/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@



clean:
	rm -rf ./obj/* ./bin/*

run: CXXFLAGS += -O2 -DNDEBUG
run: ./bin/helper
	@make > /dev/null
	@./bin/helper

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: ./bin/helper
	@make > /dev/null
	@./bin/helper

