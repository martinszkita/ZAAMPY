# __start__: obj __lines_for_space__ interp __plugin__
# 	export LD_LIBRARY_PATH="./libs"; ./interp

# obj:
# 	mkdir obj


# __lines_for_space__:
# 	@echo
# 	@echo
# 	@echo
# 	@echo
# 	@echo


# __plugin__:
# 	cd plugin; make

# CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
# LDFLAGS=-Wall
# LDLIBS += -Llibs -lInterp4Move



# interp: obj/main.o
# 	g++ ${LDFLAGS} -o interp  obj/main.o -ldl

# obj/Interp4Command.o: src/Interp4Command.cpp
# 	$(CXX) -c $(CXXFLAGS) -o obj/Interp4Command.o src/Interp4Command.cpp

# obj/main.o: src/main.cpp inc/AbstractInterp4Command.hh inc/AbstractScene.hh \
#             inc/AbstractComChannel.hh plugin/inc/Interp4Move.hh
# 	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp


# obj/ComChannel.o: src/ComChannel.cpp
# 	$(CXX) -c $(CXXFLAGS) -o obj/ComChannel.o src/ComChannel.cpp

# obj/Scene.o: src/Scene.cpp
# 	$(CXX) -c $(CXXFLAGS) -o obj/Scene.o src/Scene.cpp

# obj/MobileObj.o: src/MobileObj.cpp
# 	$(CXX) -c $(CXXFLAGS) -o obj/MobileObj.o src/MobileObj.cpp

# doc:
# 	cd dox; make

# clean:
# 	rm -f obj/* interp core*

# clean_plugin:
# 	cd plugin; make clean

# cleanall: clean
# 	cd plugin; make cleanall
# 	cd dox; make cleanall
# 	rm -f libs/*
# 	find . -name \*~ -print -exec rm {} \;

# help:
# 	@echo
# 	@echo "  Lista podcelow dla polecenia make"
# 	@echo 
# 	@echo "        - (wywolanie bez specyfikacji celu) wymusza"
# 	@echo "          kompilacje i uruchomienie programu."
# 	@echo "  clean    - usuwa produkty kompilacji oraz program"
# 	@echo "  clean_plugin - usuwa plugin"
# 	@echo "  cleanall - wykonuje wszystkie operacje dla podcelu clean oraz clean_plugin"
# 	@echo "             oprocz tego usuwa wszystkie kopie (pliki, ktorych nazwa "
# 	@echo "             konczy sie znakiem ~)."
# 	@echo "  help  - wyswietla niniejszy komunikat"
# 	@echo
# 	@echo " Przykladowe wywolania dla poszczegolnych wariantow. "
# 	@echo "  make           # kompilacja i uruchomienie programu."
# 	@echo "  make clean     # usuwa produkty kompilacji."
# 	@echo

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Iinc -Iplugin/inc
LDFLAGS = -ldl

# Directories
SRC_DIR = src
OBJ_DIR = obj
PLUGIN_DIR = plugin
LIB_DIR = libs
PLUGIN_SRC_DIR = $(PLUGIN_DIR)/src
PLUGIN_OBJ_DIR = $(PLUGIN_DIR)/obj

# Sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(PLUGIN_SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter $(SRC_DIR)/%.cpp,$(SRCS))) \
       $(patsubst $(PLUGIN_SRC_DIR)/%.cpp, $(PLUGIN_OBJ_DIR)/%.o, $(filter $(PLUGIN_SRC_DIR)/%.cpp,$(SRCS)))

# Target executable
TARGET = interp

# Libraries
LDLIBS = -L$(LIB_DIR) -lInterp4Move

# Rule to build the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# Rule to compile object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PLUGIN_OBJ_DIR)/%.o: $(PLUGIN_SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR) $(PLUGIN_OBJ_DIR):
	mkdir -p $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR)/*.o $(PLUGIN_OBJ_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: clean

