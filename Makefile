# A sample Makefile for building clone_detect and using it in
# Acceptance Tests(AT). Please tweak it it suit your environment and
# project. But you have to keep this Makefile in the top directory.
#
# SYNOPSIS:
#
#   make [all]  -- makes everything
#   make TARGET -- makes the given target, such as at, ut, target
#     make ut   -- execute ut/build/Makefile, complete by yourself
#     make at   -- execute all AT cases
#     make at CASES=[No.] -- execute story [No.] AT case
#   make clean  -- removes all files generated by make


# Please tweak the following variable definitions as needed by your
# project, except TARGET, which you can use in your own targets but
# shouldn't modify.
CXX = g++
CXXFLAGS += -g -Wall -Werror -Wextra
LDFLAGS +=

TARGET = clone_detect

# Points to source files directory of clone_detect.
SRC_DIR = ./src

# Points to directory where project header files is.
SRC_INC = ./src

# Lists of all objects. *.o means an object file of c file,
# *.oxx means an object file of cpp file. You have to list all object
# files here with directories in prefix.
SRC_OBJ = $(SRC_DIR)/conver.o $(SRC_DIR)/clone_check.o $(SRC_DIR)/readir.o $(SRC_DIR)/compare.o $(SRC_DIR)/json.o $(SRC_DIR)/clone_detect.o

.PHONY: all target ut at clean

# House-keeping build targets.
all: $(TARGET)

$(TARGET):$(SRC_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	cd AT/ && python cleanup.py
	-rm -f $(TARGET) $(SRC_OBJ)

# Rules for ut
ut:ut/build/Makefile
	@echo compiling UT
	+make -C ut/build/
    
ut_run:
	cd ut/build && ./all

# Rules to compile all c/cpp files
%.o:%.c
	@echo compiling SRC files
	$(CXX) $(CXXFLAGS) -I $(SRC_INC) -I . -c $< -o $@

%.oxx:%.cpp
	@echo compiling SRC files
	$(CXX) $(CXXFLAGS) -I $(SRC_INC) -I . -c $< -o $@


# Below scripts is only used for AT
ifndef $(CASES)
CASES:= # end of the line
endif

at:$(TARGET)
	@cp clone_detect AT/ && cd AT/ && python runtest.py $(CASES) 
    


