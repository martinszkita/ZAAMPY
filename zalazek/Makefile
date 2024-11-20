# CXX = g++
# CXXFLAGS = -std=c++20  -Wall -Iinc -Iplugin/inc -I/usr/include/xercesc 
# LDFLAGS = -ldl -lxerces-c

# # Directories
# SRC_DIR = src
# OBJ_DIR = obj
# PLUGIN_DIR = plugin
# LIB_DIR = libs
# PLUGIN_SRC_DIR = $(PLUGIN_DIR)/src
# PLUGIN_OBJ_DIR = $(PLUGIN_DIR)/obj

# # Sources
# SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(PLUGIN_SRC_DIR)/*.cpp)
# OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter $(SRC_DIR)/%.cpp,$(SRCS))) \
#        $(patsubst $(PLUGIN_SRC_DIR)/%.cpp, $(PLUGIN_OBJ_DIR)/%.o, $(filter $(PLUGIN_SRC_DIR)/%.cpp,$(SRCS)))

# # Target executable
# TARGET = interp

# # Libraries
# LDLIBS = -L$(LIB_DIR) -lInterp4Move -lInterp4Set -lInterp4Rotate -lInterp4Pause


# # Default target
# all: $(TARGET)

# # Rule to build the final executable
# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

# # Rule for main object files
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Rule for plugin object files
# $(PLUGIN_OBJ_DIR)/%.o: $(PLUGIN_SRC_DIR)/%.cpp | $(PLUGIN_OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Create directories if they don't exist
# $(OBJ_DIR) $(PLUGIN_OBJ_DIR):
# 	mkdir -p $@

# # Clean up build files
# clean:
# 	rm -rf $(OBJ_DIR)/*.o $(PLUGIN_OBJ_DIR)/*.o $(TARGET)

# # Phony targets
# .PHONY: clean all


# Flagi i ustawienia
CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinc -Iplugin/inc -I/usr/include/xercesc
LDLIBS = -Llibs -Wl,-rpath,libs -l:Interp4Move.so -l:Interp4Set.so -l:Interp4Rotate.so -l:Interp4Pause.so -ldl -lxerces-c


# Główne cele
all: plugins interp

# Tworzenie wtyczek w katalogu plugin/
plugins:
	$(MAKE) -C plugin

# Kompilacja głównego programu
interp: obj/ComChannel.o obj/main.o obj/MobileObj.o obj/Scene.o obj/XMLInterp4Config.o
	$(CXX) $(CXXFLAGS) -o interp $^ $(LDLIBS)

# Reguły dla plików obiektowych
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Czyszczenie
clean:
	rm -f obj/*.o interp
	$(MAKE) -C plugin clean

cleanall: clean
	find . -name '*~' -exec rm -f {} \;
