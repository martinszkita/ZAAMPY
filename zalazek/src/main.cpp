#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <cassert>
#include <sstream> 
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
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
// #include "../klient/inc/Port.hh"

#define SERVER_IP "127.0.0.1"  // Serwer działa lokalnie
#define PORT_NUMBER 6217

using namespace std;
using ::cout;

int SendCommand(int socket, const std::string &command) {
    ssize_t bytes_sent = send(socket, command.c_str(), command.size(), 0);
    if (bytes_sent < 0) {
        std::cerr << "Błąd wysyłania komendy: " << strerror(errno) << std::endl;
        return -1;
    }
    return 0; // Sukces
}

void loadPlugins(const std::vector<std::string>& plugins) {
    for (const auto& plugin : plugins) {
        void* handle = dlopen(plugin.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Error loading plugin: " << plugin << " - " << dlerror() << std::endl;
            throw std::runtime_error("Plugin load failed");
        }
        std::cout << "Loaded plugin: " << plugin << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <config_file.xml>\n";
        return 1;
    }

    // Wczytywanie konfiguracji XML
    const char *configFileName = argv[1];
    Configuration config;

    try {
        xercesc::XMLPlatformUtils::Initialize();

        // Tworzenie parsera SAX2
        xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
        XMLInterp4Config handler(config);

        // Konfiguracja parsera
        parser->setContentHandler(&handler);
        parser->setErrorHandler(&handler);

        std::cout << "Parsing configuration file: " << configFileName << "\n";
        parser->parse(configFileName);

        
        delete parser;
        xercesc::XMLPlatformUtils::Terminate();
    } catch (const xercesc::XMLException &e) {
        char *message = xercesc::XMLString::transcode(e.getMessage());
        std::cerr << "XML Parsing error: " << message << "\n";
        xercesc::XMLString::release(&message);
        return 1;
    }
    
    vector<void*> libraryHandles; // Do przechowywania uchwytów bibliotek
    vector<AbstractInterp4Command*> commands; // Do przechowywania obiektów wtyczek

    try {
        // Ładowanie bibliotek dynamicznych
        const vector<string> libraries = {
            "libs/Interp4Move.so",
            "libs/Interp4Rotate.so",
            "libs/Interp4Pause.so",
            "libs/Interp4Set.so"
        };

        for (const auto& libName : libraries) {
            cout << "\nfor:\n"<< libName <<endl;
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

            cout << "Załadowano bibliotekę: " << libName << endl;
        }


    }catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }


    // Przetwarzanie poleceń z pliku XML
    std::ifstream file("polecenia.xml");
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku z poleceniami!\n";
        return 1;
    }

    Scene scene;
    ComChannel comChannel;

    std::string command;
    while (std::getline(file, command)) {
        std::istringstream iss(command);
        std::string cmdType;
        iss >> cmdType;

        if (cmdType == "Move") {
            std::string objectName;
            double speed, distance;
            iss >> objectName >> speed >> distance;

            cout << "Odczytano: " << objectName << " " << speed << " " << distance << "\n";

            Interp4Move movePlugin;
            movePlugin.setRobotName(objectName); // ?
            movePlugin.setSpeed(speed);
            movePlugin.setDistance(distance);

            movePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }

        if (cmdType == "Rotate") {
            std::string objectName;
            double ang_speed, ang_deg;
            char axis;
            iss >> objectName >> axis>> ang_speed >> ang_deg;

            cout << "Odczytano: " << objectName << " " << axis  <<" "<< ang_speed<< " " << ang_deg << "\n";
    
            Interp4Rotate rotatePlugin;
            rotatePlugin.SetRobotName(objectName);
            rotatePlugin.SetAxis(axis);
            rotatePlugin.SetAngularVelocity(ang_speed);
            rotatePlugin.SetAngle(ang_deg);


            rotatePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }

        if (cmdType == "Set") {
            std::string objectName;
            double x,y,z,fi,teta,psi;
            iss >> objectName >> x >> y >> z >> fi >> teta >> psi;
            Vector3D pos_vec_tmp;
            pos_vec_tmp[0] = x;
            pos_vec_tmp[1] = y;
            pos_vec_tmp[2] = z;

            cout << "Odczytano: " << objectName << " " << x  <<" "<< y<< " " << z << " " << fi  <<" "<< teta<< " " << psi<<endl;
    
            Interp4Set setPlugin;
            setPlugin.SetRobotName(objectName);
            setPlugin.SetPosition(pos_vec_tmp);


            setPlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }

        if (cmdType == "Pause") {
            std::string objectName;
            double wait_ms;
            iss >> objectName >> wait_ms;
            cout << "Odczytano: " << objectName << " " << wait_ms << "\n";
    
            Interp4Pause rotatePlugin;
            rotatePlugin.SetRobotName(objectName);
            rotatePlugin.SetPauseTime(wait_ms);


            rotatePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }
    }

    for (auto& handle : libraryHandles) {
        dlclose(handle);
    }

return 0;

}
