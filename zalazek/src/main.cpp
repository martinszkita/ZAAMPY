#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <cassert>
#include <sstream> 
#include "AbstractInterp4Command.hh"
#include "../plugin/inc/Interp4Move.hh"
#include "Scene.hh"
#include "ComChannel.hh"


using namespace std;


int main()
{
  std::ifstream file("polecenia.xml");  // Otwieranie pliku z poleceniami
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku z poleceniami!" << std::endl;
        return 1;
    }

  void *pLibHnd_Move = dlopen("libInterp4Move.so",RTLD_LAZY); // ladowanie biblioteki dynamicznej
  AbstractInterp4Command *(*pCreateCmd_Move)(void);
  void *pFun;

  if (!pLibHnd_Move) {
    cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Move,"CreateCmd"); // uzyskanie wskaznika do obiektu w bibliotece
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd = pCreateCmd_Move();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;

  //
  std::string command;
  while (std::getline(file, command)) {  // Czytanie linii z poleceniami
      std::istringstream iss(command);
      std::string cmdType;
      iss >> cmdType;

      if (cmdType == "Move") {
          // Tworzymy wtyczkę Move i odczytujemy parametry polecenia
          std::string objectName;
          double speed, distance;
          iss >> objectName >> speed >> distance;  // Odczyt nazwy obiektu, prędkości i odległości

          // Tworzymy instancję wtyczki Move
          Interp4Move movePlugin;
          movePlugin.SetSpeed(speed);  // Przykładowe ustawienie prędkości
          movePlugin.SetDistance(distance);  // Przykładowe ustawienie odległości


          Scene scene;
          ComChannel comChannel;

          // Wykonaj polecenie
          movePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
      }
  }
  //
  
  delete pCmd;

  dlclose(pLibHnd_Move);
}
