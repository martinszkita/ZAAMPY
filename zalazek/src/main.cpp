#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <cassert>
#include <sstream>
#include <thread>
#include "XMLInterp4Config.hh"
#include "Configuration.hh"
#include "../plugin/inc/Interp4Move.hh"
#include "../plugin/inc/Interp4Set.hh"
#include "../plugin/inc/Interp4Rotate.hh"
#include "../plugin/inc/Interp4Pause.hh"
#include "MobileObj.hh"
#include "Scene.hh"
#include "ComChannel.hh"
#include "AbstractInterp4Command.hh"
#include "AbstractComChannel.hh"
#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include "Port.hh"
#include "Sender.hh"
#include "StepSize.hh"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <config_file.xml> <instructions_file.xml>" << endl;
        return 1;
    }

    // Wczytywanie konfiguracji XML
    const char *configFileName = argv[1];
    const char *instructionFile = argv[2];

    Configuration config = XMLInterp4Config::redConfigurationFromXML(configFileName);

    vector<void*> libraryHandles; // Do przechowywania uchwytów bibliotek
    vector<AbstractInterp4Command*> commands; // Do przechowywania obiektów wtyczek

    try {
        // Ładowanie bibliotek dynamicznych
        for (const auto& libName : config.plugins) {
            void* pLibHnd = dlopen(libName.c_str(), RTLD_LAZY);
            if (!pLibHnd) {
                cerr << "!!! Brak biblioteki: " << libName << endl;
                return 1;
            }

            auto* pFun = dlsym(pLibHnd, "CreateCmd");
            if (!pFun) {
                cerr << "!!! Nie znaleziono funkcji CreateCmd w bibliotece: " << libName << endl;
                dlclose(pLibHnd);
                return 1;
            }

            auto* pCreateCmd = reinterpret_cast<AbstractInterp4Command* (*)()>(pFun);
            AbstractInterp4Command* pCmd = pCreateCmd();
            commands.push_back(pCmd);
            libraryHandles.push_back(pLibHnd);

            //cout << "Załadowano bibliotekę: " << libName << endl;
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // Inicjalizacja połączenia z serwerem
    Scene scene;
    int Socket4Sending;

    if (!OpenConnection(Socket4Sending)) return 1;

    Sender ClientSender(Socket4Sending, &scene);
    std::thread Thread4Sending(Fun_CommunicationThread, &ClientSender);

    // czyscimy scene przed wyslaniem
    if (Send(Socket4Sending, "Clear \n") < 0) {
        std::cerr << "Error: Nie udalo sie wyslac na serwer!\n";
    }

    else {
        std::cout << "Scena została wyczyszczona! " << std::endl;
    }


    // Wysyłanie poleceń z config.xml do serwera
    for (const auto& cube : config.cubes) {
        ostringstream oss;
        oss << "AddObj Name=" <<  cube.name
            << " RGB=" << stringToVecString(cube.color)
            << " Scale=" << stringToVecString(cube.scale)
            << " Shift=" << stringToVecString(cube.shift)
            << " RotXYZ_deg=" << stringToVecString(cube.rotXYZ)
            << " Trans_m=" << stringToVecString(cube.trans_m) << "\n";

        if (Send(Socket4Sending, oss.str().c_str()) < 0) {
            std::cerr << "Error: Nie udalo sie wyslac na serwer!\n";
        }
        // else{
        //     std::cout << "Sukces: udalo sie wysłać na serwer!" << std::endl;
        // }

        // Dodawanie obiektu do sceny
        scene.AddMobileObj(new MobileObj(cube.name));

        if(scene.GetObjects().empty()){
            std::cerr << "Error: pusta lista obiektów na scenie!" << std::endl;
        }

    }



    std::ifstream file(instructionFile);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku z poleceniami!\n";
        return 1;
    }

    ComChannel comChannel;
    string command;

    // obsluga poleceń z pliku polecenia.xml
    while (getline(file, command)) {
        istringstream iss(command);
        string cmdType;
        iss >> cmdType;

        if (cmdType == "Move") {
            string objectName;
            double speed, distance;
            iss >> objectName >> speed >> distance;

            cout << "Odczytano: " << cmdType <<" " << objectName << " " << speed << " " << distance << "\n";

            Interp4Move movePlugin;
            movePlugin.setRobotName(objectName);
            movePlugin.setSpeed(speed);
            movePlugin.setDistance(distance);
            movePlugin.ExecCmd(scene, objectName.c_str(), comChannel);

            std::ostringstream oss;
            oss << "UpdateObj Name=" << objectName
                << " Trans_m=" << scene.FindMobileObj(objectName.c_str())->GetPosition_m() << "\n"; 
            cout << "Move wysłano: " << oss.str() << endl;
            Send(Socket4Sending, oss.str().c_str());
        }

        else if (cmdType == "Rotate") {
            string objectName;
            char axis;
            double ang_speed, ang_deg;
            iss >> objectName >> axis >> ang_speed >> ang_deg;

            cout << "Odczytano: " << cmdType <<" " << objectName << " " << ang_speed << " " << ang_deg << "\n";

            Interp4Rotate rotatePlugin;
            rotatePlugin.SetRobotName(objectName);
            rotatePlugin.SetAxis(axis);
            rotatePlugin.SetAngularVelocity(ang_speed);
            rotatePlugin.SetAngle(ang_deg);
            rotatePlugin.ExecCmd(scene, objectName.c_str(), comChannel);

            std::ostringstream oss;
            oss << "UpdateObj Name=" << objectName
                 << " RotXYZ_deg=" << scene.FindMobileObj(objectName.c_str())->GetRotXYZ_deg() << "\n";

            cout << "Rotate wysłano: " << oss.str() << endl;
            cout << "to jest dobrze::::" << oss.str() << endl;
           // Send(Socket4Sending, oss.str().c_str());

            
            unsigned int index = MobileObj::GetAxisIndex(axis);
            double dt = 50; //delay w ms
            double deltaRot = dt*ang_speed/1000;
            // (ang_deg - scene.FindMobileObj(objectName.c_str())->GetRotXYZ_deg()[index]) / STEP_SIZE;
            Vector3D currentRot  = scene.FindMobileObj(objectName.c_str())->GetRotXYZ_deg();
            double currAngle = currentRot[index];

            while(abs(ang_deg)>deltaRot/2 ) {
                currAngle += deltaRot;
                ang_deg -=deltaRot;
                std::ostringstream oss; // Tworzymy nowy strumień na każdą iterację
                currentRot[index]+=deltaRot;
                // Aktualizacja odpowiedniej osi
               

                // Tworzenie polecenia UpdateObj
                oss << "UpdateObj Name=" << objectName
                    << " RotXYZ_deg=(" << currentRot[0] << ", "
                    << currentRot[1] << ", "
                    << currentRot[2] << ")" << "\n";
                
                std::string command = oss.str(); // Przechowujemy dane w ciągu znaków
                std::cout << "Aktualizacja Rotate: " << command << std::endl;
                std::cout << ang_deg <<  endl;
                // Wysłanie polecenia do serwera
                std::cout << command.c_str() << endl;
                Send(Socket4Sending, command.c_str());

                // Opóźnienie dla płynności animacji
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            


        }
        
        else if (cmdType == "Set") {
            string objectName;
            double x, y, z, fi, teta, psi;
            iss >> objectName >> x >> y >> z >> fi >> teta >> psi;
            Vector3D pos_vec_tmp = Vector3D({x,y,z});

            cout << "Odczytano: " << cmdType <<" " << objectName << " " << x << " " << y << " " << z << " " << fi << " " << teta << " " << psi << endl;

            Interp4Set setPlugin;
            setPlugin.SetRobotName(objectName);
            setPlugin.SetPosition(pos_vec_tmp);
            setPlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }

        else if (cmdType == "Pause") {
            string objectName;
            unsigned int wait_ms;
            iss >> objectName >> wait_ms;
            cout << "Odczytano: " << cmdType <<" " << objectName << " " << wait_ms << "\n";

            Interp4Pause pausePlugin;
            pausePlugin.SetRobotName(objectName);
            pausePlugin.SetPauseTime(wait_ms);
            pausePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }
 
    }

    for (auto& handle : libraryHandles) {
        dlclose(handle);
    }

    ClientSender.CancelCountinueLooping();
    Thread4Sending.join();
    close(Socket4Sending);

    return 0;
}
