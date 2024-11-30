# Flagi i ustawienia
CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinc -Iplugin/inc -I/usr/include/xercesc -pthread -g
LDLIBS = -Llibs -Wl,-rpath,libs -l:Interp4Move.so -l:Interp4Set.so -l:Interp4Rotate.so -l:Interp4Pause.so -ldl -lxerces-c


# Główne cele
all: plugins interp

# Tworzenie wtyczek w katalogu plugin/
plugins:
	$(MAKE) -C plugin

# Kompilacja głównego programu
interp: obj/ComChannel.o obj/main.o obj/MobileObj.o obj/Scene.o obj/XMLInterp4Config.o obj/Sender.o
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
