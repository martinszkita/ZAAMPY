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

using namespace std;
using ::cout;

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
    // Ładowanie wtyczki Move
    void *pLibHnd_Move = dlopen("libs/Interp4Move.so", RTLD_LAZY);
    if (!pLibHnd_Move) {
        cerr << "!!! Brak biblioteki: Interp4Move.so\n";
        return 1;
    }

    auto *pFun = dlsym(pLibHnd_Move, "CreateCmd");
    if (!pFun) {
        cerr << "!!! Nie znaleziono funkcji CreateCmd\n";
        return 1;
    }
    auto *pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);

    AbstractInterp4Command *pCmd = pCreateCmd_Move();

    cout << "\nCommand Name: " << pCmd->GetCmdName() << "\n\n";
    pCmd->PrintSyntax();
    cout << "\n";
    pCmd->PrintCmd();
    cout << "\n";

    // Przetwarzanie poleceń z pliku XML
    std::ifstream file("polecenia.xml");
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku z poleceniami!\n";
        return 1;
    }

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

            Scene scene;
            ComChannel comChannel;

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

            Scene scene;
            ComChannel comChannel;

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

            Scene scene;
            ComChannel comChannel;

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

            Scene scene;
            ComChannel comChannel;

            rotatePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }
    }

    delete pCmd;
    dlclose(pLibHnd_Move);
    return 0;
}
