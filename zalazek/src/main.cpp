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
    
    // obsluga wczytywania pluginów wymienionych w configu
    /* kroki do wczytania pluginu:
        1.void * plugin= dlopen()
        2.auto * pFun = dlsym()
        3.auto *pCreateCmd<> = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
        4.AbstractInterp4Command *pCmd = pCreateCmd_<>();
        5.delete pCmd;
        6.dlclose(pLibHnd_Move); 
    */
    bool load_move_plugin = false;
    bool load_rotate_plugin = false;
    bool load_set_plugin = false;
    bool load_pause_plugin = false;

    unsigned int plugin_count = config.plugins.size();
    void * dlopen_plugin_ptr[plugin_count];
    void * dlsym_plugin_ptr[plugin_count];
    
    for (const auto & plugin : config.plugins){
        if (plugin == "Interp4Move.so"){
            load_move_plugin = true;
        }
        else if (plugin == "Interp4Rotate.so"){
            load_rotate_plugin = true;
        }
        else if (plugin == "Interp4Set.so"){
            load_set_plugin = true;
        }
        else if (plugin == "Interp4Pause.so"){
            load_pause_plugin = true;
        }
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
/*
    // łączenie z serwerem
int client_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_socket < 0) {
    cerr << "Błąd tworzenia gniazda!" << endl;
    return -1;
}

struct sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons(PORT); // Port serwera
inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr); // Adres serwera

if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    cerr << "Błąd połączenia z serwerem!" << endl;
    close(client_socket);
    return -1;
}

cout << "Połączono z serwerem graficznym." << endl;

// --- Wysłanie poleceń do serwera ---
SendCommand(client_socket, "Clear\n");
const auto& objects = scene.GetObjects();

for (const auto& obj : objects) {
    string command = "AddObj Name=" + obj.name;
    if (!obj.scale.empty()) command += " Scale=(" + obj.scale + ")";
    if (!obj.shift.empty()) command += " Shift=(" + obj.shift + ")";
    if (!obj.rotation.empty()) command += " RotXYZ_deg=(" + obj.rotation + ")";
    if (!obj.translation.empty()) command += " Trans_m=(" + obj.translation + ")";
    if (!obj.color.empty()) command += " RGB=(" + obj.color + ")";
    command += "\n";

    SendCommand(client_socket, command);
    cout << "Wysłano polecenie: " << command;
}

// --- Zamknięcie połączenia ---
SendCommand(client_socket, "Close\n");
close(client_socket);
*/
delete pCmd;
dlclose(pLibHnd_Move);
return 0;

}
