#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <cassert>
#include <sstream> 
#include "xmlinterp.hh"
#include "Configuration.hh"
#include "../plugin/inc/Interp4Move.hh"
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
    // } catch (...) {
    //     cerr << "An unknown error occurred during XML parsing.\n";
    //     return 1;
    // }

    // Ładowanie wtyczki Move
    void *pLibHnd_Move = dlopen("libs/libInterp4Move.so", RTLD_LAZY);
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
            movePlugin.setSpeed(speed);
            movePlugin.setDistance(distance);

            Scene scene;
            ComChannel comChannel;

            movePlugin.ExecCmd(scene, objectName.c_str(), comChannel);
        }
    }

    delete pCmd;
    dlclose(pLibHnd_Move);
    return 0;
}
