#include "ComChannel.hh"
#include <iostream>
#include <unistd.h>  // Dla funkcji close()

void ComChannel::SendMoveCommand(const std::string& objectName, double speed, double distance) {
        std::cout << "Wysyłanie komendy ruchu dla " << objectName
                  << " z prędkością " << speed << " i odległością " << distance << std::endl;
}

bool ComChannel::Init(int Socket){
     // Sprawdzenie poprawności deskryptora
    if (Socket < 0) {
        std::cerr << "Błędny deskryptor połączenia: " << Socket << std::endl;
        return false;
    }

    // Zamknięcie istniejącego deskryptora, jeśli jest ustawiony
    if (fd != -1) {
        close(fd);
    }

    // Przypisanie nowego deskryptora
    fd = Socket;
    std::cout << "Deskryptor połączenia ustawiony na: " << fd << std::endl;
    return true;
}